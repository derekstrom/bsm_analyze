// Dump Event content
//
// Given Event, Run, Luminosity dump it's content
//
// Created by Samvel Khalatyan, Jul 06, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_DUMP_EVENT_ANALYZER
#define BSM_DUMP_EVENT_ANALYZER

#include <sstream>

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

namespace bsm
{
    class DumpEventAnalyzer : public Analyzer
    {
        public:
            DumpEventAnalyzer();
            DumpEventAnalyzer(const DumpEventAnalyzer &);

            void addEvent(const uint32_t &id,
                    const uint32_t &lumi = 0,
                    const uint32_t &runi = 0);

            // Analyzer interface
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
            class EventSearcher;

            void dump(const Event *);

            void dumpPrimaryVertices(const Event *);
            void dumpJets(const Event *);
            void dumpElectrons(const Event *);
            void dumpMuons(const Event *);

            std::vector<Event::Extra> _events;

            std::ostringstream _out;

            boost::shared_ptr<PrimaryVertexSelector> _primary_vertex_selector;
            boost::shared_ptr<JetSelector> _jet_selector;
            boost::shared_ptr<ElectronSelector> _electron_selector;
            boost::shared_ptr<MuonSelector> _muon_selector;
    };
}

#endif
