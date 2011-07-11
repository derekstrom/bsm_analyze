// Monitors Visualizers
//
// Generate standartized Canvases for monitors
//
// Created by Samvel Khalatyan, May 17, 2011
// Copyright 2011, All rights reserved

#include <iostream>
#include <sstream>

#include <TCanvas.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TH2.h>

#include "bsm_core/interface/ID.h"
#include "bsm_stat/interface/Utility.h"

#include "interface/Monitor.h"
#include "interface/MonitorCanvas.h"

using namespace std;

using bsm::DeltaCanvas;
using bsm::ElectronCanvas;
using bsm::JetCanvas;
using bsm::LorentzVectorCanvas;
using bsm::MissingEnergyCanvas;
using bsm::MuonCanvas;
using bsm::PrimaryVertexCanvas;

// Delta Canvas
//
DeltaCanvas::IDPtr DeltaCanvas::_id(new core::IDCounter());

DeltaCanvas::DeltaCanvas(const string &title)
{
    ostringstream name;
    name << "delta_canvas_" << DeltaCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 1024, 480));
    _canvas->Divide(3, 2);
}

void DeltaCanvas::draw(const DeltaMonitor &monitor)
{
    _canvas->cd(1);

    _r = convert(*monitor.r());
    _r->GetXaxis()->SetTitle("#Delta R");
    _r->Draw("hist");

    _canvas->cd(2);
    _ptrel = convert(*monitor.ptrel());
    _ptrel->GetXaxis()->SetTitle("p_{T}^{rel} [GeV/c]");
    _ptrel->Draw("hist");

    _canvas->cd(3);
    _ptrel_vs_r = convert(*monitor.ptrel_vs_r());
    _ptrel_vs_r->GetXaxis()->SetTitle("p_{T}^{rel} [GeV/c]");
    _ptrel_vs_r->GetYaxis()->SetTitle("#Delta R");
    _ptrel_vs_r->Draw("colz");

    _canvas->cd(4);
    _phi = convert(*monitor.phi());
    _phi->GetXaxis()->SetTitle("#Delta #phi [rad]");
    _phi->Draw("hist");

    _canvas->cd(5);
    _eta = convert(*monitor.eta());
    _eta->GetXaxis()->SetTitle("#Delta #eta");
    _eta->Draw("hist");
}



// Electron Canvas
//
ElectronCanvas::IDPtr ElectronCanvas::_id(new core::IDCounter());

ElectronCanvas::ElectronCanvas(const string &title)
{
    ostringstream name;
    name << "electron_canvas_" << ElectronCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 800, 320));
    _canvas->Divide(3);
}

void ElectronCanvas::draw(const ElectronsMonitor &monitor)
{
    _canvas->cd(1);

    _multiplicity = convert(*monitor.multiplicity());
    _multiplicity->GetXaxis()->SetTitle("N_{e}");
    _multiplicity->Draw("hist");

    _canvas->cd(2);
    _leading_pt = convert(*monitor.leading_pt());
    _leading_pt->GetXaxis()->SetTitle("leading p^{e}_{T} [GeV/c]");
    _leading_pt->Draw("hist");

    _canvas->cd(3);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p^{e}_{T} [GeV/c]");
    _pt->Draw("hist");
}



// LorentzVector Canvas
//
LorentzVectorCanvas::IDPtr LorentzVectorCanvas::_id(new core::IDCounter());

LorentzVectorCanvas::LorentzVectorCanvas(const string &title)
{
    ostringstream name;
    name << "lorentz_vector_canvas_" << LorentzVectorCanvas::_id->add();

    _name = name.str();
    _title = title;
}

