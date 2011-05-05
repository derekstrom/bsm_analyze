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

#include "bsm_stat/interface/Utility.h"

#include "interface/Monitor.h"
#include "interface/MonitorAnalyzer.h"
#include "interface/Thread.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::MonitorAnalyzer;
using bsm::core::Files;
using bsm::core::ThreadController;
using bsm::stat::convert;
using bsm::stat::TH1Ptr;

typedef shared_ptr<MonitorAnalyzer> MonitorAnalyzerPtr;

void run(const Files &);
void plot(const MonitorAnalyzerPtr &);

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
        Files input_files;
        for(int i = 2; argc > i; ++i)
            input_files.push_back(argv[i]);

        run(input_files);
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

void run(const Files &input_files)
try
{
    // Prepare Analysis
    //
    shared_ptr<ThreadController> controller(new ThreadController());
    MonitorAnalyzerPtr analyzer(new MonitorAnalyzer());

    // Process inputs
    //
    controller->process(analyzer, input_files);

    // Plot results
    //
    plot(analyzer);

}
catch(...)
{
}

void plot(const MonitorAnalyzerPtr &analyzer)
{
    // Cheat ROOT with empty args
    //
    int empty_argc = 1;
    char *empty_argv[] = { "root" };
    shared_ptr<TRint> app(new TRint("app", &empty_argc, empty_argv));

    shared_ptr<TCanvas> jet_canvas(new TCanvas("jets", "Jets", 800, 320));
    jet_canvas->Divide(3);

    jet_canvas->cd(1);
    TH1Ptr jet_multiplicity = convert(*analyzer->monitorJets()->multiplicity());
    jet_multiplicity->Draw();

    jet_canvas->cd(2);
    TH1Ptr jet_leading_pt = convert(*analyzer->monitorJets()->leading_pt());
    jet_leading_pt->Draw();

    jet_canvas->cd(3);
    TH1Ptr jet_pt = convert(*analyzer->monitorJets()->pt());
    jet_pt->Draw();

    /*
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
    */

    app->Run();
}
