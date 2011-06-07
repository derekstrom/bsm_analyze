// Filter Events
//
// Apply selectors and filter event objects
//
// Created by Samvel Khalatyan, May 20, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_FILTER_ANALYZER
#define BSM_FILTER_ANALYZER

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/bsm_input_fwd.h"
#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

namespace bsm
{
    class FilterAnalyzer : public Analyzer
    {
        public:
            FilterAnalyzer();
            FilterAnalyzer(const FilterAnalyzer &);

            // Analyzer interface
            //
            virtual void onFileOpen(const std::string &filename, const Input *);
            virtual void process(const Event *);

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            void primaryVertices(const Event *);
            void electrons(const Event *);
            void muons(const Event *);
            void missing_energy(const Event *);

            boost::shared_ptr<Writer> _writer;

            boost::shared_ptr<Event> _event;

            boost::shared_ptr<ElectronSelector> _pf_el_selector;
            boost::shared_ptr<ElectronSelector> _gsf_el_selector;

            boost::shared_ptr<MuonSelector> _pf_mu_selector;
            boost::shared_ptr<MuonSelector> _reco_mu_selector;
    };
}

#endif
