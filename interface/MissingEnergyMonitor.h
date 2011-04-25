// Monitor MissingEnergy
//
// Generate monitoring plots given missing energy 
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_MISSING_ENERGY
#define BSM_MONITOR_MISSING_ENERGY

#include <boost/shared_ptr.hpp>

class TH1;
class TLorentzVector;

namespace bsm
{
    class MissingEnergy;

    class MissingEnergyMonitor
    {
        public:
            typedef boost::shared_ptr<TH1> H1Ptr;

            MissingEnergyMonitor();
            ~MissingEnergyMonitor();
            
            void fill(const MissingEnergy &);

            const H1Ptr pt() const;
            const H1Ptr x() const;
            const H1Ptr y() const;
            const H1Ptr z() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _pt;
            H1Ptr _x;
            H1Ptr _y;
            H1Ptr _z;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };
}

#endif
