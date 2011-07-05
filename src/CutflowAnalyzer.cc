// Cutflow Analyzer
//
// Generate cutflow table after selector is applied
//
// Created by Samvel Khalatyan, May 18, 2011
// Copyright 2011, All rights reserved

#include <ostream>

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Jet.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "interface/Selector.h"
#include "interface/CutflowAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::CutflowAnalyzer;
using bsm::MuonCutflowAnalyzer;

CutflowAnalyzer::CutflowAnalyzer()
{
    _pv_multiplicity.reset(new MultiplicityCutflow(16));

    _jet_selector.reset(new JetSelector());
    _jet_multiplicity.reset(new MultiplicityCutflow(4));

    _pf_el_selector.reset(new ElectronSelector());
    _pf_el_number_selector.reset(new MultiplicityCutflow(4));

    _gsf_el_selector.reset(new ElectronSelector());
    _gsf_el_number_selector.reset(new MultiplicityCutflow(4));

    _pf_mu_selector_step1.reset(new MuonSelector());
    _pf_mu_selector_step1->disable();
    _pf_mu_selector_step1->pt()->enable();
    _pf_mu_selector_step1->eta()->enable();

    _pf_mu_number_selector_step1.reset(new MultiplicityCutflow(4));

    _pf_mu_selector.reset(new MuonSelector());
    _pf_mu_number_selector.reset(new MultiplicityCutflow(4));

    _reco_mu_selector_step1.reset(new MuonSelector());
    _reco_mu_selector_step1->disable();
    _reco_mu_selector_step1->pt()->enable();
    _reco_mu_selector_step1->eta()->enable();

    _reco_mu_number_selector_step1.reset(new MultiplicityCutflow(4));

    _reco_mu_selector.reset(new MuonSelector());
    _reco_mu_number_selector.reset(new MultiplicityCutflow(4));

    monitor(_pv_multiplicity);

    monitor(_jet_selector);
    monitor(_jet_multiplicity);

    monitor(_pf_el_selector);
    monitor(_pf_el_number_selector);

    monitor(_gsf_el_selector);
    monitor(_gsf_el_number_selector);

    monitor(_pf_mu_selector_step1);
    monitor(_pf_mu_number_selector_step1);

    monitor(_pf_mu_selector);
    monitor(_pf_mu_number_selector);

    monitor(_reco_mu_selector_step1);
    monitor(_reco_mu_number_selector_step1);

    monitor(_reco_mu_selector);
    monitor(_reco_mu_number_selector);
}

CutflowAnalyzer::CutflowAnalyzer(const CutflowAnalyzer &object)
{
    _pv_multiplicity = 
        dynamic_pointer_cast<MultiplicityCutflow>(object._pv_multiplicity->clone());

    _jet_selector =
        dynamic_pointer_cast<JetSelector>(object._jet_selector->clone());
    _jet_multiplicity =
        dynamic_pointer_cast<MultiplicityCutflow>(object._jet_multiplicity->clone());

    _pf_el_selector = 
        dynamic_pointer_cast<ElectronSelector>(object._pf_el_selector->clone());
    _pf_el_number_selector =
        dynamic_pointer_cast<MultiplicityCutflow>(object._pf_el_number_selector->clone());

    _gsf_el_selector =
        dynamic_pointer_cast<ElectronSelector>(object._gsf_el_selector->clone());
    _gsf_el_number_selector =
        dynamic_pointer_cast<MultiplicityCutflow>(object._gsf_el_number_selector->clone());

    _pf_mu_selector_step1 =
        dynamic_pointer_cast<MuonSelector>(object._pf_mu_selector_step1->clone());
    _pf_mu_number_selector_step1 =
        dynamic_pointer_cast<MultiplicityCutflow>(object._pf_mu_number_selector_step1->clone());

    _pf_mu_selector =
        dynamic_pointer_cast<MuonSelector>(object._pf_mu_selector->clone());
    _pf_mu_number_selector =
        dynamic_pointer_cast<MultiplicityCutflow>(object._pf_mu_number_selector->clone());

    _reco_mu_selector_step1 =
        dynamic_pointer_cast<MuonSelector>(object._reco_mu_selector_step1->clone());
    _reco_mu_number_selector_step1 =
        dynamic_pointer_cast<MultiplicityCutflow>(object._reco_mu_number_selector_step1->clone());

    _reco_mu_selector =
        dynamic_pointer_cast<MuonSelector>(object._reco_mu_selector->clone());
    _reco_mu_number_selector =
        dynamic_pointer_cast<MultiplicityCutflow>(object._reco_mu_number_selector->clone());

    monitor(_pv_multiplicity);

    monitor(_jet_selector);
    monitor(_jet_multiplicity);

    monitor(_pf_el_selector);
    monitor(_pf_el_number_selector);

    monitor(_gsf_el_selector);
    monitor(_gsf_el_number_selector);

    monitor(_pf_mu_selector_step1);
    monitor(_pf_mu_number_selector_step1);

    monitor(_pf_mu_selector);
    monitor(_pf_mu_number_selector);

    monitor(_reco_mu_selector_step1);
    monitor(_reco_mu_number_selector_step1);

    monitor(_reco_mu_selector);
    monitor(_reco_mu_number_selector);
}

void CutflowAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void CutflowAnalyzer::process(const Event *event)
{
    _pv_multiplicity->apply(event->primary_vertices().size());

    if (!event->primary_vertices().size())
        return;

    jets(event);
    electrons(event);
    muons(event);
}

uint32_t CutflowAnalyzer::id() const
{
    return core::ID<CutflowAnalyzer>::get();
}

CutflowAnalyzer::ObjectPtr CutflowAnalyzer::clone() const
{
    return ObjectPtr(new CutflowAnalyzer(*this));
}

void CutflowAnalyzer::print(std::ostream &out) const
{
    out << "Primary Vertices" << endl;
    out << *_pv_multiplicity << endl;
    out << endl;

    out << "Particle-Flow Jets" << endl;
    out << "[Selector]" << endl;
    out << *_jet_selector << endl;
    out << endl;
    out << "[Multiplicity]" << endl;
    out << *_jet_multiplicity << endl;
    out << endl;

    out << "Particle-Flow Electrons" << endl;
    out << "[Selector]" << endl;
    out << *_pf_el_selector << endl;
    out << endl;
    out << "[Multiplicity]" << endl;
    out << *_pf_el_number_selector << endl;
    out << endl;

    out << "Gsf Electrons" << endl;
    out << "[Selector]" << endl;
    out << *_gsf_el_selector << endl;
    out << endl;
    out << "[Multiplicity]" << endl;
    out << *_gsf_el_number_selector << endl;
    out << endl;

    out << "Particle-Flow Muons" << endl;
    out << "[Selector: Step 1]" << endl;
    out << *_pf_mu_selector_step1 << endl;
    out << endl;
    out << "[Multiplicity: Step 1]" << endl;
    out << *_pf_mu_number_selector_step1 << endl;
    out << endl;
    out << endl;
    out << "[Selector: Step 2]" << endl;
    out << *_pf_mu_selector << endl;
    out << endl;
    out << "[Multiplicity: Step 2]" << endl;
    out << *_pf_mu_number_selector << endl;
    out << endl;

    out << "Reco Muons" << endl;
    out << "[Selector: Step 1]" << endl;
    out << *_reco_mu_selector_step1 << endl;
    out << endl;
    out << "[Multiplicity: Step 1]" << endl;
    out << *_reco_mu_number_selector_step1 << endl;
    out << endl;
    out << endl;
    out << "[Selector: Step 2]" << endl;
    out << *_reco_mu_selector << endl;
    out << endl;
    out << "[Multiplicity: Step 2]" << endl;
    out << *_reco_mu_number_selector;
}

