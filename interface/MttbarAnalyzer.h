// Mttbar Analyzer
//
// Mttbar reconstruction with Wtag
//
// Created by Samvel Khalatyan, Jun 07, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MTTBAR_ANALYZER
#define BSM_MTTBAR_ANALYZER

#include <vector>

#include <boost/shared_ptr.hpp>

#include "bsm_stat/interface/bsm_stat_fwd.h"
#include "bsm_input/interface/bsm_input_fwd.h"
#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

class TLorentzVector;

namespace bsm
{
    class MttbarAnalyzer : public Analyzer
    {
        public:
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            MttbarAnalyzer();
            MttbarAnalyzer(const MttbarAnalyzer &);

            // Getters
            //
            const H1Ptr mttbar() const;

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
            // Prevent copying
            //
            MttbarAnalyzer &operator =(const MttbarAnalyzer &);

            typedef boost::shared_ptr<H1Proxy> H1ProxyPtr;
            typedef std::vector<const Jet *> Jets;
            typedef boost::shared_ptr<LorentzVector> PBP4;

            bool muons(const Event *);
            void electrons(const Event *);
            void jets(const Event *, const Electron *);

            PBP4 leptonicLeg(const Event *, const Electron *, const Jets &);
            PBP4 hadronicLeg(const Event *, const Jet *wjet, const Jets &);

            boost::shared_ptr<ElectronSelector> _el_selector;
            boost::shared_ptr<MultiplicityCutflow> _el_multiplicity;

            boost::shared_ptr<MuonSelector> _mu_selector;
            boost::shared_ptr<MultiplicityCutflow> _mu_multiplicity;

            boost::shared_ptr<MultiplicityCutflow> _leptonic_multiplicity;
            boost::shared_ptr<MultiplicityCutflow> _hadronic_multiplicity;

            boost::shared_ptr<WJetSelector> _wjet_selector;
            boost::shared_ptr<MultiplicityCutflow> _met_solutions;

            boost::shared_ptr<algorithm::NeutrinoReconstruct> _met_reconstructor;

            H1ProxyPtr _mttbar;

            boost::shared_ptr<TLorentzVector> _p4;
    };
}

#endif
