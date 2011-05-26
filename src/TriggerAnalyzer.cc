// Trigger Analyzer
//
// Apply trigger to events
//
// Created by Samvel Khalatyan, May 26, 2011
// Copyright 2011, All rights reserved

#include <iomanip>
#include <ostream>

#include <boost/pointer_cast.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Trigger.pb.h"
#include "interface/TriggerAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::AnalyzerPtr;
using bsm::TriggerAnalyzer;

TriggerAnalyzer::TriggerAnalyzer()
{
}

TriggerAnalyzer::~TriggerAnalyzer()
{
}

AnalyzerPtr TriggerAnalyzer::clone() const
{
    return AnalyzerPtr(new TriggerAnalyzer());
}

void TriggerAnalyzer::merge(const AnalyzerPtr &analyzer_ptr)
{
    boost::shared_ptr<TriggerAnalyzer> analyzer =
        dynamic_pointer_cast<TriggerAnalyzer>(analyzer_ptr);

    if (!analyzer)
        return;
}

void TriggerAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void TriggerAnalyzer::process(const Event *event)
{
    triggers(event);
}

void TriggerAnalyzer::triggers(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Trigger> Triggers;
}

void TriggerAnalyzer::print(std::ostream &out) const
{
}

TriggerAnalyzer::operator bool() const
{
    return true;
}