void LorentzVectorCanvas::draw(const LorentzVectorMonitor &monitor)
{
    if (!_canvas)
    {
        _canvas.reset(new TCanvas(_name.c_str(), _title.c_str(), 1024, 480));
        _canvas->Divide(4, 2);
    }

    _canvas->cd(1);
    _energy = convert(*monitor.energy());
    _energy->GetXaxis()->SetTitle("E [GeV]");
    _energy->Draw("hist");

    _canvas->cd(2);
    _px = convert(*monitor.px());
    _px->GetXaxis()->SetTitle("p_{X} [GeV/c]");
    _px->Draw("hist");

    _canvas->cd(3);
    _py = convert(*monitor.py());
    _py->GetXaxis()->SetTitle("p_{Y} [GeV/c]");
    _py->Draw("hist");

    _canvas->cd(4);
    _pz = convert(*monitor.pz());
    _pz->GetXaxis()->SetTitle("p_{Z} [GeV/c]");
    _pz->Draw("hist");

    _canvas->cd(5);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    _pt->Draw("hist");

    _canvas->cd(6);
    _eta = convert(*monitor.eta());
    _eta->GetXaxis()->SetTitle("#eta");
    _eta->Draw("hist");

    _canvas->cd(7);
    _phi = convert(*monitor.phi());
    _phi->GetXaxis()->SetTitle("#phi [rad]");
    _phi->Draw("hist");

    _canvas->cd(8);
    _mass = convert(*monitor.mass());
    _mass->GetXaxis()->SetTitle("Mass [GeV/c^{2}]");
    _mass->Draw("hist");
}

void LorentzVectorCanvas::write(TDirectory *dir, const LorentzVectorMonitor &monitor)
{
    string subdir_name = _title;
    replace(subdir_name.begin(), subdir_name.end(), ' ', '_');

    TDirectory *subdir = 0;
    TObject *object = dir->FindObject(subdir_name.c_str());
    if (object)
    {
        subdir = dynamic_cast<TDirectory *>(object);
    }
    else
    {
        subdir = dir->mkdir(subdir_name.c_str());
        if (!subdir)
        {
            cerr << "failed ot create output folder: " << subdir_name
                << " in " << dir->GetName() << endl;

            return;
        }
    }

    subdir->cd();

    _energy = convert(*monitor.energy());
    _energy->GetXaxis()->SetTitle("E [GeV]");
    _energy->Write();

    _px = convert(*monitor.px());
    _px->GetXaxis()->SetTitle("p_{X} [GeV/c]");
    _px->Write();

    _py = convert(*monitor.py());
    _py->GetXaxis()->SetTitle("p_{Y} [GeV/c]");
    _py->Write();

    _pz = convert(*monitor.pz());
    _pz->GetXaxis()->SetTitle("p_{Z} [GeV/c]");
    _pz->Write();

    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    _pt->Write();

    _eta = convert(*monitor.eta());
    _eta->GetXaxis()->SetTitle("#eta");
    _eta->Write();

    _phi = convert(*monitor.phi());
    _phi->GetXaxis()->SetTitle("#phi [rad]");
    _phi->Write();

    _mass = convert(*monitor.mass());
    _mass->GetXaxis()->SetTitle("Mass [GeV/c^{2}]");
    _mass->Write();
}



// MissingEnergy Canvas
//
MissingEnergyCanvas::IDPtr MissingEnergyCanvas::_id(new core::IDCounter());

MissingEnergyCanvas::MissingEnergyCanvas(const string &title)
{
    ostringstream name;
    name << "missing_energy_canvas_" << MissingEnergyCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 640, 480));
    _canvas->Divide(2, 2);
}

void MissingEnergyCanvas::draw(const MissingEnergyMonitor &monitor)
{
    _canvas->cd(1);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p^{MET}_{T} [GeV/c]");
    _pt->Draw("hist");

    _canvas->cd(2);
    _x = convert(*monitor.x());
    _x->GetXaxis()->SetTitle("X^{MET} [cm]");
    _x->Draw("hist");

    _canvas->cd(3);
    _y = convert(*monitor.y());
    _y->GetXaxis()->SetTitle("Y^{MET} [cm]");
    _y->Draw("hist");

    _canvas->cd(4);
    _z = convert(*monitor.z());
    _z->GetXaxis()->SetTitle("Z^{MET} [cm]");
    _z->Draw("hist");
}



// Muon Canvas
//
MuonCanvas::IDPtr MuonCanvas::_id(new core::IDCounter());

MuonCanvas::MuonCanvas(const string &title)
{
    ostringstream name;
    name << "muon_canvas_" << MuonCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 800, 320));
    _canvas->Divide(3);
}

