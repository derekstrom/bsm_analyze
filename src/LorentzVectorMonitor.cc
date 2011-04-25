// Monitor Lorentz Vector
//
// Generate monitoring plots given lorentz vector
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#include <TH1F.h>
#include <TLorentzVector.h>

#include "bsm_input/interface/Physics.pb.h"
#include "interface/LorentzVectorMonitor.h"
#include "interface/Utility.h"

using bsm::LorentzVectorMonitor;

LorentzVectorMonitor::LorentzVectorMonitor()
{
    utility::SupressTHistAddDirectory supressor;

    _energy.reset(new TH1F("energy", "Energy", 100, 0, 100));
    _energy->GetXaxis()->SetTitle("E [GeV]");

    _px.reset(new TH1F("px", "P_X", 100, 0, 100));
    _px->GetXaxis()->SetTitle("P_X [GeV/c]");

    _py.reset(new TH1F("py", "P_Y", 100, 0, 100));
    _py->GetXaxis()->SetTitle("P_Y [GeV/c]");

    _pz.reset(new TH1F("pz", "P_Z", 100, 0, 100));
    _pz->GetXaxis()->SetTitle("P_Z [GeV/c]");

    _pt.reset(new TH1F("pt", "P_T", 100, 0, 100));
    _pt->GetXaxis()->SetTitle("P_T [GeV/c]");

    _eta.reset(new TH1F("eta", "#eta", 200, -10, 10));
    _eta->GetXaxis()->SetTitle("#eta");

    _phi.reset(new TH1F("phi", "#phi", 80, -4, 4));
    _phi->GetXaxis()->SetTitle("#phi");

    _p4.reset(new TLorentzVector());
}

void LorentzVectorMonitor::fill(const LorentzVector &p4)
{
    _energy->Fill(p4.e());
    _px->Fill(p4.px());
    _py->Fill(p4.py());
    _pz->Fill(p4.pz());

    utility::set(_p4.get(), &p4);
    _pt->Fill(_p4->Pt());
    _eta->Fill(_p4->Eta());
    _phi->Fill(_p4->Phi());
}

const LorentzVectorMonitor::H1Ptr LorentzVectorMonitor::energy() const
{
    return _energy;
}

const LorentzVectorMonitor::H1Ptr LorentzVectorMonitor::px() const
{
    return _px;
}

const LorentzVectorMonitor::H1Ptr LorentzVectorMonitor::py() const
{
    return _py;
}
 
const LorentzVectorMonitor::H1Ptr LorentzVectorMonitor::pz() const
{
    return _pz;
}

const LorentzVectorMonitor::H1Ptr LorentzVectorMonitor::pt() const
{
    return _pt;
}

const LorentzVectorMonitor::H1Ptr LorentzVectorMonitor::eta() const
{
    return _eta;
}

const LorentzVectorMonitor::H1Ptr LorentzVectorMonitor::phi() const
{
    return _phi;
}
