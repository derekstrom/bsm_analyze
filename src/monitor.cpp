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
#include "interface/MonitorCanvas.h"
#include "interface/MonitorAnalyzer.h"
#include "interface/Thread.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::MonitorAnalyzer;
using bsm::ThreadController;
using bsm::stat::convert;
using bsm::stat::TH1Ptr;

using namespace bsm;

typedef shared_ptr<MonitorAnalyzer> MonitorAnalyzerPtr;
typedef shared_ptr<ThreadController> ControllerPtr;

void run(ControllerPtr &);
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
        ControllerPtr controller(new ThreadController());
        for(int i = 1; argc > i; ++i)
            controller->push(argv[i]);

        run(controller);
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

void run(ControllerPtr &controller)
try
{
    // Prepare Analysis
    //
    MonitorAnalyzerPtr analyzer(new MonitorAnalyzer());

    // Process inputs
    //
    controller->use(analyzer);
    controller->start();

    cout << *analyzer << endl;

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

    shared_ptr<JetCanvas> jet_canvas(new JetCanvas("Jets"));
    jet_canvas->draw(*analyzer->monitorJets());

    shared_ptr<MuonCanvas> mu_canvas(new MuonCanvas("Muons"));
    mu_canvas->draw(*analyzer->monitorMuons());

    shared_ptr<ElectronCanvas> el_canvas(new ElectronCanvas("Electrons"));
    el_canvas->draw(*analyzer->monitorElectrons());

    shared_ptr<PrimaryVertexCanvas> pv_canvas(new PrimaryVertexCanvas("Primary Vertex"));
    pv_canvas->draw(*analyzer->monitorPrimaryVertices());

    shared_ptr<MissingEnergyCanvas> met_canvas(new MissingEnergyCanvas("Missing Energy"));
    met_canvas->draw(*analyzer->monitorMissingEnergy());

    app->Run();
}
