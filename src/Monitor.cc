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

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/GenParticle.pb.h"
#include "bsm_input/interface/MissingEnergy.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "bsm_stat/interface/H1.h"
#include "bsm_stat/interface/H2.h"
#include "bsm_stat/interface/Utility.h"

#include "interface/Monitor.h"
#include "interface/StatProxy.h"
#include "interface/Utility.h"

using std::endl;
using std::left;
using std::setw;

using bsm::DeltaMonitor;
using bsm::ElectronsMonitor;
using bsm::GenParticleMonitor;
using bsm::JetsMonitor;
using bsm::LorentzVectorMonitor;
using bsm::MissingEnergyMonitor;
using bsm::MuonsMonitor;
using bsm::PrimaryVerticesMonitor;

using bsm::stat::H1;
using bsm::stat::H2;

using bsm::H1Ptr;
using bsm::H2Ptr;

// Delta Monitor
//
DeltaMonitor::DeltaMonitor()
{
    _r.reset(new H1Proxy(50, 0, 5));
    _eta.reset(new H1Proxy(200, -5, 5));
    _phi.reset(new H1Proxy(160, -4, 4));
    _ptrel.reset(new H1Proxy(50, 0, 10));
    _ptrel_vs_r.reset(new H2Proxy(50, 0, 10, 50, 0, 5));

    monitor(_r);
    monitor(_eta);
    monitor(_phi);
    monitor(_ptrel);
    monitor(_ptrel_vs_r);

    _p4_1.reset(new TLorentzVector());
    _p4_2.reset(new TLorentzVector());
}

DeltaMonitor::DeltaMonitor(const DeltaMonitor &object)
{
    _r.reset(new H1Proxy(*object._r));
    _eta.reset(new H1Proxy(*object._eta));
    _phi.reset(new H1Proxy(*object._phi));
    _ptrel.reset(new H1Proxy(*object._ptrel));
    _ptrel_vs_r.reset(new H2Proxy(*object._ptrel_vs_r));

    monitor(_r);
    monitor(_eta);
    monitor(_phi);
    monitor(_ptrel);
    monitor(_ptrel_vs_r);

    _p4_1.reset(new TLorentzVector());
    _p4_2.reset(new TLorentzVector());
}

DeltaMonitor &DeltaMonitor::operator =(const DeltaMonitor &monitor)
{
    *r() = *monitor.r();
    *eta() = *monitor.eta();
    *phi() = *monitor.phi();
    *ptrel() = *monitor.ptrel();
    *ptrel_vs_r() = *monitor.ptrel_vs_r();

    return *this;
}

void DeltaMonitor::fill(const LorentzVector &p4_1, const LorentzVector &p4_2)
{
    utility::set(_p4_1.get(), &p4_1);
    utility::set(_p4_2.get(), &p4_2);

    r()->fill(_p4_1->DeltaR(*_p4_2));
    eta()->fill(_p4_1->Eta() - _p4_2->Eta());
    phi()->fill(_p4_1->Phi() - _p4_2->Phi());
    ptrel()->fill(_p4_1->Vect().Perp(_p4_2->Vect()));

    ptrel_vs_r()->fill(_p4_1->Vect().Perp(_p4_2->Vect()),
            _p4_1->DeltaR(*_p4_2));
}

const H1Ptr DeltaMonitor::r() const
{
    return _r->histogram();
}

const H1Ptr DeltaMonitor::eta() const
{
    return _eta->histogram();
}

const H1Ptr DeltaMonitor::phi() const
{
    return _phi->histogram();
}

const H1Ptr DeltaMonitor::ptrel() const
{
    return _ptrel->histogram();
}

const H2Ptr DeltaMonitor::ptrel_vs_r() const
{
    return _ptrel_vs_r->histogram();
}

uint32_t DeltaMonitor::id() const
{
    return core::ID<DeltaMonitor>::get();
}

DeltaMonitor::ObjectPtr DeltaMonitor::clone() const
{
    return ObjectPtr(new DeltaMonitor(*this));
}

void DeltaMonitor::print(std::ostream &out) const
{
    out << setw(15) << left << " [R]" << *r() << endl;
    out << setw(15) << left << " [eta]" << *eta() << endl;
    out << setw(15) << left << " [phi] " << *phi() << endl;
    out << setw(15) << left << " [pTrel]" << *ptrel() << endl;
    out << setw(14) << left << " [pTrel vs R]" << *ptrel_vs_r();
}



