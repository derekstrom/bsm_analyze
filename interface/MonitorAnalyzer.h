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
#include "interface/bsm_fwd.h"

namespace bsm
{
    class MonitorAnalyzer : public Analyzer
    {
        public:
            typedef boost::shared_ptr<ElectronsMonitor> ElMonitorPtr;
            typedef boost::shared_ptr<JetsMonitor> JetMonitorPtr;
            typedef boost::shared_ptr<MissingEnergyMonitor> METMonitorPtr;
            typedef boost::shared_ptr<MuonsMonitor> MuMonitorPtr;
            typedef boost::shared_ptr<PrimaryVerticesMonitor> PVMonitorPtr;

            MonitorAnalyzer();
            MonitorAnalyzer(const MonitorAnalyzer &);

            // Getters
            //
            const ElMonitorPtr pfElectrons() const;
            const ElMonitorPtr gsfElectrons() const;

            const MuMonitorPtr pfMuons() const;
            const MuMonitorPtr recoMuons() const;

            const JetMonitorPtr jets() const;
            const METMonitorPtr missingEnergy() const;
            const PVMonitorPtr primaryVertices() const;

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
            MonitorAnalyzer &operator =(const MonitorAnalyzer &);

            ElMonitorPtr _pf_electrons;
            ElMonitorPtr _gsf_electrons;

            MuMonitorPtr _pf_muons;
            MuMonitorPtr _reco_muons;

            JetMonitorPtr _jets;
            METMonitorPtr _missing_energy;
            PVMonitorPtr _primary_vertices;
    };
}

#endif
