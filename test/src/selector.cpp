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
#include "interface/Selector.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::Reader;
using bsm::Event;
using bsm::ElectronMonitor;
using bsm::stat::convert;
using bsm::stat::TH1Ptr;
using bsm::selector::ElectronSelector;

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
        for(int i = 1; argc > i; ++i)
        {
            shared_ptr<Reader> reader(new Reader(argv[i]));
            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(*event);
                    ++events_read)
            {
                all_pf_electrons->fill(event->pf_electrons());

                Electrons selected_electrons;
                for(Electrons::const_iterator electron = event->pf_electrons().begin();
                        event->pf_electrons().end() != electron;
                        ++electron)
                {
                    if (selector->operator()(*electron))
                        *selected_electrons.Add() = *electron;
                }

                selected_pf_electrons->fill(selected_electrons);

                event->Clear();
            }

            cout << "Events Read: " << events_read << endl;
        }
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

    shared_ptr<TCanvas> all_pf_electron_canvas(new TCanvas("all_pf_electrons", "All PF Electrons", 800, 320));
    all_pf_electron_canvas->Divide(3);

    all_pf_electron_canvas->cd(1);
    TH1Ptr all_pf_electron_multiplicity = convert(*all_pf_electrons->multiplicity());
    all_pf_electron_multiplicity->Draw();

    all_pf_electron_canvas->cd(2);
    TH1Ptr all_pf_electron_leading_pt = convert(*all_pf_electrons->leading_pt());
    all_pf_electron_leading_pt->Draw();

    all_pf_electron_canvas->cd(3);
    TH1Ptr all_pf_electron_pt = convert(*all_pf_electrons->pt());
    all_pf_electron_pt->Draw();

    shared_ptr<TCanvas> selected_pf_electron_canvas(new TCanvas("selected_pf_electrons", "Selected PF Electrons", 800, 320));
    selected_pf_electron_canvas->Divide(3);

    selected_pf_electron_canvas->cd(1);
    TH1Ptr selected_pf_electron_multiplicity = convert(*selected_pf_electrons->multiplicity());
    selected_pf_electron_multiplicity->Draw();

    selected_pf_electron_canvas->cd(2);
    TH1Ptr selected_pf_electron_leading_pt = convert(*selected_pf_electrons->leading_pt());
    selected_pf_electron_leading_pt->Draw();

    selected_pf_electron_canvas->cd(3);
    TH1Ptr selected_pf_electron_pt = convert(*selected_pf_electrons->pt());
    selected_pf_electron_pt->Draw();

    /*
    shared_ptr<TCanvas> jet_canvas(new TCanvas("jets", "Jets", 800, 320));
    jet_canvas->Divide(3);

    jet_canvas->cd(1);
    TH1Ptr jet_multiplicity = convert(*jets->multiplicity());
    jet_multiplicity->Draw();

    jet_canvas->cd(2);
    TH1Ptr jet_leading_pt = convert(*jets->leading_pt());
    jet_leading_pt->Draw();

    jet_canvas->cd(3);
    TH1Ptr jet_pt = convert(*jets->pt());
    jet_pt->Draw();

    shared_ptr<TCanvas> muon_canvas(new TCanvas("muons", "Muons", 800, 320));
    muon_canvas->Divide(3);

    muon_canvas->cd(1);
    TH1Ptr muon_multiplicity = convert(*muons->multiplicity());
    muon_multiplicity->Draw();

    muon_canvas->cd(2);
    TH1Ptr muon_leading_pt = convert(*muons->leading_pt());
    muon_leading_pt->Draw();

    muon_canvas->cd(3);
    TH1Ptr muon_pt = convert(*muons->pt());
    muon_pt->Draw();

    shared_ptr<TCanvas> primary_vertex_canvas(new TCanvas("primary_vertices", "Priamary Vertices", 640, 480));
    primary_vertex_canvas->Divide(2, 2);

    primary_vertex_canvas->cd(1);
    TH1Ptr pv_multiplicity = convert(*primary_vertices->multiplicity());
    pv_multiplicity->Draw();

    primary_vertex_canvas->cd(2);
    TH1Ptr pv_x = convert(*primary_vertices->x());
    pv_x->Draw();

    primary_vertex_canvas->cd(3);
    TH1Ptr pv_y = convert(*primary_vertices->y());
    pv_y->Draw();

    primary_vertex_canvas->cd(4);
    TH1Ptr pv_z = convert(*primary_vertices->z());
    pv_z->Draw();

    shared_ptr<TCanvas> missing_energy_canvas(new TCanvas("missing_energy", "Missing Energy", 640, 480));
    missing_energy_canvas->Divide(2, 2);

    missing_energy_canvas->cd(1);
    TH1Ptr missing_energy_pt = convert(*missing_energy->pt());
    missing_energy_pt->Draw();

    missing_energy_canvas->cd(2);
    TH1Ptr missing_energy_x = convert(*missing_energy->x());
    missing_energy_x->Draw();

    missing_energy_canvas->cd(3);
    TH1Ptr missing_energy_y = convert(*missing_energy->y());
    missing_energy_y->Draw();

    missing_energy_canvas->cd(4);
    TH1Ptr missing_energy_z = convert(*missing_energy->z());
    missing_energy_z->Draw();
    */

    app->Run();
}
