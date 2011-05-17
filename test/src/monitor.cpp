// Monitor BSM Input
//
// Read events and plot basic distributions
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

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::Reader;
using bsm::Event;
using bsm::ElectronMonitor;
using bsm::GenParticleMonitor;
using bsm::JetMonitor;
using bsm::MissingEnergyMonitor;
using bsm::MuonMonitor;
using bsm::PrimaryVertexMonitor;
using bsm::stat::convert;
using bsm::stat::TH1Ptr;

shared_ptr<ElectronMonitor> electrons;
shared_ptr<GenParticleMonitor> gen_particles;
shared_ptr<JetMonitor> jets;
shared_ptr<MissingEnergyMonitor> missing_energy;
shared_ptr<MuonMonitor> muons;
shared_ptr<PrimaryVertexMonitor> primary_vertices;

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

    electrons.reset(new ElectronMonitor());
    gen_particles.reset(new GenParticleMonitor());
    jets.reset(new JetMonitor());
    missing_energy.reset(new MissingEnergyMonitor());
    muons.reset(new MuonMonitor());
    primary_vertices.reset(new PrimaryVertexMonitor());

    {
        for(int i = 1; argc > i; ++i)
        {
            shared_ptr<Reader> reader(new Reader(argv[i]));
            uint32_t events_read = 0;
            for(shared_ptr<Event> event(new Event());
                    reader->read(*event);
                    ++events_read)
            {
                jets->fill(event->jets());

                for(JetMonitor::Jets::const_iterator jet = event->jets().begin();
                        event->jets().end() != jet;
                        ++jet)
                {
                    if (!jet->has_gen_parton())
                        continue;

                    gen_particles->fill(jet->gen_parton());
                }

                muons->fill(event->pf_muons());
                electrons->fill(event->pf_electrons());
                primary_vertices->fill(event->primary_vertices());

                if (event->has_missing_energy())
                    missing_energy->fill(event->missing_energy());

                event->Clear();
            }

            cout << "Events Read: " << events_read << endl;
        }
    }

    cout << *jets << endl;
    cout << *gen_particles << endl;
    cout << *muons << endl;
    cout << *electrons << endl;
    cout << *primary_vertices << endl;
    cout << *missing_energy << endl;

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

    shared_ptr<TCanvas> electron_canvas(new TCanvas("electrons", "Electrons", 800, 320));
    electron_canvas->Divide(3);

    electron_canvas->cd(1);
    TH1Ptr electron_multiplicity = convert(*electrons->multiplicity());
    electron_multiplicity->Draw();

    electron_canvas->cd(2);
    TH1Ptr electron_leading_pt = convert(*electrons->leading_pt());
    electron_leading_pt->Draw();

    electron_canvas->cd(3);
    TH1Ptr electron_pt = convert(*electrons->pt());
    electron_pt->Draw();

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

    app->Run();
}
