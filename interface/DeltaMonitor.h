// Monitor delta between two Lorentz Vectors
//
// Generate delta R, Eta, Phi monitoring plots given two Lorentz Vectors
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_DELTA_LORENTZ_VECTOR
#define BSM_MONITOR_DELTA_LORENTZ_VECTOR

#include <boost/shared_ptr.hpp>

class TH1;
class TLorentzVector;

namespace bsm
{
    class LorentzVector;

    class DeltaMonitor
    {
        public:
            typedef boost::shared_ptr<TH1> H1Ptr;

            DeltaMonitor();

            void fill(const LorentzVector &, const LorentzVector &);

            const H1Ptr r() const;
            const H1Ptr eta() const;
            const H1Ptr phi() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _r;
            H1Ptr _eta;
            H1Ptr _phi;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4_1;
            P4 _p4_2;
    };
}

#endif
