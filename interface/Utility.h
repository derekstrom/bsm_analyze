// Utility Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_UTILITY
#define BSM_UTILITY

namespace bsm
{
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
    }
}

#endif
