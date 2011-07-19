// Jet Energy Correction Analyzer
//
// Given Jet uncorrected P4 apply Jet Energy Corrections and
// plot comparison histograms of the corrected in CMSSW P4 and offline
// corrections applied P4
//
// Created by Samvel Khalatyan, Jul 12, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_JET_ENERGY_CORRECTIONS_ANALYZER
#define BSM_JET_ENERGY_CORRECTIONS_ANALYZER

#include <string>
#include <sstream>

#include <boost/shared_ptr.hpp>

#include "JetMETObjects/interface/FactorizedJetCorrector.h"
#include "JetMETObjects/interface/JetCorrectorParameters.h"
#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

class JetCorrectorParameters;

namespace bsm
{
    class JetEnergyCorrectionsAnalyzer : public Analyzer
    {
        public:
            typedef boost::shared_ptr<LorentzVectorMonitor> P4MonitorPtr;

            JetEnergyCorrectionsAnalyzer();
            JetEnergyCorrectionsAnalyzer(const JetEnergyCorrectionsAnalyzer &);

            void loadCorrections(const std::string &);
            bool didCorrectionsLoad() const;

            const P4MonitorPtr jetCmsswCorrectedP4() const;
            const P4MonitorPtr jetUncorrectedP4() const;
            const P4MonitorPtr jetOfflineCorrectedP4() const;

            // Anlayzer interface
            //
            virtual void onFileOpen(const std::string &filename, const Input *);
            virtual void process(const Event *);

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            virtual void merge(const ObjectPtr &);

            virtual void print(std::ostream &) const;

        private:
            void jets(const Event *);

            boost::shared_ptr<JetSelector> _jet_selector;

            P4MonitorPtr _jet_cmssw_corrected_p4;
            P4MonitorPtr _jet_uncorrected_p4;
            P4MonitorPtr _jet_offline_corrected_p4;

            std::vector<JetCorrectorParameters> _parameters;
            boost::shared_ptr<FactorizedJetCorrector> _jec;

            std::ostringstream _out;
    };
}

#endif
