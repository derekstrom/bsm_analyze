// Test BSM Multiplicity Cutflow
//
// Open inputs, apply multiplicity cutflow and monitor selected objects
//
// Created by Samvel Khalatyan, Jun 03, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Reader.h"
#include "interface/Selector.h"

using namespace bsm;
using namespace boost;
using namespace std;

int main(int argc, char *argv[])
try
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    {
        typedef shared_ptr<MultiplicityCutflow> MultiplicityPtr;

        MultiplicityPtr cutflow(new MultiplicityCutflow(5));

        MultiplicityPtr clone1 =
            dynamic_pointer_cast<MultiplicityCutflow>(cutflow->clone());

        MultiplicityPtr clone2 =
            dynamic_pointer_cast<MultiplicityCutflow>(cutflow->clone());

        MultiplicityPtr clone3 =
            dynamic_pointer_cast<MultiplicityCutflow>(cutflow->clone());

        for(int i = 1; argc > i; ++i)
        {
            MultiplicityPtr clone = i % 2
                ? clone1
                : clone2;

            shared_ptr<Reader> reader(new Reader(argv[i]));
            reader->open();

            if (!reader->isOpen())
                continue;

            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(event);
                    ++events_read)
            {
                cutflow->apply(event->primary_vertices().size());
                clone->apply(event->primary_vertices().size());

                event->Clear();
            }

            cout << "Events Read: " << events_read << endl;
        }

        clone3->merge(clone1);
        clone3->merge(clone2);

        MultiplicityPtr clone4 =
            dynamic_pointer_cast<MultiplicityCutflow>(clone1->clone());
        clone4->merge(clone2);

        cout << "Primary Vertices Cutflow" << endl;
        cout << *cutflow << endl;
        cout << endl;

        cout << "Clone1" << endl;
        cout << *clone1 << endl;
        cout << endl;

        cout << "Clone2" << endl;
        cout << *clone2 << endl;
        cout << endl;

        cout << "Clone3" << endl;
        cout << *clone3 << endl;
        cout << endl;

        cout << "Clone4" << endl;
        cout << *clone4 << endl;
        cout << endl;
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
