// Test BSM Selectors
//
// Open inputs, apply selectors and monitor selected objects
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include <TCanvas.h>
#include <TH1.h>
#include <TRint.h>

#include "bsm_input/interface/Reader.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_stat/interface/H1.h"
#include "bsm_stat/interface/Utility.h"
#include "interface/Monitor.h"
#include "interface/MonitorCanvas.h"
#include "interface/Selector.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::stat::convert;
using bsm::stat::TH1Ptr;
using bsm::selector::ElectronSelector;

using namespace bsm;

shared_ptr<ElectronMonitor> all_pf_electrons;
shared_ptr<ElectronMonitor> selected_pf_electrons;

typedef ElectronMonitor::Electrons Electrons;

void plot();

int main(int argc, char *argv[])
try
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    all_pf_electrons.reset(new ElectronMonitor());
    selected_pf_electrons.reset(new ElectronMonitor());

    {
        shared_ptr<ElectronSelector> selector(new ElectronSelector());
        shared_ptr<ElectronSelector> test_selector(new ElectronSelector());
        for(int i = 1; argc > i; ++i)
        {
            shared_ptr<ElectronSelector> per_file_selector(new ElectronSelector());

            shared_ptr<Reader> reader(new Reader(argv[i]));
            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(*event);
                    ++events_read)
            {
                if (!event->primary_vertices().size()
                        || !event->pf_electrons().size())
                    continue;

                all_pf_electrons->fill(event->pf_electrons());

                const PrimaryVertex &pv = *event->primary_vertices().begin();

                Electrons selected_electrons;
                for(Electrons::const_iterator electron = event->pf_electrons().begin();
                        event->pf_electrons().end() != electron;
                        ++electron)
                {
                    per_file_selector->operator()(*electron, pv);
                    if (selector->operator()(*electron, pv))
                        *selected_electrons.Add() = *electron;
                }

                if (!selected_electrons.size())
                    continue;

                selected_pf_electrons->fill(selected_electrons);

                event->Clear();
            }

            test_selector->merge(per_file_selector);

            cout << "Events Read: " << events_read << endl;
        }

        cout << "Original Selector" << endl;
        cout << *selector << endl;

        cout << "Test Selector" << endl;
        cout << *test_selector << endl;
    }

    cout << "All PF Electrons" << endl;
    cout << *all_pf_electrons << endl;
    cout << endl;
    cout << "Selected PF Electrons" << endl;
    cout << *selected_pf_electrons << endl;

    plot();

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

void plot()
{
    // Cheat ROOT with empty args
    //
    int empty_argc = 1;
    char *empty_argv[] = { "root" };
    shared_ptr<TRint> app(new TRint("app", &empty_argc, empty_argv));

    shared_ptr<ElectronCanvas> all_pf_electrons_canvas(new ElectronCanvas("All PF Electrons"));
    all_pf_electrons_canvas->draw(*all_pf_electrons);

    shared_ptr<ElectronCanvas> selected_pf_electron_canvas(new ElectronCanvas("Selected PF Electrons"));
    selected_pf_electron_canvas->draw(*selected_pf_electrons);

    app->Run();
}
