// Synchronization Exercises
//
// Different Analyzers that performan the sinchronization exercises at
// different development stages
//
// Created by Samvel Khalatyan, Jul 05, 2011
// Copyright 2011, All rights reserved

#include "bsm_core/interface/ID.h"
#include "interface/SynchAnalyzer.h"

using bsm::SynchJuly2011Analyzer;

SynchJuly2011Analyzer::SynchJuly2011Analyzer()
{
}

SynchJuly2011Analyzer::SynchJuly2011Analyzer(const SynchJuly2011Analyzer &)
{
}

void SynchJuly2011Analyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void SynchJuly2011Analyzer::process(const Event *event)
{
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
}
