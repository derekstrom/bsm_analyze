// Synchronization Exercises
//
// Different Analyzers that performan the sinchronization exercises at
// different development stages
//
// Created by Samvel Khalatyan, Jul 05, 2011
// Copyright 2011, All rights reserved

#include <ostream>
#include <string>

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/Event.pb.h"
#include "interface/Monitor.h"
#include "interface/Selector.h"
#include "interface/SynchAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::SynchJuly2011Analyzer;

SynchJuly2011Analyzer::SynchJuly2011Analyzer(const LeptonMode &mode):
    _lepton_mode(mode)
{
    _cutflow.reset(new MultiplicityCutflow(4));
    _cutflow->cut(0)->setName("pre-selection");
    _cutflow->cut(1)->setName("Good Primary Vertex");
    _cutflow->cut(2)->setName("2 Good Jets");
    _cutflow->cut(3)->setName(string("Good ")
            + (ELECTRON == _lepton_mode ? "Electron" : "Muon"));
    _cutflow->cut(4)->setName(string("Veto Good ")
            + (ELECTRON == _lepton_mode ? "Muon" : "Electron"));
    monitor(_cutflow);

    // Selectrors
    //
    _primary_vertex_selector.reset(new PrimaryVertexSelector());
    _jet_selector.reset(new JetSelector());
    _electron_selector.reset(new ElectronSelector());
    _electron_veto_selector.reset(new ElectronSelector());
    _muon_selector.reset(new MuonSelector());
    _muon_veto_selector.reset(new MuonSelector());

    _electron_selector->primary_vertex()->disable();
    _electron_veto_selector->primary_vertex()->disable();

    _muon_selector->primary_vertex()->disable();
    _muon_veto_selector->primary_vertex()->disable();

    _muon_selector->pt()->setValue(35);
    _muon_veto_selector->pt()->setValue(35);

    monitor(_primary_vertex_selector);
    monitor(_jet_selector);
    monitor(_electron_selector);
    monitor(_electron_veto_selector);
    monitor(_muon_selector);
    monitor(_muon_veto_selector);

    // Monitors
    //
    _leading_jet.reset(new LorentzVectorMonitor());

    _electron_before_veto.reset(new LorentzVectorMonitor());
    _muon_to_veto.reset(new LorentzVectorMonitor());
    _electron_after_veto.reset(new LorentzVectorMonitor());

    _muon_before_veto.reset(new LorentzVectorMonitor());
    _electron_to_veto.reset(new LorentzVectorMonitor());
    _muon_after_veto.reset(new LorentzVectorMonitor());

    monitor(_leading_jet);

    monitor(_electron_before_veto);
    monitor(_muon_to_veto);
    monitor(_electron_after_veto);

    monitor(_muon_before_veto);
    monitor(_electron_to_veto);
    monitor(_muon_after_veto);
}

SynchJuly2011Analyzer::SynchJuly2011Analyzer(const SynchJuly2011Analyzer &object):
    _lepton_mode(object._lepton_mode)
{
    _cutflow = 
        dynamic_pointer_cast<MultiplicityCutflow>(object._cutflow->clone());

    monitor(_cutflow);

    // Selectors
    //
    _primary_vertex_selector = 
        dynamic_pointer_cast<PrimaryVertexSelector>(object._primary_vertex_selector->clone());

    _jet_selector = 
        dynamic_pointer_cast<JetSelector>(object._jet_selector->clone());

    _electron_selector = 
        dynamic_pointer_cast<ElectronSelector>(object._electron_selector->clone());

    _electron_veto_selector = 
        dynamic_pointer_cast<ElectronSelector>(object._electron_veto_selector->clone());

    _muon_selector = 
        dynamic_pointer_cast<MuonSelector>(object._muon_selector->clone());

    _muon_veto_selector = 
        dynamic_pointer_cast<MuonSelector>(object._muon_veto_selector->clone());

    monitor(_primary_vertex_selector);
    monitor(_jet_selector);
    monitor(_electron_selector);
    monitor(_electron_veto_selector);
    monitor(_muon_selector);
    monitor(_muon_veto_selector);

    // Monitors
    //
    // Monitors
    //
    _leading_jet = 
        dynamic_pointer_cast<LorentzVectorMonitor>(object._leading_jet->clone());

    _electron_before_veto = 
        dynamic_pointer_cast<LorentzVectorMonitor>(object._electron_before_veto->clone());

    _muon_to_veto = 
        dynamic_pointer_cast<LorentzVectorMonitor>(object._muon_to_veto->clone());

    _electron_after_veto = 
        dynamic_pointer_cast<LorentzVectorMonitor>(object._electron_after_veto->clone());

    _muon_before_veto = 
        dynamic_pointer_cast<LorentzVectorMonitor>(object._muon_before_veto->clone());

    _electron_to_veto = 
        dynamic_pointer_cast<LorentzVectorMonitor>(object._electron_to_veto->clone());

    _muon_after_veto = 
        dynamic_pointer_cast<LorentzVectorMonitor>(object._muon_after_veto->clone());

    monitor(_leading_jet);

    monitor(_electron_before_veto);
    monitor(_muon_to_veto);
    monitor(_electron_after_veto);

    monitor(_muon_before_veto);
    monitor(_electron_to_veto);
    monitor(_muon_after_veto);
}

