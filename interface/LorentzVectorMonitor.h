// Monitor Lorentz Vector
//
// Generate monitoring plots given lorentz vector
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_LORENTZ_VECTOR
#define BSM_MONITOR_LORENTZ_VECTOR

#include <boost/shared_ptr.hpp>

class TH1;
class TLorentzVector;

namespace bsm
{
    class LorentzVector;

    class LorentzVectorMonitor
    {
        public:
            typedef boost::shared_ptr<TH1> H1Ptr;

            LorentzVectorMonitor();

            void fill(const LorentzVector &);

            const H1Ptr energy() const;
            const H1Ptr px() const;
            const H1Ptr py() const;
            const H1Ptr pz() const;

            const H1Ptr pt() const;
            const H1Ptr eta() const;
            const H1Ptr phi() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _energy;
            H1Ptr _px;
            H1Ptr _py;
            H1Ptr _pz;

            H1Ptr _pt;
            H1Ptr _eta;
            H1Ptr _phi;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };
}

#endif
