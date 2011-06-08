// Mttbar Analyzer
//
// Mttbar reconstruction with Wtag
//
// Created by Samvel Khalatyan, Jun 07, 2011
// Copyright 2011, All rights reserved

#include <iomanip>
#include <iostream>
#include <ostream>

#include <boost/pointer_cast.hpp>

#include <TLorentzVector.h>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Jet.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_stat/interface/H1.h"
#include "interface/Algorithm.h"
#include "interface/Monitor.h"
#include "interface/Selector.h"
#include "interface/StatProxy.h"
#include "interface/Utility.h"
#include "interface/MttbarAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::MttbarAnalyzer;

using bsm::stat::H1;

MttbarAnalyzer::MttbarAnalyzer()
{
    _el_selector.reset(new ElectronSelector());
    _el_multiplicity.reset(new MultiplicityCutflow(4));
    _el_monitor.reset(new LorentzVectorMonitor());

    _mu_selector.reset(new MuonSelector());
    _mu_multiplicity.reset(new MultiplicityCutflow(4));

    _wjet_selector.reset(new WJetSelector());
    _wjet_monitor.reset(new LorentzVectorMonitor());

    _mttbar.reset(new H1Proxy(25, 500, 3000));

    monitor(_el_selector);
    monitor(_el_multiplicity);
    monitor(_el_monitor);

    monitor(_mu_selector);
    monitor(_mu_multiplicity);

    monitor(_wjet_selector);
    monitor(_wjet_monitor);

    monitor(_mttbar);
}

MttbarAnalyzer::MttbarAnalyzer(const MttbarAnalyzer &object)
{
    _el_selector =
        dynamic_pointer_cast<ElectronSelector>(object._el_selector->clone());
    _el_multiplicity =
        dynamic_pointer_cast<MultiplicityCutflow>(object._el_multiplicity->clone());
    _el_monitor =
        dynamic_pointer_cast<LorentzVectorMonitor>(object._el_monitor->clone());

    _mu_selector =
        dynamic_pointer_cast<MuonSelector>(object._mu_selector->clone());
    _mu_multiplicity =
        dynamic_pointer_cast<MultiplicityCutflow>(object._mu_multiplicity->clone());

    _wjet_selector =
        dynamic_pointer_cast<WJetSelector>(object._wjet_selector->clone());
    _wjet_monitor =
        dynamic_pointer_cast<LorentzVectorMonitor>(object._wjet_monitor->clone());

    _mttbar = dynamic_pointer_cast<H1Proxy>(object._mttbar->clone());

    monitor(_el_selector);
    monitor(_el_multiplicity);
    monitor(_el_monitor);

    monitor(_mu_selector);
    monitor(_mu_multiplicity);

    monitor(_wjet_selector);
    monitor(_wjet_monitor);

    monitor(_mttbar);
}

const MttbarAnalyzer::H1Ptr MttbarAnalyzer::mttbar() const
{
    return _mttbar->histogram();
}

const MttbarAnalyzer::P4MonitorPtr MttbarAnalyzer::electronMonitor() const
{
    return _el_monitor;
}

const MttbarAnalyzer::P4MonitorPtr MttbarAnalyzer::wjetMonitor() const
{
    return _wjet_monitor;
}

void MttbarAnalyzer::onFileOpen(const std::string &filename, const Input *input)
{
}

void MttbarAnalyzer::process(const Event *event)
{
    if (!event->primary_vertices().size()
            || !event->has_missing_energy())
        return;

    if (muons(event))
        return;

    electrons(event);
}

uint32_t MttbarAnalyzer::id() const
{
    return core::ID<MttbarAnalyzer>::get();
}

MttbarAnalyzer::ObjectPtr MttbarAnalyzer::clone() const
{
    return ObjectPtr(new MttbarAnalyzer(*this));
}

void MttbarAnalyzer::print(std::ostream &out) const
{
    out << "PF Muons" << endl;
    out << *_mu_selector << endl;
    out << endl;

    out << "PF Muons Multiplicity" << endl;
    out << *_mu_multiplicity << endl;
    out << endl;

    out << "PF Electrons" << endl;
    out << *_el_selector << endl;
    out << endl;

    out << "PF Electrons Multiplcitiy" << endl;
    out << *_el_multiplicity << endl;
    out << endl;

    out << "PF Electron P4" << endl;
    out << *_el_monitor << endl;
    out << endl;

    out << "WJets" << endl;
    out << *_wjet_selector << endl;
    out << endl;

    out << "WJet monitor" << endl;
    out << *_wjet_monitor << endl;
    out << endl;

    out << "Mttbar" << endl;
    out << *mttbar() << endl;
}

// Privates
//
bool MttbarAnalyzer::muons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    uint32_t good_muons = 0;

    const PrimaryVertex &pv = event->primary_vertices().Get(0);

    LockSelectorEventCounterOnUpdate lock(*_mu_selector);
    for(Muons::const_iterator muon = event->pf_muons().begin();
            event->pf_muons().end() != muon;
            ++muon)
    {
        if (_mu_selector->apply(*muon, pv))
            ++good_muons;
    }

    _mu_multiplicity->apply(good_muons);

    return 0 < good_muons;
}

void MttbarAnalyzer::electrons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const PrimaryVertex &pv = event->primary_vertices().Get(0);

    LockSelectorEventCounterOnUpdate lock(*_el_selector);
    const Electron *electron = 0;
    uint32_t good_electrons = 0;
    for(Electrons::const_iterator el = event->pf_electrons().begin();
            event->pf_electrons().end() != el;
            ++el)
    {
        if (_el_selector->apply(*el, pv))
        {
            ++good_electrons;

            electron = &*el;
        }
    }

    _el_multiplicity->apply(good_electrons);

    if (1 != good_electrons)
        return;

    // Continue Processing
    //
    jets(event, electron);
}

void MttbarAnalyzer::jets(const Event *event, const Electron *electron)
{
    _el_monitor->fill(electron->physics_object().p4());

    if (3 > event->jets().size())
        return;

    typedef TTbarDeltaRReconstruct::Jets JetPtrs;

    JetPtrs jets;

    const Jet *wjet = 0;
    uint32_t wjets = 0;

    LockSelectorEventCounterOnUpdate lock(*_wjet_selector);
    // Take all permutations of jets and select the one that has minimum
    // leptonic decay DR and hadronic decay DR
    //
    typedef ::google::protobuf::RepeatedPtrField<Jet> PBJets;
    for(PBJets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet;
            ++jet)
    {
        if (!_wjet_selector->apply(*jet))
        {
            jets.push_back(&*jet);

            continue;
        }

        wjet = &*jet;
        ++wjets;
    }

    if (1 != wjets)
        return;

    _wjet_monitor->fill(wjet->physics_object().p4());

    if (2 > jets.size())
        return;

    /*
    TTbarDeltaReconstruct ttbar;

    ttbar.apply(jets, electron->physics_object().p4(), event->missing_energy().p4(), wjet);
    */
}
