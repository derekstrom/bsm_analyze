// Filter BSM Input
//
// Read events, filter only GOOD physics objects: muons, electrons, jets, etc.
//
// Created by Samvel Khalatyan, Jun 07, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Reader.h"
#include "bsm_input/interface/Event.pb.h"
#include "interface/FilterAnalyzer.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using namespace bsm;

int main(int argc, char *argv[])
try
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    shared_ptr<FilterAnalyzer> filter(new FilterAnalyzer());

    {
        for(int i = 1; argc > i; ++i)
        {
            shared_ptr<Reader> reader(new Reader(argv[i]));
            reader->open();

            filter->onFileOpen(reader->filename(), reader->input().get());

            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(*event);
                    ++events_read)
            {
                filter->process(event.get());

                event->Clear();
            }

            reader->close();

            cout << "Events Read: " << events_read << endl;
        }
    }

    cout << *filter << endl;


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