const SynchJuly2011Analyzer::P4MonitorPtr SynchJuly2011Analyzer::leadingJet() const
{
    return _leading_jet;
}

const SynchJuly2011Analyzer::P4MonitorPtr SynchJuly2011Analyzer::electronBeforeVeto() const
{
    return _electron_before_veto;
}

const SynchJuly2011Analyzer::P4MonitorPtr SynchJuly2011Analyzer::muonToVeto() const
{
    return _muon_to_veto;
}

const SynchJuly2011Analyzer::P4MonitorPtr SynchJuly2011Analyzer::electronAfterVeto() const
{
    return _electron_after_veto;
}

const SynchJuly2011Analyzer::P4MonitorPtr SynchJuly2011Analyzer::muonBeforeVeto() const
{
    return _muon_before_veto;
}

const SynchJuly2011Analyzer::P4MonitorPtr SynchJuly2011Analyzer::electronToVeto() const
{
    return _electron_to_veto;
}

const SynchJuly2011Analyzer::P4MonitorPtr SynchJuly2011Analyzer::muonAfterVeto() const
{
    return _muon_after_veto;
}


void SynchJuly2011Analyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void SynchJuly2011Analyzer::process(const Event *event)
{
    _cutflow->apply(PRESELECTION);

    if (!event->primary_vertices().size()
            || !_primary_vertex_selector->apply(*event->primary_vertices().begin()))
        return;

    _cutflow->apply(PRIMARY_VERTEX);

    if (!jets(event))
        return;

    _cutflow->apply(JET);

    bool lepton_cut = false;
    switch(_lepton_mode)
    {
        case ELECTRON:  lepton_cut = electron(event);
                        break;

        case MUON:      lepton_cut = muon(event);
                        break;

        default:        break;
    }

    if (!lepton_cut)
        return;

    _cutflow->apply(VETO_SECOND_LEPTON);

    _passed_events.push_back(event->extra());
}

uint32_t SynchJuly2011Analyzer::id() const
{
    return core::ID<SynchJuly2011Analyzer>::get();
}

SynchJuly2011Analyzer::ObjectPtr SynchJuly2011Analyzer::clone() const
{
    return ObjectPtr(new SynchJuly2011Analyzer(*this));
}

void SynchJuly2011Analyzer::merge(const ObjectPtr &pointer)
{
    if (pointer->id() != id())
        return;

    boost::shared_ptr<SynchJuly2011Analyzer> object =
        dynamic_pointer_cast<SynchJuly2011Analyzer>(pointer);

    if (!object)
        return;

    Object::merge(pointer);

    _passed_events.insert(_passed_events.end(),
            object->_passed_events.begin(),
            object->_passed_events.end());
}

void SynchJuly2011Analyzer::print(std::ostream &out) const
{
    out << "Survived Events" << endl;
    out << " " << setw(10) << left << "Run" << setw(10) << "Lumi" << "Event" << endl;
    for(std::vector<Event::Extra>::const_iterator extra = _passed_events.begin();
            _passed_events.end() != extra;
            ++extra)
    {
        out << " "
            << setw(10) << left << extra->run()
            << setw(10) << left << extra->lumi()
            << extra->id() << endl;
    }
    out << endl;

    out << "Cutflow [" << _lepton_mode << " mode]" << endl;
    out << *_cutflow << endl;
    out << endl;

    out << "Jet Selector" << endl;
    out << *_jet_selector << endl;
    out << endl;

    switch(_lepton_mode)
    {
        case ELECTRON: out << "Electron Selector" << endl;
                       out << *_electron_selector << endl;
                       out << endl;
                       out << "Muon Veto" << endl;
                       out << *_muon_veto_selector << endl;
                       out << endl;
                       out << "Electron before Veto" << endl;
                       out << *electronBeforeVeto() << endl;
                       out << endl;
                       out << "Muon to Veto" << endl;
                       out << *muonToVeto() << endl;
                       out << endl;
                       out << "Electron after Veto" << endl;
                       out << *electronAfterVeto();
                       break;

        case MUON:     out << "Muon Selector" << endl;
                       out << *_muon_selector << endl;
                       out << endl;
                       out << "Electron Veto" << endl;
                       out << *_electron_veto_selector << endl;
                       out << endl;
                       out << "Muon before Veto" << endl;
                       out << *muonBeforeVeto() << endl;
                       out << endl;
                       out << "Electron to Veto" << endl;
                       out << *electronToVeto() << endl;
                       out << endl;
                       out << "Muon after Veto" << endl;
                       out << *muonAfterVeto();
                       break;

        default: break;
    }
}

