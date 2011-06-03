// ClosestJet Analyzer
//
// Generate monitoring plots for the closest jet to leptons
//
// Created by Samvel Khalatyan, May 09, 2011
// Copyright 2011, All rights reserved

#include <ostream>
#include <stdexcept>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "interface/Algorithm.h"
#include "interface/ClosestJetAnalyzer.h"
#include "interface/Monitor.h"

using std::endl;

using bsm::ClosestJetAnalyzer;

ClosestJetAnalyzer::ClosestJetAnalyzer()
{
    _monitor_electrons.reset(new LorentzVectorMonitor());
    _monitor_electron_jets.reset(new LorentzVectorMonitor());
    _monitor_electron_delta.reset(new DeltaMonitor());

    _monitor_muons.reset(new LorentzVectorMonitor());
    _monitor_muon_jets.reset(new LorentzVectorMonitor());
    _monitor_muon_delta.reset(new DeltaMonitor());

    monitor(_monitor_electrons);
    monitor(_monitor_electron_jets);
    monitor(_monitor_electron_delta);

    monitor(_monitor_muons);
    monitor(_monitor_muon_jets);
    monitor(_monitor_muon_delta);

    _closest_jet_finder.reset(new algorithm::ClosestJet());
}

ClosestJetAnalyzer::ClosestJetAnalyzer(const ClosestJetAnalyzer &object)
{
    _monitor_electrons.reset(
            new LorentzVectorMonitor(*object._monitor_electrons));
    _monitor_electron_jets.reset(
            new LorentzVectorMonitor(*object._monitor_electron_jets));
    _monitor_electron_delta.reset(
            new DeltaMonitor(*object._monitor_electron_delta));

    _monitor_muons.reset(
            new LorentzVectorMonitor(*object._monitor_muons));
    _monitor_muon_jets.reset(
            new LorentzVectorMonitor(*object._monitor_muon_jets));
    _monitor_muon_delta.reset(
            new DeltaMonitor(*object._monitor_muon_delta));

    monitor(_monitor_electrons);
    monitor(_monitor_electron_jets);
    monitor(_monitor_electron_delta);

    monitor(_monitor_muons);
    monitor(_monitor_muon_jets);
    monitor(_monitor_muon_delta);

    _closest_jet_finder.reset(new algorithm::ClosestJet());
}

const ClosestJetAnalyzer::P4MonitorPtr
    ClosestJetAnalyzer::monitorElectrons() const
{
    return _monitor_electrons;
}

const ClosestJetAnalyzer::P4MonitorPtr
    ClosestJetAnalyzer::monitorElectronJets() const
{
    return _monitor_electron_jets;
}

const ClosestJetAnalyzer::DeltaMonitorPtr
    ClosestJetAnalyzer::monitorElectronDelta() const
{
    return _monitor_electron_delta;
}

const ClosestJetAnalyzer::P4MonitorPtr
    ClosestJetAnalyzer::monitorMuons() const
{
    return _monitor_muons;
}

const ClosestJetAnalyzer::P4MonitorPtr
    ClosestJetAnalyzer::monitorMuonJets() const
{
    return _monitor_muon_jets;
}

const ClosestJetAnalyzer::DeltaMonitorPtr
    ClosestJetAnalyzer::monitorMuonDelta() const
{
    return _monitor_muon_delta;
}

// Analyzer interface
//
void ClosestJetAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void ClosestJetAnalyzer::process(const Event *event)
{
    if (event->pf_electrons().size())
        processElectrons(event);

    if (event->pf_muons().size())
        processMuons(event);
}

// Object interface
//
uint32_t ClosestJetAnalyzer::id() const
{
    return core::ID<ClosestJetAnalyzer>::get();
}

ClosestJetAnalyzer::ObjectPtr ClosestJetAnalyzer::clone() const
{
    return ObjectPtr(new ClosestJetAnalyzer(*this));
}

void ClosestJetAnalyzer::print(std::ostream &out) const
{
    out << "ClosestJet Analyzer" << endl;
    out << *_monitor_electrons << endl;
    out << *_monitor_electron_jets << endl;
    out << *_monitor_electron_delta << endl;

    out << *_monitor_muons << endl;
    out << *_monitor_muon_jets << endl;
    out << *_monitor_muon_delta;
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
        const Jet *jet = _closest_jet_finder->find(event->jets(), *electron);

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
        const Jet *jet = _closest_jet_finder->find(event->jets(), *muon);

        if (!jet)
            continue;

        _monitor_muons->fill(muon->physics_object().p4());
        _monitor_muon_jets->fill(jet->physics_object().p4());
        _monitor_muon_delta->fill(jet->physics_object().p4(),
                muon->physics_object().p4());
    }
}
