// Monitor Jets
//
// Generate monitoring plots given jets collection
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_JET
#define BSM_MONITOR_JET

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Jet.pb.h"

class TH1;
class TLorentzVector;

namespace bsm
{
    class JetMonitor
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;
            typedef boost::shared_ptr<TH1> H1Ptr;

            JetMonitor();
            ~JetMonitor();
            
            void fill(const Jets &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_jet_pt() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _multiplicity;
            H1Ptr _pt;
            H1Ptr _leading_jet_pt;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };
}

#endif
