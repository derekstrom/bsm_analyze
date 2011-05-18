// Cutflow Analyzer
//
// Generate cutflow table after selector is applied
//
// Created by Samvel Khalatyan, May 18, 2011
// Copyright 2011, All rights reserved

#include <ostream>

#include <boost/pointer_cast.hpp>

#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "interface/Selector.h"
#include "interface/CutflowAnalyzer.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::AnalyzerPtr;
using bsm::CutflowAnalyzer;

CutflowAnalyzer::CutflowAnalyzer()
{
    _el_selector.reset(new ElectronSelector());
    _mu_selector.reset(new MuonSelector());
    _mu_selector->d0_bsp()->disable();
}

CutflowAnalyzer::~CutflowAnalyzer()
{
}

AnalyzerPtr CutflowAnalyzer::clone() const
{
    return AnalyzerPtr(new CutflowAnalyzer());
}

void CutflowAnalyzer::merge(const AnalyzerPtr &analyzer_ptr)
{
    boost::shared_ptr<CutflowAnalyzer> analyzer =
        dynamic_pointer_cast<CutflowAnalyzer>(analyzer_ptr);

    if (!analyzer)
        return;

    _el_selector->merge(analyzer->_el_selector);
    _mu_selector->merge(analyzer->_mu_selector);
}

void CutflowAnalyzer::process(const Event *event)
{
    if (!event->primary_vertices().size())
        return;

    electrons(event);
    muons(event);
}

void CutflowAnalyzer::electrons(const Event *event)
{
    if (!event->pf_electrons().size())
        return;

    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    Electrons selected_electrons;

    selector::LockSelectorEventCounterOnUpdate lock(*_el_selector);
    for(Electrons::const_iterator el = event->pf_electrons().begin();
            event->pf_electrons().end() != el;
            ++el)
    {
        if ((*_el_selector)(*el, pv))
            *selected_electrons.Add() = *el;
    }
}

void CutflowAnalyzer::muons(const Event *event)
{
    if (!event->pf_muons().size())
        return;

    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    Muons selected_muons;
    selector::LockSelectorEventCounterOnUpdate lock(*_mu_selector);
    for(Muons::const_iterator mu = event->pf_muons().begin();
            event->pf_muons().end() != mu;
            ++mu)
    {
        if ((*_mu_selector)(*mu, pv))
            *selected_muons.Add() = *mu;
    }
}

void CutflowAnalyzer::print(std::ostream &out) const
{
    out << "Electron Selector" << endl;
    out << *_el_selector << endl;
    out << endl;

    out << "Muon Selector" << endl;
    out << *_mu_selector << endl;
}

CutflowAnalyzer::operator bool() const
{
    return _el_selector
        && _mu_selector;
}
