// Common Selectors Classes/functions
//
// Physics objects selectors: muons, electrons, jets
//
// Created by Samvel Khalatyan, May 16, 2011
// Copyright 2011, All rights reserved

#include <cmath>
#include <iomanip>
#include <ostream>

#include <boost/pointer_cast.hpp>

#include <TLorentzVector.h>

#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Jet.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "bsm_input/interface/PrimaryVertex.pb.h"
#include "interface/Selector.h"
#include "interface/Utility.h"

using std::endl;
using std::left;
using std::right;
using std::ostream;
using std::setw;
using std::setfill;

using bsm::selector::Cut;
using bsm::selector::Comparator;
using bsm::selector::Counter;
using bsm::selector::CounterLock;
using bsm::selector::CounterLockOnUpdate;
using bsm::selector::ElectronSelector;
using bsm::selector::JetSelector;
using bsm::selector::LockSelectorEventCounterOnUpdate;
using bsm::selector::MuonSelector;
using bsm::selector::MultiplicityCutflow;
using bsm::selector::Selector;
using bsm::selector::WJetSelector;

// ElectronSelector
//
ElectronSelector::ElectronSelector()
{
    _et.reset(new Comparator<>(30));
    _eta.reset(new Comparator<std::less<double> >(2.5));
    _primary_vertex.reset(new Comparator<std::less<double> >(1));

    _p4.reset(new TLorentzVector());
}

ElectronSelector::~ElectronSelector()
{
}

bool ElectronSelector::operator()(const Electron &electron,
        const PrimaryVertex &pv)
{
    utility::set(_p4.get(), &electron.physics_object().p4());

    return _et->operator()(_p4->Et())
        && _eta->operator()(fabs(_p4->Eta()))
        && _primary_vertex->operator()(fabs(electron.physics_object().vertex().z() - pv.vertex().z()));
}

ElectronSelector::CutPtr ElectronSelector::et() const
{
    return _et;
}

ElectronSelector::CutPtr ElectronSelector::eta() const
{
    return _eta;
}

ElectronSelector::CutPtr ElectronSelector::primary_vertex() const
{
    return _primary_vertex;
}

void ElectronSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << " [+]                Et > " << *_et << endl;
    out << " [+]             |eta| < " << *_eta << endl;
    out << " [+] |el.z() - pv.z()| < " << *_primary_vertex << endl;
}

Selector::SelectorPtr ElectronSelector::clone() const
{
    boost::shared_ptr<ElectronSelector>
        selector(new ElectronSelector());

    *selector->et() = *et();
    *selector->eta() = *eta();
    *selector->primary_vertex() = *primary_vertex();

    return selector;
}

void ElectronSelector::merge(const SelectorPtr &selector)
{
    if (!selector)
        return;

    boost::shared_ptr<ElectronSelector> el_selector =
        boost::dynamic_pointer_cast<ElectronSelector>(selector);
    if (!el_selector)
        return;

    *et() += *el_selector->et();
    *eta() += *el_selector->eta();
    *primary_vertex() += *el_selector->primary_vertex();
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



// JetSelector
//
JetSelector::JetSelector()
{
    _pt.reset(new Comparator<>(50));
    _eta.reset(new Comparator<std::less<double> >(2.4));

    _p4.reset(new TLorentzVector());
}

JetSelector::~JetSelector()
{
}

bool JetSelector::operator()(const Jet &jet)
{
    utility::set(_p4.get(), &jet.physics_object().p4());

    return _pt->operator()(_p4->Pt())
        && _eta->operator()(fabs(_p4->Eta()));
}

JetSelector::CutPtr JetSelector::pt() const
{
    return _pt;
}

JetSelector::CutPtr JetSelector::eta() const
{
    return _eta;
}

void JetSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << " [+]                Pt > " << *_pt << endl;
    out << " [+]             |eta| < " << *_eta << endl;
}

