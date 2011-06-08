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
            typedef boost::shared_ptr<LorentzVectorMonitor> P4MonitorPtr;
            typedef boost::shared_ptr<DeltaMonitor> DeltaMonitorPtr;

            MttbarAnalyzer();
            MttbarAnalyzer(const MttbarAnalyzer &);

            // Getters
            //
            const H1Ptr mttbar() const;
            const P4MonitorPtr electronMonitor() const;
            const P4MonitorPtr wjetMonitor() const;
            const P4MonitorPtr ltopMonitor() const;
            const P4MonitorPtr htopMonitor() const;
            const DeltaMonitorPtr topDeltaMonitor() const;

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

            bool muons(const Event *);
            void electrons(const Event *);
            void jets(const Event *, const Electron *);

            boost::shared_ptr<ElectronSelector> _el_selector;
            boost::shared_ptr<MultiplicityCutflow> _el_multiplicity;
            P4MonitorPtr _el_monitor;

            boost::shared_ptr<MuonSelector> _mu_selector;
            boost::shared_ptr<MultiplicityCutflow> _mu_multiplicity;

            boost::shared_ptr<WJetSelector> _wjet_selector;
            P4MonitorPtr _wjet_monitor;

            P4MonitorPtr _ltop_monitor;
            P4MonitorPtr _htop_monitor;

            DeltaMonitorPtr _top_delta_monitor;

            H1ProxyPtr _mttbar;
    };
}

#endif
