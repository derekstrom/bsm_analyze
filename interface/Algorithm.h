// Common Algorithm Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_ALGORITHM
#define BSM_ALGORITHM

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Jet.pb.h"

class TLorentzVector;

namespace bsm
{
    class Electron;
    class Muon;

    namespace algorithm
    {
        // Search for closest jet to electron, muon
        //
        class ClosestJet
        {
            public:
                typedef ::google::protobuf::RepeatedPtrField<Jet>
                    Jets;

                ClosestJet();

                const Jet *operator()(const Jets &, const Electron &);
                const Jet *operator()(const Jets &, const Muon &);

            private:
                typedef boost::shared_ptr<TLorentzVector> P4;

                const Jet *operator()(const Jets &);

                P4 _lepton_p4;
                P4 _jet_p4;
        };
    }

    using algorithm::ClosestJet;
}

#endif
