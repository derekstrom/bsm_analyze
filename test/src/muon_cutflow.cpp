// Cutflow table for mu+nu+j + j+j channel
//
// Read inputs and generate cutflow tables
//
// Created by Samvel Khalatyan, Jun 13, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Reader.h"
#include "interface/CutflowAnalyzer.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using namespace bsm;

typedef shared_ptr<MuonCutflowAnalyzer> MuonCutflowAnalyzerPtr;

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
        MuonCutflowAnalyzerPtr analyzer(new MuonCutflowAnalyzer());
        for(int i = 1; argc > i; ++i)
        {
            shared_ptr<Reader> reader(new Reader(argv[i]));
            reader->open();
            if (!reader->isOpen())
            {
                cerr << "Failed to open: " << argv[i] << endl;

                continue;
            }

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
