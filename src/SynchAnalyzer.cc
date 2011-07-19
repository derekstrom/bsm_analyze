// Synchronization Exercises
//
// Different Analyzers that performan the sinchronization exercises at
// different development stages
//
// Created by Samvel Khalatyan, Jul 05, 2011
// Copyright 2011, All rights reserved

#include <ostream>
#include <iostream>
#include <string>
#include <utility>

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/Event.pb.h"
<<<<<<< HEAD
#include "bsm_input/interface/Input.pb.h"
#include "bsm_input/interface/Trigger.pb.h"
#include "interface/TriggerAnalyzer.h"
=======
#include "JetMETObjects/interface/FactorizedJetCorrector.h"
>>>>>>> upstream/master
#include "interface/Monitor.h"
#include "interface/Selector.h"
#include "interface/SynchAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::SynchJuly2011Analyzer;
using bsm::SynchJECJuly2011Analyzer;

int processevts = 0;
int passevts = 0;
int passtrig1 = 0;
int passtrig2 = 0;
int passtrig3 = 0;
int passtrig4 = 0;
int passtrig5 = 0;
int passtrig6 = 0;
int passtrig7 = 0;
int passtrig8 = 0;
int passtrig9 = 0;
int passtrig10 = 0;
int passtrig11 = 0;
int passtrig12 = 0;
int passtrig13 = 0;
int passtrig14 = 0;
int passtrig15 = 0;
int passtrig16 = 0;


