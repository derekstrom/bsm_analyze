// A collection of Monitors
//
// Monitors are used to easily check different quantities, physics objects,
// jets, etc.
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#include <iomanip>
#include <ostream>

#include <TLorentzVector.h>

#include "bsm_input/interface/GenParticle.pb.h"
#include "bsm_input/interface/MissingEnergy.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "bsm_stat/interface/H1.h"

#include "interface/Monitor.h"
#include "interface/Utility.h"

using std::endl;

using bsm::DeltaMonitor;
using bsm::ElectronMonitor;
using bsm::GenParticleMonitor;
using bsm::JetMonitor;
using bsm::LorentzVectorMonitor;
using bsm::MissingEnergyMonitor;
using bsm::MuonMonitor;
using bsm::PrimaryVertexMonitor;

using bsm::stat::H1;

// Jet Monitor
//
JetMonitor::JetMonitor()
{
    _multiplicity.reset(new H1(10, 0, 10));
    _pt.reset(new H1(100, 0, 100));
    _leading_pt.reset(new H1(100, 0, 100));

    _p4.reset(new TLorentzVector());
}

JetMonitor::~JetMonitor()
{
}

void JetMonitor::fill(const Jets &jets)
{
    _multiplicity->fill(jets.size());

    double leading_pt = 0;
    double pt = 0;
    for(Jets::const_iterator jet = jets.begin();
            jets.end() != jet;
            ++jet)
    {
        _p4->SetPxPyPzE(jet->physics_object().p4().px(),
                jet->physics_object().p4().py(),
                jet->physics_object().p4().pz(),
                jet->physics_object().p4().e());

        pt = _p4->Pt();

        if (leading_pt < pt)
            leading_pt = pt;

        _pt->fill(pt);
    }

    if (leading_pt)
        _leading_pt->fill(leading_pt);
}

const JetMonitor::H1Ptr JetMonitor::multiplicity() const
{
    return _multiplicity;
}

const JetMonitor::H1Ptr JetMonitor::pt() const
{
    return _pt;
}

const JetMonitor::H1Ptr JetMonitor::leading_pt() const
{
    return _leading_pt;
}



// Delta Monitor
//
DeltaMonitor::DeltaMonitor()
{
    _r.reset(new H1(50, 0, 5));
    _eta.reset(new H1(200, -5, 5));
    _phi.reset(new H1(160, -4, 4));

    _p4_1.reset(new TLorentzVector());
    _p4_2.reset(new TLorentzVector());
}

