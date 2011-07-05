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
            SynchJuly2011Analyzer();
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
                LEPTON = 3
            };

            CutflowPtr _cutflow;
    };
}

#endif
