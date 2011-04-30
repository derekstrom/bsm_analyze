// Analyzer base class
//
// All analyzers should inherit from Analyzer base and implement abstract
// methods.
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CORE_ANALYZER
#define BSM_CORE_ANALYZER

#include <iosfwd>

namespace bsm
{
    class Event;

    namespace core
    {
        class Analyzer
        {
            public:
                Analyzer();
                virtual ~Analyzer();

                virtual void process(const Event *) = 0;

                // Each analyzer should define to print it on screen
                //
                virtual void print(std::ostream &) const = 0;
        };

        // Helpers
        //
        std::ostream &operator <<(std::ostream &, const Analyzer &);
    }
}

#endif
