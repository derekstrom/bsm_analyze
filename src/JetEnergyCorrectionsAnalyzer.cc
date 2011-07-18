// Jet Energy Correction Analyzer
//
// Given Jet uncorrected P4 apply Jet Energy Corrections and
// plot comparison histograms of the corrected in CMSSW P4 and offline
// corrections applied P4
//
// Created by Samvel Khalatyan, Jul 12, 2011
// Copyright 2011, All rights reserved

#include <ostream>
#include <string>
#include <vector>

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/Event.pb.h"
#include "interface/Monitor.h"
#include "interface/Selector.h"
#include "interface/JetEnergyCorrectionsAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::JetEnergyCorrectionsAnalyzer;

JetEnergyCorrectionsAnalyzer::JetEnergyCorrectionsAnalyzer()
{
    // Selectrors
    //
    _jet_selector.reset(new JetSelector());

    monitor(_jet_selector);

    // Monitors
    //
    _jet_cmssw_corrected_p4.reset(new LorentzVectorMonitor());
    _jet_uncorrected_p4.reset(new LorentzVectorMonitor());
    _jet_offline_corrected_p4.reset(new LorentzVectorMonitor());

    monitor(_jet_cmssw_corrected_p4);
    monitor(_jet_uncorrected_p4);
    monitor(_jet_offline_corrected_p4);
}

JetEnergyCorrectionsAnalyzer::JetEnergyCorrectionsAnalyzer(const JetEnergyCorrectionsAnalyzer &object)
{
    // Selectors
    //
    _jet_selector = 
        dynamic_pointer_cast<JetSelector>(object._jet_selector->clone());

    monitor(_jet_selector);

    // Monitors
    //
    // Monitors
    //
    _jet_cmssw_corrected_p4 =
        dynamic_pointer_cast<LorentzVectorMonitor>(object._jet_cmssw_corrected_p4->clone());

    _jet_uncorrected_p4 =
        dynamic_pointer_cast<LorentzVectorMonitor>(object._jet_uncorrected_p4->clone());

    _jet_offline_corrected_p4 =
        dynamic_pointer_cast<LorentzVectorMonitor>(object._jet_offline_corrected_p4->clone());

    monitor(_jet_cmssw_corrected_p4);
    monitor(_jet_uncorrected_p4);
    monitor(_jet_offline_corrected_p4);

    // Jet Energy Corrections
    //
    if (object._parameters.size())
    {
        _parameters.clear();
        _parameters.resize(object._parameters.size());
        std::copy(object._parameters.begin(), object._parameters.end(), _parameters.begin());

        _jec.reset(new FactorizedJetCorrector(_parameters));
    }
}

void JetEnergyCorrectionsAnalyzer::loadCorrections(const std::string &filename)
{
    _parameters.push_back(JetCorrectorParameters(filename));

    _jec.reset(new FactorizedJetCorrector(_parameters));
}

bool JetEnergyCorrectionsAnalyzer::didCorrectionsLoad() const
{
    return _jec;
}

const JetEnergyCorrectionsAnalyzer::P4MonitorPtr
    JetEnergyCorrectionsAnalyzer::jetCmsswCorrectedP4() const
{ 
    return _jet_cmssw_corrected_p4;
}

const JetEnergyCorrectionsAnalyzer::P4MonitorPtr
    JetEnergyCorrectionsAnalyzer::jetUncorrectedP4() const
{
    return _jet_uncorrected_p4;
}

const JetEnergyCorrectionsAnalyzer::P4MonitorPtr
    JetEnergyCorrectionsAnalyzer::jetOfflineCorrectedP4() const
{
    return _jet_offline_corrected_p4;
}

void JetEnergyCorrectionsAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void JetEnergyCorrectionsAnalyzer::process(const Event *event)
{
    if (!_jec)
        throw std::runtime_error("Jet Energy Corrections are not loaded");

    _out << "Event: " << event->extra().id()
        << " Lumi: " << event->extra().lumi()
        << " Run: " << event->extra().run() << endl;

    jets(event);
}

uint32_t JetEnergyCorrectionsAnalyzer::id() const
{
    return core::ID<JetEnergyCorrectionsAnalyzer>::get();
}

JetEnergyCorrectionsAnalyzer::ObjectPtr JetEnergyCorrectionsAnalyzer::clone() const
{
    return ObjectPtr(new JetEnergyCorrectionsAnalyzer(*this));
}

void JetEnergyCorrectionsAnalyzer::merge(const ObjectPtr &pointer)
{
    if (id() != pointer->id());

    boost::shared_ptr<JetEnergyCorrectionsAnalyzer> object =
        dynamic_pointer_cast<JetEnergyCorrectionsAnalyzer>(pointer);

    if (!object)
        return;

    Object::merge(pointer);

    _out << endl;
    _out << object->_out.str();
}

void JetEnergyCorrectionsAnalyzer::print(std::ostream &out) const
{
    out << _out.str();
}

// Private
//
void JetEnergyCorrectionsAnalyzer::jets(const Event *event)
{
    if (!event->jets().size()
            || !event->has_extra())
        return;

    _out << "Jets" << endl;

    typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;

    LockSelectorEventCounterOnUpdate lock(*_jet_selector);
    const float rho = event->extra().rho();
    uint32_t id = 1;
    for(Jets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet;
            ++jet, ++id)
    {
        if (!jet->has_extra())
            continue;

        if (_jet_selector->apply(*jet))
        {
            const LorentzVector &cmssw_p4 = jet->physics_object().p4();
            _jet_cmssw_corrected_p4->fill(cmssw_p4);

            const LorentzVector &uncorrected_p4 = jet->uncorrected_p4();
            _jet_uncorrected_p4->fill(uncorrected_p4);

            _jec->setJetEta(eta(uncorrected_p4));
            _jec->setJetPt(pt(uncorrected_p4));
            _jec->setJetE(uncorrected_p4.e());
            _jec->setNPV(event->primary_vertices().size());
            _jec->setJetA(jet->extra().area());
            _jec->setRho(rho);

            float jec = _jec->getCorrection();

            LorentzVector jec_p4 = uncorrected_p4;
            jec_p4 *= jec;
            _jet_offline_corrected_p4->fill(jec_p4);

            _out << "[" << setw(2) << right << id << "] Jet Energy Correction: "
                << jec << endl;

            _out << setw(5) << " "
                << "  CMSSW JEC "
                << "pT: " << pt(cmssw_p4)
                << " eta: " << eta(cmssw_p4)
                << endl;

            _out << setw(5) << " "
                << "     NO JEC "
                << "pT: " << pt(uncorrected_p4)
                << " eta: " << eta(uncorrected_p4)
                << endl;

            _out << setw(5) << " "
                << "Offline JEC "
                << "pT: " << pt(jec_p4)
                << " eta: " << eta(jec_p4)
                << endl;
        }
    }

    _out << endl;
}
