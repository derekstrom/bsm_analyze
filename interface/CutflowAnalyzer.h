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
    class MultiplicityCutflow;

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
            void muons(const Event *);

            boost::shared_ptr<MultiplicityCutflow> _pv_multiplicity;

            boost::shared_ptr<selector::ElectronSelector> _pf_el_selector;
            boost::shared_ptr<MultiplicityCutflow> _pf_el_number_selector;

            boost::shared_ptr<selector::ElectronSelector> _gsf_el_selector;
            boost::shared_ptr<MultiplicityCutflow> _gsf_el_number_selector;

            // Et and Eta cuts only
            //
            boost::shared_ptr<selector::MuonSelector> _pf_mu_selector_step1;
            boost::shared_ptr<MultiplicityCutflow> _pf_mu_number_selector_step1;

            boost::shared_ptr<selector::MuonSelector> _pf_mu_selector;
            boost::shared_ptr<MultiplicityCutflow> _pf_mu_number_selector;

            boost::shared_ptr<selector::MuonSelector> _reco_mu_selector_step1;
            boost::shared_ptr<MultiplicityCutflow> _reco_mu_number_selector_step1;

            boost::shared_ptr<selector::MuonSelector> _reco_mu_selector;
            boost::shared_ptr<MultiplicityCutflow> _reco_mu_number_selector;
    };

    class MultiplicityCutflow
    {
        public:
            typedef boost::shared_ptr<MultiplicityCutflow> CutflowPtr;
            typedef boost::shared_ptr<selector::Cut> CutPtr;

            MultiplicityCutflow(const uint32_t &max);
            virtual ~MultiplicityCutflow();

            // Test if selector is passed
            //
            virtual void operator()(const uint32_t &);

            // Cuts accessors
            //
            CutPtr cut(const uint32_t &) const;

            // Cutflow interface
            //
            virtual void print(std::ostream &) const;

            virtual CutflowPtr clone() const;
            virtual void merge(const CutflowPtr &);

        private:
            typedef std::vector<CutPtr> Cuts;

            Cuts _cuts;
    };

    std::ostream &operator <<(std::ostream &, const MultiplicityCutflow &);
}

#endif
