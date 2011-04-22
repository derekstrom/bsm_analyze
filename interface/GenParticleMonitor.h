// Monitor GenParticle
//
// Generate monitoring plots given gen particle
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_GENPARTICLE
#define BSM_MONITOR_GENPARTICLE

#include <boost/shared_ptr.hpp>

class TH1;
class TLorentzVector;

namespace bsm
{
    class GenParticle;

    class GenParticleMonitor
    {
        public:
            typedef boost::shared_ptr<TH1> H1Ptr;

            GenParticleMonitor();
            ~GenParticleMonitor();
            
            void fill(const GenParticle &);

            const H1Ptr id() const;
            const H1Ptr status() const;
            const H1Ptr pt() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _id;
            H1Ptr _status;
            H1Ptr _pt;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };
}

#endif
