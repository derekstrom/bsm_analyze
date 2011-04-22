// Monitor Electrons
//
// Generate monitoring plots given electrons collection
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_ELECTRON
#define BSM_MONITOR_ELECTRON

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Electron.pb.h"

class TH1;
class TLorentzVector;

namespace bsm
{
    class ElectronMonitor
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;
            typedef boost::shared_ptr<TH1> H1Ptr;

            ElectronMonitor();
            ~ElectronMonitor();
            
            void fill(const Electrons &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_electron_pt() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _multiplicity;
            H1Ptr _pt;
            H1Ptr _leading_electron_pt;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };
}

#endif
