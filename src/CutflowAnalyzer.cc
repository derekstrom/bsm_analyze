// Cutflow Analyzer
//
// Generate cutflow table after selector is applied
//
// Created by Samvel Khalatyan, May 18, 2011
// Copyright 2011, All rights reserved

#include <iomanip>
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
using bsm::MultiplicityCutflow;

CutflowAnalyzer::CutflowAnalyzer()
{
    _el_selector.reset(new ElectronSelector());
    _el_number_selector.reset(new MultiplicityCutflow(4));

    _mu_selector.reset(new MuonSelector());
    _mu_number_selector.reset(new MultiplicityCutflow(4));
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
    _el_number_selector->merge(analyzer->_el_number_selector);

    _mu_selector->merge(analyzer->_mu_selector);
    _mu_number_selector->merge(analyzer->_mu_number_selector);
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

    (*_el_number_selector)(selected_electrons.size());
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

    (*_mu_number_selector)(selected_muons.size());
}

void CutflowAnalyzer::print(std::ostream &out) const
{
    out << "Electron Selector" << endl;
    out << *_el_selector << endl;
    out << endl;
    out << "Electron Multiplicity" << endl;
    out << *_el_number_selector << endl;
    out << endl;

    out << "Muon Selector" << endl;
    out << *_mu_selector << endl;
    out << endl;
    out << "Muon Multiplicity" << endl;
    out << *_mu_number_selector << endl;
    out << endl;
}

CutflowAnalyzer::operator bool() const
{
    return _el_selector
        && _el_number_selector
        && _mu_selector
        && _mu_number_selector;
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
    for(Cuts::iterator cut = _cuts.begin();
            _cuts.end() != cut;
            ++cut)
    {
        (**cut)(number);
    }
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
        *cut(i) = *selector->cut(i);
    }
}



// Helpers
//
std::ostream &bsm::operator <<(std::ostream &out,
        const MultiplicityCutflow &cutflow)
{
    cutflow.print(out);

    return out;
}
