// Analyzer base class
//
// All analyzers should inherit from Analyzer base and implement abstract
// methods.
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CORE_ANALYZER
#define BSM_CORE_ANALYZER

#include <boost/shared_ptr.hpp>

#include <iosfwd>

namespace bsm
{
    class Event;

    namespace core
    {
        class Analyzer
        {
            public:
                typedef boost::shared_ptr<Analyzer> AnalyzerPtr;

                Analyzer();
                virtual ~Analyzer();

                // Custom analyzers know how to clone themself
                //
                virtual AnalyzerPtr clone() const = 0;

                virtual void process(const Event *) = 0;

                // Each analyzer should know how to print itself on screen
                //
                virtual void print(std::ostream &) const = 0;

            private:
                // Prevent Copying: clone() should be used instead
                //
                Analyzer(const Analyzer &);
                Analyzer &operator =(const Analyzer &);
        };

        // Helpers
        //
        std::ostream &operator <<(std::ostream &, const Analyzer &);
    }
}

#endif
