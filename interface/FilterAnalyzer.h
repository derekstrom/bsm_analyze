// Filter Events
//
// Apply selectors and filter event objects
//
// Created by Samvel Khalatyan, May 20, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_FILTER_ANALYZER
#define BSM_FILTER_ANALYZER

#include <iosfwd>

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/bsm_input_fwd.h"
#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

namespace bsm
{
    class FilterAnalyzer : public core::Analyzer
    {
        public:
            FilterAnalyzer();
            virtual ~FilterAnalyzer();

            // Analyzer interface
            //
            virtual AnalyzerPtr clone() const;

            virtual void merge(const AnalyzerPtr &);

            virtual void onFileOpen(const std::string &filename, const Input *);

            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            virtual operator bool() const;

        private:
            void primaryVertices(const Event *);
            void electrons(const Event *);
            void muons(const Event *);

            boost::shared_ptr<Writer> _writer;

            boost::shared_ptr<Event> _event;

            boost::shared_ptr<selector::ElectronSelector> _pf_el_selector;
            boost::shared_ptr<selector::ElectronSelector> _gsf_el_selector;

            boost::shared_ptr<selector::MuonSelector> _pf_mu_selector;
            boost::shared_ptr<selector::MuonSelector> _reco_mu_selector;
    };
}

#endif
