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
try
{
    _monitor_electrons.reset(new ElectronMonitor());
    _monitor_jets.reset(new JetMonitor());
    _monitor_missing_energy.reset(new MissingEnergyMonitor());
    _monitor_muons.reset(new MuonMonitor());
    _monitor_primary_vertices.reset(new PrimaryVertexMonitor());
}
catch(const std::bad_alloc &)
{
    cerr << "failed to initialize MonitorAnalyzer - insufficient memory"
        << endl;

    _monitor_electrons.reset();
    _monitor_jets.reset();
    _monitor_missing_energy.reset();
    _monitor_muons.reset();
    _monitor_primary_vertices.reset();
}

MonitorAnalyzer::~MonitorAnalyzer()
{
}

MonitorAnalyzer::AnalyzerPtr MonitorAnalyzer::clone() const
{
    MonitorAnalyzer *analyzer = new MonitorAnalyzer();

    if (*this)
    {
        *analyzer->_monitor_electrons = *_monitor_electrons;
        *analyzer->_monitor_jets = *_monitor_jets;
        *analyzer->_monitor_missing_energy = *_monitor_missing_energy;
        *analyzer->_monitor_muons = *_monitor_muons;
        *analyzer->_monitor_primary_vertices = *_monitor_primary_vertices;
    }
    else
    {
        analyzer->_monitor_electrons.reset();
        analyzer->_monitor_jets.reset();
        analyzer->_monitor_missing_energy.reset();
        analyzer->_monitor_muons.reset();
        analyzer->_monitor_primary_vertices.reset();
    }

    return AnalyzerPtr(analyzer);
}

void MonitorAnalyzer::merge(const AnalyzerPtr &analyzer)
{
    boost::shared_ptr<MonitorAnalyzer> analyzer_ptr =
        dynamic_pointer_cast<MonitorAnalyzer>(analyzer);

    if (!analyzer_ptr
            || !*this
            || !*analyzer)
        return;

    bsm::merge(*_monitor_electrons, *analyzer_ptr->_monitor_electrons);
    bsm::merge(*_monitor_jets, *analyzer_ptr->_monitor_jets);
    bsm::merge(*_monitor_missing_energy, *analyzer_ptr->_monitor_missing_energy);
    bsm::merge(*_monitor_muons, *analyzer_ptr->_monitor_muons);
    bsm::merge(*_monitor_primary_vertices, *analyzer_ptr->_monitor_primary_vertices);
}

void MonitorAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void MonitorAnalyzer::process(const Event *event)
{
    // It is assumed that analyzer is in valid state
    //
    _monitor_electrons->fill(event->pf_electrons());
    _monitor_jets->fill(event->jets());
    _monitor_muons->fill(event->pf_muons());
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
    out << *_monitor_electrons << endl;
    out << *_monitor_jets << endl;
    out << *_monitor_missing_energy << endl;
    out << *_monitor_muons << endl;
    out << *_monitor_primary_vertices << endl;
}

MonitorAnalyzer::operator bool() const
{
    return _monitor_electrons
        && _monitor_jets
        && _monitor_missing_energy
        && _monitor_muons
        && _monitor_primary_vertices;
}

const MonitorAnalyzer::ElectronMonitorPtr MonitorAnalyzer::monitorElectrons() const
{
    return _monitor_electrons;
}

const MonitorAnalyzer::JetMonitorPtr MonitorAnalyzer::monitorJets() const
{
    return _monitor_jets;
}

const MonitorAnalyzer::MissingEnergyMonitorPtr MonitorAnalyzer::monitorMissingEnergy() const
{
    return _monitor_missing_energy;
}

const MonitorAnalyzer::MuonMonitorPtr MonitorAnalyzer::monitorMuons() const
{
    return _monitor_muons;
}

const MonitorAnalyzer::PrimaryVertexMonitorPtr MonitorAnalyzer::monitorPrimaryVertices() const
{
    return _monitor_primary_vertices;
}