// Electrons Monitor
//
ElectronsMonitor::ElectronsMonitor()
{
    _multiplicity.reset(new H1Proxy(10, 0, 10));
    _pt.reset(new H1Proxy(100, 0, 100));
    _leading_pt.reset(new H1Proxy(100, 0, 100));

    monitor(_multiplicity);
    monitor(_pt);
    monitor(_leading_pt);
}

ElectronsMonitor::ElectronsMonitor(const ElectronsMonitor &object)
{
    _multiplicity.reset(new H1Proxy(*object._multiplicity));
    _pt.reset(new H1Proxy(*object._pt));
    _leading_pt.reset(new H1Proxy(*object._leading_pt));

    monitor(_multiplicity);
    monitor(_pt);
    monitor(_leading_pt);
}

void ElectronsMonitor::fill(const Electrons &electrons)
{
    multiplicity()->fill(electrons.size());

    double max_el_pt = 0;
    double el_pt = 0;
    for(Electrons::const_iterator electron = electrons.begin();
            electrons.end() != electron;
            ++electron)
    {
        el_pt = bsm::pt(electron->physics_object().p4());

        pt()->fill(el_pt);

        if (el_pt <= max_el_pt)
            continue;

        max_el_pt = el_pt;
    }

    if (max_el_pt)
        leading_pt()->fill(max_el_pt);
}

const H1Ptr ElectronsMonitor::multiplicity() const
{
    return _multiplicity->histogram();
}

const H1Ptr ElectronsMonitor::pt() const
{
    return _pt->histogram();
}

const H1Ptr ElectronsMonitor::leading_pt() const
{
    return _leading_pt->histogram();
}

uint32_t ElectronsMonitor::id() const
{
    return core::ID<ElectronsMonitor>::get();
}

ElectronsMonitor::ObjectPtr ElectronsMonitor::clone() const
{
    return ObjectPtr(new ElectronsMonitor(*this));
}

void ElectronsMonitor::print(std::ostream &out) const
{
    out << setw(16) << left << " [multiplicity]" << *multiplicity() << endl;
    out << setw(16) << left << " [pt]" << *pt() << endl;
    out << setw(16) << left << " [leading pt] " << *leading_pt();
}



// Gen Particle Monitor
//
GenParticleMonitor::GenParticleMonitor()
{
    _pdg_id.reset(new H1Proxy(100, -50, 50));
    _status.reset(new H1Proxy(10, 0, 10));
    _pt.reset(new H1Proxy(100, 0, 100));

    monitor(_pdg_id);
    monitor(_status);
    monitor(_pt);
}

GenParticleMonitor::GenParticleMonitor(const GenParticleMonitor &object)
{
    _pdg_id.reset(new H1Proxy(*object._pdg_id));
    _status.reset(new H1Proxy(*object._status));
    _pt.reset(new H1Proxy(*object._pt));

    monitor(_pdg_id);
    monitor(_status);
    monitor(_pt);
}

void GenParticleMonitor::fill(const GenParticle &particle)
{
    pdgid()->fill(particle.id());
    status()->fill(particle.status());

    pt()->fill(bsm::pt(particle.physics_object().p4()));
}

const GenParticleMonitor::H1Ptr GenParticleMonitor::pdgid() const
{
    return _pdg_id->histogram();
}

const GenParticleMonitor::H1Ptr GenParticleMonitor::status() const
{
    return _status->histogram();
}

const GenParticleMonitor::H1Ptr GenParticleMonitor::pt() const
{
    return _pt->histogram();
}

uint32_t GenParticleMonitor::id() const
{
    return core::ID<GenParticleMonitor>::get();
}

GenParticleMonitor::ObjectPtr GenParticleMonitor::clone() const
{
    return ObjectPtr(new GenParticleMonitor(*this));
}

void GenParticleMonitor::print(std::ostream &out) const
{
    out << setw(10) << left << " [PDG id]" << *pdgid() << endl;
    out << setw(10) << left << " [status]" << *status() << endl;
    out << setw(10) << left << " [pt] " << *pt();

}



// Jets Monitor
//
JetsMonitor::JetsMonitor()
{
    _multiplicity.reset(new H1Proxy(10, 0, 10));
    _pt.reset(new H1Proxy(100, 0, 100));
    _uncorrected_pt.reset(new H1Proxy(100, 0, 100));
    _leading_pt.reset(new H1Proxy(100, 0, 100));
    _leading_uncorrected_pt.reset(new H1Proxy(100, 0, 100));
    _children.reset(new H1Proxy(10, 0, 10));

    monitor(_multiplicity);
    monitor(_pt);
    monitor(_uncorrected_pt);
    monitor(_leading_pt);
    monitor(_leading_uncorrected_pt);
    monitor(_children);
}