Selector::SelectorPtr JetSelector::clone() const
{
    boost::shared_ptr<JetSelector> selector(new JetSelector());

    *selector->pt() = *pt();
    *selector->eta() = *eta();

    return selector;
}

void JetSelector::merge(const SelectorPtr &selector_ptr)
{
    if (!selector_ptr)
        return;

    boost::shared_ptr<JetSelector> selector =
        boost::dynamic_pointer_cast<JetSelector>(selector_ptr);
    if (!selector)
        return;

    *pt() += *selector->pt();
    *eta() += *selector->eta();
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



// WJetSelector
//
WJetSelector::WJetSelector()
{
    _children.reset(new Comparator<std::equal_to<uint32_t> >(2));
    _pt.reset(new Comparator<>(200));
    _mass_drop.reset(new Comparator<std::less<double> >(0.4));
    _mass_lower_bound.reset(new Comparator<>(60));
    _mass_upper_bound.reset(new Comparator<std::less<double> >(130));

    _p4.reset(new TLorentzVector());
}

WJetSelector::~WJetSelector()
{
}

bool WJetSelector::operator()(const Jet &jet)
{
    if (!_children->operator()(jet.children().size()))
        return false;

    utility::set(_p4.get(), &jet.physics_object().p4());
    if (!_pt->operator()(_p4->Pt()))
        return false;

    double mass = _p4->M();

    utility::set(_p4.get(), &jet.children().Get(0).physics_object().p4());
    double m1 = _p4->M();

    utility::set(_p4.get(), &jet.children().Get(0).physics_object().p4());
    double m2 = _p4->M();

    return _mass_drop->operator()(std::max(m1, m2) / mass)
        && _mass_lower_bound->operator()(mass)
        && _mass_upper_bound->operator()(mass);
}

WJetSelector::CutPtr WJetSelector::children() const
{
    return _children;
}

WJetSelector::CutPtr WJetSelector::pt() const
{
    return _pt;
}

WJetSelector::CutPtr WJetSelector::mass_drop() const
{
    return _mass_drop;
}

WJetSelector::CutPtr WJetSelector::mass_lower_bound() const
{
    return _mass_lower_bound;
}

WJetSelector::CutPtr WJetSelector::mass_upper_bound() const
{
    return _mass_upper_bound;
}

void WJetSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << " [+]          Children = " << *_children << endl;
    out << " [+]                Pt > " << *_pt << endl;
    out << " [+]         Mass Drop < " << *_mass_drop << endl;
    out << " [+]  Mass Lower Bound > " << *_mass_lower_bound << endl;
    out << " [+]  Mass Upper Bound < " << *_mass_upper_bound << endl;
}

Selector::SelectorPtr WJetSelector::clone() const
{
    boost::shared_ptr<WJetSelector> selector(new WJetSelector());

    *selector->children() = *children();
    *selector->pt() = *pt();
    *selector->mass_drop() = *mass_drop();
    *selector->mass_lower_bound() = *mass_lower_bound();
    *selector->mass_upper_bound() = *mass_upper_bound();

    return selector;
}

