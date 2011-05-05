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
            typedef boost::shared_ptr<JetMonitor> JetMonitorPtr;

            MonitorAnalyzer();
            virtual ~MonitorAnalyzer();

            // Analyzer interface
            //
            virtual AnalyzerPtr clone() const;

            virtual void merge(const AnalyzerPtr &);

            virtual void process(const Event *);

            virtual void print(std::ostream &) const;

            const JetMonitorPtr monitorJets() const;

        private:
            // Prevent copying
            //
            MonitorAnalyzer &operator =(const MonitorAnalyzer &);

            JetMonitorPtr _monitor_jets;
    };
}

#endif
