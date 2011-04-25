// Monitor PrimaryVertices
//
// Generate monitoring plots given primary vertices collection
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_PRIMARY_VERTICES
#define BSM_MONITOR_PRIMARY_VERTICES

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/PrimaryVertex.pb.h"

class TH1;

namespace bsm
{
    class PrimaryVertexMonitor
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<PrimaryVertex>
                PrimaryVertices;
            typedef boost::shared_ptr<TH1> H1Ptr;

            PrimaryVertexMonitor();
            ~PrimaryVertexMonitor();
            
            void fill(const PrimaryVertices &);

            const H1Ptr multiplicity() const;
            const H1Ptr x() const;
            const H1Ptr y() const;
            const H1Ptr z() const;

        private:
            H1Ptr _multiplicity;
            H1Ptr _x;
            H1Ptr _y;
            H1Ptr _z;
    };
}

#endif