void MuonCanvas::draw(const MuonsMonitor &monitor)
{
    _canvas->cd(1);

    _multiplicity = convert(*monitor.multiplicity());
    _multiplicity->GetXaxis()->SetTitle("N_{#mu}");
    _multiplicity->Draw("hist");

    _canvas->cd(2);
    _leading_pt = convert(*monitor.leading_pt());
    _leading_pt->GetXaxis()->SetTitle("leading p^{#mu}_{T} [GeV/c]");
    _leading_pt->Draw("hist");

    _canvas->cd(3);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p^{#mu}_{T} [GeV/c]");
    _pt->Draw("hist");
}



// Jet Canvas
//
JetCanvas::IDPtr JetCanvas::_id(new core::IDCounter());

JetCanvas::JetCanvas(const string &title)
{
    ostringstream name;
    name << "jet_canvas_" << JetCanvas::_id->add();

    _name = name.str();
    _title = title;
}

void JetCanvas::draw(const JetsMonitor &monitor)
{
    if (!_canvas)
    {
        _canvas.reset(new TCanvas(_name.c_str(), _title.c_str(), 1024, 320));
        _canvas->Divide(4);
    }

    _canvas->cd(1);
    _multiplicity = convert(*monitor.multiplicity());
    _multiplicity->GetXaxis()->SetTitle("N_{jet}");
    _multiplicity->Draw("hist");

    _canvas->cd(2);
    _leading_pt = convert(*monitor.leading_pt());
    _leading_pt->GetXaxis()->SetTitle("leading p^{jet}_{T} [GeV/c]");
    _leading_pt->Draw("hist");

    _canvas->cd(3);
    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p^{jet}_{T} [GeV/c]");
    _pt->Draw("hist");

    _canvas->cd(4);
    _children = convert(*monitor.children());
    _children->GetXaxis()->SetTitle("N_{children}");
    _children->Draw("hist");
}

void JetCanvas::write(TDirectory *dir, const JetsMonitor &monitor)
{
    string subdir_name = _title;
    replace(subdir_name.begin(), subdir_name.end(), ' ', '_');

    TDirectory *subdir = 0;
    TObject *object = dir->FindObject(subdir_name.c_str());
    if (object)
    {
        subdir = dynamic_cast<TDirectory *>(object);
    }
    else
    {
        subdir = dir->mkdir(subdir_name.c_str());
        if (!subdir)
        {
            cerr << "failed ot create output folder: " << subdir_name
                << " in " << dir->GetName() << endl;

            return;
        }
    }

    subdir->cd();

    _multiplicity = convert(*monitor.multiplicity());
    _multiplicity->GetXaxis()->SetTitle("N_{jet}");
    _multiplicity->Write();

    _leading_pt = convert(*monitor.leading_pt());
    _leading_pt->GetXaxis()->SetTitle("leading p^{jet}_{T} [GeV/c]");
    _leading_pt->Write();

    _pt = convert(*monitor.pt());
    _pt->GetXaxis()->SetTitle("p^{jet}_{T} [GeV/c]");
    _pt->Write();

    _children = convert(*monitor.children());
    _children->GetXaxis()->SetTitle("N_{children}");
    _children->Write();
}



// PrimaryVertex Canvas
//
PrimaryVertexCanvas::IDPtr PrimaryVertexCanvas::_id(new core::IDCounter());

PrimaryVertexCanvas::PrimaryVertexCanvas(const string &title)
{
    ostringstream name;
    name << "primary_vertex_canvas_" << PrimaryVertexCanvas::_id->add();

    _canvas.reset(new TCanvas(name.str().c_str(), title.c_str(), 640, 480));
    _canvas->Divide(2, 2);
}

void PrimaryVertexCanvas::draw(const PrimaryVerticesMonitor &monitor)
{
    _canvas->cd(1);
    _multiplicity = convert(*monitor.multiplicity());
    _multiplicity->GetXaxis()->SetTitle("N_{PV}");
    _multiplicity->Draw("hist");

    _canvas->cd(2);
    _x = convert(*monitor.x());
    _x->GetXaxis()->SetTitle("X^{PV} [cm]");
    _x->Draw("hist");

    _canvas->cd(3);
    _y = convert(*monitor.y());
    _y->GetXaxis()->SetTitle("Y^{PV} [cm]");
    _y->Draw("hist");

    _canvas->cd(4);
    _z = convert(*monitor.z());
    _z->GetXaxis()->SetTitle("Z^{PV} [cm]");
    _z->Draw("hist");
}