void CutflowAnalyzer::electrons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    if (event->pf_electrons().size())
    {
        uint32_t selected_electrons;

        LockSelectorEventCounterOnUpdate lock(*_pf_el_selector);
        for(Electrons::const_iterator el = event->pf_electrons().begin();
                event->pf_electrons().end() != el;
                ++el)
        {
            if (_pf_el_selector->apply(*el, pv))
                ++selected_electrons;
        }

        _pf_el_number_selector->apply(selected_electrons);
    }

    if (event->gsf_electrons().size())
    {
        uint32_t selected_electrons = 0;

        LockSelectorEventCounterOnUpdate lock(*_gsf_el_selector);
        for(Electrons::const_iterator el = event->gsf_electrons().begin();
                event->gsf_electrons().end() != el;
                ++el)
        {
            if (_gsf_el_selector->apply(*el, pv))
                ++selected_electrons;
        }

        _gsf_el_number_selector->apply(selected_electrons);
    }
}

void CutflowAnalyzer::jets(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;

    if (event->jets().size())
    {
        uint32_t selected_jets;

        LockSelectorEventCounterOnUpdate lock(*_jet_selector);
        for(Jets::const_iterator jet = event->jets().begin();
                event->jets().end() != jet;
                ++jet)
        {
            if (_jet_selector->apply(*jet))
                ++selected_jets;
        }

        _jet_multiplicity->apply(selected_jets);
    }
}

void CutflowAnalyzer::muons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    if (event->pf_muons().size())
    {
        uint32_t selected_muons = 0;
        uint32_t selected_muons_step1 = 0;

        LockSelectorEventCounterOnUpdate lock_step1(*_pf_mu_selector_step1);
        LockSelectorEventCounterOnUpdate lock(*_pf_mu_selector);
        for(Muons::const_iterator mu = event->pf_muons().begin();
                event->pf_muons().end() != mu;
                ++mu)
        {
            if (_pf_mu_selector_step1->apply(*mu, pv))
                ++selected_muons_step1;

            if (_pf_mu_selector->apply(*mu, pv))
                ++selected_muons;
        }

        _pf_mu_number_selector_step1->apply(selected_muons_step1);
        _pf_mu_number_selector->apply(selected_muons);
    }

    if (event->reco_muons().size())
    {
        uint32_t selected_muons = 0;
        uint32_t selected_muons_step1 = 0;

        LockSelectorEventCounterOnUpdate lock_step1(*_reco_mu_selector_step1);
        LockSelectorEventCounterOnUpdate lock(*_reco_mu_selector);
        for(Muons::const_iterator mu = event->reco_muons().begin();
                event->reco_muons().end() != mu;
                ++mu)
        {
            if (_reco_mu_selector_step1->apply(*mu, pv))
                ++selected_muons_step1;

            if (_reco_mu_selector->apply(*mu, pv))
                ++selected_muons;
        }

        _reco_mu_number_selector_step1->apply(selected_muons_step1);
        _reco_mu_number_selector->apply(selected_muons);
    }
}



// Muon channel cutflow
//
MuonCutflowAnalyzer::MuonCutflowAnalyzer()
{
    _cutflow.reset(new MultiplicityCutflow(4));

    _pv_multiplicity.reset(new MultiplicityCutflow(4));

    _jet_selector.reset(new JetSelector());
    _jet_multiplicity.reset(new MultiplicityCutflow(4));

    _el_selector.reset(new ElectronSelector());
    _el_number_selector.reset(new MultiplicityCutflow(4));

    _mu_selector.reset(new MuonSelector());
    _mu_number_selector.reset(new MultiplicityCutflow(4));

    monitor(_cutflow);

    monitor(_pv_multiplicity);

    monitor(_jet_selector);
    monitor(_jet_multiplicity);

    monitor(_el_selector);
    monitor(_el_number_selector);

    monitor(_mu_selector);
    monitor(_mu_number_selector);
}

