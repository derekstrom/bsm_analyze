// Common Selectors Classes/functions
//
// Physics objects selectors: muons, electrons, jets
//
// Created by Samvel Khalatyan, May 16, 2011
// Copyright 2011, All rights reserved

#include <cmath>
#include <functional>
#include <iomanip>
#include <ostream>

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Jet.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "bsm_input/interface/PrimaryVertex.pb.h"
#include "interface/Cut.h"
#include "interface/Selector.h"
#include "interface/Utility.h"

using std::endl;
using std::left;
using std::right;
using std::ostream;
using std::setw;
using std::setfill;

using boost::dynamic_pointer_cast;

using bsm::CutPtr;
using bsm::ElectronSelector;
using bsm::JetSelector;
using bsm::MultiplicityCutflow;
using bsm::MuonSelector;
using bsm::PrimaryVertexSelector;
using bsm::WJetSelector;
using bsm::LockSelectorEventCounterOnUpdate;

// ElectronSelector
//
ElectronSelector::ElectronSelector()
{
    _et.reset(new Comparator<>(30, "Et"));
    _eta.reset(new Comparator<std::less<double> >(2.5, "|eta|"));
    _primary_vertex.reset(new Comparator<std::less<double> >(1, "|el.z() - pv.z()|"));

    monitor(_et);
    monitor(_eta);
    monitor(_primary_vertex);
}

ElectronSelector::ElectronSelector(const ElectronSelector &object)
{
    _et = dynamic_pointer_cast<Cut>(object._et->clone());
    _eta = dynamic_pointer_cast<Cut>(object._eta->clone());
    _primary_vertex = dynamic_pointer_cast<Cut>(object._primary_vertex->clone());

    monitor(_et);
    monitor(_eta);
    monitor(_primary_vertex);
}

bool ElectronSelector::apply(const Electron &electron, const PrimaryVertex &pv)
{
    return _et->apply(bsm::et(electron.physics_object().p4()))
        && _eta->apply(fabs(bsm::eta(electron.physics_object().p4())))
        && _primary_vertex->apply(fabs(electron.physics_object().vertex().z()
                    - pv.vertex().z()));
}

CutPtr ElectronSelector::et() const
{
    return _et;
}

CutPtr ElectronSelector::eta() const
{
    return _eta;
}

CutPtr ElectronSelector::primary_vertex() const
{
    return _primary_vertex;
}

void ElectronSelector::enable()
{
    et()->enable();
    eta()->enable();
    primary_vertex()->enable();
}

void ElectronSelector::disable()
{
    et()->disable();
    eta()->disable();
    primary_vertex()->disable();
}

uint32_t ElectronSelector::id() const
{
    return core::ID<ElectronSelector>::get();
}

ElectronSelector::ObjectPtr ElectronSelector::clone() const
{
    return ObjectPtr(new ElectronSelector(*this));
}

void ElectronSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << *_et << endl;
    out << *_eta << endl;
    out << *_primary_vertex;
}



// JetSelector
//
JetSelector::JetSelector()
{
    _pt.reset(new Comparator<>(50, "Pt"));
    _eta.reset(new Comparator<std::less<double> >(2.4, "|eta|"));

    monitor(_pt);
    monitor(_eta);
}

JetSelector::JetSelector(const JetSelector &object)
{
    _pt = dynamic_pointer_cast<Cut>(object._pt->clone());;
    _eta = dynamic_pointer_cast<Cut>(object._eta->clone());;

    monitor(_pt);
    monitor(_eta);
}

bool JetSelector::apply(const Jet &jet)
{
    return _pt->apply(bsm::pt(jet.physics_object().p4()))
        && _eta->apply(fabs(bsm::eta(jet.physics_object().p4())));
}

CutPtr JetSelector::pt() const
{
    return _pt;
}

CutPtr JetSelector::eta() const
{
    return _eta;
}

