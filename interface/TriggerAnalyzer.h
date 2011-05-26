// Trigger Analyzer
//
// Apply trigger to events
//
// Created by Samvel Khalatyan, May 26, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_TRIGGER_ANALYZER
#define BSM_TRIGGER_ANALYZER

#include <iosfwd>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "interface/bsm_fwd.h"
#include "interface/Analyzer.h"

namespace bsm
{
    class Trigger;
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
            typedef std::map<std::size_t, std::string> HLTMap;

            typedef std::map<bsm::Trigger, uint32_t> HLTCutflow;

            HLTMap _hlt_map;
            HLTCutflow _hlt_cutflow;
    };

    // Helpers
    //
    bool operator <(const bsm::Trigger &, const bsm::Trigger &);
    std::ostream &operator <<(std::ostream &, const bsm::Trigger &);
}

#endif
