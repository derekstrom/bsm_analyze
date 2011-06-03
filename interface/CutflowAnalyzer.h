// Cutflow Analyzer
//
// Generate cutflow table after selector is applied
//
// Created by Samvel Khalatyan, May 18, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CUTFLOW_ANALYZER
#define BSM_CUTFLOW_ANALYZER

#include <boost/shared_ptr.hpp>

#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

namespace bsm
{
    class CutflowAnalyzer : public Analyzer
    {
        public:
            CutflowAnalyzer();
            CutflowAnalyzer(const CutflowAnalyzer &);

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
            void electrons(const Event *);
            void jets(const Event *);
            void muons(const Event *);

            typedef boost::shared_ptr<MultiplicityCutflow> CutflowPtr;
            typedef boost::shared_ptr<ElectronSelector> ElSelectorPtr;
            typedef boost::shared_ptr<MuonSelector> MuSelectorPtr;

            CutflowPtr _pv_multiplicity;

            boost::shared_ptr<JetSelector> _jet_selector;
            CutflowPtr _jet_multiplicity;

            ElSelectorPtr _pf_el_selector;
            CutflowPtr _pf_el_number_selector;

            ElSelectorPtr _gsf_el_selector;
            CutflowPtr _gsf_el_number_selector;

            // Et and Eta cuts only
            //
            MuSelectorPtr _pf_mu_selector_step1;
            CutflowPtr _pf_mu_number_selector_step1;

            MuSelectorPtr _pf_mu_selector;
            CutflowPtr _pf_mu_number_selector;

            MuSelectorPtr _reco_mu_selector_step1;
            CutflowPtr _reco_mu_number_selector_step1;

            MuSelectorPtr _reco_mu_selector;
            CutflowPtr _reco_mu_number_selector;
    };
}

#endif