void WJetSelector::merge(const SelectorPtr &selector_ptr)
{
    if (!selector_ptr)
        return;

    boost::shared_ptr<WJetSelector> selector =
        boost::dynamic_pointer_cast<WJetSelector>(selector_ptr);
    if (!selector)
        return;

    *children() += *selector->children();
    *pt() += *selector->pt();
    *mass_drop() += *selector->mass_drop();
    *mass_lower_bound() += *selector->mass_lower_bound();
    *mass_upper_bound() += *selector->mass_upper_bound();
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



// Muon Selector
//
MuonSelector::MuonSelector()
{
    _pt.reset(new Comparator<>(30));
    _eta.reset(new Comparator<std::less<double> >(2.1));
    _is_global.reset(new Comparator<std::logical_and<bool> >(true));
    _is_tracker.reset(new Comparator<std::logical_and<bool> >(true));
    _muon_segments.reset(new Comparator<>(1));
    _muon_hits.reset(new Comparator<>(0));
    _muon_normalized_chi2.reset(new Comparator<std::less<double> >(10));
    _tracker_hits.reset(new Comparator<>(10));
    _pixel_hits.reset(new Comparator<>(0));
    _d0_bsp.reset(new Comparator<std::less<double> >(0.02));
    _primary_vertex.reset(new Comparator<std::less<double> >(1));

    _p4.reset(new TLorentzVector());
}

MuonSelector::~MuonSelector()
{
}

bool MuonSelector::operator()(const Muon &muon, const PrimaryVertex &pv)
{
    utility::set(_p4.get(), &muon.physics_object().p4());

    return _pt->operator()(_p4->Pt())
        && _eta->operator()(fabs(_p4->Eta()))
        && _is_global->operator()(muon.extra().is_global())
        && _is_tracker->operator()(muon.extra().is_tracker())
        && _muon_segments->operator()(muon.extra().number_of_matches())
        && _muon_hits->operator()(muon.global_track().hits())
        && _muon_normalized_chi2->operator()(muon.global_track().normalized_chi2())
        && _tracker_hits->operator()(muon.inner_track().hits())
        && _pixel_hits->operator()(muon.extra().pixel_hits())
        && _d0_bsp->operator()(muon.extra().d0_bsp())
        && _primary_vertex->operator()(fabs(muon.physics_object().vertex().z() - pv.vertex().z()));
}

MuonSelector::CutPtr MuonSelector::pt() const
{
    return _pt;
}

MuonSelector::CutPtr MuonSelector::eta() const
{
    return _eta;
}

MuonSelector::CutPtr MuonSelector::is_global() const
{
    return _is_global;
}

MuonSelector::CutPtr MuonSelector::is_tracker() const
{
    return _is_tracker;
}

MuonSelector::CutPtr MuonSelector::muon_segments() const
{
    return _muon_segments;
}

MuonSelector::CutPtr MuonSelector::muon_hits() const
{
    return _muon_hits;
}

MuonSelector::CutPtr MuonSelector::muon_normalized_chi2() const
{
    return _muon_normalized_chi2;
}

MuonSelector::CutPtr MuonSelector::tracker_hits() const
{
    return _tracker_hits;
}

MuonSelector::CutPtr MuonSelector::pixel_hits() const
{
    return _pixel_hits;
}

MuonSelector::CutPtr MuonSelector::d0_bsp() const
{
    return _d0_bsp;
}

MuonSelector::CutPtr MuonSelector::primary_vertex() const
{
    return _primary_vertex;
}

void MuonSelector::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    out << " [+]                pT > " << *_pt << endl;
    out << " [+]             |eta| < " << *_eta << endl;
    out << " [+] is Global           " << *_is_global << endl;
    out << " [+] is Tracker          " << *_is_tracker << endl;
    out << " [+]     Muon Segments > " << *_muon_segments << endl;
    out << " [+]         Muon Hits > " << *_muon_hits << endl;
    out << " [+]  Muon Chi2 / ndof < " << *_muon_normalized_chi2 << endl;
    out << " [+]      Tracker Hits > " << *_tracker_hits << endl;
    out << " [+]        Pixel Hits > " << *_pixel_hits << endl;
    out << " [+]          |d0_bsp| < " << *_d0_bsp << endl;
    out << " [+] |mu.z() - pv.z()| < " << *_primary_vertex << endl;
}

Selector::SelectorPtr MuonSelector::clone() const
{
    boost::shared_ptr<MuonSelector> selector(new MuonSelector());

    *selector->pt() = *pt();
    *selector->eta() = *eta();
    *selector->is_global() = *is_global();
    *selector->is_tracker() = *is_tracker();
    *selector->muon_segments() = *muon_segments();
    *selector->muon_hits() = *muon_hits();
    *selector->muon_normalized_chi2() = *muon_normalized_chi2();
    *selector->tracker_hits() = *tracker_hits();
    *selector->pixel_hits() = *pixel_hits();
    *selector->d0_bsp() = *d0_bsp();
    *selector->primary_vertex() = *primary_vertex();

    return selector;
}

