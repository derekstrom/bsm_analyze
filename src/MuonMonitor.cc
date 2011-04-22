// Monitor Muons
//
// Generate monitoring plots given muons collection
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#include <TH1I.h>
#include <TLorentzVector.h>

#include "interface/MuonMonitor.h"
#include "interface/Utility.h"

using bsm::MuonMonitor;

MuonMonitor::MuonMonitor()
{
    utility::SupressTHistAddDirectory supressor;

    _multiplicity.reset(new TH1I("muon_multiplicity", "Muons Multiplicity",
                10, 0, 10));
    _multiplicity->GetXaxis()->SetTitle("N_{muons}");

    _pt.reset(new TH1F("muon_pt", "Muon P_{T}", 100, 0, 100));
    _pt->GetXaxis()->SetTitle("P_{T}^{muons} [GeV/c]");

    _leading_muon_pt.reset(new TH1F("leading_muon_pt", "Leading Muon P_{T}",
                100, 0, 100));
    _leading_muon_pt->GetXaxis()->SetTitle("P_{T}^{leading muon} [GeV/c]");

    _p4.reset(new TLorentzVector());
}

MuonMonitor::~MuonMonitor()
{
}

void MuonMonitor::fill(const Muons &muons)
{
    _multiplicity->Fill(muons.size());

    double leading_muon_pt = 0;
    double pt = 0;
    for(Muons::const_iterator muon = muons.begin();
            muons.end() != muon;
            ++muon)
    {
        _p4->SetPxPyPzE(muon->physics_object().p4().px(),
                muon->physics_object().p4().py(),
                muon->physics_object().p4().pz(),
                muon->physics_object().p4().e());

        pt = _p4->Pt();

        if (leading_muon_pt < pt)
            leading_muon_pt = pt;

        _pt->Fill(pt);
    }

    if (leading_muon_pt)
        _leading_muon_pt->Fill(leading_muon_pt);
}

const MuonMonitor::H1Ptr MuonMonitor::multiplicity() const
{
    return _multiplicity;
}

const MuonMonitor::H1Ptr MuonMonitor::pt() const
{
    return _pt;
}

const MuonMonitor::H1Ptr MuonMonitor::leading_muon_pt() const
{
    return _leading_muon_pt;
}
