// Synchronization exercise July 2011
// (multi-threads)
//
// Created by Samvel Khalatyan, Jul 05, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "interface/SynchAnalyzer.h"
#include "interface/Thread.h"

using namespace std;

using boost::shared_ptr;

using bsm::SynchJuly2011Analyzer;
using bsm::ThreadController;

typedef shared_ptr<SynchJuly2011Analyzer> SynchAnalyzerPtr;
typedef shared_ptr<ThreadController> ControllerPtr;

void run(ControllerPtr &);

int main(int argc, char *argv[])
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    int result = 0;
    try
    {
        ControllerPtr controller(new ThreadController());
        for(int i = 1; argc > i; ++i)
            controller->push(argv[i]);

        run(controller);
    }
    catch(...)
    {
        cerr << "Unknown error" << endl;

        result = 1;
    }

    // Clean Up any memory allocated by libprotobuf
    //
    google::protobuf::ShutdownProtobufLibrary();

    return result;
}

void run(ControllerPtr &controller)
try
{
    // Prepare Analysis
    //
    SynchAnalyzerPtr analyzer(new SynchJuly2011Analyzer());

    // Process inputs
    //
    controller->use(analyzer);
    controller->start();

    cout << *analyzer << endl;
}
catch(...)
{
}