void MuonSelector::merge(const SelectorPtr &selector)
{
    if (!selector)
        return;

    boost::shared_ptr<MuonSelector> mu_selector =
        boost::dynamic_pointer_cast<MuonSelector>(selector);
    if (!mu_selector)
        return;

    *pt() += *mu_selector->pt();
    *eta() += *mu_selector->eta();
    *is_global() += *mu_selector->is_global();
    *is_tracker() += *mu_selector->is_tracker();
    *muon_segments() += *mu_selector->muon_segments();
    *muon_hits() += *mu_selector->muon_hits();
    *muon_normalized_chi2() += *mu_selector->muon_normalized_chi2();
    *tracker_hits() += *mu_selector->tracker_hits();
    *pixel_hits() += *mu_selector->pixel_hits();
    *d0_bsp() += *mu_selector->d0_bsp();
    *primary_vertex() += *mu_selector->primary_vertex();
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



// Multiplicity Cutflow
//
MultiplicityCutflow::MultiplicityCutflow(const uint32_t &max)
{
    using selector::Comparator;

    for(uint32_t i = 0; max > i; ++i)
    {
        _cuts.push_back(CutPtr(new Comparator<std::equal_to<uint32_t> >(i)));
    }
    _cuts.push_back(CutPtr(new Comparator<std::greater_equal<uint32_t> >(max)));
}

MultiplicityCutflow::~MultiplicityCutflow()
{
}

void MultiplicityCutflow::operator()(const uint32_t &number)
{
    // It does not make sense to apply all cuts. Only Nth one:
    //
    if (_cuts.size() > number)
        (*_cuts[number])(number);
    else
        (**(_cuts.end() - 1))(number);
}

MultiplicityCutflow::CutPtr
    MultiplicityCutflow::cut(const uint32_t &cut) const
{
    return cut > _cuts.size()
        ? *_cuts.end()
        : _cuts[cut];
}

void MultiplicityCutflow::print(std::ostream &out) const
{
    out << "     CUT                 " << setw(5) << " "
        << " Objects Events" << endl;
    out << setw(45) << setfill('-') << left << " " << setfill(' ') << endl;
    for(uint32_t cut = 0, max = _cuts.size() - 1; max > cut; ++cut)
    {
        out << " [+] " << setw(20) << right << " = " << *_cuts[cut] << endl;
    }
    out << " [+] " << setw(20) << right << " >= "
        << **(_cuts.end() - 1) << endl;
}

MultiplicityCutflow::CutflowPtr MultiplicityCutflow::clone() const
{
    boost::shared_ptr<MultiplicityCutflow>
        selector(new MultiplicityCutflow(_cuts.size()));

    for(uint32_t i = 0; _cuts.size() > i; ++i)
    {
        *selector->cut(i) = *cut(i);
    }

    return selector;
}

void MultiplicityCutflow::merge(const CutflowPtr &selector_ptr)
{
    if (!selector_ptr)
        return;

    boost::shared_ptr<MultiplicityCutflow> selector =
        boost::dynamic_pointer_cast<MultiplicityCutflow>(selector_ptr);
    if (!selector
            || _cuts.size() != selector->_cuts.size())
        return;

    for(uint32_t i = 0; _cuts.size() > i; ++i)
    {
        *cut(i) += *selector->cut(i);
    }
}



// Counter
//
Counter::Counter():
    _count(0),
    _is_locked(false),
    _is_lock_on_update(false)
{
}

Counter &Counter::operator +=(const Counter &counter)
{
    if (!isLocked())
    {
        _count += counter;

        if (isLockOnUpdate())
        {
            lock();
            _is_lock_on_update = false;
        }
    }

    return *this;
}

Counter::operator uint32_t() const
{
    return _count;
}

bool Counter::isLocked() const
{
    return _is_locked;
}

bool Counter::isLockOnUpdate() const
{
    return _is_lock_on_update;
}

void Counter::lock()
{
    _is_locked = true;
}

void Counter::lockOnUpdate()
{
    _is_lock_on_update = true;
}

void Counter::unlock()
{
    _is_locked = false;
    _is_lock_on_update = false;
}

Counter &Counter::operator ++()
{
    if (!isLocked())
    {
        ++_count;

        if (isLockOnUpdate())
        {
            lock();
            _is_lock_on_update = false;
        }
    }

    return *this;
}



// Counter Lock
//
CounterLock::CounterLock(Counter &counter):
    _counter(counter)
{
    _counter.lock();
}

CounterLock::~CounterLock()
{
    _counter.unlock();
}



// Counter Lock on Update
//
CounterLockOnUpdate::CounterLockOnUpdate(Counter &counter):
    _counter(counter)
{
    _counter.lockOnUpdate();
}

CounterLockOnUpdate::~CounterLockOnUpdate()
{
    _counter.unlock();
}



// Lock Selector Event Counter on Update
//
LockSelectorEventCounterOnUpdate::LockSelectorEventCounterOnUpdate(
        ElectronSelector &selector)
{
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.et()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.eta()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.primary_vertex()->events())));
}

