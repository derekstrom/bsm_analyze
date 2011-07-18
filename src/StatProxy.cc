// Proxy to Stat objects to make them clonable
//
// Created by Samvel Khalatyan, Jun 01, 2011
// Copyright 2011, All rights reserved

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_stat/interface/H1.h"
#include "bsm_stat/interface/H2.h"
#include "interface/StatProxy.h"

using bsm::H1Proxy;
using bsm::H2Proxy;

H1Proxy::H1Proxy(const uint32_t &bins, const float &min, const float &max)
{
    _histogram.reset(new stat::H1(bins, min, max));
}

H1Proxy::H1Proxy(const H1Proxy &proxy)
{
    _histogram.reset(new stat::H1(*proxy._histogram));
}

const H1Proxy::H1Ptr H1Proxy::histogram() const
{
    return _histogram;
}

uint32_t H1Proxy::id() const
{
    return core::ID<H1Proxy>::get();
}

H1Proxy::ObjectPtr H1Proxy::clone() const
{
    return ObjectPtr(new H1Proxy(*this));
}

void H1Proxy::merge(const ObjectPtr &pointer)
{
    if (id() != pointer->id())
        return;

    boost::shared_ptr<H1Proxy> object =
        boost::dynamic_pointer_cast<H1Proxy>(pointer);

    if (!object)
        return;

    *_histogram += *object->_histogram;
}

void H1Proxy::print(std::ostream &out) const
{
    out << *_histogram;
}



// H2 Proxy
//
H2Proxy::H2Proxy(const uint32_t &x_bins,
        const float &x_min,
        const float &x_max,

        const uint32_t &y_bins,
        const float &y_min,
        const float &y_max)
{
    _histogram.reset(new stat::H2(x_bins, x_min, x_max, y_bins, y_min, y_max));
}

H2Proxy::H2Proxy(const H2Proxy &proxy)
{
    _histogram.reset(new stat::H2(*proxy._histogram));
}

const H2Proxy::H2Ptr H2Proxy::histogram() const
{
    return _histogram;
}

uint32_t H2Proxy::id() const
{
    return core::ID<H2Proxy>::get();
}

H2Proxy::ObjectPtr H2Proxy::clone() const
{
    return ObjectPtr(new H2Proxy(*this));
}

void H2Proxy::merge(const ObjectPtr &pointer)
{
    if (id() != pointer->id())
        return;

    boost::shared_ptr<H2Proxy> object =
        boost::dynamic_pointer_cast<H2Proxy>(pointer);

    if (!object)
        return;

    *_histogram += *object->_histogram;
}

void H2Proxy::print(std::ostream &out) const
{
    out << *_histogram;
}
