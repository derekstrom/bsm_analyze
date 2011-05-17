// Monitors Visualizers
//
// Generate standartized Canvases for monitors
//
// Created by Samvel Khalatyan, May 17, 2011
// Copyright 2011, All rights reserved

#include <sstream>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>

#include "bsm_core/interface/ID.h"
#include "bsm_stat/interface/Utility.h"

#include "interface/Monitor.h"
#include "interface/MonitorCanvas.h"

using bsm::DeltaCanvas;
using bsm::ElectronCanvas;
using bsm::LorentzVectorCanvas;
using bsm::MuonCanvas;
using bsm::JetCanvas;

// Delta Canvas
//
DeltaCanvas::IDPtr DeltaCanvas::_id(new core::IDCounter());

DeltaCanvas::DeltaCanvas(const std::string &title)
{
    std::ostringstream name;
    name << "delta_canvas_" << DeltaCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 1024, 480));
    _canvas->Divide(3, 2);
}

void DeltaCanvas::draw(const DeltaMonitor &monitor)
{
    _canvas->cd(1);

    _r = convert(*monitor.r());
    _r->GetXaxis()->SetTitle("#Delta R");
    _r->Draw();

    _canvas->cd(2);
    _ptrel = convert(*monitor.ptrel());
    _ptrel->GetXaxis()->SetTitle("p_{T}^{rel} [GeV/c]");
    _ptrel->Draw();

    _canvas->cd(3);
    _ptrel_vs_r = convert(*monitor.ptrel_vs_r());
    _ptrel_vs_r->GetXaxis()->SetTitle("p_{T}^{rel} [GeV/c]");
    _ptrel_vs_r->GetYaxis()->SetTitle("#Delta R");
    _ptrel_vs_r->Draw("colz");

    _canvas->cd(4);
    _phi = convert(*monitor.phi());
    _phi->GetXaxis()->SetTitle("#Delta #phi [rad]");
    _phi->Draw();

    _canvas->cd(5);
    _eta = convert(*monitor.eta());
    _eta->GetXaxis()->SetTitle("#Delta #eta");
    _eta->Draw();
}



// Electron Canvas
//
ElectronCanvas::IDPtr ElectronCanvas::_id(new core::IDCounter());

ElectronCanvas::ElectronCanvas(const std::string &title)
{
    std::ostringstream name;
    name << "electron_canvas_" << ElectronCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 800, 320));
    _canvas->Divide(3);
}

void ElectronCanvas::draw(const ElectronMonitor &monitor)
{
    _canvas->cd(1);

    _multiplicity = convert(*monitor.multiplicity());
    _multiplicity->GetXaxis()->SetTitle("N_{e}");
    _multiplicity->Draw();

    _canvas->cd(2);
    _leading_pt = convert(*monitor.leading_pt());
    _leading_pt->GetXaxis()->SetTitle("leading p^{e}_{T} [GeV/c]");
    _leading_pt->Draw();

    _canvas->cd(3);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p^{e}_{T} [GeV/c]");
    _pt->Draw();
}



// LorentzVector Canvas
//
LorentzVectorCanvas::IDPtr LorentzVectorCanvas::_id(new core::IDCounter());

LorentzVectorCanvas::LorentzVectorCanvas(const std::string &title)
{
    std::ostringstream name;
    name << "lorentz_vector_canvas_" << LorentzVectorCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 1024, 480));
    _canvas->Divide(4, 2);
}

void LorentzVectorCanvas::draw(const LorentzVectorMonitor &monitor)
{
    _canvas->cd(1);
    _energy = convert(*monitor.energy());
    _energy->GetXaxis()->SetTitle("E [GeV]");
    _energy->Draw();

    _canvas->cd(2);
    _px = convert(*monitor.px());
    _px->GetXaxis()->SetTitle("p_{X} [GeV/c]");
    _px->Draw();

    _canvas->cd(3);
    _py = convert(*monitor.py());
    _py->GetXaxis()->SetTitle("p_{Y} [GeV/c]");
    _py->Draw();

    _canvas->cd(4);
    _pz = convert(*monitor.pz());
    _pz->GetXaxis()->SetTitle("p_{Z} [GeV/c]");
    _pz->Draw();

    _canvas->cd(5);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    _pt->Draw();

    _canvas->cd(6);
    _eta = convert(*monitor.eta());
    _eta->GetXaxis()->SetTitle("#eta");
    _eta->Draw();

    _canvas->cd(7);
    _phi = convert(*monitor.phi());
    _phi->GetXaxis()->SetTitle("#phi [rad]");
    _phi->Draw();
}



// Muon Canvas
//
MuonCanvas::IDPtr MuonCanvas::_id(new core::IDCounter());

MuonCanvas::MuonCanvas(const std::string &title)
{
    std::ostringstream name;
    name << "muon_canvas_" << MuonCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 800, 320));
    _canvas->Divide(3);
}

void MuonCanvas::draw(const MuonMonitor &monitor)
{
    _canvas->cd(1);

    _multiplicity = convert(*monitor.multiplicity());
    _multiplicity->GetXaxis()->SetTitle("N_{#mu}");
    _multiplicity->Draw();

    _canvas->cd(2);
    _leading_pt = convert(*monitor.leading_pt());
    _leading_pt->GetXaxis()->SetTitle("leading p^{#mu}_{T} [GeV/c]");
    _leading_pt->Draw();

    _canvas->cd(3);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p^{#mu}_{T} [GeV/c]");
    _pt->Draw();
}



// Jet Canvas
//
JetCanvas::IDPtr JetCanvas::_id(new core::IDCounter());

JetCanvas::JetCanvas(const std::string &title)
{
    std::ostringstream name;
    name << "jet_canvas_" << JetCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 800, 320));
    _canvas->Divide(3);

    if (!title.empty())
        _canvas->SetTitle(title.c_str());
}

void JetCanvas::draw(const JetMonitor &monitor)
{
    _canvas->cd(1);

    _multiplicity = convert(*monitor.multiplicity());
    _multiplicity->GetXaxis()->SetTitle("N_{jet}");
    _multiplicity->Draw();

    _canvas->cd(2);
    _leading_pt = convert(*monitor.leading_pt());
    _leading_pt->GetXaxis()->SetTitle("leading p^{jet}_{T} [GeV/c]");
    _leading_pt->Draw();

    _canvas->cd(3);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p^{jet}_{T} [GeV/c]");
    _pt->Draw();
}
