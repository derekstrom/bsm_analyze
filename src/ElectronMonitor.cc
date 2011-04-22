// Monitor Electrons
//
// Generate monitoring plots given electrons collection
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#include <TH1I.h>
#include <TLorentzVector.h>

#include "interface/ElectronMonitor.h"

using bsm::ElectronMonitor;

ElectronMonitor::ElectronMonitor()
{
    bool status = TH1::AddDirectoryStatus();
    TH1::AddDirectory(false);

    _multiplicity.reset(new TH1I("electron_multiplicity", "Electrons Multiplicity",
                10, 0, 10));
    _multiplicity->GetXaxis()->SetTitle("N_{electrons}");

    _pt.reset(new TH1F("electron_pt", "Electron P_{T}", 100, 0, 100));
    _pt->GetXaxis()->SetTitle("P_{T}^{electrons} [GeV/c]");

    _leading_electron_pt.reset(new TH1F("leading_electron_pt", "Leading Electron P_{T}",
                100, 0, 100));
    _leading_electron_pt->GetXaxis()->SetTitle("P_{T}^{leading electron} [GeV/c]");

    _p4.reset(new TLorentzVector());

    TH1::AddDirectory(status);
}

ElectronMonitor::~ElectronMonitor()
{
}

void ElectronMonitor::fill(const Electrons &electrons)
{
    _multiplicity->Fill(electrons.size());

    bool is_leading_electron = true;
    for(Electrons::const_iterator electron = electrons.begin();
            electrons.end() != electron;
            ++electron)
    {
        _p4->SetPxPyPzE(electron->physics_object().p4().px(),
                electron->physics_object().p4().py(),
                electron->physics_object().p4().pz(),
                electron->physics_object().p4().e());

        if (is_leading_electron)
        {
            _leading_electron_pt->Fill(_p4->Pt());

            is_leading_electron = false;
        }

        _pt->Fill(_p4->Pt());
    }
}

const ElectronMonitor::H1Ptr ElectronMonitor::multiplicity() const
{
    return _multiplicity;
}

const ElectronMonitor::H1Ptr ElectronMonitor::pt() const
{
    return _pt;
}

const ElectronMonitor::H1Ptr ElectronMonitor::leading_electron_pt() const
{
    return _leading_electron_pt;
}
