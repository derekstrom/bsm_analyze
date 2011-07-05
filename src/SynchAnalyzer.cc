// Synchronization Exercises
//
// Different Analyzers that performan the sinchronization exercises at
// different development stages
//
// Created by Samvel Khalatyan, Jul 05, 2011
// Copyright 2011, All rights reserved

#include <ostream>

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "interface/Selector.h"
#include "interface/SynchAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::SynchJuly2011Analyzer;

SynchJuly2011Analyzer::SynchJuly2011Analyzer()
{
    _cutflow.reset(new MultiplicityCutflow(4));

    monitor(_cutflow);
}

SynchJuly2011Analyzer::SynchJuly2011Analyzer(const SynchJuly2011Analyzer &object)
{
    _cutflow = 
        dynamic_pointer_cast<MultiplicityCutflow>(object._cutflow->clone());

    monitor(_cutflow);
}

void SynchJuly2011Analyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void SynchJuly2011Analyzer::process(const Event *event)
{
    _cutflow->apply(PRESELECTION);
}

uint32_t SynchJuly2011Analyzer::id() const
{
    return core::ID<SynchJuly2011Analyzer>::get();
}

SynchJuly2011Analyzer::ObjectPtr SynchJuly2011Analyzer::clone() const
{
    return ObjectPtr(new SynchJuly2011Analyzer(*this));
}

void SynchJuly2011Analyzer::print(std::ostream &out) const
{
    out << "Cutflow" << endl;
    out << *_cutflow;
}
