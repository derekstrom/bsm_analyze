// Synchronization exercise July 2011
// (single thread)
//
// Created by Samvel Khalatyan, Jul 05, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Reader.h"
#include "interface/SynchAnalyzer.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using namespace bsm;

typedef shared_ptr<SynchJuly2011Analyzer> SynchAnalyzerPtr;

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
        SynchAnalyzerPtr analyzer(new SynchJuly2011Analyzer());
        for(int i = 1; argc > i; ++i)
        {
            shared_ptr<Reader> reader(new Reader(argv[i]));
            reader->open();
            if (!reader->isOpen())
                continue;

            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(event);
                    ++events_read)
            {
                analyzer->process(event.get());

                event->Clear();
            }
        }

        cout << endl;
        cout << *analyzer << endl;
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
