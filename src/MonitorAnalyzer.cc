// Monitoring Analyzer
//
// Generate monitoring plots
//
// Created by Samvel Khalatyan, May 05, 2011
// Copyright 2011, All rights reserved

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
    _monitor_jets.reset(new JetMonitor());
}
catch(const std::bad_alloc &)
{
    cerr << "failed to initialize MonitorAnalyzer - insufficient memory"
        << endl;

    _monitor_jets.reset();
}

MonitorAnalyzer::~MonitorAnalyzer()
{
}

MonitorAnalyzer::AnalyzerPtr MonitorAnalyzer::clone() const
{
    MonitorAnalyzer *analyzer = new MonitorAnalyzer();

    *analyzer->_monitor_jets = *_monitor_jets;

    return AnalyzerPtr(analyzer);
}

void MonitorAnalyzer::merge(const AnalyzerPtr &analyzer)
{
    boost::shared_ptr<MonitorAnalyzer> analyzer_ptr =
        dynamic_pointer_cast<MonitorAnalyzer>(analyzer);

    if (!analyzer_ptr)
        return;

    bsm::merge(*_monitor_jets, *analyzer_ptr->_monitor_jets);
}

void MonitorAnalyzer::process(const Event *event)
{
    _monitor_jets->fill(event->jets());
}

void MonitorAnalyzer::print(std::ostream &out) const
{
    out << "MonitorAnalyzer" << endl;
    out << *_monitor_jets << endl;
}

const MonitorAnalyzer::JetMonitorPtr MonitorAnalyzer::monitorJets() const
{
    return _monitor_jets;
}