void JetSelector::enable()
{
    pt()->enable();
    eta()->enable();
}

void JetSelector::disable()
{
    pt()->disable();
    eta()->disable();
}

uint32_t JetSelector::id() const
{
    return core::ID<JetSelector>::get();
}

JetSelector::ObjectPtr JetSelector::clone() const
{
    return ObjectPtr(new JetSelector(*this));
}

void JetSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << *_pt << endl;
    out << *_eta;
}



// Multiplicity Cutflow
//
MultiplicityCutflow::MultiplicityCutflow(const uint32_t &max)
{
    for(uint32_t i = 0; max > i; ++i)
    {
        _cuts.push_back(CutPtr(new Comparator<std::equal_to<uint32_t> >(i)));
    }
    _cuts.push_back(CutPtr(new Comparator<std::greater_equal<uint32_t> >(max)));

    for(Cuts::const_iterator cut = _cuts.begin();
            _cuts.end() != cut;
            ++cut)
    {
        monitor(*cut);
    }
}

MultiplicityCutflow::MultiplicityCutflow(const MultiplicityCutflow &object)
{
    for(Cuts::const_iterator cut = object._cuts.begin();
            object._cuts.end() != cut;
            ++cut)
    {
        const CutPtr clone = dynamic_pointer_cast<Cut>((*cut)->clone());

        _cuts.push_back(clone);
        monitor(clone);
    }
}

void MultiplicityCutflow::apply(const uint32_t &number)
{
    // It does not make sense to apply all cuts. Only Nth one:
    //
    if (_cuts.size() > number)
        _cuts[number]->apply(number);
    else
        (*--_cuts.end())->apply(number);
}

bsm::CutPtr MultiplicityCutflow::cut(const uint32_t &cut) const
{
    return cut >= _cuts.size()
        ? *(--_cuts.end())
        : _cuts[cut];
}

void MultiplicityCutflow::enable()
{
    for(Cuts::const_iterator cut = _cuts.begin();
            _cuts.end() != cut;
            ++cut)
    {
        (*cut)->enable();
    }
}

void MultiplicityCutflow::disable()
{
    for(Cuts::const_iterator cut = _cuts.begin();
            _cuts.end() != cut;
            ++cut)
    {
        (*cut)->disable();
    }
}

uint32_t MultiplicityCutflow::id() const
{
    return core::ID<MultiplicityCutflow>::get();
}

MultiplicityCutflow::ObjectPtr MultiplicityCutflow::clone() const
{
    return ObjectPtr(new MultiplicityCutflow(*this));
}

void MultiplicityCutflow::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    for(uint32_t cut = 0, max = _cuts.size() - 1; max > cut; ++cut)
    {
        out << *_cuts[cut] << endl;
    }
    out << **(_cuts.end() - 1);
}



// Muon Selector
//
MuonSelector::MuonSelector()
{
    _pt.reset(new Comparator<>(30, "pT"));
    _eta.reset(new Comparator<std::less<double> >(2.1, "|eta|"));
    _is_global.reset(new Comparator<std::logical_and<bool> >(true, "is Global"));
    _is_tracker.reset(new Comparator<std::logical_and<bool> >(true, "is Tracker"));
    _muon_segments.reset(new Comparator<>(1, "Muon Segments"));
    _muon_hits.reset(new Comparator<>(0, "Muon Hits"));
    _muon_normalized_chi2.reset(new Comparator<std::less<double> >(10, "Muon Chi2 / ndof"));
    _tracker_hits.reset(new Comparator<>(10, "Tracker hits"));
    _pixel_hits.reset(new Comparator<>(0, "Pixel hits"));
    _d0_bsp.reset(new Comparator<std::less<double> >(0.02, "|d0_bsp|"));
    _primary_vertex.reset(new Comparator<std::less<double> >(1, "|mu.z() - pv.z()|"));

    monitor(_pt);
    monitor(_eta);
    monitor(_is_global);
    monitor(_is_tracker);
    monitor(_muon_segments);
    monitor(_muon_hits);
    monitor(_muon_normalized_chi2);
    monitor(_tracker_hits);
    monitor(_pixel_hits);
    monitor(_d0_bsp);
    monitor(_primary_vertex);
}

