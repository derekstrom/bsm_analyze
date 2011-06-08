// Mttbar reconstructions
//
// Created by Samvel Khalatyan, Jun 07, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/shared_ptr.hpp>

#include <TCanvas.h>
#include <TH1.h>
#include <TRint.h>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_stat/interface/bsm_stat_fwd.h"
#include "bsm_stat/interface/Utility.h"
#include "interface/MonitorCanvas.h"
#include "interface/MttbarAnalyzer.h"
#include "interface/Thread.h"

using namespace std;

using boost::shared_ptr;

using bsm::LorentzVectorCanvas;
using bsm::DeltaCanvas;
using bsm::MttbarAnalyzer;
using bsm::ThreadController;

typedef shared_ptr<MttbarAnalyzer> AnalyzerPtr;
typedef shared_ptr<ThreadController> ControllerPtr;

void run(ControllerPtr &, char *[]);

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

        run(controller, argv);
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

void run(ControllerPtr &controller, char *argv[])
try
{
    // Prepare Analysis
    //
    AnalyzerPtr analyzer(new MttbarAnalyzer());

    // Process inputs
    //
    controller->use(analyzer);
    controller->start();

    cout << *analyzer << endl;

    {
        typedef bsm::stat::TH1Ptr TH1Ptr;

        int empty_argc = 1;
        char *empty_argv[] = { argv[0] };

        boost::shared_ptr<TRint>
            app(new TRint("app", &empty_argc, empty_argv));

        TH1Ptr mttbar = convert(*analyzer->mttbar());
        mttbar->SetName("mttbar");
        mttbar->GetXaxis()->SetTitle("m_{t#bar{t}} [GeV/c^{2}]");
        mttbar->Draw();

        mttbar->SaveAs("mttbar.root");

        boost::shared_ptr<LorentzVectorCanvas> el_p4(
                new LorentzVectorCanvas("Selected PF Electron"));
        el_p4->draw(*analyzer->electronMonitor());

        boost::shared_ptr<LorentzVectorCanvas> wjet_p4(
                new LorentzVectorCanvas("W-tagged Jet"));
        wjet_p4->draw(*analyzer->wjetMonitor());

        boost::shared_ptr<LorentzVectorCanvas> ltop_p4(
                new LorentzVectorCanvas("Leptonic Top"));
        ltop_p4->draw(*analyzer->ltopMonitor());

        boost::shared_ptr<LorentzVectorCanvas> htop_p4(
                new LorentzVectorCanvas("Hadronic Top"));
        htop_p4->draw(*analyzer->htopMonitor());

        boost::shared_ptr<DeltaCanvas> top_delta(
                new DeltaCanvas("Delta between Leptonic and Hadronic tops"));
        top_delta->draw(*analyzer->topDeltaMonitor());

        app->Run();
    }
}
catch(...)
{
}
