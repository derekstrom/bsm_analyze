// ClosestJet Analyzer
//
// Generate monitoring plots for the closest jet to leptons
//
// Created by Samvel Khalatyan, May 09, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CLOSEST_JET_ANALYZER
#define BSM_CLOSEST_JET_ANALYZER

#include <boost/shared_ptr.hpp>

#include "bsm_stat/interface/statfwd.h"
#include "interface/Analyzer.h"
#include "interface/bsmfwd.h"

class TLorentzVector;

namespace bsm
{
    class DeltaMonitor;
    class LorentzVectorMonitor;

    class ClosestJetAnalyzer : public core::Analyzer
    {
        public:
            typedef Analyzer::AnalyzerPtr AnalyzerPtr;

            typedef boost::shared_ptr<DeltaMonitor> DeltaMonitorPtr;
            typedef boost::shared_ptr<LorentzVectorMonitor> 
                LorentzVectorMonitorPtr;
            typedef boost::shared_ptr<stat::H2> H2Ptr;

            ClosestJetAnalyzer();
            virtual ~ClosestJetAnalyzer();

            // Analyzer interface
            //
            virtual AnalyzerPtr clone() const;

            virtual void merge(const AnalyzerPtr &);

            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            virtual operator bool() const;

            // Getters
            //
            const LorentzVectorMonitorPtr monitorElectrons() const;
            const LorentzVectorMonitorPtr monitorElectronJets() const;
            const DeltaMonitorPtr monitorElectronDelta() const;
            const H2Ptr electronDelta() const;

            const LorentzVectorMonitorPtr monitorMuons() const;
            const LorentzVectorMonitorPtr monitorMuonJets() const;
            const DeltaMonitorPtr monitorMuonDelta() const;
            const H2Ptr muonDelta() const;

        private:
            void processElectrons(const Event *event);
            void processMuons(const Event *event);

            // Prevent copying
            //
            ClosestJetAnalyzer &operator =(const ClosestJetAnalyzer &);

            LorentzVectorMonitorPtr _monitor_electrons;
            LorentzVectorMonitorPtr _monitor_electron_jets;
            DeltaMonitorPtr _monitor_electron_delta;
            H2Ptr _electron_delta;

            LorentzVectorMonitorPtr _monitor_muons;
            LorentzVectorMonitorPtr _monitor_muon_jets;
            DeltaMonitorPtr _monitor_muon_delta;
            H2Ptr _muon_delta;

            boost::shared_ptr<algorithm::ClosestJet> _closest_jet_finder;

            typedef boost::shared_ptr<TLorentzVector> P4;

            // Temporary Lorentz Vectors that are used to calculate pTrel
            //
            P4 _p4_1;
            P4 _p4_2;
    };
}

#endif