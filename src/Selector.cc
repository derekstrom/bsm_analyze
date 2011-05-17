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
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "bsm_input/interface/PrimaryVertex.pb.h"
#include "interface/Selector.h"
#include "interface/Utility.h"

using std::ostream;
using std::endl;
using std::setw;
using std::left;

using bsm::selector::Cut;
using bsm::selector::Comparator;
using bsm::selector::ElectronSelector;
using bsm::selector::MuonSelector;
using bsm::selector::Selector;

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
    out << " [+]                Et > " << setw(5) << left << _et->value()
        << " " << *_et << endl;
    out << " [+]             |eta| < " << setw(5) << left << _eta->value()
        << " " << *_eta << endl;
    out << " [+] |el.z() - pv.z()| < " << setw(5) << left
        << _primary_vertex->value() << " " << *_primary_vertex << endl;
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



// Muon Selector
//
MuonSelector::MuonSelector()
{
    _is_global.reset(new Comparator<std::logical_and<bool> >(true));
    _is_tracker.reset(new Comparator<std::logical_and<bool> >(true));
    _muon_segments.reset(new Comparator<>(1));
    _muon_hits.reset(new Comparator<>(0));
    _muon_normalized_chi2.reset(new Comparator<std::less<double> >(10));
    _tracker_hits.reset(new Comparator<>(10));
    _pixel_hits.reset(new Comparator<>(0));
    _d0_bsp.reset(new Comparator<std::less<double> >(0.02));
    _primary_vertex.reset(new Comparator<std::less<double> >(1));
    _pt.reset(new Comparator<>(20));
    _eta.reset(new Comparator<std::less<double> >(2.1));

    _p4.reset(new TLorentzVector());
}

MuonSelector::~MuonSelector()
{
}

bool MuonSelector::operator()(const Muon &muon, const PrimaryVertex &pv)
{
    utility::set(_p4.get(), &muon.physics_object().p4());

    return _is_global->operator()(muon.is_global())
        && _is_tracker->operator()(muon.is_tracker())
        && _muon_segments->operator()(muon.number_of_matches())
        && _muon_hits->operator()(muon.global_track().hits())
        && _muon_normalized_chi2->operator()(muon.global_track().normalized_chi2())
        && _tracker_hits->operator()(muon.inner_track().hits())
        && _pixel_hits->operator()(muon.pixel_hits())
        && _d0_bsp->operator()(muon.d0_bsp())
        && _primary_vertex->operator()(fabs(muon.physics_object().vertex().z() - pv.vertex().z()))
        && _pt->operator()(_p4->Pt())
        && _eta->operator()(fabs(_p4->Eta()));
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

MuonSelector::CutPtr MuonSelector::pt() const
{
    return _pt;
}

MuonSelector::CutPtr MuonSelector::eta() const
{
    return _eta;
}

void MuonSelector::print(std::ostream &out) const
{
    out << " [+] is Global           " << setw(5) << left << " "
        << " " << *_is_global << endl;
    out << " [+] is Tracker          " << setw(5) << left << " "
        << " " << *_is_tracker << endl;
    out << " [+]     Muon Segments > " << setw(5) << left
        << _muon_segments->value() << " " << *_muon_segments << endl;
    out << " [+]         Muon Hits > " << setw(5) << left
        << _muon_hits->value() << " " << *_muon_hits << endl;
    out << " [+]  Muon Chi2 / ndof < " << setw(5) << left
        << _muon_normalized_chi2->value()
        << " " << *_muon_normalized_chi2 << endl;
    out << " [+]      Tracker Hits > " << setw(5) << left
        << _tracker_hits->value() << " " << *_tracker_hits << endl;
    out << " [+]        Pixel Hits > " << setw(5) << left 
        << _pixel_hits->value() << " " << *_pixel_hits << endl;
    out << " [+]          |d0_bsp| < " << setw(5) << left
        << _d0_bsp->value() << " " << *_d0_bsp << endl;
    out << " [+] |mu.z() - pv.z()| < " << setw(5) << left
        << _primary_vertex->value() << " " << *_primary_vertex << endl;
    out << " [+]                pT > " << setw(5) << left
        << _eta->value() << " " << *_pt << endl;
    out << " [+]             |eta| < " << setw(5) << left
        << _eta->value() << " " << *_eta << endl;
}

Selector::SelectorPtr MuonSelector::clone() const
{
    boost::shared_ptr<MuonSelector> selector(new MuonSelector());

    *selector->is_global() = *is_global();
    *selector->is_tracker() = *is_tracker();
    *selector->muon_segments() = *muon_segments();
    *selector->muon_hits() = *muon_hits();
    *selector->muon_normalized_chi2() = *muon_normalized_chi2();
    *selector->tracker_hits() = *tracker_hits();
    *selector->pixel_hits() = *pixel_hits();
    *selector->d0_bsp() = *d0_bsp();
    *selector->primary_vertex() = *primary_vertex();
    *selector->pt() = *pt();
    *selector->eta() = *eta();

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

    *is_global() += *mu_selector->is_global();
    *is_tracker() += *mu_selector->is_tracker();
    *muon_segments() += *mu_selector->muon_segments();
    *muon_hits() += *mu_selector->muon_hits();
    *muon_normalized_chi2() += *mu_selector->muon_normalized_chi2();
    *tracker_hits() += *mu_selector->tracker_hits();
    *pixel_hits() += *mu_selector->pixel_hits();
    *d0_bsp() += *mu_selector->d0_bsp();
    *primary_vertex() += *mu_selector->primary_vertex();
    *pt() += *mu_selector->pt();
    *eta() += *mu_selector->eta();
}



// Cut
//
Cut::Cut(const double &value):
    _value(value),
    _count(0)
{}

Cut &Cut::operator +=(const Cut &cut)
{
    _count += cut;

    return *this;
}

Cut::operator uint32_t() const
{
    return _count;
}

double Cut::value() const
{
    return _value;
}

bool Cut::operator()(const double &value)
{
    return isPass(value)
        ? (++_count, true)
        : false;
}



// Helpers
//
std::ostream &bsm::selector::operator <<(std::ostream &out, const Selector &s)
{
    s.print(out);

    return out;
}
