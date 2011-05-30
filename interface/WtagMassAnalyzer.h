// WtagMass Analyzer
//
// Apply Wtag to jets and reconstruct Mttbar
//
// Created by Samvel Khalatyan, May 27, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_WTAGMASS_ANALYZER
#define BSM_WTAGMASS_ANALYZER

#include <iosfwd>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "bsm_stat/interface/bsm_stat_fwd.h"
#include "interface/bsm_fwd.h"
#include "interface/Analyzer.h"

class TLorentzVector;

namespace bsm
{
    class Electron;
    class Jet;
    class LorentzVector;

    class WtagMassAnalyzer : public core::Analyzer
    {
        public:
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            WtagMassAnalyzer();
            virtual ~WtagMassAnalyzer();

            const H1Ptr mttbar() const;

            // Analyzer interface
            //
            virtual AnalyzerPtr clone() const;

            virtual void merge(const AnalyzerPtr &);

            virtual void onFileOpen(const std::string &filename, const Input *);
            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            virtual operator bool() const;

        private:
            typedef std::vector<const Jet *> Jets;
            typedef boost::shared_ptr<LorentzVector> PBP4;

            bool muons(const Event *);
            void electrons(const Event *);
            void jets(const Event *, const Electron *);

            PBP4 leptonicLeg(const Event *, const Electron *, const Jets &);
            PBP4 hadronicLeg(const Event *, const Jet *wjet, const Jets &);

            boost::shared_ptr<selector::ElectronSelector> _el_selector;
            boost::shared_ptr<selector::MultiplicityCutflow> _el_multiplicity;

            boost::shared_ptr<selector::MuonSelector> _mu_selector;
            boost::shared_ptr<selector::MultiplicityCutflow> _mu_multiplicity;

            boost::shared_ptr<selector::MultiplicityCutflow> _leptonic_multiplicity;
            boost::shared_ptr<selector::MultiplicityCutflow> _hadronic_multiplicity;

            boost::shared_ptr<selector::WJetSelector> _wjet_selector;
            boost::shared_ptr<selector::MultiplicityCutflow> _met_solutions;
            boost::shared_ptr<algorithm::MissingEnergyCorrection> _met_corrector;

            H1Ptr _mttbar;

            boost::shared_ptr<TLorentzVector> _p4;
    };
}

#endif
