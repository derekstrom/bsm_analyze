// Decay Analyzer
//
// Generate decay table of processed events
//
// Created by Samvel Khalatyan, Jun 29, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_DECAY_ANALYZER
#define BSM_DECAY_ANALYZER

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/GenParticle.pb.h"
#include "bsm_stat/interface/bsm_stat_fwd.h"
#include "interface/Analyzer.h"
#include "interface/bsm_fwd.h"

namespace bsm
{
    typedef boost::shared_ptr<H2Proxy> H2ProxyPtr;
    typedef boost::shared_ptr<stat::H2> H2Ptr;

    class DecayAnalyzer : public Analyzer
    {
        public:
            DecayAnalyzer();
            DecayAnalyzer(const DecayAnalyzer &);

            // Analyzer interface
            //
            virtual void onFileOpen(const std::string &filename, const Input *);
            virtual void process(const Event *);

            const H2Ptr decay_level_1() const;
            const H2Ptr decay_level_2() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            typedef ::google::protobuf::RepeatedPtrField<GenParticle> GenParticles;

            void genParticles(const GenParticles &,
                    const GenParticle *parent = 0,
                    const uint32_t &level = 0);

            H2ProxyPtr _decay_level_1;
            H2ProxyPtr _decay_level_2;
    };
}

#endif
