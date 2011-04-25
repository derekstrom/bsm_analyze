// Monitor MissingEnergy
//
// Generate monitoring plots given missing energy 
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#include <TH1F.h>
#include <TLorentzVector.h>

#include "bsm_input/interface/MissingEnergy.pb.h"
#include "interface/MissingEnergyMonitor.h"
#include "interface/Utility.h"

using bsm::MissingEnergyMonitor;

MissingEnergyMonitor::MissingEnergyMonitor()
{
    utility::SupressTHistAddDirectory supressor;

    _pt.reset(new TH1F("missing_energy_pt", "MissingEnergy P_{T}",
                100, 0, 100));
    _pt->GetXaxis()->SetTitle("P_{T}^{missing energy} [GeV/c]");

    _x.reset(new TH1F("missing_energy_x", "MissingEnergy X", 100, -50, 50));
    _x->GetXaxis()->SetTitle("X [cm]");

    _y.reset(new TH1F("missing_energy_y", "MissingEnergy Y", 100, -50, 50));
    _y->GetXaxis()->SetTitle("Y [cm]");

    _z.reset(new TH1F("missing_energy_z", "MissingEnergy Z", 100, -50, 50));
    _z->GetXaxis()->SetTitle("Z [cm]");

    _p4.reset(new TLorentzVector());
}

MissingEnergyMonitor::~MissingEnergyMonitor()
{
}

void MissingEnergyMonitor::fill(const MissingEnergy &missing_energy)
{
    _p4->SetPxPyPzE(missing_energy.physics_object().p4().px(),
            missing_energy.physics_object().p4().py(),
            missing_energy.physics_object().p4().pz(),
            missing_energy.physics_object().p4().e());

    _pt->Fill(_p4->Pt());
    _x->Fill(missing_energy.physics_object().vertex().x());
    _y->Fill(missing_energy.physics_object().vertex().y());
    _z->Fill(missing_energy.physics_object().vertex().z());
}

const MissingEnergyMonitor::H1Ptr MissingEnergyMonitor::pt() const
{
    return _pt;
}

const MissingEnergyMonitor::H1Ptr MissingEnergyMonitor::x() const
{
    return _x;
}

const MissingEnergyMonitor::H1Ptr MissingEnergyMonitor::y() const
{
    return _y;
}

const MissingEnergyMonitor::H1Ptr MissingEnergyMonitor::z() const
{
    return _z;
}