void DeltaMonitor::fill(const LorentzVector &p4_1, const LorentzVector &p4_2)
{
    utility::set(_p4_1.get(), &p4_1);
    utility::set(_p4_2.get(), &p4_2);

    _r->fill(_p4_1->DeltaR(*_p4_2));
    _eta->fill(_p4_1->Eta() - _p4_2->Eta());
    _phi->fill(_p4_1->Phi() - _p4_2->Phi());
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



// Electron Monitor
ElectronMonitor::ElectronMonitor()
{
    _multiplicity.reset(new H1(10, 0, 10));
    _pt.reset(new H1(100, 0, 100));
    _leading_pt.reset(new H1(100, 0, 100));

    _p4.reset(new TLorentzVector());
}

ElectronMonitor::~ElectronMonitor()
{
}

void ElectronMonitor::fill(const Electrons &electrons)
{
    _multiplicity->fill(electrons.size());

    double leading_pt = 0;
    double pt = 0;
    for(Electrons::const_iterator electron = electrons.begin();
            electrons.end() != electron;
            ++electron)
    {
        _p4->SetPxPyPzE(electron->physics_object().p4().px(),
                electron->physics_object().p4().py(),
                electron->physics_object().p4().pz(),
                electron->physics_object().p4().e());

        pt = _p4->Pt();

        if (leading_pt < pt)
            leading_pt = pt;

        _pt->fill(_p4->Pt());
    }

    if (leading_pt)
        _leading_pt->fill(leading_pt);
}

const ElectronMonitor::H1Ptr ElectronMonitor::multiplicity() const
{
    return _multiplicity;
}

const ElectronMonitor::H1Ptr ElectronMonitor::pt() const
{
    return _pt;
}

const ElectronMonitor::H1Ptr ElectronMonitor::leading_pt() const
{
    return _leading_pt;
}



// Gen Particle Monitor
//
GenParticleMonitor::GenParticleMonitor()
{
    _id.reset(new H1(100, -50, 50));
    _status.reset(new H1(10, 0, 10));
    _pt.reset(new H1(100, 0, 100));

    _p4.reset(new TLorentzVector());
}

GenParticleMonitor::~GenParticleMonitor()
{
}

void GenParticleMonitor::fill(const GenParticle &particle)
{
    _id->fill(particle.id());
    _status->fill(particle.status());

    _p4->SetPxPyPzE(particle.physics_object().p4().px(),
            particle.physics_object().p4().py(),
            particle.physics_object().p4().pz(),
            particle.physics_object().p4().e());

    _pt->fill(_p4->Pt());
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



// Lorentz Vector Monitor
//
LorentzVectorMonitor::LorentzVectorMonitor()
{
    _energy.reset(new H1(100, 0, 100));
    _px.reset(new H1(100, 0, 100));
    _py.reset(new H1(100, 0, 100));
    _pz.reset(new H1(100, 0, 100));
    _pt.reset(new H1(500, 0, 500));
    _eta.reset(new H1(100, -5, 5));
    _phi.reset(new H1(80, -4, 4));

    _p4.reset(new TLorentzVector());
}

void LorentzVectorMonitor::fill(const LorentzVector &p4)
{
    _energy->fill(p4.e());
    _px->fill(p4.px());
    _py->fill(p4.py());
    _pz->fill(p4.pz());

    utility::set(_p4.get(), &p4);
    _pt->fill(_p4->Pt());
    _eta->fill(_p4->Eta());
    _phi->fill(_p4->Phi());
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



// Missing Energy Monitor
//
MissingEnergyMonitor::MissingEnergyMonitor()
{
    _pt.reset(new H1(100, 0, 100));
    _x.reset(new H1(100, -50, 50));
    _y.reset(new H1(100, -50, 50));
    _z.reset(new H1(100, -50, 50));

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

    _pt->fill(_p4->Pt());
    _x->fill(missing_energy.physics_object().vertex().x());
    _y->fill(missing_energy.physics_object().vertex().y());
    _z->fill(missing_energy.physics_object().vertex().z());
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



// Muon Monitor
//
MuonMonitor::MuonMonitor()
{
    _multiplicity.reset(new H1(10, 0, 10));
    _pt.reset(new H1(100, 0, 100));
    _leading_pt.reset(new H1(100, 0, 100));

    _p4.reset(new TLorentzVector());
}

MuonMonitor::~MuonMonitor()
{
}

void MuonMonitor::fill(const Muons &muons)
{
    _multiplicity->fill(muons.size());

    double leading_pt = 0;
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

        if (leading_pt < pt)
            leading_pt = pt;

        _pt->fill(pt);
    }

    if (leading_pt)
        _leading_pt->fill(leading_pt);
}

const MuonMonitor::H1Ptr MuonMonitor::multiplicity() const
{
    return _multiplicity;
}

const MuonMonitor::H1Ptr MuonMonitor::pt() const
{
    return _pt;
}

const MuonMonitor::H1Ptr MuonMonitor::leading_pt() const
{
    return _leading_pt;
}



// Primary Vertex Monitor
//
PrimaryVertexMonitor::PrimaryVertexMonitor()
{
    _multiplicity.reset(new H1(20, 0, 20));
    _x.reset(new H1(200, -.1, .1));
    _y.reset(new H1(200, -.1, .1));
    _z.reset(new H1(100, -50, 50));
}

PrimaryVertexMonitor::~PrimaryVertexMonitor()
{
}

void PrimaryVertexMonitor::fill(const PrimaryVertices &primary_vertices)
{
    _multiplicity->fill(primary_vertices.size());

    for(PrimaryVertices::const_iterator primary_vertex = primary_vertices.begin();
            primary_vertices.end() != primary_vertex;
            ++primary_vertex)
    {
        _x->fill(primary_vertex->vertex().x());
        _y->fill(primary_vertex->vertex().y());
        _z->fill(primary_vertex->vertex().z());
    }
}

const PrimaryVertexMonitor::H1Ptr PrimaryVertexMonitor::multiplicity() const
{
    return _multiplicity;
}

const PrimaryVertexMonitor::H1Ptr PrimaryVertexMonitor::x() const
{
    return _x;
}

const PrimaryVertexMonitor::H1Ptr PrimaryVertexMonitor::y() const
{
    return _y;
}

const PrimaryVertexMonitor::H1Ptr PrimaryVertexMonitor::z() const
{
    return _z;
}



// Helpers
//
std::ostream &bsm::operator<<(std::ostream &out, const DeltaMonitor &monitor)
{
    using std::setw;
    using std::left;

    out << "Delta Monitor" << endl;
    out << setw(5) << left << " [R]" << *monitor.r() << endl;
    out << setw(5) << left << " [eta]" << *monitor.eta() << endl;
    out << setw(5) << left << " [phi] " << *monitor.phi() << endl;

    return out;
}

std::ostream &bsm::operator<<(std::ostream &out, const ElectronMonitor &monitor)
{
    using std::setw;
    using std::left;

    out << "Electron Monitor" << endl;
    out << setw(16) << left << " [multiplicity]" << *monitor.multiplicity() << endl;
    out << setw(16) << left << " [pt]" << *monitor.pt() << endl;
    out << setw(16) << left << " [leading pt] " << *monitor.leading_pt() << endl;

    return out;
}

std::ostream &bsm::operator<<(std::ostream &out,
        const GenParticleMonitor &monitor)
{
    using std::setw;
    using std::left;

    out << "Gen Particle Monitor" << endl;
    out << setw(10) << left << " [id]" << *monitor.id() << endl;
    out << setw(10) << left << " [status]" << *monitor.status() << endl;
    out << setw(10) << left << " [pt] " << *monitor.pt() << endl;

    return out;
}

std::ostream &bsm::operator<<(std::ostream &out, const JetMonitor &monitor)
{
    using std::setw;
    using std::left;

    out << "Jet Monitor" << endl;
    out << setw(16) << left << " [multiplicity]"
        << *monitor.multiplicity() << endl;
    out << setw(16) << left << " [pt]" << *monitor.pt() << endl;
    out << setw(16) << left << " [leading pt] " << *monitor.leading_pt() << endl;

    return out;
}

std::ostream &bsm::operator<<(std::ostream &out,
        const LorentzVectorMonitor &monitor)
{
    using std::setw;
    using std::left;

    out << "Lorentz Vector Monitor" << endl;
    out << setw(16) << left << " [e]" << *monitor.energy() << endl;
    out << setw(16) << left << " [px]" << *monitor.px() << endl;
    out << setw(16) << left << " [py]" << *monitor.py() << endl;
    out << setw(16) << left << " [pz]" << *monitor.pz() << endl;
    out << setw(16) << left << " [pt]" << *monitor.pt() << endl;
    out << setw(16) << left << " [eta]" << *monitor.eta() << endl;
    out << setw(16) << left << " [phi]" << *monitor.phi() << endl;

    return out;
}

std::ostream &bsm::operator<<(std::ostream &out,
        const MissingEnergyMonitor &monitor)
{
    using std::setw;
    using std::left;

    out << "Missing Energy Monitor" << endl;
    out << setw(16) << left << " [pt]" << *monitor.pt() << endl;
    out << setw(16) << left << " [x]" << *monitor.x() << endl;
    out << setw(16) << left << " [y]" << *monitor.y() << endl;
    out << setw(16) << left << " [z]" << *monitor.z() << endl;

    return out;
}

std::ostream &bsm::operator<<(std::ostream &out, const MuonMonitor &monitor)
{
    using std::setw;
    using std::left;

    out << "Muon Monitor" << endl;
    out << setw(16) << left << " [multiplicity]"
        << *monitor.multiplicity() << endl;
    out << setw(16) << left << " [pt]" << *monitor.pt() << endl;
    out << setw(16) << left << " [leading pt] " << *monitor.leading_pt() << endl;

    return out;
}

std::ostream &bsm::operator<<(std::ostream &out,
        const PrimaryVertexMonitor &monitor)
{
    using std::setw;
    using std::left;

    out << "Primary Vertex Monitor" << endl;
    out << setw(16) << left << " [multiplicity]"
        << *monitor.multiplicity() << endl;
    out << setw(16) << left << " [x]" << *monitor.x() << endl;
    out << setw(16) << left << " [y]" << *monitor.y() << endl;
    out << setw(16) << left << " [z]" << *monitor.z() << endl;

    return out;
}