JetsMonitor::JetsMonitor(const JetsMonitor &object)
{
    _multiplicity.reset(new H1Proxy(*object._multiplicity));
    _pt.reset(new H1Proxy(*object._pt));
    _uncorrected_pt.reset(new H1Proxy(*object._uncorrected_pt));
    _leading_pt.reset(new H1Proxy(*object._leading_pt));
    _leading_uncorrected_pt.reset(new H1Proxy(*object._leading_uncorrected_pt));
    _children.reset(new H1Proxy(*object._children));

    monitor(_multiplicity);
    monitor(_pt);
    monitor(_uncorrected_pt);
    monitor(_leading_pt);
    monitor(_leading_uncorrected_pt);
    monitor(_children);
}

void JetsMonitor::fill(const Jets &jets)
{
    multiplicity()->fill(jets.size());

    double max_jet_pt = 0;
    double max_jet_uncorrected_pt = 0;
    double jet_pt = 0;
    double jet_uncorrected_pt = 0;
    for(Jets::const_iterator jet = jets.begin();
            jets.end() != jet;
            ++jet)
    {
        children()->fill(jet->children().size());

        jet_pt = bsm::pt(jet->physics_object().p4());
        jet_uncorrected_pt = 0;

        pt()->fill(jet_pt);

        if (jet->has_uncorrected_p4())
        {
            jet_uncorrected_pt = bsm::pt(jet->uncorrected_p4());

            uncorrected_pt()->fill(jet_uncorrected_pt);
        }

        if (jet_pt <= max_jet_pt)
            continue;

        max_jet_pt = jet_pt;
        max_jet_uncorrected_pt = jet_uncorrected_pt;
    }

    if (max_jet_pt)
    {
        leading_pt()->fill(max_jet_pt);
        leading_uncorrected_pt()->fill(max_jet_uncorrected_pt);
    }
}

const H1Ptr JetsMonitor::multiplicity() const
{
    return _multiplicity->histogram();
}

const H1Ptr JetsMonitor::pt() const
{
    return _pt->histogram();
}

const H1Ptr JetsMonitor::uncorrected_pt() const
{
    return _uncorrected_pt->histogram();
}

const H1Ptr JetsMonitor::leading_pt() const
{
    return _leading_pt->histogram();
}

const H1Ptr JetsMonitor::leading_uncorrected_pt() const
{
    return _leading_uncorrected_pt->histogram();
}

const H1Ptr JetsMonitor::children() const
{
    return _children->histogram();
}

uint32_t JetsMonitor::id() const
{
    return core::ID<JetsMonitor>::get();
}

JetsMonitor::ObjectPtr JetsMonitor::clone() const
{
    return ObjectPtr(new JetsMonitor(*this));
}

void JetsMonitor::print(std::ostream &out) const
{
    out << setw(16) << left << " [multiplicity]"
        << *multiplicity() << endl;
    out << setw(16) << left << " [pt]" << *pt() << endl;
    out << setw(16) << left << " [uncorrected pt]" << *uncorrected_pt() << endl;
    out << setw(16) << left << " [leading uncorrected pt] " << *leading_uncorrected_pt()
        << endl;
    out << setw(16) << left << " [leading pt] " << *leading_pt()
        << endl;
    out << setw(16) << left << " [children]" << *children();
}



// Lorentz Vector Monitor
//
LorentzVectorMonitor::LorentzVectorMonitor()
{
    _energy.reset(new H1Proxy(100, 0, 100));
    _px.reset(new H1Proxy(100, 0, 100));
    _py.reset(new H1Proxy(100, 0, 100));
    _pz.reset(new H1Proxy(100, 0, 100));
    _pt.reset(new H1Proxy(100, 0, 500));
    _eta.reset(new H1Proxy(100, -5, 5));
    _phi.reset(new H1Proxy(80, -4, 4));
    _mass.reset(new H1Proxy(60, 0, 300));

    monitor(_energy);
    monitor(_px);
    monitor(_py);
    monitor(_pz);
    monitor(_pt);
    monitor(_eta);
    monitor(_phi);
    monitor(_mass);

    _p4.reset(new TLorentzVector());
}

LorentzVectorMonitor::LorentzVectorMonitor(const LorentzVectorMonitor &object)
{
    _energy.reset(new H1Proxy(*object._energy));
    _px.reset(new H1Proxy(*object._px));
    _py.reset(new H1Proxy(*object._py));
    _pz.reset(new H1Proxy(*object._pz));
    _pt.reset(new H1Proxy(*object._pt));
    _eta.reset(new H1Proxy(*object._eta));
    _phi.reset(new H1Proxy(*object._phi));
    _mass.reset(new H1Proxy(*object._mass));

    monitor(_energy);
    monitor(_px);
    monitor(_py);
    monitor(_pz);
    monitor(_pt);
    monitor(_eta);
    monitor(_phi);
    monitor(_mass);

    _p4.reset(new TLorentzVector());
}

