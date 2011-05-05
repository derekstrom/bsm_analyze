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

    cout << *jets << endl;
    cout << *gen_particles << endl;
    cout << *muons << endl;
    cout << *electrons << endl;
    cout << *primary_vertices << endl;
    cout << *missing_energy << endl;

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