SynchJuly2011Analyzer::SynchJuly2011Analyzer(const LeptonMode &mode):
  _lepton_mode(mode)
{
<<<<<<< HEAD
  _cutflow.reset(new MultiplicityCutflow(4));
  _cutflow->cut(0)->setName("pre-selection");
  _cutflow->cut(1)->setName("Good Primary Vertex");
  _cutflow->cut(2)->setName("2 Good Jets");
  _cutflow->cut(3)->setName(string("Good ")
			    + (ELECTRON == _lepton_mode ? "Electron" : "Muon"));
  _cutflow->cut(4)->setName(string("Veto Good ")
			    + (ELECTRON == _lepton_mode ? "Muon" : "Electron"));
  monitor(_cutflow);
  
  // Selectors
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
  
=======
    _cutflow.reset(new MultiplicityCutflow(4));
    _cutflow->cut(PRESELECTION)->setName("pre-selection");
    _cutflow->cut(PRIMARY_VERTEX)->setName("Good Primary Vertex");
    _cutflow->cut(JET)->setName("2 Good Jets");
    _cutflow->cut(LEPTON)->setName(string("Good ")
            + (ELECTRON == _lepton_mode ? "Electron" : "Muon"));
    _cutflow->cut(VETO_SECOND_LEPTON)->setName(string("Veto Good ")
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
>>>>>>> upstream/master
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


void SynchJuly2011Analyzer::onFileOpen(const std::string &filename, const Input *input)
{
  if (!input->has_info())
    return;
  
  
  
  typedef ::google::protobuf::RepeatedPtrField<TriggerItem> TriggerItems;
  for(TriggerItems::const_iterator hlt = input->info().triggers().begin();
      input->info().triggers().end() != hlt;
      ++hlt)
    {
      if (_hlt_map.end() != _hlt_map.find(hlt->hash()))
	continue;
      
      _hlt_map[hlt->hash()] = hlt->name();
    }
}

void SynchJuly2011Analyzer::process(const Event *event)
{
  
  ++processevts;
  
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
  
  
  if (!trigger(event))
    return;
  
  ++passevts;
  
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
  
  cout << endl << endl;
  cout << "processevts: " << processevts << endl;
  cout << "passevts: " << passevts << endl;
  cout << "passtrig1: " << passtrig1 << endl;
  cout << "passtrig2: " << passtrig2 << endl;
  cout << "passtrig3: " << passtrig3 << endl;
  cout << "passtrig4: " << passtrig4 << endl;
  cout << "passtrig5: " << passtrig5 << endl;
  cout << "passtrig6: " << passtrig6 << endl;
  cout << "passtrig7: " << passtrig7 << endl;
  cout << "passtrig8: " << passtrig8 << endl;
  cout << "passtrig9: " << passtrig9 << endl;
  cout << "passtrig10: " << passtrig10 << endl;
  cout << "passtrig11: " << passtrig11 << endl;
  cout << "passtrig12: " << passtrig12 << endl;
  cout << "passtrig13: " << passtrig13 << endl;
  cout << "passtrig14: " << passtrig14 << endl;
  cout << "passtrig15: " << passtrig15 << endl;
  cout << "passtrig16: " << passtrig16 << endl;
  
}




// Private
//
bool SynchJuly2011Analyzer::jets(const Event *event)
{
<<<<<<< HEAD
  if (!event->jets().size())
    return false;
  
  typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;
  
  uint32_t selected_jets = 0;
  const Jet *leading_jet = 0;
  double leading_jet_pt = 0;
  double leading_jet_pt_cut = 250;
  
  LockSelectorEventCounterOnUpdate lock(*_jet_selector);
  for(Jets::const_iterator jet = event->jets().begin();
      event->jets().end() != jet
	&& 2 > selected_jets;
      ++jet)
=======
    if (!event->jets().size())
        return false;

    typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;

    uint32_t selected_jets = 0;
    const Jet *leading_jet = 0;
    float leading_jet_pt = 0;
    LockSelectorEventCounterOnUpdate lock(*_jet_selector);
    for(Jets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet
                && 2 > selected_jets;
            ++jet)
>>>>>>> upstream/master
    {
      
      
      if (_jet_selector->apply(*jet))
        {
<<<<<<< HEAD
	  ++selected_jets;
	  
	  const double jet_pt = pt(jet->physics_object().p4());
	  if (!leading_jet
	      || jet_pt > leading_jet_pt)
=======
            ++selected_jets;

            const float jet_pt = pt(jet->physics_object().p4());
            if (!leading_jet
                    || jet_pt > leading_jet_pt)
>>>>>>> upstream/master
            {
	      leading_jet = &*jet;
	      leading_jet_pt = jet_pt;
            }
        }
      
    }
  
  if (1 < selected_jets
      && leading_jet && leading_jet_pt > leading_jet_pt_cut)
    _leading_jet->fill(leading_jet->physics_object().p4());
  
  //    return 1 < selected_jets; 
  return 1 < selected_jets && leading_jet_pt > leading_jet_pt_cut;
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

bool SynchJuly2011Analyzer::trigger(const Event *event)
{
  if (!event->hlts().size())
    return false;
  
  typedef ::google::protobuf::RepeatedPtrField<Trigger> Triggers;
  
  for(Triggers::const_iterator hlt = event->hlts().begin();
      event->hlts().end() != hlt;
      ++hlt)
    {
      
      if (_hlt_map[hlt->hash()] == "hlt_ele10_caloidt_caloisovl_trkidt_trkisovl_ht200" && hlt->pass())  // 3  1   40568
	++passtrig1;
      if (_hlt_map[hlt->hash()] == "hlt_ele45_caloidvt_trkidt" && hlt->pass())                          // 2  1   27071
	++passtrig2;
      if (_hlt_map[hlt->hash()] == "hlt_ele25_caloidvt_trkidt_centraljet30" && hlt->pass())             // 2  1   35936
	++passtrig3;
      if (_hlt_map[hlt->hash()] == "hlt_ele25_caloidvt_trkidt_centraltrijet30" && hlt->pass())          // 2  1   30261
	++passtrig4;
      if (_hlt_map[hlt->hash()] == "hlt_ele8_caloidl_caloisovl_jet40" && hlt->pass())                   // 2  1   59030
	++passtrig5;
      if (_hlt_map[hlt->hash()] == "hlt_ele27_caloidvt_caloisot_trkidt_trkisot" && hlt->pass())         // 2  1   31390
	++passtrig6;
      if (_hlt_map[hlt->hash()] == "hlt_ele32_caloidl_caloisovl_sc17" && hlt->pass())                   // 2  1   17206
	++passtrig7;
      if (_hlt_map[hlt->hash()] == "hlt_ele25_caloidvt_trkidt_centraljet40_btagip" && hlt->pass())      // 2  1   30835
	++passtrig8;
      if (_hlt_map[hlt->hash()] == "hlt_ele32_caloidvt_caloisot_trkidt_trkisot" && hlt->pass())         // 1  1   29449
	++passtrig9;
      if (_hlt_map[hlt->hash()] == "hlt_ele25_caloidvt_trkidt_centraldijet30" && hlt->pass())           // 2  1   34957
	++passtrig10;
      if (_hlt_map[hlt->hash()] == "hlt_ele17_caloidl_caloisovl" && hlt->pass())                        // 2  1   53099
	++passtrig11;
      if (_hlt_map[hlt->hash()] == "hlt_ele10_caloidl_caloisovl_trkidvl_trkisovl_ht200" && hlt->pass()) // 3  1   46410
	++passtrig12;
      if (_hlt_map[hlt->hash()] == "hlt_ele15_caloidvt_caloisot_trkidt_trkisot" && hlt->pass())       //  2  1   35389
	++passtrig13;
      if (_hlt_map[hlt->hash()] == "hlt_ele8_caloidl_trkidvl" && hlt->pass())                           // 2  1   67331
	++passtrig14;
      if (_hlt_map[hlt->hash()] == "hlt_ele17_caloidvt_caloisovt_trkidt_trkisovt_sc8_mass30" && hlt->pass()) // 2  1   16791
	++passtrig15;
      if (_hlt_map[hlt->hash()] == "hlt_ele8_caloidl_caloisovl" && hlt->pass())                       //  2  1   59231
	++passtrig16;

    }
  
  return 1;
}


<<<<<<< HEAD
=======

// Synch with Jet Energy corrections
//
SynchJECJuly2011Analyzer::SynchJECJuly2011Analyzer(const LeptonMode &mode):
    _lepton_mode(mode)
{
    _cutflow.reset(new MultiplicityCutflow(6));
    _cutflow->cut(PRESELECTION)->setName("pre-selection");
    _cutflow->cut(SCRAPING)->setName("Scraping Veto");
    _cutflow->cut(HBHENOISE)->setName("HBHE Noise");
    _cutflow->cut(PRIMARY_VERTEX)->setName("Good Primary Vertex");
    _cutflow->cut(JET)->setName("2 Good Jets");
    _cutflow->cut(LEPTON)->setName(string("Good ")
            + (ELECTRON == _lepton_mode ? "Electron" : "Muon"));
    _cutflow->cut(VETO_SECOND_LEPTON)->setName(string("Veto Good ")
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

SynchJECJuly2011Analyzer::SynchJECJuly2011Analyzer(const SynchJECJuly2011Analyzer &object):
    _lepton_mode(object._lepton_mode)
{
    setJetEnergyCorrections(object._corrections);

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

void SynchJECJuly2011Analyzer::setJetEnergyCorrections(const Corrections &corrections)
{
    copyCorrections(corrections);

    _jec.reset(new FactorizedJetCorrector(_corrections));
}

const SynchJECJuly2011Analyzer::P4MonitorPtr SynchJECJuly2011Analyzer::leadingJet() const
{
    return _leading_jet;
}

const SynchJECJuly2011Analyzer::P4MonitorPtr SynchJECJuly2011Analyzer::electronBeforeVeto() const
{
    return _electron_before_veto;
}

const SynchJECJuly2011Analyzer::P4MonitorPtr SynchJECJuly2011Analyzer::muonToVeto() const
{
    return _muon_to_veto;
}

const SynchJECJuly2011Analyzer::P4MonitorPtr SynchJECJuly2011Analyzer::electronAfterVeto() const
{
    return _electron_after_veto;
}

const SynchJECJuly2011Analyzer::P4MonitorPtr SynchJECJuly2011Analyzer::muonBeforeVeto() const
{
    return _muon_before_veto;
}

const SynchJECJuly2011Analyzer::P4MonitorPtr SynchJECJuly2011Analyzer::electronToVeto() const
{
    return _electron_to_veto;
}

const SynchJECJuly2011Analyzer::P4MonitorPtr SynchJECJuly2011Analyzer::muonAfterVeto() const
{
    return _muon_after_veto;
}

void SynchJECJuly2011Analyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void SynchJECJuly2011Analyzer::process(const Event *event)
{
    if (!_jec)
        return;

    _cutflow->apply(PRESELECTION);

    if (event->has_filters()
            && !event->filters().scraping_veto())
        return;

    _cutflow->apply(SCRAPING);

    _cutflow->apply(HBHENOISE);

    if (!event->primary_vertices().size()
            || !_primary_vertex_selector->apply(*event->primary_vertices().begin()))
        return;

    _cutflow->apply(PRIMARY_VERTEX);

    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();
    GoodElectrons good_electrons;

    LockSelectorEventCounterOnUpdate electron_lock(*_electron_selector);
    for(Electrons::const_iterator electron = event->pf_electrons().begin();
            event->pf_electrons().end() != electron;
            ++electron)
    {
        if (_electron_selector->apply(*electron, pv))
            good_electrons.push_back(&*electron);
    }

    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    GoodMuons good_muons;

    LockSelectorEventCounterOnUpdate muon_lock(*_muon_selector);
    for(Muons::const_iterator muon = event->pf_muons().begin();
            event->pf_muons().end() != muon;
            ++muon)
    {
        if (_muon_selector->apply(*muon, pv))
            good_muons.push_back(&*muon);
    }

    GoodJets good_jets = jets(event, good_electrons, good_muons);

    if (2 > good_jets.size())
        return;

    _cutflow->apply(JET);

    if (ELECTRON == _lepton_mode)
    {
        if (1 != good_electrons.size())
            return;

        _cutflow->apply(LEPTON);

        if (good_muons.size())
            return;
    }
    else if (MUON == _lepton_mode)
    {
        if (1 != good_muons.size())
            return;

        _cutflow->apply(LEPTON);

        if (good_electrons.size())
            return;
    }
    else
        return;

    _cutflow->apply(VETO_SECOND_LEPTON);

    _passed_events.push_back(event->extra());
}

uint32_t SynchJECJuly2011Analyzer::id() const
{
    return core::ID<SynchJECJuly2011Analyzer>::get();
}

SynchJECJuly2011Analyzer::ObjectPtr SynchJECJuly2011Analyzer::clone() const
{
    return ObjectPtr(new SynchJECJuly2011Analyzer(*this));
}

void SynchJECJuly2011Analyzer::merge(const ObjectPtr &pointer)
{
    if (pointer->id() != id())
        return;

    boost::shared_ptr<SynchJECJuly2011Analyzer> object =
        dynamic_pointer_cast<SynchJECJuly2011Analyzer>(pointer);

    if (!object)
        return;

    Object::merge(pointer);

    _passed_events.insert(_passed_events.end(),
            object->_passed_events.begin(),
            object->_passed_events.end());

    _out << endl;
    _out << object->_out.str();
}

void SynchJECJuly2011Analyzer::print(std::ostream &out) const
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

    out << endl;
    out << _out.str();
}

// Private
//
void SynchJECJuly2011Analyzer::copyCorrections(const Corrections &corrections)
{
    _corrections.clear();

    if (!corrections.size())
        return;

    _corrections.resize(corrections.size());
    copy(corrections.begin(), corrections.end(), _corrections.begin());
}

SynchJECJuly2011Analyzer::GoodJets
    SynchJECJuly2011Analyzer::jets(const Event *event,
            const GoodElectrons &electrons,
            const GoodMuons &muons)
{
    GoodJets good_jets;

    typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;
    typedef ::google::protobuf::RepeatedPtrField<Jet::Child> Children;

    const float rho = event->extra().rho();

    std::ostringstream jets_out;

    std::ostringstream out;

    /*
    for(GoodElectrons::const_iterator electron = electrons.begin();
            electrons.end() != electron;
            ++electron)
    {
        jets_out << "el: ";
        printP4(jets_out, (*electron)->physics_object().p4());
        jets_out << endl;
    }

    for(GoodMuons::const_iterator muon = muons.begin();
            muons.end() != muon;
            ++muon)
    {
        jets_out << "mu: ";
        printP4(jets_out, (*muon)->physics_object().p4());
        jets_out << endl;
    }
    */

    LockSelectorEventCounterOnUpdate lock(*_jet_selector);
    for(Jets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet;
            ++jet)
    {
        // Skip jet is required information is not available
        //
        if (!jet->has_uncorrected_p4())
            continue;

        out.str("");

        // Remove Leptons
        //
        LorentzVector corrected_p4 = jet->uncorrected_p4();
        for(Children::const_iterator child = jet->children().begin();
                jet->children().end() != child;
                ++child)
        {
            const LorentzVector &child_p4 = child->physics_object().p4();

            // Electrons
            //
            for(GoodElectrons::const_iterator electron = electrons.begin();
                    electrons.end() != electron;
                    ++electron)
            {
                const LorentzVector &electron_p4 = (*electron)->physics_object().p4();
                if (electron_p4 == child_p4)
                {
                    corrected_p4 -= electron_p4;

                    out << setw(20) << "Electron ";
                    printP4(out, electron_p4);
                    out << endl;
                }
            }

            // Muons
            //
            for(GoodMuons::const_iterator muon = muons.begin();
                    muons.end() != muon;
                    ++muon)
            {
                const LorentzVector &muon_p4 = (*muon)->physics_object().p4();
                if (muon_p4 == child_p4)
                {
                    corrected_p4 -= muon_p4;

                    out << setw(20) << "Muon ";
                    printP4(out, muon_p4);
                    out << endl;
                }
            }
        }

        // fix p4
        //
        _jec->setJetEta(eta(corrected_p4));
        _jec->setJetPt(pt(corrected_p4));
        _jec->setJetE(corrected_p4.e());
        _jec->setNPV(event->primary_vertices().size());
        _jec->setJetA(jet->extra().area());
        _jec->setRho(rho);

        float correction = _jec->getCorrection();
        corrected_p4 *= correction;

        if (!out.str().empty())
        {
            jets_out << setw(20) << "Uncorrected Jet ";
            printP4(jets_out,  jet->uncorrected_p4());
            jets_out << endl << out.str();
            jets_out << setw(20) << "Corrected Jet ";
            printP4(jets_out, corrected_p4);
            jets_out << endl;
            jets_out << setw(20) << "PAT Jet ";
            printP4(jets_out, jet->physics_object().p4());
            jets_out << endl;
        }

        // Original jet in the event can not be modified and Jet Selector can
        // only be applied to jet: therefore copy jet, set corrected p4 and
        // apply selector
        //
        Jet corrected_jet = *jet;

        *corrected_jet.mutable_physics_object()->mutable_p4() = corrected_p4;

        if (!_jet_selector->apply(corrected_jet))
            continue;

        // Store original jet and corrected p4
        //
        CorrectedJet tmp;
        tmp.jet = &*jet;
        tmp.corrected_p4 = corrected_p4;

        good_jets.push_back(tmp);
    }

    if (!jets_out.str().empty())
    {
        _out << "Event: " << event->extra().id()
            << " Lumi: " << event->extra().lumi()
            << " Run: " << event->extra().run() << endl;
        _out << "good el: " << electrons.size() << " mu: " << muons.size() << endl;

        _out << jets_out.str() << endl;
    }

    return good_jets;
}

void SynchJECJuly2011Analyzer::printP4(std::ostream &out, const LorentzVector &p4)
{
    out << "pT: " << pt(p4)
    << " eta: " << eta(p4)
    << " phi: " << phi(p4);
}


>>>>>>> upstream/master
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

std::ostream &bsm::operator <<(std::ostream &out, const SynchJECJuly2011Analyzer::LeptonMode &mode)
{
    switch(mode)
    {
        case SynchJECJuly2011Analyzer::ELECTRON:   out << "electron";
                                                break;

        case SynchJECJuly2011Analyzer::MUON:       out << "muon";
                                                break;

        default:                                out << "unknown";
                                                break;
    }

    return out;
}