void LorentzVectorMonitor::fill(const LorentzVector &p4)
{
    energy()->fill(p4.e());
    px()->fill(p4.px());
    py()->fill(p4.py());
    pz()->fill(p4.pz());

    utility::set(_p4.get(), &p4);
    pt()->fill(_p4->Pt());
    eta()->fill(_p4->Eta());
    phi()->fill(_p4->Phi());
    mass()->fill(_p4->M());
}

const H1Ptr LorentzVectorMonitor::energy() const
{
    return _energy->histogram();
}

const H1Ptr LorentzVectorMonitor::px() const
{
    return _px->histogram();
}

const H1Ptr LorentzVectorMonitor::py() const
{
    return _py->histogram();
}
 
const H1Ptr LorentzVectorMonitor::pz() const
{
    return _pz->histogram();
}

const H1Ptr LorentzVectorMonitor::pt() const
{
    return _pt->histogram();
}

const H1Ptr LorentzVectorMonitor::eta() const
{
    return _eta->histogram();
}

const H1Ptr LorentzVectorMonitor::phi() const
{
    return _phi->histogram();
}

const H1Ptr LorentzVectorMonitor::mass() const
{
    return _mass->histogram();
}

uint32_t LorentzVectorMonitor::id() const
{
    return core::ID<LorentzVectorMonitor>::get();
}

LorentzVectorMonitor::ObjectPtr LorentzVectorMonitor::clone() const
{
    return ObjectPtr(new LorentzVectorMonitor(*this));
}

void LorentzVectorMonitor::print(std::ostream &out) const
{
    out << setw(16) << left << " [e]" << *energy() << endl;
    out << setw(16) << left << " [px]" << *px() << endl;
    out << setw(16) << left << " [py]" << *py() << endl;
    out << setw(16) << left << " [pz]" << *pz() << endl;
    out << setw(16) << left << " [pt]" << *pt() << endl;
    out << setw(16) << left << " [eta]" << *eta() << endl;
    out << setw(16) << left << " [phi]" << *phi() << endl;
    out << setw(16) << left << " [mass]" << *mass();
}



// Missing Energy Monitor
//
MissingEnergyMonitor::MissingEnergyMonitor()
{
    _pt.reset(new H1Proxy(100, 0, 100));
    _x.reset(new H1Proxy(100, -50, 50));
    _y.reset(new H1Proxy(100, -50, 50));
    _z.reset(new H1Proxy(100, -50, 50));

    monitor(_pt);
    monitor(_x);
    monitor(_y);
    monitor(_z);
}

MissingEnergyMonitor::MissingEnergyMonitor(const MissingEnergyMonitor &object)
{
    _pt.reset(new H1Proxy(*object._pt));
    _x.reset(new H1Proxy(*object._x));
    _y.reset(new H1Proxy(*object._y));
    _z.reset(new H1Proxy(*object._z));

    monitor(_pt);
    monitor(_x);
    monitor(_y);
    monitor(_z);
}

void MissingEnergyMonitor::fill(const MissingEnergy &missing_energy)
{
    pt()->fill(bsm::pt(missing_energy.p4()));
}

const H1Ptr MissingEnergyMonitor::pt() const
{
    return _pt->histogram();
}

const H1Ptr MissingEnergyMonitor::x() const
{
    return _x->histogram();
}

const H1Ptr MissingEnergyMonitor::y() const
{
    return _y->histogram();
}

const H1Ptr MissingEnergyMonitor::z() const
{
    return _z->histogram();
}

uint32_t MissingEnergyMonitor::id() const
{
    return core::ID<MissingEnergyMonitor>::get();
}

MissingEnergyMonitor::ObjectPtr MissingEnergyMonitor::clone() const
{
    return ObjectPtr(new MissingEnergyMonitor(*this));
}

void MissingEnergyMonitor::print(std::ostream &out) const
{
    out << setw(16) << left << " [pt]" << *pt() << endl;
    out << setw(16) << left << " [x]" << *x() << endl;
    out << setw(16) << left << " [y]" << *y() << endl;
    out << setw(16) << left << " [z]" << *z();
}



