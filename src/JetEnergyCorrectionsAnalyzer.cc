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

void JetEnergyCorrectionsAnalyzer::print(std::ostream &out) const
{
}

// Private
//
void JetEnergyCorrectionsAnalyzer::jets(const Event *event)
{
    if (!event->jets().size())
        return;

    typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;

    LockSelectorEventCounterOnUpdate lock(*_jet_selector);
    for(Jets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet;
            ++jet)
    {
        if (_jet_selector->apply(*jet))
        {
        }
    }
}
