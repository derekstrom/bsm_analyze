// WtagMass Analyzer
//
// Apply Wtag to jets and reconstruct Mttbar
//
// Created by Samvel Khalatyan, May 27, 2011
// Copyright 2011, All rights reserved

#include <iomanip>
#include <iostream>
#include <ostream>

#include <boost/pointer_cast.hpp>

#include <TLorentzVector.h>

#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Jet.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_stat/interface/H1.h"
#include "interface/Algorithm.h"
#include "interface/Selector.h"
#include "interface/WtagMassAnalyzer.h"
#include "interface/Utility.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::AnalyzerPtr;
using bsm::WtagMassAnalyzer;

using bsm::stat::H1;

WtagMassAnalyzer::WtagMassAnalyzer()
{
    _el_selector.reset(new ElectronSelector());
    _el_multiplicity.reset(new MultiplicityCutflow(4));

    _mu_selector.reset(new MuonSelector());
    _mu_multiplicity.reset(new MultiplicityCutflow(4));

    _leptonic_multiplicity.reset(new MultiplicityCutflow(3));
    _hadronic_multiplicity.reset(new MultiplicityCutflow(3));

    _wjet_selector.reset(new WJetSelector());
    _met_solutions.reset(new MultiplicityCutflow(2));
    _met_corrector.reset(new MissingEnergyCorrection(80.399, 0.00051099891));

    _mttbar.reset(new H1(25, 500, 3000));

    _p4.reset(new TLorentzVector());
}

WtagMassAnalyzer::~WtagMassAnalyzer()
{
}

const WtagMassAnalyzer::H1Ptr WtagMassAnalyzer::mttbar() const
{
    return _mttbar;
}

AnalyzerPtr WtagMassAnalyzer::clone() const
{
    return AnalyzerPtr(new WtagMassAnalyzer());
}

void WtagMassAnalyzer::merge(const AnalyzerPtr &analyzer_ptr)
{
    boost::shared_ptr<WtagMassAnalyzer> analyzer =
        dynamic_pointer_cast<WtagMassAnalyzer>(analyzer_ptr);

    if (!analyzer)
        return;

    _el_selector->merge(analyzer->_el_selector);
    _el_multiplicity->merge(analyzer->_el_multiplicity);

    _mu_selector->merge(analyzer->_mu_selector);
    _mu_multiplicity->merge(analyzer->_mu_multiplicity);

    _leptonic_multiplicity->merge(analyzer->_leptonic_multiplicity);
    _hadronic_multiplicity->merge(analyzer->_hadronic_multiplicity);

    _wjet_selector->merge(analyzer->_wjet_selector);
    _met_solutions->merge(analyzer->_met_solutions);

    *_mttbar += *analyzer->_mttbar;
}

void WtagMassAnalyzer::onFileOpen(const std::string &filename, const Input *input)
{
}

void WtagMassAnalyzer::process(const Event *event)
{
    if (!event->primary_vertices().size()
            || !event->has_missing_energy())
        return;

    if (muons(event))
        return;

    electrons(event);
}

bool WtagMassAnalyzer::muons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    uint32_t good_muons = 0;

    const PrimaryVertex &pv = event->primary_vertices().Get(0);

    selector::LockSelectorEventCounterOnUpdate lock(*_mu_selector);
    for(Muons::const_iterator muon = event->pf_muons().begin();
            event->pf_muons().end() != muon;
            ++muon)
    {
        if (_mu_selector->operator()(*muon, pv))
            ++good_muons;
    }

    _mu_multiplicity->operator()(good_muons);

    return 0 < good_muons;
}

void WtagMassAnalyzer::electrons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const PrimaryVertex &pv = event->primary_vertices().Get(0);

    selector::LockSelectorEventCounterOnUpdate lock(*_el_selector);
    const Electron *electron = 0;
    uint32_t good_electrons = 0;
    for(Electrons::const_iterator el = event->pf_electrons().begin();
            event->pf_electrons().end() != el;
            ++el)
    {
        if (_el_selector->operator()(*el, pv))
        {
            ++good_electrons;

            electron = &*el;
        }
    }

    _el_multiplicity->operator()(good_electrons);

    if (1 != good_electrons)
        return;

    // Process jets
    //
    jets(event, electron);
}

