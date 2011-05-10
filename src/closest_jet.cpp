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

#include "bsm_stat/interface/Utility.h"

#include "bsm_input/interface/Event.pb.h"
#include "interface/ClosestJetAnalyzer.h"
#include "interface/Monitor.h"
#include "interface/Thread.h"

using std::cerr;
using std::cout;
using std::endl;

using boost::shared_ptr;

using bsm::ClosestJetAnalyzer;
using bsm::core::Files;
using bsm::core::ThreadController;
using bsm::stat::convert;
using bsm::stat::TH1Ptr;

typedef shared_ptr<ClosestJetAnalyzer> ClosestJetAnalyzerPtr;

void run(const Files &);
void plot(const ClosestJetAnalyzerPtr &);

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
    ClosestJetAnalyzerPtr analyzer(new ClosestJetAnalyzer());

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

void plot(const ClosestJetAnalyzerPtr &analyzer)
{
    // Cheat ROOT with empty args
    //
    int empty_argc = 1;
    char *empty_argv[] = { "root" };
    shared_ptr<TRint> app(new TRint("app", &empty_argc, empty_argv));

    shared_ptr<TCanvas> el_canvas(new TCanvas("electrons", "Electrons", 800, 320));
    el_canvas->Divide(3);

    el_canvas->cd(1);
    TH1Ptr el_pt = convert(*analyzer->monitorElectrons()->pt());
    el_pt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    el_pt->SetNdivisions(6);
    el_pt->Draw();

    el_canvas->cd(2);
    TH1Ptr el_phi = convert(*analyzer->monitorElectrons()->phi());
    el_phi->GetXaxis()->SetTitle("#phi [rad]");
    el_phi->Draw();

    el_canvas->cd(3);
    TH1Ptr el_eta = convert(*analyzer->monitorElectrons()->eta());
    el_eta->GetXaxis()->SetTitle("#eta");
    el_eta->Draw();

    shared_ptr<TCanvas>
        el_closest_jet_canvas(new TCanvas("electron_closest_jet",
                    "Electron Closest Jet", 800, 320));
    el_closest_jet_canvas->Divide(3);

    el_closest_jet_canvas->cd(1);
    TH1Ptr el_closest_jet_pt = convert(*analyzer->monitorElectronJets()->pt());
    el_closest_jet_pt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    el_closest_jet_pt->SetNdivisions(6);
    el_closest_jet_pt->Draw();

    el_closest_jet_canvas->cd(2);
    TH1Ptr el_closest_jet_phi = convert(*analyzer->monitorElectronJets()->phi());
    el_closest_jet_phi->GetXaxis()->SetTitle("#phi [rad]");
    el_closest_jet_phi->Draw();

    el_closest_jet_canvas->cd(3);
    TH1Ptr el_closest_jet_eta = convert(*analyzer->monitorElectronJets()->eta());
    el_closest_jet_eta->GetXaxis()->SetTitle("#eta");
    el_closest_jet_eta->Draw();

    shared_ptr<TCanvas>
        el_delta_canvas(new TCanvas("electron_delta",
                    "Electron Closest Jet Delta", 800, 320));
    el_delta_canvas->Divide(3);

    el_delta_canvas->cd(1);
    TH1Ptr el_delta_r = convert(*analyzer->monitorElectronDelta()->r());
    el_delta_r->GetXaxis()->SetTitle("#Delta R");
    el_delta_r->Draw();

    el_delta_canvas->cd(2);
    TH1Ptr el_delta_phi = convert(*analyzer->monitorElectronDelta()->phi());
    el_delta_phi->GetXaxis()->SetTitle("#Delta #phi [rad]");
    el_delta_phi->Draw();

    el_delta_canvas->cd(3);
    TH1Ptr el_delta_eta = convert(*analyzer->monitorElectronDelta()->eta());
    el_delta_eta->GetXaxis()->SetTitle("#Delta #eta");
    el_delta_eta->Draw();

    shared_ptr<TCanvas> mu_canvas(new TCanvas("muons", "Muons", 800, 320));
    mu_canvas->Divide(3);

    mu_canvas->cd(1);
    TH1Ptr mu_pt = convert(*analyzer->monitorMuons()->pt());
    mu_pt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    mu_pt->SetNdivisions(6);
    mu_pt->Draw();

    mu_canvas->cd(2);
    TH1Ptr mu_phi = convert(*analyzer->monitorMuons()->phi());
    mu_phi->GetXaxis()->SetTitle("#phi [rad]");
    mu_phi->Draw();

    mu_canvas->cd(3);
    TH1Ptr mu_eta = convert(*analyzer->monitorMuons()->eta());
    mu_eta->GetXaxis()->SetTitle("#eta");
    mu_eta->Draw();

    shared_ptr<TCanvas>
        mu_closest_jet_canvas(new TCanvas("muon_closest_jet",
                    "Muon Closest Jet", 800, 320));
    mu_closest_jet_canvas->Divide(3);

    mu_closest_jet_canvas->cd(1);
    TH1Ptr mu_closest_jet_pt = convert(*analyzer->monitorMuonJets()->pt());
    mu_closest_jet_pt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    mu_closest_jet_pt->SetNdivisions(6);
    mu_closest_jet_pt->Draw();

    mu_closest_jet_canvas->cd(2);
    TH1Ptr mu_closest_jet_phi = convert(*analyzer->monitorMuonJets()->phi());
    mu_closest_jet_phi->GetXaxis()->SetTitle("#phi [rad]");
    mu_closest_jet_phi->Draw();

    mu_closest_jet_canvas->cd(3);
    TH1Ptr mu_closest_jet_eta = convert(*analyzer->monitorMuonJets()->eta());
    mu_closest_jet_eta->GetXaxis()->SetTitle("#eta");
    mu_closest_jet_eta->Draw();

    shared_ptr<TCanvas>
        mu_delta_canvas(new TCanvas("muon_delta",
                    "Muon Closest Jet Delta", 800, 320));
    mu_delta_canvas->Divide(3);

    mu_delta_canvas->cd(1);
    TH1Ptr mu_delta_r = convert(*analyzer->monitorMuonDelta()->r());
    mu_delta_r->GetXaxis()->SetTitle("#Delta R");
    mu_delta_r->Draw();

    mu_delta_canvas->cd(2);
    TH1Ptr mu_delta_phi = convert(*analyzer->monitorMuonDelta()->phi());
    mu_delta_phi->GetXaxis()->SetTitle("#Delta #phi [rad]");
    mu_delta_phi->Draw();

    mu_delta_canvas->cd(3);
    TH1Ptr mu_delta_eta = convert(*analyzer->monitorMuonDelta()->eta());
    mu_delta_eta->GetXaxis()->SetTitle("#Delta #eta");
    mu_delta_eta->Draw();

    app->Run();
}
