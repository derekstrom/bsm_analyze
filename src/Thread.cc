// Threads Base
//
// All threads should inherit from this base class
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include "interface/Thread.h"

using std::cerr;
using std::endl;

using bsm::core::Condition;

// Condition class
//
Condition::Condition() throw()
try
{
    _mutex.reset(new boost::mutex());
    _variable.reset(new boost::condition_variable());
}
catch(const std::bad_alloc &error)
{
    cerr << "Condition: failed to allocate memory" << endl;

    _mutex.reset();
    _variable.reset();
}

Condition::MutexPtr Condition::mutex() const
{
    return _mutex;
}

Condition::VariablePtr Condition::variable() const
{
    return _variable;
}
