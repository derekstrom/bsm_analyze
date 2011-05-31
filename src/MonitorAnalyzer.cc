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

#include "bsm_input/interface/Event.pb.h"
#include "interface/Monitor.h"
#include "interface/MonitorAnalyzer.h"

using std::cerr;
using std::endl;

using boost::dynamic_pointer_cast;

using bsm::MonitorAnalyzer;

MonitorAnalyzer::MonitorAnalyzer()
{
    _monitor_pf_electrons.reset(new ElectronMonitor());
    _monitor_gsf_electrons.reset(new ElectronMonitor());

    _monitor_pf_muons.reset(new MuonMonitor());
    _monitor_reco_muons.reset(new MuonMonitor());

    _monitor_jets.reset(new JetMonitor());
    _monitor_missing_energy.reset(new MissingEnergyMonitor());
    _monitor_primary_vertices.reset(new PrimaryVertexMonitor());
}

MonitorAnalyzer::~MonitorAnalyzer()
{
}

MonitorAnalyzer::AnalyzerPtr MonitorAnalyzer::clone() const
{
    return AnalyzerPtr(new MonitorAnalyzer());
}

void MonitorAnalyzer::merge(const AnalyzerPtr &analyzer)
{
    boost::shared_ptr<MonitorAnalyzer> analyzer_ptr =
        dynamic_pointer_cast<MonitorAnalyzer>(analyzer);

    if (!analyzer_ptr
            || !*this
            || !*analyzer)
        return;

    bsm::merge(*_monitor_pf_electrons, *analyzer_ptr->_monitor_pf_electrons);
    bsm::merge(*_monitor_gsf_electrons, *analyzer_ptr->_monitor_gsf_electrons);

    bsm::merge(*_monitor_pf_muons, *analyzer_ptr->_monitor_pf_muons);
    bsm::merge(*_monitor_reco_muons, *analyzer_ptr->_monitor_reco_muons);

    bsm::merge(*_monitor_jets, *analyzer_ptr->_monitor_jets);
    bsm::merge(*_monitor_missing_energy, *analyzer_ptr->_monitor_missing_energy);
    bsm::merge(*_monitor_primary_vertices, *analyzer_ptr->_monitor_primary_vertices);
}

void MonitorAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void MonitorAnalyzer::process(const Event *event)
{
    // It is assumed that analyzer is in valid state
    //
    _monitor_pf_electrons->fill(event->pf_electrons());
    _monitor_gsf_electrons->fill(event->gsf_electrons());

    _monitor_pf_muons->fill(event->pf_muons());
    _monitor_reco_muons->fill(event->reco_muons());

    _monitor_jets->fill(event->jets());
    _monitor_primary_vertices->fill(event->primary_vertices());

    if (event->has_missing_energy())
        _monitor_missing_energy->fill(event->missing_energy());
}

void MonitorAnalyzer::print(std::ostream &out) const
{
    if (!*this)
    {
        out << "Monitor Analyzer is invalid" << endl;

        return;
    }

    out << "Monitor Analyzer" << endl;
    out << "Particle Flow Electrons" << endl;
    out << *_monitor_pf_electrons << endl;
    out << "GSF Electrons" << endl;
    out << *_monitor_gsf_electrons << endl;
    out << endl;

    out << "Particle Flow Muons" << endl;
    out << *_monitor_pf_muons << endl;
    out << "Reco Muons" << endl;
    out << *_monitor_reco_muons << endl;
    out << endl;

    out << *_monitor_jets << endl;
    out << *_monitor_missing_energy << endl;
    out << *_monitor_primary_vertices << endl;
}

MonitorAnalyzer::operator bool() const
{
    return _monitor_pf_electrons
        && _monitor_gsf_electrons
        && _monitor_pf_muons
        && _monitor_reco_muons
        && _monitor_jets
        && _monitor_missing_energy
        && _monitor_primary_vertices;
}

const MonitorAnalyzer::ElectronMonitorPtr
    MonitorAnalyzer::monitorPFElectrons() const
{
    return _monitor_pf_electrons;
}

const MonitorAnalyzer::ElectronMonitorPtr
    MonitorAnalyzer::monitorGSFElectrons() const
{
    return _monitor_gsf_electrons;
}

const MonitorAnalyzer::MuonMonitorPtr MonitorAnalyzer::monitorPFMuons() const
{
    return _monitor_pf_muons;
}

const MonitorAnalyzer::MuonMonitorPtr MonitorAnalyzer::monitorRecoMuons() const
{
    return _monitor_reco_muons;
}

const MonitorAnalyzer::JetMonitorPtr MonitorAnalyzer::monitorJets() const
{
    return _monitor_jets;
}

const MonitorAnalyzer::MissingEnergyMonitorPtr MonitorAnalyzer::monitorMissingEnergy() const
{
    return _monitor_missing_energy;
}

const MonitorAnalyzer::PrimaryVertexMonitorPtr MonitorAnalyzer::monitorPrimaryVertices() const
{
    return _monitor_primary_vertices;
}
