// Utility Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#include <TH1.h>

#include "interface/Utility.h"

using namespace bsm::utility;

SupressTHistAddDirectory::SupressTHistAddDirectory():
    _flag(TH1::AddDirectoryStatus())
{
    TH1::AddDirectory(false);
}

SupressTHistAddDirectory::~SupressTHistAddDirectory()
{
    TH1::AddDirectory(_flag);
}
