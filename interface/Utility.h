// Utility Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_UTILITY
#define BSM_UTILITY

class TLorentzVector;

namespace bsm
{
    class LorentzVector;

    namespace utility
    {
        class SupressTHistAddDirectory
        {
            public:
                SupressTHistAddDirectory();
                ~SupressTHistAddDirectory();

            private:
                bool _flag;
        };

        void set(TLorentzVector *root_p4, const LorentzVector *bsm_p4);
    }
}

#endif