// Muons Monitor
//
MuonsMonitor::MuonsMonitor()
{
    _multiplicity.reset(new H1Proxy(10, 0, 10));
    _pt.reset(new H1Proxy(100, 0, 100));
    _leading_pt.reset(new H1Proxy(100, 0, 100));

    monitor(_multiplicity);
    monitor(_pt);
    monitor(_leading_pt);
}

MuonsMonitor::MuonsMonitor(const MuonsMonitor &object)
{
    _multiplicity.reset(new H1Proxy(*object._multiplicity));
    _pt.reset(new H1Proxy(*object._pt));
    _leading_pt.reset(new H1Proxy(*object._leading_pt));

    monitor(_multiplicity);
    monitor(_pt);
    monitor(_leading_pt);
}

void MuonsMonitor::fill(const Muons &muons)
{
    multiplicity()->fill(muons.size());

    double max_muon_pt = 0;
    double muon_pt = 0;
    for(Muons::const_iterator muon = muons.begin();
            muons.end() != muon;
            ++muon)
    {
        muon_pt = bsm::pt(muon->physics_object().p4());

        pt()->fill(muon_pt);

        if (muon_pt <= max_muon_pt)
            continue;

        max_muon_pt = muon_pt;
    }

    if (max_muon_pt)
        leading_pt()->fill(max_muon_pt);
}

const H1Ptr MuonsMonitor::multiplicity() const
{
    return _multiplicity->histogram();
}

const H1Ptr MuonsMonitor::pt() const
{
    return _pt->histogram();
}

const H1Ptr MuonsMonitor::leading_pt() const
{
    return _leading_pt->histogram();
}

uint32_t MuonsMonitor::id() const
{
    return core::ID<MuonsMonitor>::get();
}

MuonsMonitor::ObjectPtr MuonsMonitor::clone() const
{
    return ObjectPtr(new MuonsMonitor(*this));
}

void MuonsMonitor::print(std::ostream &out) const
{
    out << setw(16) << left << " [multiplicity]"
        << *multiplicity() << endl;
    out << setw(16) << left << " [pt]" << *pt() << endl;
    out << setw(16) << left << " [leading pt] " << *leading_pt();
}



// Primary Vertices Monitor
//
PrimaryVerticesMonitor::PrimaryVerticesMonitor()
{
    _multiplicity.reset(new H1Proxy(20, 0, 20));
    _x.reset(new H1Proxy(200, -.1, .1));
    _y.reset(new H1Proxy(200, -.1, .1));
    _z.reset(new H1Proxy(100, -50, 50));

    monitor(_multiplicity);
    monitor(_x);
    monitor(_y);
    monitor(_z);
}

PrimaryVerticesMonitor::PrimaryVerticesMonitor(const PrimaryVerticesMonitor &object)
{
    _multiplicity.reset(new H1Proxy(*object._multiplicity));
    _x.reset(new H1Proxy(*object._x));
    _y.reset(new H1Proxy(*object._y));
    _z.reset(new H1Proxy(*object._z));

    monitor(_multiplicity);
    monitor(_x);
    monitor(_y);
    monitor(_z);
}

void PrimaryVerticesMonitor::fill(const PrimaryVertices &primary_vertices)
{
    multiplicity()->fill(primary_vertices.size());

    for(PrimaryVertices::const_iterator primary_vertex = primary_vertices.begin();
            primary_vertices.end() != primary_vertex;
            ++primary_vertex)
    {
        x()->fill(primary_vertex->vertex().x());
        y()->fill(primary_vertex->vertex().y());
        z()->fill(primary_vertex->vertex().z());
    }
}

const H1Ptr PrimaryVerticesMonitor::multiplicity() const
{
    return _multiplicity->histogram();
}

const H1Ptr PrimaryVerticesMonitor::x() const
{
    return _x->histogram();
}

const H1Ptr PrimaryVerticesMonitor::y() const
{
    return _y->histogram();
}

const H1Ptr PrimaryVerticesMonitor::z() const
{
    return _z->histogram();
}

uint32_t PrimaryVerticesMonitor::id() const
{
    return core::ID<PrimaryVerticesMonitor>::get();
}

PrimaryVerticesMonitor::ObjectPtr PrimaryVerticesMonitor::clone() const
{
    return ObjectPtr(new PrimaryVerticesMonitor(*this));
}

void PrimaryVerticesMonitor::print(std::ostream &out) const
{
    out << setw(16) << left << " [multiplicity]"
        << *multiplicity() << endl;
    out << setw(16) << left << " [x]" << *x() << endl;
    out << setw(16) << left << " [y]" << *y() << endl;
    out << setw(16) << left << " [z]" << *z();
}