MuonCutflowAnalyzer::MuonCutflowAnalyzer(const MuonCutflowAnalyzer &object)
{
    _cutflow = 
        dynamic_pointer_cast<MultiplicityCutflow>(object._cutflow->clone());

    _pv_multiplicity = 
        dynamic_pointer_cast<MultiplicityCutflow>(object._pv_multiplicity->clone());

    _jet_selector =
        dynamic_pointer_cast<JetSelector>(object._jet_selector->clone());
    _jet_multiplicity =
        dynamic_pointer_cast<MultiplicityCutflow>(object._jet_multiplicity->clone());

    _el_selector = 
        dynamic_pointer_cast<ElectronSelector>(object._el_selector->clone());
    _el_number_selector =
        dynamic_pointer_cast<MultiplicityCutflow>(object._el_number_selector->clone());

    _mu_selector =
        dynamic_pointer_cast<MuonSelector>(object._mu_selector->clone());
    _mu_number_selector =
        dynamic_pointer_cast<MultiplicityCutflow>(object._mu_number_selector->clone());

    monitor(_cutflow);

    monitor(_pv_multiplicity);

    monitor(_jet_selector);
    monitor(_jet_multiplicity);

    monitor(_el_selector);
    monitor(_el_number_selector);

    monitor(_mu_selector);
    monitor(_mu_number_selector);
}

void MuonCutflowAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void MuonCutflowAnalyzer::process(const Event *event)
{
    _pv_multiplicity->apply(event->primary_vertices().size());

    if (!event->primary_vertices().size())
        return;

    // Preselection
    //
    jets(event);

    _cutflow->apply(PRESELECTION);

    // There should be at least one good muon
    //
    if (!muons(event))
        return;

    _cutflow->apply(GOOD_MUON);

    // No leading electron should be present in the event
    //
    if (electrons(event))
        return;

    _cutflow->apply(NO_LEADING_ELECTRON);

    _cutflow->apply(HT_LEP);
}

uint32_t MuonCutflowAnalyzer::id() const
{
    return core::ID<MuonCutflowAnalyzer>::get();
}

MuonCutflowAnalyzer::ObjectPtr MuonCutflowAnalyzer::clone() const
{
    return ObjectPtr(new MuonCutflowAnalyzer(*this));
}

void MuonCutflowAnalyzer::print(std::ostream &out) const
{
    out << "Primary Vertices" << endl;
    out << *_pv_multiplicity << endl;
    out << endl;

    out << "Particle-Flow Jets" << endl;
    out << "[Selector]" << endl;
    out << *_jet_selector << endl;
    out << endl;
    out << "[Multiplicity]" << endl;
    out << *_jet_multiplicity << endl;
    out << endl;

    out << "Particle-Flow Muons" << endl;
    out << "[Selector]" << endl;
    out << *_mu_selector << endl;
    out << endl;
    out << "[Multiplicity]" << endl;
    out << *_mu_number_selector << endl;
    out << endl;

    out << "Particle-Flow Electrons" << endl;
    out << "[Selector]" << endl;
    out << *_el_selector << endl;
    out << endl;
    out << "[Multiplicity]" << endl;
    out << *_el_number_selector << endl;
    out << endl;

    out << "Cutflow" << endl;
    out << *_cutflow << endl;
}

// Private
//
uint32_t MuonCutflowAnalyzer::jets(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;

    uint32_t selected_jets = 0;

    LockSelectorEventCounterOnUpdate lock(*_jet_selector);
    for(Jets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet;
            ++jet)
    {
        if (_jet_selector->apply(*jet))
            ++selected_jets;
    }

    _jet_multiplicity->apply(selected_jets);

    return selected_jets;
}

uint32_t MuonCutflowAnalyzer::muons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    uint32_t selected_muons = 0;

    LockSelectorEventCounterOnUpdate lock(*_mu_selector);
    for(Muons::const_iterator mu = event->pf_muons().begin();
            event->pf_muons().end() != mu;
            ++mu)
    {
        if (_mu_selector->apply(*mu, pv))
            ++selected_muons;
    }

    _mu_number_selector->apply(selected_muons);

    return selected_muons;
}

uint32_t MuonCutflowAnalyzer::electrons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    uint32_t selected_electrons = 0;

    LockSelectorEventCounterOnUpdate lock(*_el_selector);
    for(Electrons::const_iterator el = event->pf_electrons().begin();
            event->pf_electrons().end() != el;
            ++el)
    {
        if (_el_selector->apply(*el, pv))
            ++selected_electrons;
    }

    _el_number_selector->apply(selected_electrons);

    return selected_electrons;
}
