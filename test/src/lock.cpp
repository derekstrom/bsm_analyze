// Test Threads Lock
//
// Creat temporary lock
//
// Created by Samvel Khalatyan, May 04, 2011
// Copyright 2011, All rights reserved

#include <iostream>
#include <stdexcept>

#include "interface/Thread.h"

using namespace std;

using bsm::core::Condition;
using bsm::core::ConditionPtr;
using bsm::core::Lock;

int main(int argc, char *argv[])
try
{
    {
        ConditionPtr condition(new Condition());
        Lock lock(condition);
    }

    return 0;
}
catch(const exception &error)
{
    cerr << "error: " << error.what() << endl;

    return 1;
}
catch(...)
{
    cerr << "Unknown error" << endl;

    return 1;
}
