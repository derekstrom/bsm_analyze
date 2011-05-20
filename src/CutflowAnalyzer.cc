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
    _pv_multiplicity.reset(new MultiplicityCutflow(16));

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

    _pv_multiplicity->merge(analyzer->_pv_multiplicity);

    _pf_el_selector->merge(analyzer->_pf_el_selector);
    _pf_el_number_selector->merge(analyzer->_pf_el_number_selector);

    _gsf_el_selector->merge(analyzer->_gsf_el_selector);
    _gsf_el_number_selector->merge(analyzer->_gsf_el_number_selector);

    _pf_mu_selector_step1->merge(analyzer->_pf_mu_selector_step1);
    _pf_mu_number_selector_step1->merge(analyzer->_pf_mu_number_selector_step1);

    _pf_mu_selector->merge(analyzer->_pf_mu_selector);
    _pf_mu_number_selector->merge(analyzer->_pf_mu_number_selector);

    _reco_mu_selector_step1->merge(analyzer->_reco_mu_selector_step1);
    _reco_mu_number_selector_step1->merge(analyzer->_reco_mu_number_selector_step1);

    _reco_mu_selector->merge(analyzer->_reco_mu_selector);
    _reco_mu_number_selector->merge(analyzer->_reco_mu_number_selector);
}

void CutflowAnalyzer::process(const Event *event)
{
    (*_pv_multiplicity)(event->primary_vertices().size());

    if (!event->primary_vertices().size())
        return;

    electrons(event);
    muons(event);
}

void CutflowAnalyzer::electrons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    if (event->pf_electrons().size())
    {
        uint32_t selected_electrons;

        selector::LockSelectorEventCounterOnUpdate lock(*_pf_el_selector);
        for(Electrons::const_iterator el = event->pf_electrons().begin();
                event->pf_electrons().end() != el;
                ++el)
        {
            if ((*_pf_el_selector)(*el, pv))
                ++selected_electrons;
        }

        (*_pf_el_number_selector)(selected_electrons);
    }

    if (event->gsf_electrons().size())
    {
        uint32_t selected_electrons = 0;

        selector::LockSelectorEventCounterOnUpdate lock(*_gsf_el_selector);
        for(Electrons::const_iterator el = event->gsf_electrons().begin();
                event->gsf_electrons().end() != el;
                ++el)
        {
            if ((*_gsf_el_selector)(*el, pv))
                ++selected_electrons;
        }

        (*_gsf_el_number_selector)(selected_electrons);
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

        selector::LockSelectorEventCounterOnUpdate lock(*_pf_mu_selector);
        for(Muons::const_iterator mu = event->pf_muons().begin();
                event->pf_muons().end() != mu;
                ++mu)
        {
            if ((*_pf_mu_selector_step1)(*mu, pv))
                ++selected_muons_step1;

            if ((*_pf_mu_selector)(*mu, pv))
                ++selected_muons;
        }

        (*_pf_mu_number_selector_step1)(selected_muons_step1);
        (*_pf_mu_number_selector)(selected_muons);
    }

    if (event->reco_muons().size())
    {
        uint32_t selected_muons = 0;
        uint32_t selected_muons_step1 = 0;

        selector::LockSelectorEventCounterOnUpdate lock(*_reco_mu_selector);
        for(Muons::const_iterator mu = event->reco_muons().begin();
                event->reco_muons().end() != mu;
                ++mu)
        {
            if ((*_reco_mu_selector_step1)(*mu, pv))
                ++selected_muons_step1;

            if ((*_reco_mu_selector)(*mu, pv))
                ++selected_muons;
        }

        (*_reco_mu_number_selector_step1)(selected_muons_step1);
        (*_reco_mu_number_selector)(selected_muons);
    }
}

void CutflowAnalyzer::print(std::ostream &out) const
{
    out << "Primary Vertices" << endl;
    out << *_pv_multiplicity << endl;

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
    out << *_reco_mu_number_selector << endl;
    out << endl;
}

CutflowAnalyzer::operator bool() const
{
    return _pv_multiplicity
        && _pf_el_selector
        && _pf_el_number_selector
        && _gsf_el_selector
        && _gsf_el_number_selector
        && _pf_mu_selector_step1
        && _pf_mu_number_selector_step1
        && _pf_mu_selector
        && _pf_mu_number_selector
        && _reco_mu_selector_step1
        && _reco_mu_number_selector_step1
        && _reco_mu_selector
        && _reco_mu_number_selector;
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



// Helpers
//
std::ostream &bsm::operator <<(std::ostream &out,
        const MultiplicityCutflow &cutflow)
{
    cutflow.print(out);

    return out;
}
