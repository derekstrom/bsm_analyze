// ClosestJet Analyzer
//
// Generate monitoring plots for the closest jet to leptons
//
// Created by Samvel Khalatyan, May 09, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CLOSEST_JET_ANALYZER
#define BSM_CLOSEST_JET_ANALYZER

#include <boost/shared_ptr.hpp>

#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

namespace bsm
{
    class ClosestJetAnalyzer : public Analyzer
    {
        public:
            typedef boost::shared_ptr<DeltaMonitor> DeltaMonitorPtr;
            typedef boost::shared_ptr<LorentzVectorMonitor> P4MonitorPtr;

            ClosestJetAnalyzer();
            ClosestJetAnalyzer(const ClosestJetAnalyzer &);

            // Getters
            //
            const P4MonitorPtr monitorElectrons() const;
            const P4MonitorPtr monitorElectronJets() const;
            const DeltaMonitorPtr monitorElectronDelta() const;

            const P4MonitorPtr monitorMuons() const;
            const P4MonitorPtr monitorMuonJets() const;
            const DeltaMonitorPtr monitorMuonDelta() const;

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
            void processElectrons(const Event *event);
            void processMuons(const Event *event);

            // Prevent copying
            //
            ClosestJetAnalyzer &operator =(const ClosestJetAnalyzer &);

            P4MonitorPtr _monitor_electrons;
            P4MonitorPtr _monitor_electron_jets;
            DeltaMonitorPtr _monitor_electron_delta;

            P4MonitorPtr _monitor_muons;
            P4MonitorPtr _monitor_muon_jets;
            DeltaMonitorPtr _monitor_muon_delta;

            boost::shared_ptr<algorithm::ClosestJet> _closest_jet_finder;
    };
}

#endif
