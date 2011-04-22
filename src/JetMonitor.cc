// Monitor Jets
//
// Generate monitoring plots given jets collection
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#include <TH1I.h>
#include <TLorentzVector.h>

#include "interface/JetMonitor.h"

using bsm::JetMonitor;

JetMonitor::JetMonitor()
{
    bool status = TH1::AddDirectoryStatus();
    TH1::AddDirectory(false);

    _multiplicity.reset(new TH1I("jet_multiplicity", "Jets Multiplicity",
                10, 0, 10));
    _multiplicity->GetXaxis()->SetTitle("N_{jets}");

    _pt.reset(new TH1F("jet_pt", "Jet P_{T}", 100, 0, 100));
    _pt->GetXaxis()->SetTitle("P_{T}^{jets} [GeV/c]");

    _leading_jet_pt.reset(new TH1F("leading_jet_pt", "Leading Jet P_{T}",
                100, 0, 100));
    _leading_jet_pt->GetXaxis()->SetTitle("P_{T}^{leading jet} [GeV/c]");

    _p4.reset(new TLorentzVector());

    TH1::AddDirectory(status);
}

JetMonitor::~JetMonitor()
{
}

void JetMonitor::fill(const Jets &jets)
{
    _multiplicity->Fill(jets.size());

    bool is_leading_jet = true;
    for(Jets::const_iterator jet = jets.begin();
            jets.end() != jet;
            ++jet)
    {
        _p4->SetPxPyPzE(jet->physics_object().p4().px(),
                jet->physics_object().p4().py(),
                jet->physics_object().p4().pz(),
                jet->physics_object().p4().e());

        if (is_leading_jet)
        {
            _leading_jet_pt->Fill(_p4->Pt());

            is_leading_jet = false;
        }

        _pt->Fill(_p4->Pt());
    }
}

const JetMonitor::H1Ptr JetMonitor::multiplicity() const
{
    return _multiplicity;
}

const JetMonitor::H1Ptr JetMonitor::pt() const
{
    return _pt;
}

const JetMonitor::H1Ptr JetMonitor::leading_jet_pt() const
{
    return _leading_jet_pt;
}
