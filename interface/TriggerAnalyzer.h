// Trigger Analyzer
//
// Apply trigger to events
//
// Created by Samvel Khalatyan, May 26, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_TRIGGER_ANALYZER
#define BSM_TRIGGER_ANALYZER

#include <iosfwd>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "interface/bsm_fwd.h"
#include "interface/Analyzer.h"

namespace bsm
{
    class TriggerAnalyzer : public core::Analyzer
    {
        public:
            TriggerAnalyzer();
            virtual ~TriggerAnalyzer();

            // Analyzer interface
            //
            virtual AnalyzerPtr clone() const;

            virtual void merge(const AnalyzerPtr &);

            virtual void onFileOpen(const std::string &filename, const Input *);
            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            virtual operator bool() const;

        private:
            void triggers(const Event *);
    };
}

#endif