LockSelectorEventCounterOnUpdate::LockSelectorEventCounterOnUpdate(
        JetSelector &selector)
{
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.pt()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.eta()->events())));
}

LockSelectorEventCounterOnUpdate::LockSelectorEventCounterOnUpdate(
        WJetSelector &selector)
{
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.children()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.pt()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.mass_drop()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.mass_lower_bound()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.mass_upper_bound()->events())));
}

LockSelectorEventCounterOnUpdate::LockSelectorEventCounterOnUpdate(
        MuonSelector &selector)
{
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.is_global()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.is_tracker()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.muon_segments()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.muon_hits()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.muon_normalized_chi2()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.tracker_hits()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.pixel_hits()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.d0_bsp()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.primary_vertex()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.pt()->events())));
    _lockers.push_back(Locker(
                new CounterLockOnUpdate(selector.eta()->events())));
}



// Cut
//
Cut::Cut(const double &value):
    _value(value),
    _is_disabled(false)
{}

Cut &Cut::operator +=(const Cut &cut)
{
    _objects += cut.objects();
    _events += cut.events();

    return *this;
}

Counter &Cut::objects()
{
    return _objects;
}

const Counter &Cut::objects() const
{
    return _objects;
}

Counter &Cut::events()
{
    return _events;
}

const Counter &Cut::events() const
{
    return _events;
}

double Cut::value() const
{
    return _value;
}

bool Cut::operator()(const double &value)
{
    if (isDisabled())
        return true;

    if (!isPass(value))
        return false;

    ++_objects;
    ++_events;

    return true;
}

bool Cut::isDisabled() const
{
    return _is_disabled;
}

void Cut::disable()
{
    _is_disabled = true;
}

void Cut::enable()
{
    _is_disabled = false;
}



// Helpers
//
std::ostream &bsm::selector::operator <<(std::ostream &out, const Cut &cut)
{
    out << setw(5) << left << cut.value() << " ";
   
    if (!cut.isDisabled())
    {
        out << setw(7) << left << cut.objects()
            << " " << cut.events();
    }

    return out;
}

std::ostream &bsm::selector::operator <<(std::ostream &out, const Selector &s)
{
    s.print(out);

    return out;
}

std::ostream &bsm::selector::operator <<(std::ostream &out,
        const MultiplicityCutflow &cutflow)
{
    cutflow.print(out);

    return out;
}
