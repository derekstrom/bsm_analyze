// Forward declarations
//
// Analysis classes forward declarations
//
// Created by Samvel Khalatyan, May 10, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_FWD
#define BSM_FWD

#include <boost/shared_ptr.hpp>

namespace bsm
{
    namespace core
    {
        class Analyzer;
    }

    typedef boost::shared_ptr<core::Analyzer> AnalyzerPtr;

    namespace algorithm
    {
        class ClosestJet;
    }

    namespace selector
    {
        class Cut;
        class ElectronSelector;
        class MultiplicitySelector;
        class MuonSelector;
    }
}

#endif