MuonSelector::MuonSelector(const MuonSelector &object)
{
    _pt = dynamic_pointer_cast<Cut>(object._pt->clone());
    _eta = dynamic_pointer_cast<Cut>(object._eta->clone());
    _is_global = dynamic_pointer_cast<Cut>(object._is_global->clone());
    _is_tracker = dynamic_pointer_cast<Cut>(object._is_tracker->clone());
    _muon_segments = dynamic_pointer_cast<Cut>(object._muon_segments->clone());
    _muon_hits = dynamic_pointer_cast<Cut>(object._muon_hits->clone());
    _muon_normalized_chi2 = dynamic_pointer_cast<Cut>(object._muon_normalized_chi2->clone());
    _tracker_hits = dynamic_pointer_cast<Cut>(object._tracker_hits->clone());
    _pixel_hits = dynamic_pointer_cast<Cut>(object._pixel_hits->clone());
    _d0_bsp = dynamic_pointer_cast<Cut>(object._d0_bsp->clone());
    _primary_vertex = dynamic_pointer_cast<Cut>(object._primary_vertex->clone());

    monitor(_pt);
    monitor(_eta);
    monitor(_is_global);
    monitor(_is_tracker);
    monitor(_muon_segments);
    monitor(_muon_hits);
    monitor(_muon_normalized_chi2);
    monitor(_tracker_hits);
    monitor(_pixel_hits);
    monitor(_d0_bsp);
    monitor(_primary_vertex);
}

bool MuonSelector::apply(const Muon &muon, const PrimaryVertex &pv)
{
    return _pt->apply(bsm::pt(muon.physics_object().p4()))
        && _eta->apply(fabs(bsm::eta(muon.physics_object().p4())))
        && _is_global->apply(muon.extra().is_global())
        && _is_tracker->apply(muon.extra().is_tracker())
        && _muon_segments->apply(muon.extra().number_of_matches())
        && _muon_hits->apply(muon.global_track().hits())
        && _muon_normalized_chi2->apply(muon.global_track().normalized_chi2())
        && _tracker_hits->apply(muon.inner_track().hits())
        && _pixel_hits->apply(muon.extra().pixel_hits())
        && _d0_bsp->apply(fabs(muon.extra().d0_bsp()))
        && _primary_vertex->apply(fabs(muon.physics_object().vertex().z() - pv.vertex().z()));
}

CutPtr MuonSelector::pt() const
{
    return _pt;
}

CutPtr MuonSelector::eta() const
{
    return _eta;
}

CutPtr MuonSelector::is_global() const
{
    return _is_global;
}

CutPtr MuonSelector::is_tracker() const
{
    return _is_tracker;
}

CutPtr MuonSelector::muon_segments() const
{
    return _muon_segments;
}

CutPtr MuonSelector::muon_hits() const
{
    return _muon_hits;
}

CutPtr MuonSelector::muon_normalized_chi2() const
{
    return _muon_normalized_chi2;
}

CutPtr MuonSelector::tracker_hits() const
{
    return _tracker_hits;
}

CutPtr MuonSelector::pixel_hits() const
{
    return _pixel_hits;
}

CutPtr MuonSelector::d0_bsp() const
{
    return _d0_bsp;
}

CutPtr MuonSelector::primary_vertex() const
{
    return _primary_vertex;
}

void MuonSelector::enable()
{
    pt()->enable();
    eta()->enable();
    is_global()->enable();
    is_tracker()->enable();
    muon_segments()->enable();
    muon_hits()->enable();
    muon_normalized_chi2()->enable();
    tracker_hits()->enable();
    pixel_hits()->enable();
    d0_bsp()->enable();
    primary_vertex()->enable();
}

