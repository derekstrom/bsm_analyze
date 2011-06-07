// Monitor Analyzer test
//
// Test if analyzer can be correctly clones, merged, printed
//
// Created by Samvel Khalatyan, May 06, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Reader.h"
#include "bsm_input/interface/Event.pb.h"
#include "interface/MonitorAnalyzer.h"

using namespace std;
using namespace boost;
using namespace bsm;

typedef shared_ptr<MonitorAnalyzer> AnalyzerPtr;

int main(int argc, char *argv[])
try
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    AnalyzerPtr monitor(new MonitorAnalyzer());
    AnalyzerPtr monitor_copy_1 = dynamic_pointer_cast<MonitorAnalyzer>(monitor->clone());

    cout << "monitor: " << monitor << endl;
    cout << "copye  : " << monitor_copy_1 << endl;

    cout << "Original Monitor" << endl;
    cout << *monitor << endl;
    cout << endl;
    cout << "Monitor Copy1" << endl;
    cout << *monitor_copy_1 << endl;
    cout << endl;

    {
        shared_ptr<Reader> reader(new Reader(argv[1]));
        reader->open();

        if (reader->isOpen())
        {
            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(event);
                    ++events_read)
            {
                monitor_copy_1->process(event.get());

                event->Clear();
            }

            cout << "Events Read: " << events_read << endl;
        }
    }

    cout << "Original Monitor" << endl;
    cout << *monitor << endl;
    cout << endl;
    cout << "Monitor Copy1" << endl;
    cout << *monitor_copy_1 << endl;
    cout << endl;

    cout << "Merge" << endl;
    monitor->merge(monitor_copy_1);
    cout << "Original Monitor" << endl;
    cout << *monitor << endl;

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
