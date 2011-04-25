// Monitor delta between two Lorentz Vectors
//
// Generate delta R, Eta, Phi monitoring plots given two Lorentz Vectors
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#include <TH1F.h>
#include <TLorentzVector.h>

#include "bsm_input/interface/Physics.pb.h"
#include "interface/DeltaMonitor.h"
#include "interface/Utility.h"

using bsm::DeltaMonitor;

DeltaMonitor::DeltaMonitor()
{
    utility::SupressTHistAddDirectory supressor;

    _r.reset(new TH1F("delta_r", "#Delta_{R}", 50, 0, 5));
    _r->GetXaxis()->SetTitle("#Delta_{R}");

    _eta.reset(new TH1F("delta_eta", "#Delta_{#eta}", 200, -5, 5));
    _eta->GetXaxis()->SetTitle("#Delta_{#eta}");

    _phi.reset(new TH1F("delta_phi", "#Delta_{#phi}", 160, -4, 4));
    _phi->GetXaxis()->SetTitle("#Delta_{#phi}");

    _p4_1.reset(new TLorentzVector());
    _p4_2.reset(new TLorentzVector());
}

void DeltaMonitor::fill(const LorentzVector &p4_1, const LorentzVector &p4_2)
{
    utility::set(_p4_1.get(), &p4_1);
    utility::set(_p4_2.get(), &p4_2);

    _r->Fill(_p4_1->DeltaR(*_p4_2));
    _eta->Fill(_p4_1->Eta() - _p4_2->Eta());
    _phi->Fill(_p4_1->Phi() - _p4_2->Phi());
}

const DeltaMonitor::H1Ptr DeltaMonitor::r() const
{
    return _r;
}

const DeltaMonitor::H1Ptr DeltaMonitor::eta() const
{
    return _eta;
}

const DeltaMonitor::H1Ptr DeltaMonitor::phi() const
{
    return _phi;
}