void MuonSelector::disable()
{
    pt()->disable();
    eta()->disable();
    is_global()->disable();
    is_tracker()->disable();
    muon_segments()->disable();
    muon_hits()->disable();
    muon_normalized_chi2()->disable();
    tracker_hits()->disable();
    pixel_hits()->disable();
    d0_bsp()->disable();
    primary_vertex()->disable();
}

uint32_t MuonSelector::id() const
{
    return core::ID<MuonSelector>::get();
}

MuonSelector::ObjectPtr MuonSelector::clone() const
{
    return ObjectPtr(new MuonSelector(*this));
}

void MuonSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << *_pt << endl;
    out << *_eta << endl;
    out << *_is_global << endl;
    out << *_is_tracker << endl;
    out << *_muon_segments << endl;
    out << *_muon_hits << endl;
    out << *_muon_normalized_chi2 << endl;
    out << *_tracker_hits << endl;
    out << *_pixel_hits << endl;
    out << *_d0_bsp << endl;
    out << *_primary_vertex;
}



// PrimaryVertex Selector
//
PrimaryVertexSelector::PrimaryVertexSelector()
{
    _ndof.reset(new Comparator<std::greater_equal<float> >(4, "ndof"));
    _vertex_z.reset(new Comparator<std::less_equal<float> >(24, "|pv.z()|"));
    _rho.reset(new Comparator<std::less_equal<float> >(4.0, "|pv.rho()|"));

    monitor(_ndof);
    monitor(_vertex_z);
    monitor(_rho);
}

PrimaryVertexSelector::PrimaryVertexSelector(const PrimaryVertexSelector &object)
{
    _ndof = dynamic_pointer_cast<Cut>(object._ndof->clone());
    _vertex_z = dynamic_pointer_cast<Cut>(object._vertex_z->clone());
    _rho = dynamic_pointer_cast<Cut>(object._rho->clone());

    monitor(_ndof);
    monitor(_vertex_z);
    monitor(_rho);
}

bool PrimaryVertexSelector::apply(const PrimaryVertex &pv)
{
    return _ndof->apply(pv.extra().ndof())
        && _vertex_z->apply(pv.vertex().z())
        && _rho->apply(pv.extra().rho());
}

CutPtr PrimaryVertexSelector::ndof() const
{
    return _ndof;
}

CutPtr PrimaryVertexSelector::vertex_z() const
{
    return _vertex_z;
}

CutPtr PrimaryVertexSelector::rho() const
{
    return _rho;
}

void PrimaryVertexSelector::enable()
{
    ndof()->enable();
    vertex_z()->enable();
    rho()->enable();
}

void PrimaryVertexSelector::disable()
{
    ndof()->disable();
    vertex_z()->disable();
    rho()->disable();
}

uint32_t PrimaryVertexSelector::id() const
{
    return core::ID<PrimaryVertexSelector>::get();
}

PrimaryVertexSelector::ObjectPtr PrimaryVertexSelector::clone() const
{
    return ObjectPtr(new PrimaryVertexSelector(*this));
}

void PrimaryVertexSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << *_ndof << endl;
    out << *_vertex_z << endl;
    out << *_rho;
}



// WJetSelector
//
WJetSelector::WJetSelector()
{
    _children.reset(new Comparator<std::equal_to<uint32_t> >(2, "Children"));
    _pt.reset(new Comparator<>(200, "pT"));
    _mass_drop.reset(new Comparator<std::less<double> >(0.4, "Mass drop"));
    _mass_lower_bound.reset(new Comparator<>(60, "Mass lower bound"));
    _mass_upper_bound.reset(new Comparator<std::less<double> >(130, "Mass upper bound"));

    monitor(_children);
    monitor(_pt);
    monitor(_mass_drop);
    monitor(_mass_lower_bound);
    monitor(_mass_upper_bound);
}

