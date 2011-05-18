// Cutflow Analyzer
//
// Generate cutflow table after selector is applied
//
// Created by Samvel Khalatyan, May 18, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CUTFLOW_ANALYZER
#define BSM_CUTFLOW_ANALYZER

#include <boost/shared_ptr.hpp>

#include "interface/bsm_fwd.h"
#include "interface/Analyzer.h"

namespace bsm
{
    class CutflowAnalyzer : public core::Analyzer
    {
        public:
            CutflowAnalyzer();
            virtual ~CutflowAnalyzer();

            // Analyzer interface
            //
            virtual AnalyzerPtr clone() const;

            virtual void merge(const AnalyzerPtr &);

            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            virtual operator bool() const;

        private:
            void electrons(const Event *);
            void muons(const Event *);

            boost::shared_ptr<selector::ElectronSelector> _el_selector;
            boost::shared_ptr<selector::MuonSelector> _mu_selector;
    };
}

#endif
