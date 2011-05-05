// Monitoring Analyzer
//
// Generate monitoring plots
//
// Created by Samvel Khalatyan, May 05, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_ANALYZER
#define BSM_MONITOR_ANALYZER

#include <boost/shared_ptr.hpp>

#include "interface/Analyzer.h"

namespace bsm
{
    class JetMonitor;

    class MonitorAnalyzer : public core::Analyzer
    {
        public:
            typedef Analyzer::AnalyzerPtr AnalyzerPtr;
            typedef boost::shared_ptr<ElectronMonitor> ElectronMonitorPtr;
            typedef boost::shared_ptr<JetMonitor> JetMonitorPtr;
            typedef boost::shared_ptr<MissingEnergyMonitor>
                MissingEnergyMonitorPtr;
            typedef boost::shared_ptr<MuonMonitor> MuonMonitorPtr;
            typedef boost::shared_ptr<PrimaryVertexMonitor>
                PrimaryVertexMonitorPtr;

            MonitorAnalyzer();
            virtual ~MonitorAnalyzer();

            // Analyzer interface
            //
            virtual AnalyzerPtr clone() const;

            virtual void merge(const AnalyzerPtr &);

            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            virtual operator bool() const;

            // Getter
            //
            const ElectronMonitorPtr monitorElectrons() const;
            const JetMonitorPtr monitorJets() const;
            const MissingEnergyMonitorPtr monitorMissingEnergy() const;
            const MuonMonitorPtr monitorMuons() const;
            const PrimaryVertexMonitorPtr monitorPrimaryVertices() const;

        private:
            // Prevent copying
            //
            MonitorAnalyzer &operator =(const MonitorAnalyzer &);

            ElectronMonitorPtr _monitor_electrons;
            JetMonitorPtr _monitor_jets;
            MissingEnergyMonitorPtr _monitor_missing_energy;
            MuonMonitorPtr _monitor_muons;
            PrimaryVertexMonitorPtr _monitor_primary_vertices;
    };
}

#endif
