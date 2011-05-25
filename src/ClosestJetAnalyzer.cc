// ClosestJet Analyzer
//
// Generate monitoring plots for the closest jet to leptons
//
// Created by Samvel Khalatyan, May 09, 2011
// Copyright 2011, All rights reserved

#include <iostream>
#include <ostream>
#include <stdexcept>

#include <boost/pointer_cast.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "bsm_stat/interface/H2.h"
#include "interface/Algorithm.h"
#include "interface/Monitor.h"
#include "interface/ClosestJetAnalyzer.h"
#include "interface/Utility.h"

using std::cerr;
using std::endl;

using boost::dynamic_pointer_cast;

using bsm::ClosestJetAnalyzer;
using bsm::stat::H1;
using bsm::stat::H2;

ClosestJetAnalyzer::ClosestJetAnalyzer()
try
{
    _monitor_electrons.reset(new LorentzVectorMonitor());
    _monitor_electron_jets.reset(new LorentzVectorMonitor());
    _monitor_electron_delta.reset(new DeltaMonitor());

    _monitor_muons.reset(new LorentzVectorMonitor());
    _monitor_muon_jets.reset(new LorentzVectorMonitor());
    _monitor_muon_delta.reset(new DeltaMonitor());

    _closest_jet_finder.reset(new algorithm::ClosestJet());
}
catch(const std::bad_alloc &)
{
    cerr << "failed to initialize ClosestJetAnalyzer - insufficient memory"
        << endl;

    _monitor_electrons.reset();
    _monitor_electron_jets.reset();
    _monitor_electron_delta.reset();

    _monitor_muons.reset();
    _monitor_muon_jets.reset();
    _monitor_muon_delta.reset();

    _closest_jet_finder.reset();
}

ClosestJetAnalyzer::~ClosestJetAnalyzer()
{
}

ClosestJetAnalyzer::AnalyzerPtr ClosestJetAnalyzer::clone() const
{
    ClosestJetAnalyzer *analyzer = new ClosestJetAnalyzer();

    if (*this)
    {
        *analyzer->_monitor_electrons = *_monitor_electrons;
        *analyzer->_monitor_electron_jets = *_monitor_electron_jets;
        *analyzer->_monitor_electron_delta = *_monitor_electron_delta;

        *analyzer->_monitor_muons = *_monitor_muons;
        *analyzer->_monitor_muon_jets = *_monitor_muon_jets;
        *analyzer->_monitor_muon_delta = *_monitor_muon_delta;

        *analyzer->_closest_jet_finder = *_closest_jet_finder;
    }
    else
    {
        analyzer->_monitor_electrons.reset();
        analyzer->_monitor_electron_jets.reset();
        analyzer->_monitor_electron_delta.reset();

        analyzer->_monitor_muons.reset();
        analyzer->_monitor_muon_jets.reset();
        analyzer->_monitor_muon_delta.reset();

        analyzer->_closest_jet_finder.reset();
    }

    return AnalyzerPtr(analyzer);
}

void ClosestJetAnalyzer::merge(const AnalyzerPtr &analyzer)
{
    boost::shared_ptr<ClosestJetAnalyzer> analyzer_ptr =
        dynamic_pointer_cast<ClosestJetAnalyzer>(analyzer);

    if (!analyzer_ptr
            || !*this
            || !*analyzer)
        return;

    bsm::merge(*_monitor_electrons, *analyzer_ptr->_monitor_electrons);
    bsm::merge(*_monitor_electron_jets, *analyzer_ptr->_monitor_electron_jets);
    bsm::merge(*_monitor_electron_delta, *analyzer_ptr->_monitor_electron_delta);
    
    bsm::merge(*_monitor_muons, *analyzer_ptr->_monitor_muons);
    bsm::merge(*_monitor_muon_jets, *analyzer_ptr->_monitor_muon_jets);
    bsm::merge(*_monitor_muon_delta, *analyzer_ptr->_monitor_muon_delta);
}

void ClosestJetAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void ClosestJetAnalyzer::process(const Event *event)
{
    // It is assumed that analyzer is in valid state
    //
    if (event->pf_electrons().size())
        processElectrons(event);

    if (event->pf_muons().size())
        processMuons(event);
}

void ClosestJetAnalyzer::print(std::ostream &out) const
{
    if (!*this)
    {
        out << "ClosestJet Analyzer is invalid" << endl;

        return;
    }

    out << "ClosestJet Analyzer" << endl;
    out << *_monitor_electrons << endl;
    out << *_monitor_electron_jets << endl;
    out << *_monitor_electron_delta << endl;

    out << *_monitor_muons << endl;
    out << *_monitor_muon_jets << endl;
    out << *_monitor_muon_delta << endl;
}

ClosestJetAnalyzer::operator bool() const
{
    return _monitor_electrons
        && _monitor_electron_jets
        && _monitor_electron_delta

        && _monitor_muons
        && _monitor_muon_jets
        && _monitor_muon_delta

        && _closest_jet_finder;
}

const ClosestJetAnalyzer::LorentzVectorMonitorPtr
    ClosestJetAnalyzer::monitorElectrons() const
{
    return _monitor_electrons;
}

const ClosestJetAnalyzer::LorentzVectorMonitorPtr
    ClosestJetAnalyzer::monitorElectronJets() const
{
    return _monitor_electron_jets;
}

const ClosestJetAnalyzer::DeltaMonitorPtr
    ClosestJetAnalyzer::monitorElectronDelta() const
{
    return _monitor_electron_delta;
}

const ClosestJetAnalyzer::LorentzVectorMonitorPtr
    ClosestJetAnalyzer::monitorMuons() const
{
    return _monitor_muons;
}

const ClosestJetAnalyzer::LorentzVectorMonitorPtr
    ClosestJetAnalyzer::monitorMuonJets() const
{
    return _monitor_muon_jets;
}

const ClosestJetAnalyzer::DeltaMonitorPtr
    ClosestJetAnalyzer::monitorMuonDelta() const
{
    return _monitor_muon_delta;
}

// Privates
//
void ClosestJetAnalyzer::processElectrons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const Electrons &electrons = event->pf_electrons();
    for(Electrons::const_iterator electron = electrons.begin();
            electrons.end() != electron;
            ++electron)
    {
        const Jet *jet = _closest_jet_finder->operator()(event->jets(), *electron);

        if (!jet)
            continue;

        _monitor_electrons->fill(electron->physics_object().p4());
        _monitor_electron_jets->fill(jet->physics_object().p4());
        _monitor_electron_delta->fill(jet->physics_object().p4(),
                electron->physics_object().p4());
    }
}

void ClosestJetAnalyzer::processMuons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    const Muons &muons = event->pf_muons();
    for(Muons::const_iterator muon = muons.begin();
            muons.end() != muon;
            ++muon)
    {
        const Jet *jet = _closest_jet_finder->operator()(event->jets(), *muon);

        if (!jet)
            continue;

        _monitor_muons->fill(muon->physics_object().p4());
        _monitor_muon_jets->fill(jet->physics_object().p4());
        _monitor_muon_delta->fill(jet->physics_object().p4(),
                muon->physics_object().p4());
    }
}
