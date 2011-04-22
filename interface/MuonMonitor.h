// Monitor Muons
//
// Generate monitoring plots given muons collection
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_MUON
#define BSM_MONITOR_MUON

#include <boost/shared_ptr.hpp>

#include "input/interface/Muon.pb.h"

class TH1;
class TLorentzVector;

namespace bsm
{
    class MuonMonitor
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;
            typedef boost::shared_ptr<TH1> H1Ptr;

            MuonMonitor();
            ~MuonMonitor();
            
            void fill(const Muons &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_muon_pt() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _multiplicity;
            H1Ptr _pt;
            H1Ptr _leading_muon_pt;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };
}

#endif
