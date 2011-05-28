// Cutflow Analyzer
//
// Generate cutflow table after selector is applied
//
// Created by Samvel Khalatyan, May 18, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CUTFLOW_ANALYZER
#define BSM_CUTFLOW_ANALYZER

#include <iosfwd>
#include <vector>

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

            virtual void onFileOpen(const std::string &filename, const Input *);
            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            virtual operator bool() const;

        private:
            void electrons(const Event *);
            void jets(const Event *);
            void muons(const Event *);

            boost::shared_ptr<selector::MultiplicityCutflow> _pv_multiplicity;

            boost::shared_ptr<selector::JetSelector> _jet_selector;
            boost::shared_ptr<selector::MultiplicityCutflow> _jet_multiplicity;

            boost::shared_ptr<selector::ElectronSelector> _pf_el_selector;
            boost::shared_ptr<selector::MultiplicityCutflow> _pf_el_number_selector;

            boost::shared_ptr<selector::ElectronSelector> _gsf_el_selector;
            boost::shared_ptr<selector::MultiplicityCutflow> _gsf_el_number_selector;

            // Et and Eta cuts only
            //
            boost::shared_ptr<selector::MuonSelector> _pf_mu_selector_step1;
            boost::shared_ptr<selector::MultiplicityCutflow> _pf_mu_number_selector_step1;

            boost::shared_ptr<selector::MuonSelector> _pf_mu_selector;
            boost::shared_ptr<selector::MultiplicityCutflow> _pf_mu_number_selector;

            boost::shared_ptr<selector::MuonSelector> _reco_mu_selector_step1;
            boost::shared_ptr<selector::MultiplicityCutflow> _reco_mu_number_selector_step1;

            boost::shared_ptr<selector::MuonSelector> _reco_mu_selector;
            boost::shared_ptr<selector::MultiplicityCutflow> _reco_mu_number_selector;
    };
}

#endif
