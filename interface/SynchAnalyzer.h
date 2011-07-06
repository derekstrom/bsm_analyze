// Synchronization Exercises
//
// Different Analyzers that performan the sinchronization exercises at
// different development stages
//
// Created by Samvel Khalatyan, Jul 05, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_SYNCHRONIZATION_ANALYZER
#define BSM_SYNCHRONIZATION_ANALYZER

#include <boost/shared_ptr.hpp>

#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

namespace bsm
{
    class SynchJuly2011Analyzer : public Analyzer
    {
        public:
            enum LeptonMode
            {
                ELECTRON = 1,
                MUON = 2
            };

            SynchJuly2011Analyzer(const LeptonMode & = ELECTRON);
            SynchJuly2011Analyzer(const SynchJuly2011Analyzer &);

            // Anlayzer interface
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
            typedef boost::shared_ptr<MultiplicityCutflow> CutflowPtr;

            enum
            {
                PRESELECTION = 0,
                PRIMARY_VERTEX = 1,
                JET = 2,
                LEPTON = 3,
                VETO_SECOND_LEPTON = 4
            };

            bool jets(const Event *);
            bool electron(const Event *);
            bool muon(const Event *);

            LeptonMode _lepton_mode;

            CutflowPtr _cutflow;

            boost::shared_ptr<PrimaryVertexSelector> _primary_vertex_selector;
            boost::shared_ptr<JetSelector> _jet_selector;
            boost::shared_ptr<ElectronSelector> _electron_selector;
            boost::shared_ptr<ElectronSelector> _electron_veto_selector;
            boost::shared_ptr<MuonSelector> _muon_selector;
            boost::shared_ptr<MuonSelector> _muon_veto_selector;
    };

    std::ostream &operator <<(std::ostream &, const SynchJuly2011Analyzer::LeptonMode &);
}

#endif