// Private
//
bool SynchJuly2011Analyzer::jets(const Event *event)
{
    if (!event->jets().size())
        return false;

    typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;

    uint32_t selected_jets = 0;
    const Jet *leading_jet = 0;
    double leading_jet_pt = 0;
    LockSelectorEventCounterOnUpdate lock(*_jet_selector);
    for(Jets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet
                && 2 > selected_jets;
            ++jet)
    {
        if (_jet_selector->apply(*jet))
        {
            ++selected_jets;

            const double jet_pt = pt(jet->physics_object().p4());
            if (!leading_jet
                    || jet_pt > leading_jet_pt)
            {
                leading_jet = &*jet;
                leading_jet_pt = jet_pt;
            }
        }
    }

    if (1 < selected_jets
            && leading_jet)
        _leading_jet->fill(leading_jet->physics_object().p4());

    return 1 < selected_jets;
}

bool SynchJuly2011Analyzer::electron(const Event *event)
{
    if (!event->pf_electrons().size())
        return false;

    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;
    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    uint32_t selected_electrons = 0;
    const Electron *selected_electron = 0;
    LockSelectorEventCounterOnUpdate electron_lock(*_electron_selector);
    for(Electrons::const_iterator electron = event->pf_electrons().begin();
            event->pf_electrons().end() != electron
                && 2 > selected_electrons;
            ++electron)
    {
        if (_electron_selector->apply(*electron, pv))
        {
            ++selected_electrons;

            if (!selected_electron)
                selected_electron = &*electron;
        }
    }

    if (1 != selected_electrons)
        return false;

    _cutflow->apply(LEPTON);

    if (selected_electron)
        _electron_before_veto->fill(selected_electron->physics_object().p4());

    uint32_t selected_muons = 0;
    const Muon *selected_muon = 0;
    LockSelectorEventCounterOnUpdate muon_lock(*_muon_veto_selector);
    for(Muons::const_iterator muon = event->pf_muons().begin();
            event->pf_muons().end() != muon
                &&  !selected_muons;
            ++muon)
    {
        if (_muon_veto_selector->apply(*muon, pv))
        {
            ++selected_muons;

            if (!selected_muon)
                selected_muon = &*muon;
        }
    }

    if (selected_muon)
        _muon_to_veto->fill(selected_muon->physics_object().p4());
    else if (selected_electron)
        _electron_after_veto->fill(selected_electron->physics_object().p4());

    return !selected_muons;
}

bool SynchJuly2011Analyzer::muon(const Event *event)
{
    if (!event->pf_muons().size())
        return false;

    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;
    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    uint32_t selected_muons = 0;
    const Muon *selected_muon = 0;
    LockSelectorEventCounterOnUpdate muon_lock(*_muon_selector);
    for(Muons::const_iterator muon = event->pf_muons().begin();
            event->pf_muons().end() != muon
                && 2 > selected_muons;
            ++muon)
    {
        if (_muon_selector->apply(*muon, pv))
        {
            ++selected_muons;

            if (!selected_muon)
                selected_muon = &*muon;
        }
    }

    if (1 != selected_muons)
        return false;

    _cutflow->apply(LEPTON);
    
    if (selected_muon)
        _muon_before_veto->fill(selected_muon->physics_object().p4());

    uint32_t selected_electrons = 0;
    const Electron *selected_electron = 0;
    LockSelectorEventCounterOnUpdate electron_lock(*_electron_veto_selector);
    for(Electrons::const_iterator electron = event->pf_electrons().begin();
            event->pf_electrons().end() != electron
                &&  !selected_electrons;
            ++electron)
    {
        if (_electron_veto_selector->apply(*electron, pv))
        {
            ++selected_electrons;

            if (!selected_electron)
                selected_electron = &*electron;
        }
    }

    if (selected_electron)
        _electron_to_veto->fill(selected_electron->physics_object().p4());
    else if (selected_muon)
        _muon_after_veto->fill(selected_muon->physics_object().p4());

    return !selected_electrons;
}



// Helpers
//
std::ostream &bsm::operator <<(std::ostream &out, const SynchJuly2011Analyzer::LeptonMode &mode)
{
    switch(mode)
    {
        case SynchJuly2011Analyzer::ELECTRON:   out << "electron";
                                                break;

        case SynchJuly2011Analyzer::MUON:       out << "muon";
                                                break;

        default:                                out << "unknown";
                                                break;
    }

    return out;
}
