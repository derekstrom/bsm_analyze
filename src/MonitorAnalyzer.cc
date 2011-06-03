// Monitoring Analyzer
//
// Generate monitoring plots
//
// Created by Samvel Khalatyan, May 05, 2011
// Copyright 2011, All rights reserved

#include <iostream>
#include <ostream>
#include <stdexcept>

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Event.pb.h"
#include "interface/Monitor.h"
#include "interface/MonitorAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::MonitorAnalyzer;

MonitorAnalyzer::MonitorAnalyzer()
{
    _pf_electrons.reset(new ElectronsMonitor());
    _gsf_electrons.reset(new ElectronsMonitor());

    _pf_muons.reset(new MuonsMonitor());
    _reco_muons.reset(new MuonsMonitor());

    _jets.reset(new JetsMonitor());
    _missing_energy.reset(new MissingEnergyMonitor());
    _primary_vertices.reset(new PrimaryVerticesMonitor());

    monitor(_pf_electrons);
    monitor(_gsf_electrons);

    monitor(_pf_muons);
    monitor(_reco_muons);

    monitor(_jets);
    monitor(_missing_energy);
    monitor(_primary_vertices);
}

MonitorAnalyzer::MonitorAnalyzer(const MonitorAnalyzer &object)
{
    _pf_electrons =
        dynamic_pointer_cast<ElectronsMonitor>(object._pf_electrons->clone());
    _gsf_electrons =
        dynamic_pointer_cast<ElectronsMonitor>(object._gsf_electrons->clone());

    _pf_muons =
        dynamic_pointer_cast<MuonsMonitor>(object._pf_muons->clone());
    _reco_muons =
        dynamic_pointer_cast<MuonsMonitor>(object._reco_muons->clone());

    _jets = dynamic_pointer_cast<JetsMonitor>(object._jets->clone());
    _missing_energy =
        dynamic_pointer_cast<MissingEnergyMonitor>(
                object._missing_energy->clone());

    _primary_vertices =
        dynamic_pointer_cast<PrimaryVerticesMonitor>(
                object._primary_vertices->clone());

    monitor(_pf_electrons);
    monitor(_gsf_electrons);

    monitor(_pf_muons);
    monitor(_reco_muons);

    monitor(_jets);
    monitor(_missing_energy);
    monitor(_primary_vertices);
}

const MonitorAnalyzer::ElMonitorPtr MonitorAnalyzer::pfElectrons() const
{
    return _pf_electrons;
}

const MonitorAnalyzer::ElMonitorPtr MonitorAnalyzer::gsfElectrons() const
{
    return _gsf_electrons;
}

const MonitorAnalyzer::MuMonitorPtr MonitorAnalyzer::pfMuons() const
{
    return _pf_muons;
}

const MonitorAnalyzer::MuMonitorPtr MonitorAnalyzer::recoMuons() const
{
    return _reco_muons;
}

const MonitorAnalyzer::JetMonitorPtr MonitorAnalyzer::jets() const
{
    return _jets;
}

const MonitorAnalyzer::METMonitorPtr MonitorAnalyzer::missingEnergy() const
{
    return _missing_energy;
}

const MonitorAnalyzer::PVMonitorPtr MonitorAnalyzer::primaryVertices() const
{
    return _primary_vertices;
}

void MonitorAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void MonitorAnalyzer::process(const Event *event)
{
    _pf_electrons->fill(event->pf_electrons());
    _gsf_electrons->fill(event->gsf_electrons());

    _pf_muons->fill(event->pf_muons());
    _reco_muons->fill(event->reco_muons());

    _jets->fill(event->jets());
    _primary_vertices->fill(event->primary_vertices());

    if (event->has_missing_energy())
        _missing_energy->fill(event->missing_energy());
}

uint32_t MonitorAnalyzer::id() const
{
    return core::ID<MonitorAnalyzer>::get();
}

MonitorAnalyzer::ObjectPtr MonitorAnalyzer::clone() const
{
    return ObjectPtr(new MonitorAnalyzer(*this));
}

void MonitorAnalyzer::print(std::ostream &out) const
{
    out << "Monitor Analyzer" << endl;
    out << "Particle Flow Electrons" << endl;
    out << *_pf_electrons << endl;
    out << endl;

    out << "GSF Electrons" << endl;
    out << *_gsf_electrons << endl;
    out << endl;

    out << "Particle Flow Muons" << endl;
    out << *_pf_muons << endl;
    out << endl;

    out << "Reco Muons" << endl;
    out << *_reco_muons << endl;
    out << endl;

    out << "Jets" << endl;
    out << *_jets << endl;
    out << endl;

    out << "Missing Energy" << endl;
    out << *_missing_energy << endl;
    out << endl;

    out << "Primary Vertices" << endl;
    out << *_primary_vertices;
}
