// Proxy to Stat objects to make them clonable
//
// Created by Samvel Khalatyan, Jun 01, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_STAT_PROXY
#define BSM_STAT_PROXY

#include <boost/shared_ptr.hpp>

#include "bsm_core/interface/Object.h"
#include "bsm_stat/interface/bsm_stat_fwd.h"

namespace bsm
{
    class H1Proxy : public core::Object
    {
        public:
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            H1Proxy(const uint32_t &bins, const double &min, const double &max);
            H1Proxy(const H1Proxy &);

            const H1Ptr histogram() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            virtual void merge(const ObjectPtr &);

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            H1Proxy &operator =(const H1Proxy &);

            H1Ptr _histogram;
    };

    class H2Proxy : public core::Object
    {
        public:
            typedef boost::shared_ptr<stat::H2> H2Ptr;

            H2Proxy(const uint32_t &x_bins,
                    const double &x_min,
                    const double &x_max,
                    
                    const uint32_t &y_bins,
                    const double &y_min,
                    const double &y_max);

            H2Proxy(const H2Proxy &);

            const H2Ptr histogram() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            virtual void merge(const ObjectPtr &);

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            H2Proxy &operator =(const H2Proxy &);

            H2Ptr _histogram;
    };
}

#endif
