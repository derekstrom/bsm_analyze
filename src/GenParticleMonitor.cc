// Monitor GenParticle
//
// Generate monitoring plots given gen particle
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#include <TH1I.h>
#include <TLorentzVector.h>

#include "bsm_input/interface/GenParticle.pb.h"
#include "interface/GenParticleMonitor.h"
#include "interface/Utility.h"

using bsm::GenParticleMonitor;

GenParticleMonitor::GenParticleMonitor()
{
    utility::SupressTHistAddDirectory supressor;

    _id.reset(new TH1I("gen_particle_id", "GenParticle ID",
                100, -50, 50));
    _id->GetXaxis()->SetTitle("ID_{pdg}^{gen particle}");

    _status.reset(new TH1I("gen_particle_status", "GenParticle Status",
                10, 0, 10));
    _status->GetXaxis()->SetTitle("Status^{gen particle}");

    _pt.reset(new TH1F("gen_particle_pt", "GenParticle P_{T}", 100, 0, 100));
    _pt->GetXaxis()->SetTitle("P_{T}^{gen particle} [GeV/c]");

    _p4.reset(new TLorentzVector());
}

GenParticleMonitor::~GenParticleMonitor()
{
}

void GenParticleMonitor::fill(const GenParticle &particle)
{
    _id->Fill(particle.id());
    _status->Fill(particle.status());

    _p4->SetPxPyPzE(particle.physics_object().p4().px(),
            particle.physics_object().p4().py(),
            particle.physics_object().p4().pz(),
            particle.physics_object().p4().e());

    _pt->Fill(_p4->Pt());
}

const GenParticleMonitor::H1Ptr GenParticleMonitor::id() const
{
    return _id;
}

const GenParticleMonitor::H1Ptr GenParticleMonitor::status() const
{
    return _status;
}

const GenParticleMonitor::H1Ptr GenParticleMonitor::pt() const
{
    return _pt;
}
