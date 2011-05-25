// Filter Events
//
// Apply selectors and filter event objects
//
// Created by Samvel Khalatyan, May 20, 2011
// Copyright 2011, All rights reserved

#include <ostream>

#include <boost/filesystem.hpp>
#include <boost/pointer_cast.hpp>

#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Input.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Writer.h"
#include "interface/FilterAnalyzer.h"
#include "interface/Selector.h"

using bsm::FilterAnalyzer;

using boost::dynamic_pointer_cast;

using std::ostream;
using std::endl;

FilterAnalyzer::FilterAnalyzer()
{
    _pf_el_selector.reset(new ElectronSelector());
    _gsf_el_selector.reset(new ElectronSelector());

    _pf_mu_selector.reset(new MuonSelector());
    _reco_mu_selector.reset(new MuonSelector());
}

FilterAnalyzer::~FilterAnalyzer()
{
}

FilterAnalyzer::AnalyzerPtr FilterAnalyzer::clone() const
{
    return AnalyzerPtr(new FilterAnalyzer());
}

void FilterAnalyzer::merge(const AnalyzerPtr &analyzer_ptr)
{
    boost::shared_ptr<FilterAnalyzer> analyzer =
        dynamic_pointer_cast<FilterAnalyzer>(analyzer_ptr);

    if (!analyzer)
        return;

    _pf_el_selector->merge(analyzer->_pf_el_selector);
    _gsf_el_selector->merge(analyzer->_gsf_el_selector);

    _pf_mu_selector->merge(analyzer->_pf_mu_selector);
    _reco_mu_selector->merge(analyzer->_reco_mu_selector);
}

void FilterAnalyzer::onFileOpen(const std::string &filename, const Input *input)
{
    namespace fs = boost::filesystem;

    fs::path path(filename);
    _writer.reset(new Writer(path.stem().generic_string() + std::string("_filtered")
                + path.extension().generic_string()));

    *_writer->input() = *input;

    _event.reset(new Event());
}

void FilterAnalyzer::process(const Event *event)
{
    if (!_writer
            || !event->primary_vertices().size())
        return;

    primaryVertices(event);
    electrons(event);
    muons(event);

    _writer->write(*_event);

    _event->Clear();
}

void FilterAnalyzer::print(std::ostream &out) const
{
    out << "Particle-Flow Electrons" << endl;
    out << "[Selector]" << endl;
    out << *_pf_el_selector << endl;
    out << endl;

    out << "Gsf Electrons" << endl;
    out << "[Selector]" << endl;
    out << *_gsf_el_selector << endl;
    out << endl;

    out << "Particle-Flow Muons" << endl;
    out << "[Selector]" << endl;
    out << *_pf_mu_selector << endl;

    out << "Reco Muons" << endl;
    out << "[Selector]" << endl;
    out << *_reco_mu_selector << endl;
    out << endl;
}

FilterAnalyzer::operator bool() const
{
    return _pf_el_selector
        && _gsf_el_selector
        && _pf_mu_selector
        && _reco_mu_selector;
}

// Private
//
void FilterAnalyzer::primaryVertices(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<PrimaryVertex> PrimaryVertices;

    for(PrimaryVertices::const_iterator pv = event->primary_vertices().begin();
            event->primary_vertices().end() != pv;
            ++pv)
    {
        bsm::PrimaryVertex *vertex = _event->add_primary_vertices();

        *vertex = *pv;
    }
}

void FilterAnalyzer::electrons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    if (event->pf_electrons().size())
    {
        selector::LockSelectorEventCounterOnUpdate lock(*_pf_el_selector);
        for(Electrons::const_iterator el = event->pf_electrons().begin();
                event->pf_electrons().end() != el;
                ++el)
        {
            if ((*_pf_el_selector)(*el, pv))
            {
                bsm::Electron *electron = _event->add_pf_electrons();

                *electron = *el;
            }
        }
    }

    if (event->gsf_electrons().size())
    {
        selector::LockSelectorEventCounterOnUpdate lock(*_gsf_el_selector);
        for(Electrons::const_iterator el = event->gsf_electrons().begin();
                event->gsf_electrons().end() != el;
                ++el)
        {
            if ((*_gsf_el_selector)(*el, pv))
            {
                bsm::Electron *electron = _event->add_gsf_electrons();

                *electron = *el;
            }
        }
    }
}

void FilterAnalyzer::muons(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

    const PrimaryVertex &pv = *event->primary_vertices().begin();

    if (event->pf_muons().size())
    {
        selector::LockSelectorEventCounterOnUpdate lock(*_pf_mu_selector);
        for(Muons::const_iterator mu = event->pf_muons().begin();
                event->pf_muons().end() != mu;
                ++mu)
        {
            if ((*_pf_mu_selector)(*mu, pv))
            {
                bsm::Muon *muon = _event->add_pf_muons();

                *muon = *mu;
            }
        }
    }

    if (event->reco_muons().size())
    {
        selector::LockSelectorEventCounterOnUpdate lock(*_reco_mu_selector);
        for(Muons::const_iterator mu = event->reco_muons().begin();
                event->reco_muons().end() != mu;
                ++mu)
        {
            if ((*_reco_mu_selector)(*mu, pv))
            {
                bsm::Muon *muon = _event->add_reco_muons();

                *muon = *mu;
            }
        }
    }
}
