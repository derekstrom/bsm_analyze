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
    class ElectronMonitor;
    class JetMonitor;
    class MissingEnergyMonitor;
    class MuonMonitor;
    class PrimaryVertexMonitor;

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

            virtual void onFileOpen(const std::string &filename, const Input *);
            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            virtual operator bool() const;

            // Getter
            //
            const ElectronMonitorPtr monitorPFElectrons() const;
            const ElectronMonitorPtr monitorGSFElectrons() const;
            const MuonMonitorPtr monitorPFMuons() const;
            const MuonMonitorPtr monitorRecoMuons() const;

            const JetMonitorPtr monitorJets() const;
            const MissingEnergyMonitorPtr monitorMissingEnergy() const;
            const PrimaryVertexMonitorPtr monitorPrimaryVertices() const;

        private:
            // Prevent copying
            //
            MonitorAnalyzer &operator =(const MonitorAnalyzer &);

            ElectronMonitorPtr _monitor_pf_electrons;
            ElectronMonitorPtr _monitor_gsf_electrons;

            MuonMonitorPtr _monitor_pf_muons;
            MuonMonitorPtr _monitor_reco_muons;

            JetMonitorPtr _monitor_jets;
            MissingEnergyMonitorPtr _monitor_missing_energy;
            PrimaryVertexMonitorPtr _monitor_primary_vertices;
    };
}

#endif