WJetSelector::WJetSelector(const WJetSelector &object)
{
    _children = dynamic_pointer_cast<Cut>(object._children->clone());;
    _pt = dynamic_pointer_cast<Cut>(object._pt->clone());;
    _mass_drop = dynamic_pointer_cast<Cut>(object._mass_drop->clone());;
    _mass_lower_bound = dynamic_pointer_cast<Cut>(object._mass_lower_bound->clone());;
    _mass_upper_bound = dynamic_pointer_cast<Cut>(object._mass_upper_bound->clone());;

    monitor(_children);
    monitor(_pt);
    monitor(_mass_drop);
    monitor(_mass_lower_bound);
    monitor(_mass_upper_bound);
}

bool WJetSelector::apply(const Jet &jet)
{
    if (!_children->apply(jet.children().size()))
        return false;

    if (!_pt->apply(bsm::pt(jet.physics_object().p4())))
        return false;

    double m0 = bsm::mass(jet.physics_object().p4());
    double m1 = bsm::mass(jet.children().Get(0).physics_object().p4());
    double m2 = bsm::mass(jet.children().Get(1).physics_object().p4());
    double m12 = bsm::mass(jet.children().Get(0).physics_object().p4()
            + jet.children().Get(1).physics_object().p4());

    return _mass_drop->apply(std::max(m1, m2) / m0)
        && _mass_lower_bound->apply(m12)
        && _mass_upper_bound->apply(m12);
}

CutPtr WJetSelector::children() const
{
    return _children;
}

CutPtr WJetSelector::pt() const
{
    return _pt;
}

CutPtr WJetSelector::mass_drop() const
{
    return _mass_drop;
}

CutPtr WJetSelector::mass_lower_bound() const
{
    return _mass_lower_bound;
}

CutPtr WJetSelector::mass_upper_bound() const
{
    return _mass_upper_bound;
}

void WJetSelector::enable()
{
    children()->enable();
    pt()->enable();
    mass_drop()->enable();
    mass_lower_bound()->enable();
    mass_upper_bound()->enable();
}

void WJetSelector::disable()
{
    children()->disable();
    pt()->disable();
    mass_drop()->disable();
    mass_lower_bound()->disable();
    mass_upper_bound()->disable();
}

uint32_t WJetSelector::id() const
{
    return core::ID<WJetSelector>::get();
}

WJetSelector::ObjectPtr WJetSelector::clone() const
{
    return ObjectPtr(new WJetSelector(*this));
}

void WJetSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << *_children << endl;
    out << *_pt << endl;
    out << *_mass_drop << endl;
    out << *_mass_lower_bound << endl;
    out << *_mass_upper_bound;
}



// Lock Selector Event Counter on Update
//
LockSelectorEventCounterOnUpdate::LockSelectorEventCounterOnUpdate(
        ElectronSelector &selector)
{
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.et()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.eta()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.primary_vertex()->events())));
}

LockSelectorEventCounterOnUpdate::LockSelectorEventCounterOnUpdate(
        JetSelector &selector)
{
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.pt()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.eta()->events())));
}

LockSelectorEventCounterOnUpdate::LockSelectorEventCounterOnUpdate(
        MuonSelector &selector)
{
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.pt()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.eta()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.is_global()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.is_tracker()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.muon_segments()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.muon_hits()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.muon_normalized_chi2()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.tracker_hits()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.pixel_hits()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.d0_bsp()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.primary_vertex()->events())));
}

LockSelectorEventCounterOnUpdate::LockSelectorEventCounterOnUpdate(
        WJetSelector &selector)
{
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.children()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.pt()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.mass_drop()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.mass_lower_bound()->events())));
    _lockers.push_back(Locker(
                new LockCounterOnUpdate(selector.mass_upper_bound()->events())));
}
