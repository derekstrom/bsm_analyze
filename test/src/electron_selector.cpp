// Test BSM Elecrton Selector
//
// Open inputs, apply electron selectors and monitor selected objects
//
// Created by Samvel Khalatyan, Jun 03, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Reader.h"
#include "interface/Selector.h"

using namespace bsm;
using namespace boost;
using namespace std;

typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

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
        typedef shared_ptr<ElectronSelector> ElSelectorPtr;
        ElSelectorPtr el_selector(new ElectronSelector());
        ElSelectorPtr el_selector_clone1 =
            dynamic_pointer_cast<ElectronSelector>(el_selector->clone());

        ElSelectorPtr el_selector_clone2 =
            dynamic_pointer_cast<ElectronSelector>(el_selector->clone());
        ElSelectorPtr el_selector_clone3 =
            dynamic_pointer_cast<ElectronSelector>(el_selector->clone());

        ElSelectorPtr el_selector_clone4 =
            dynamic_pointer_cast<ElectronSelector>(el_selector->clone());

        for(int i = 1; argc > i; ++i)
        {
            ElSelectorPtr selector = i % 2
                ? el_selector_clone2
                : el_selector_clone3;

            shared_ptr<Reader> reader(new Reader(argv[i]));
            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(*event);
                    ++events_read)
            {
                if (!event->primary_vertices().size())
                    continue;

                const PrimaryVertex &pv = *event->primary_vertices().begin();

                for(Electrons::const_iterator electron = event->pf_electrons().begin();
                        event->pf_electrons().end() != electron;
                        ++electron)
                {
                    el_selector->apply(*electron, pv);
                    el_selector_clone1->apply(*electron, pv);
                    selector->apply(*electron, pv);
                }

                event->Clear();
            }

            cout << "Events Read: " << events_read << endl;
        }

        el_selector_clone4->merge(el_selector_clone2);
        el_selector_clone4->merge(el_selector_clone3);

        cout << "Electron Selector" << endl;
        cout << *el_selector << endl;

        cout << "Clone 1" << endl;
        cout << *el_selector_clone1 << endl;

        cout << "Clone 2" << endl;
        cout << *el_selector_clone2 << endl;

        cout << "Clone 3" << endl;
        cout << *el_selector_clone3 << endl;

        cout << "Clone 4" << endl;
        cout << *el_selector_clone4 << endl;
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
