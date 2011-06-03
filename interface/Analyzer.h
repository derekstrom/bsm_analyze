// Analyzer base class
//
// All analyzers should inherit from Analyzer base and implement abstract
// methods.
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_ANALYZER
#define BSM_ANALYZER

#include <string>

#include "bsm_core/interface/Object.h"
#include "bsm_input/interface/bsm_input_fwd.h"

namespace bsm
{
    class Analyzer : public core::Object
    {
        public:
            virtual void onFileOpen(const std::string &, const Input *) = 0;
            virtual void process(const Event *) = 0;
    };
}

#endif
