// Test Threads
//
// Create N threads on M files
//
// Created by Samvel Khalatyan, May 04, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "bsm_input/interface/Reader.h"
#include "bsm_stat/interface/Utility.h"

#include "interface/MonitorAnalyzer.h"
#include "interface/Thread.h"

using namespace std;

using boost::lexical_cast;
using boost::shared_ptr;

using bsm::AnalyzerOperation;
using bsm::MonitorAnalyzer;
using bsm::ThreadController;

typedef shared_ptr<ThreadController> ControllerPtr;

void run(ControllerPtr &controller);

int main(int argc, char *argv[])
try
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;
        cerr << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    {
        ControllerPtr controller(new ThreadController());
        for(int i = 1; argc > i; ++i)
            controller->push(argv[i]);

        run(controller);
    }

    // Clean Up any memory allocated by libprotobuf
    //
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
 }
catch(...)
{
    // Clean Up any memory allocated by libprotobuf
    //
    google::protobuf::ShutdownProtobufLibrary();

    cerr << "Unknown error" << endl;

    return 1;
}

void run(ControllerPtr &controller)
try
{
    // Nothing will be processed since analyzer is empty. One could instantiate
    // a real analyzer, say MonitorAnalyzer and test if the code would work fine
    //
    bsm::AnalyzerPtr analyzer;

    controller->use(analyzer);

    cout << "Start Controller: empty analyzer" << endl;

    controller->start();

    cout << "Controller has finished job" << endl;

    analyzer.reset(new MonitorAnalyzer());
    controller->use(analyzer);

    cout << "Start Controller: non-empty analyzer" << endl;
    controller->start();

    cout << "Controller has finished job" << endl;
}
catch(...)
{
    cerr << "Unknown error" << endl;
}