void WtagMassAnalyzer::jets(const Event *event, const Electron *el)
{
    typedef ::google::protobuf::RepeatedPtrField<Jet> PBJets;

    // Store all leptonic side jets but electron and Missing Energy
    //
    Jets leptonic;

    // Store all hadronic side jets but w-tagged jet(s)
    //
    Jets hadronic;

    // Cache electron LorentzVector
    //
    TLorentzVector el_p4;
    utility::set(&el_p4, &el->physics_object().p4());

    double delta_phi_cut = TMath::Pi() / 2;

    selector::LockSelectorEventCounterOnUpdate lock(*_wjet_selector);
    const Jet *wjet = 0;
    uint32_t wjets = 0;
    for(PBJets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet;
            ++jet)
    {
        utility::set(_p4.get(), &jet->physics_object().p4());
        if (_p4->DeltaPhi(el_p4) < delta_phi_cut)
        {
            leptonic.push_back(&*jet);

            continue;
        }

        // Check if the jet is a W: w-boson is expected to be in the hadronic
        // leg
        //
        if (!_wjet_selector->operator()(*jet))
        {
            hadronic.push_back(&*jet);

            continue;
        }

        wjet = &*jet;
        ++wjets;
    }

    if (1 != wjets)
        return;

    _leptonic_multiplicity->operator()(leptonic.size());
    _hadronic_multiplicity->operator()(hadronic.size());

    // there should be at least one jet on leptonic/hadronic side
    //
    if (!leptonic.size()
            || !hadronic.size())

        return;

    // Get LorentzVector of the leptonic/hadronic sides
    PBP4 leptonic_p4 = leptonicLeg(event, el, leptonic);
    PBP4 hadronic_p4 = hadronicLeg(event, wjet, hadronic);

    if (!leptonic_p4
            || !hadronic_p4)
        return;

    *leptonic_p4 += *hadronic_p4;

    _mttbar->fill(mass(*leptonic_p4));
}

WtagMassAnalyzer::PBP4 WtagMassAnalyzer::leptonicLeg(const Event *event,
        const Electron *el,
        const Jets &jets)
{
    // Calculate LorentzVector of the leptonic leg: lepton + MET + jet
    //
    PBP4 p4(new LorentzVector());
    
    *p4 += el->physics_object().p4();

    // Apply pZ correction to the Missing Energy
    //
    uint32_t solutions = _met_corrector->operator()(el->physics_object().p4(),
            event->missing_energy().p4());

    _met_solutions->operator()(solutions);

    // Always take the largest solution
    //
    *p4 += *_met_corrector->solution(1);

    // Find the jet with max Mass
    //
    const Jet *jet = 0;
    double max_mass = 0;
    for(Jets::const_iterator ijet = jets.begin();
            jets.end() != ijet;
            ++ijet)
    {
        double jet_mass = mass((*ijet)->physics_object().p4());
        if (jet_mass <= max_mass)
            continue;

        jet = *ijet;
        max_mass = jet_mass;
    }

    *p4 += jet->physics_object().p4();

    return p4;
}

WtagMassAnalyzer::PBP4 WtagMassAnalyzer::hadronicLeg(const Event *event,
        const Jet *wjet,
        const Jets &jets)
{
    PBP4 p4(new LorentzVector());

    *p4 += wjet->physics_object().p4();

    // Find jet with max Mass and Wjet
    //
    const Jet *jet = 0;
    double max_mass = 0;
    for(Jets::const_iterator ijet = jets.begin();
            jets.end() != ijet;
            ++ijet)
    {
        double jet_mass = mass((*ijet)->physics_object().p4());
        if (jet_mass <= max_mass)
            continue;
       
        jet = *ijet;
        max_mass = jet_mass;
    }

    *p4 += jet->physics_object().p4();

    return p4;
}

void WtagMassAnalyzer::print(std::ostream &out) const
{
    out << "PF Muons" << endl;
    out << *_mu_selector << endl;
    out << endl;
    out << "PF Muons Multiplicity" << endl;
    out << *_mu_multiplicity << endl;
    out << endl;

    out << "PF Electrons" << endl;
    out << *_el_selector << endl;
    out << endl;
    out << "PF Electrons Multiplcitiy" << endl;
    out << *_el_multiplicity << endl;
    out << endl;

    out << "WJets" << endl;
    out << *_wjet_selector << endl;
    out << endl;

    out << "Leptonic Leg Multiplicity" << endl;
    out << *_leptonic_multiplicity << endl;
    out << endl;

    out << endl;
    out << "Hadronic Leg Multiplicity" << endl;
    out << *_hadronic_multiplicity << endl;
    out << endl;

    out << "MET Solutions" << endl;
    out << *_met_solutions << endl;

    out << "Mttbar" << endl;
    out << *_mttbar << endl;
}

WtagMassAnalyzer::operator bool() const
{
    return _el_selector
        && _el_multiplicity
        && _mu_selector
        && _mu_multiplicity
        && _leptonic_multiplicity
        && _hadronic_multiplicity
        && _wjet_selector
        && _met_solutions
        && _met_corrector
        && _mttbar;
}
