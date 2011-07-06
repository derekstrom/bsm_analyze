// Utility Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_UTILITY
#define BSM_UTILITY

#include <ostream>
#include <functional>

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

    template<typename T>
        std::ostream &operator <<(std::ostream &, const std::equal_to<T> &);

    template<typename T>
        std::ostream &operator <<(std::ostream &, const std::greater<T> &);

    template<typename T>
        std::ostream &operator <<(std::ostream &, const std::greater_equal<T> &);

    template<typename T>
        std::ostream &operator <<(std::ostream &, const std::less<T> &);

    template<typename T>
        std::ostream &operator <<(std::ostream &, const std::less_equal<T> &);

    template<typename T>
        std::ostream &operator <<(std::ostream &, const std::logical_and<T> &);
}

template<typename T>
    std::ostream &bsm::operator <<(std::ostream &out, const std::equal_to<T> &)
{
    return out << "==";
}

template<typename T>
    std::ostream &bsm::operator <<(std::ostream &out, const std::greater<T> &)
{
    return out << " >";
}

template<typename T>
    std::ostream &bsm::operator <<(std::ostream &out, const std::greater_equal<T> &)
{
    return out << ">=";
}

template<typename T>
    std::ostream &bsm::operator <<(std::ostream &out, const std::less<T> &)
{
    return out << " <";
}

template<typename T>
    std::ostream &bsm::operator <<(std::ostream &out, const std::less_equal<T> &)
{
    return out << "<=";
}

template<typename T>
    std::ostream &bsm::operator <<(std::ostream &out, const std::logical_and<T> &)
{
    return out << "&&";
}

#endif
