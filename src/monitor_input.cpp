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
#include "interface/ElectronMonitor.h"
#include "interface/GenParticleMonitor.h"
#include "interface/Monitor.h"
#include "interface/MissingEnergyMonitor.h"
#include "interface/MuonMonitor.h"
#include "interface/PrimaryVertexMonitor.h"

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

int main(int argc, char *argv[])
try
{
    if (2 > argc)
    {
        cerr << "Usage: " << argv[0] << " input.pb" << endl;

        return 0;
    }

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    shared_ptr<ElectronMonitor> electrons(new ElectronMonitor());
    shared_ptr<GenParticleMonitor> gen_particles(new GenParticleMonitor());
    shared_ptr<JetMonitor> jets(new JetMonitor());
    shared_ptr<MissingEnergyMonitor> missing_energy(new MissingEnergyMonitor());
    shared_ptr<MuonMonitor> muons(new MuonMonitor());
    shared_ptr<PrimaryVertexMonitor> primary_vertices(new PrimaryVertexMonitor());

    {
        shared_ptr<Reader> reader(new Reader(argv[1]));
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

            muons->fill(event->muons());
            electrons->fill(event->electrons());
            primary_vertices->fill(event->primary_vertices());

            if (event->has_missing_energy())
                missing_energy->fill(event->missing_energy());

            event->Clear();
        }

        cout << "Events Read: " << events_read << endl;
    }

    {
        int empty_argc = 1;
        char *empty_argv[] = { argv[0] };
        shared_ptr<TRint> app(new TRint("app", &empty_argc, empty_argv));

        /*
        shared_ptr<TCanvas> jet_canvas(new TCanvas("jets", "Jets", 800, 320));
        jet_canvas->Divide(3);

        jet_canvas->cd(1);
        jets->multiplicity()->Draw();

        jet_canvas->cd(2);
        jets->leading_jet_pt()->Draw();

        jet_canvas->cd(3);
        jets->pt()->Draw();
        */
        cout << *jets << endl;

        shared_ptr<TCanvas> gen_particle_canvas(new TCanvas("gen_particles", "GenParticles", 800, 320));
        gen_particle_canvas->Divide(3);

        gen_particle_canvas->cd(1);
        gen_particles->id()->Draw();

        gen_particle_canvas->cd(2);
        gen_particles->status()->Draw();

        gen_particle_canvas->cd(3);
        gen_particles->pt()->Draw();

        shared_ptr<TCanvas> muon_canvas(new TCanvas("muons", "Muons", 800, 320));
        muon_canvas->Divide(3);

        muon_canvas->cd(1);
        muons->multiplicity()->Draw();

        muon_canvas->cd(2);
        muons->leading_muon_pt()->Draw();

        muon_canvas->cd(3);
        muons->pt()->Draw();

        shared_ptr<TCanvas> electron_canvas(new TCanvas("electrons", "Electrons", 800, 320));
        electron_canvas->Divide(3);

        electron_canvas->cd(1);
        electrons->multiplicity()->Draw();

        electron_canvas->cd(2);
        electrons->leading_electron_pt()->Draw();

        electron_canvas->cd(3);
        electrons->pt()->Draw();

        shared_ptr<TCanvas> primary_vertex_canvas(new TCanvas("primary_vertices", "Priamary Vertices", 640, 480));
        primary_vertex_canvas->Divide(2, 2);

        primary_vertex_canvas->cd(1);
        primary_vertices->multiplicity()->Draw();

        primary_vertex_canvas->cd(2);
        primary_vertices->x()->Draw();

        primary_vertex_canvas->cd(3);
        primary_vertices->y()->Draw();

        primary_vertex_canvas->cd(4);
        primary_vertices->z()->Draw();

        shared_ptr<TCanvas> missing_energy_canvas(new TCanvas("missing_energy", "Missing Energy", 640, 480));
        missing_energy_canvas->Divide(2, 2);

        missing_energy_canvas->cd(1);
        missing_energy->pt()->Draw();

        missing_energy_canvas->cd(2);
        missing_energy->x()->Draw();

        missing_energy_canvas->cd(3);
        missing_energy->y()->Draw();

        missing_energy_canvas->cd(4);
        missing_energy->z()->Draw();

        app->Run();
    }

    jets.reset();

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
