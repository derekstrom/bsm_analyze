// Monitor Closest jet
//
// Read events and plot closest jet kinematics
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include <TCanvas.h>
#include <TH1.h>
#include <TRint.h>

#include "bsm_input/interface/Reader.h"
#include "bsm_input/interface/Event.pb.h"
#include "interface/Monitor.h"
#include "interface/Algorithm.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::DeltaMonitor;
using bsm::Event;
using bsm::Electron;
using bsm::Jet;
using bsm::JetMonitor;
using bsm::Reader;
using bsm::LorentzVectorMonitor;
using bsm::algorithm::ClosestJet;

int main(int argc, char *argv[])
try
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    shared_ptr<LorentzVectorMonitor> jet_monitor(new LorentzVectorMonitor());
    shared_ptr<DeltaMonitor> delta_monitor(new DeltaMonitor());

    {
        shared_ptr<ClosestJet> closest_jet(new ClosestJet());
        shared_ptr<Reader> reader(new Reader(argv[1]));

        uint32_t events_read = 0;
        for(shared_ptr<Event> event(new Event());
                reader->read(*event);
                ++events_read)
        {
            if (!event->electrons().size()
                    || !event->jets().size())
                continue;

            const Electron &electron = *event->electrons().begin();
            const Jet *jet = closest_jet->operator()(event->jets(), electron);

            if (!jet)
                continue;

            jet_monitor->fill(jet->physics_object().p4());
            delta_monitor->fill(jet->physics_object().p4(),
                    electron.physics_object().p4());

            event->Clear();
        }

        cout << "Events Read: " << events_read << endl;
    }

    {
        /*
        int empty_argc = 1;
        char *empty_argv[] = { argv[0] };
        shared_ptr<TRint> app(new TRint("app", &empty_argc, empty_argv));

        shared_ptr<TCanvas> jet_canvas_1(new TCanvas("ClosestJet_1",
                    "Closest Jet: P4 (1)", 640, 480));
        jet_canvas_1->Divide(2, 2);

        jet_canvas_1->cd(1);
        jet_monitor->energy()->Draw();

        jet_canvas_1->cd(2);
        jet_monitor->px()->Draw();

        jet_canvas_1->cd(3);
        jet_monitor->py()->Draw();

        jet_canvas_1->cd(4);
        jet_monitor->pz()->Draw();

        shared_ptr<TCanvas> jet_canvas_2(new TCanvas("ClosestJet_2",
                    "Closest Jet: P4 (2)", 800, 320));
        jet_canvas_2->Divide(3);

        jet_canvas_2->cd(1);
        jet_monitor->pt()->Draw();

        jet_canvas_2->cd(2);
        jet_monitor->eta()->Draw();

        jet_canvas_2->cd(3);
        jet_monitor->phi()->Draw();

        shared_ptr<TCanvas> delta_canvas(new TCanvas("Delta",
                    "Delta", 800, 320));
        delta_canvas->Divide(3);

        delta_canvas->cd(1);
        delta_monitor->r()->Draw();

        delta_canvas->cd(2);
        delta_monitor->eta()->Draw();

        delta_canvas->cd(3);
        delta_monitor->phi()->Draw();

        app->Run();
        */
    }

    cout << *jet_monitor << endl;
    cout << *delta_monitor << endl;

    delta_monitor.reset();
    jet_monitor.reset();

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
