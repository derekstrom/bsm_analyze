// Analyzer base class
//
// All analyzers should inherit from Analyzer base and implement abstract
// methods.
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#include <ostream>

#include "interface/Analyzer.h"

using bsm::core::Analyzer;

Analyzer::Analyzer()
{
}

Analyzer::~Analyzer()
{
}



// Helpers
//
std::ostream &bsm::core::operator <<(std::ostream &out,
                                    const Analyzer &analyzer)
{
    analyzer.print(out);

    return out;
}
