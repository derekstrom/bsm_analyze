// Filter Events
//
// Apply selectors and filter event objects
//
// Created by Samvel Khalatyan, May 20, 2011
// Copyright 2011, All rights reserved

#include <ostream>

#include <boost/version.hpp>
#include <boost/filesystem.hpp>
#include <boost/pointer_cast.hpp>

#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Input.pb.h"
#include "bsm_input/interface/MissingEnergy.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Writer.h"
#include "interface/FilterAnalyzer.h"
#include "interface/Selector.h"

using bsm::FilterAnalyzer;

using boost::dynamic_pointer_cast;
namespace fs = boost::filesystem;

using std::ostream;
using std::endl;

FilterAnalyzer::FilterAnalyzer()
{
    _pf_el_selector.reset(new ElectronSelector());
    _gsf_el_selector.reset(new ElectronSelector());

    _pf_mu_selector.reset(new MuonSelector());
    _reco_mu_selector.reset(new MuonSelector());

    monitor(_pf_el_selector);
    monitor(_gsf_el_selector);

    monitor(_pf_mu_selector);
    monitor(_reco_mu_selector);
}

FilterAnalyzer::FilterAnalyzer(const FilterAnalyzer &object)
{
    _pf_el_selector = 
        dynamic_pointer_cast<ElectronSelector>(object._pf_el_selector->clone());

    _gsf_el_selector =
        dynamic_pointer_cast<ElectronSelector>(object._gsf_el_selector->clone());

    _pf_mu_selector =
        dynamic_pointer_cast<MuonSelector>(object._pf_mu_selector->clone());

    _reco_mu_selector =
        dynamic_pointer_cast<MuonSelector>(object._reco_mu_selector->clone());

    monitor(_pf_el_selector);
    monitor(_gsf_el_selector);

    monitor(_pf_mu_selector);
    monitor(_reco_mu_selector);
}

void FilterAnalyzer::onFileOpen(const std::string &filename, const Input *input)
{
    fs::path path(filename);

#if BOOST_VERSION < 104600
    _writer.reset(new Writer(path.stem() + std::string("_filtered")
                + path.extension()));
#else
    _writer.reset(new Writer(path.stem().generic_string() + std::string("_filtered")
                + path.extension().generic_string()));
#endif

    _writer->open();
    if (!_writer->isOpen())
    {
        _writer.reset();

        return;
    }

    *_writer->input() = *input;

    _event.reset(new Event());
}

void FilterAnalyzer::process(const Event *event)
{
    if (!_writer
            || !_event
            || !event->primary_vertices().size())
        return;

    primaryVertices(event);
    electrons(event);
    muons(event);
    missing_energy(event);
    jets(event);

    _writer->write(_event);

    _event->Clear();
}

uint32_t FilterAnalyzer::id() const
{
    return core::ID<FilterAnalyzer>::get();
}

FilterAnalyzer::ObjectPtr FilterAnalyzer::clone() const
{
    return ObjectPtr(new FilterAnalyzer(*this));
}

void FilterAnalyzer::print(std::ostream &out) const
{
    out << "Particle-Flow Electrons" << endl;
    out << *_pf_el_selector << endl;
    out << endl;

    out << "Gsf Electrons" << endl;
    out << *_gsf_el_selector << endl;
    out << endl;

    out << "Particle-Flow Muons" << endl;
    out << *_pf_mu_selector << endl;
    out << endl;

    out << "Reco Muons" << endl;
    out << *_reco_mu_selector;
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
        LockSelectorEventCounterOnUpdate lock(*_pf_el_selector);
        for(Electrons::const_iterator el = event->pf_electrons().begin();
                event->pf_electrons().end() != el;
                ++el)
        {
            if (_pf_el_selector->apply(*el, pv))
            {
                bsm::Electron *electron = _event->add_pf_electrons();

                *electron = *el;
            }
        }
    }

    if (event->gsf_electrons().size())
    {
        LockSelectorEventCounterOnUpdate lock(*_gsf_el_selector);
        for(Electrons::const_iterator el = event->gsf_electrons().begin();
                event->gsf_electrons().end() != el;
                ++el)
        {
            if (_gsf_el_selector->apply(*el, pv))
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
        LockSelectorEventCounterOnUpdate lock(*_pf_mu_selector);
        for(Muons::const_iterator mu = event->pf_muons().begin();
                event->pf_muons().end() != mu;
                ++mu)
        {
            if (_pf_mu_selector->apply(*mu, pv))
            {
                bsm::Muon *muon = _event->add_pf_muons();

                *muon = *mu;
            }
        }
    }

    if (event->reco_muons().size())
    {
        LockSelectorEventCounterOnUpdate lock(*_reco_mu_selector);
        for(Muons::const_iterator mu = event->reco_muons().begin();
                event->reco_muons().end() != mu;
                ++mu)
        {
            if (_reco_mu_selector->apply(*mu, pv))
            {
                bsm::Muon *muon = _event->add_reco_muons();

                *muon = *mu;
            }
        }
    }
}

void FilterAnalyzer::missing_energy(const Event *event)
{
    if (!event->has_missing_energy())
        return;

    bsm::MissingEnergy *met = _event->mutable_missing_energy();

    *met = event->missing_energy();
}

void FilterAnalyzer::jets(const Event *event)
{
    typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;

    for(Jets::const_iterator jet = event->jets().begin();
            event->jets().end() != jet;
            ++jet)
    {
        bsm::Jet *pb_jet = _event->add_jets();

        *pb_jet = *jet;
    }
}
