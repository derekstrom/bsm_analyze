// Read plot from file, scale it, and write back
//
// Created by Samvel Khalatyan, Jun 08, 2011
// Copyright 2011, All rights reserved

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>

#include <TFile.h>
#include <TH1.h>
#include <THStack.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TRint.h>

using namespace std;
using namespace boost;

// usage: merge LUMI CROSS_SECTION EVENTS FILTER_EFF file.root:/path/to/histogram
//
int main(int argc, char *argv[])
{
    if (5 != argc)
    {
        cerr << "Usage: " << argv[0]
            << " data.root zprime.root ttjets.root wjets.root"
            << endl;

        return 1;
    }

    TFile *data = TFile::Open(argv[1], "read");
    if (!data->IsOpen())
    {
        cerr << "failed to open Data file: " << argv[1] << endl;

        return 1;
    }

    TFile *zprime = TFile::Open(argv[2], "read");
    if (!zprime->IsOpen())
    {
        cerr << "failed to open Z' file: " << argv[2] << endl;

        return 1;
    }

    TFile *ttjets = TFile::Open(argv[3], "read");
    if (!ttjets->IsOpen())
    {
        cerr << "failed to open TTjets file: " << argv[3] << endl;

        return 1;
    }

    TFile *wjets = TFile::Open(argv[4], "read");
    if (!wjets->IsOpen())
    {
        cerr << "failed to open Wjets file: " << argv[4] << endl;

        return 1;
    }

    TH1 *h_data = dynamic_cast<TH1 *>(data->Get("mttbar"));
    if (!h_data)
    {
        cerr << "failed to extract Data histogram" << endl;

        return 1;
    }

    TH1 *h_zprime = dynamic_cast<TH1 *>(zprime->Get("mttbar"));
    if (!h_zprime)
    {
        cerr << "failed to extract Z' histogram" << endl;

        return 1;
    }
    h_zprime->SetFillColor(kBlue - 7);
    h_zprime->SetLineColor(kBlue - 7);
    h_zprime->SetFillStyle(3004);

    TH1 *h_ttjets = dynamic_cast<TH1 *>(ttjets->Get("mttbar"));
    if (!h_ttjets)
    {
        cerr << "failed to extract TTjets histogram" << endl;

        return 1;
    }
    h_ttjets->SetFillColor(kRed + 1);
    h_ttjets->SetFillStyle(1001);

    TH1 *h_wjets = dynamic_cast<TH1 *>(wjets->Get("mttbar"));
    if (!h_wjets)
    {
        cerr << "failed to extract Wjets histogram" << endl;

        return 1;
    }
    h_wjets->SetFillColor(kGreen - 3);
    h_wjets->SetFillStyle(1001);

    boost::shared_ptr<TLegend> legend(new TLegend(0.65,  0.52,  0.92,  0.72));
    legend->SetBorderSize(0);
    legend->SetLineStyle(0);
    legend->SetTextFont(42);
    legend->SetTextSizePixels(24);
    legend->SetFillStyle(0);

    legend->AddEntry(h_data, "Data", "lp");
    legend->AddEntry(h_ttjets, "TTjets", "f");
    legend->AddEntry(h_wjets, "Wjets", "f");
    legend->AddEntry(h_zprime, "Z' (@ 1GeV/c^{2})", "f");

    boost::shared_ptr<TLatex> label(new TLatex(3.570061, 23.08044, "#splitline{CMS: Work in Progress}{230 pb^{-1} at #sqrt{s} = 7 TeV}"));
    label->SetNDC();
    label->SetTextAlign(13);
    label->SetX(0.4);
    label->SetY(0.908);

    {
        int empty_argc = 1;
        char *empty_argv[] = { argv[0] };

        boost::shared_ptr<TRint>
            app(new TRint("app", &empty_argc, empty_argv));

        boost::shared_ptr<THStack> stack(new THStack("mttbar_bg", "Mttbar"));
        stack->Add(h_ttjets);
        stack->Add(h_wjets);

        h_zprime->Draw("hist");
        h_data->Draw("same");
        stack->Draw("hist same");

        legend->Draw();
        label->Draw();

        app->Run();
    }


    return 0;
}
