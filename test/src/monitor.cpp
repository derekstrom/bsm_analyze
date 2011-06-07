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
#include "interface/MonitorCanvas.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using namespace bsm;

using bsm::stat::convert;
using bsm::stat::TH1Ptr;

shared_ptr<ElectronsMonitor> electrons;
shared_ptr<GenParticleMonitor> gen_particles;
shared_ptr<JetsMonitor> jets;
shared_ptr<MissingEnergyMonitor> missing_energy;
shared_ptr<MuonsMonitor> muons;
shared_ptr<PrimaryVerticesMonitor> primary_vertices;

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

    electrons.reset(new ElectronsMonitor());
    gen_particles.reset(new GenParticleMonitor());
    jets.reset(new JetsMonitor());
    missing_energy.reset(new MissingEnergyMonitor());
    muons.reset(new MuonsMonitor());
    primary_vertices.reset(new PrimaryVerticesMonitor());

    {
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
                jets->fill(event->jets());

                for(JetsMonitor::Jets::const_iterator jet = event->jets().begin();
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

    shared_ptr<JetCanvas> jet_canvas(new JetCanvas("Jets"));
    jet_canvas->draw(*jets);

    shared_ptr<MuonCanvas> muon_canvas(new MuonCanvas("Muons"));
    muon_canvas->draw(*muons);

    shared_ptr<ElectronCanvas> electron_canvas(new ElectronCanvas("Electrons"));
    electron_canvas->draw(*electrons);

    shared_ptr<PrimaryVertexCanvas> pv_canvas(new PrimaryVertexCanvas("Priamary Vertices"));
    pv_canvas->draw(*primary_vertices);

    shared_ptr<MissingEnergyCanvas> met_canvas(new MissingEnergyCanvas("Missing Energy"));
    met_canvas->draw(*missing_energy);

    app->Run();
}
