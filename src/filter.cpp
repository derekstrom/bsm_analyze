// Filter events
//
// Apply pre-selection and filter events
//
// Created by Samvel Khalatyan, May 23, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "interface/FilterAnalyzer.h"
#include "interface/Thread.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

//using bsm::FilterAnalyzer;
using bsm::ThreadController;

//typedef shared_ptr<FilterAnalyzer> FilterAnalyzerPtr;
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
    /*
    // Prepare Analysis
    //
    FilterAnalyzerPtr analyzer(new FilterAnalyzer());

    // Process inputs
    //
    controller->use(analyzer);
    controller->start();

    cout << *analyzer << endl;
    */
}
catch(...)
{
}
