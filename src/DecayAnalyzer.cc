// Decay Analyzer
//
// Generate decay table of processed events
//
// Created by Samvel Khalatyan, Jun 29, 2011
// Copyright 2011, All rights reserved

#include <ostream>

#include <boost/pointer_cast.hpp>

#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Jet.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_stat/interface/H2.h"
#include "interface/DecayAnalyzer.h"
#include "interface/Selector.h"
#include "interface/StatProxy.h"

using namespace std;

using boost::dynamic_pointer_cast;

using bsm::DecayAnalyzer;

DecayAnalyzer::DecayAnalyzer()
{
    _decay_level_1.reset(new H2Proxy(200, -100, 100, 200, -100, 100));
    _decay_level_2.reset(new H2Proxy(200, -100, 100, 200, -100, 100));

    monitor(_decay_level_1);
    monitor(_decay_level_2);
}

DecayAnalyzer::DecayAnalyzer(const DecayAnalyzer &object)
{
    _decay_level_1.reset(new H2Proxy(*object._decay_level_1));
    _decay_level_2.reset(new H2Proxy(*object._decay_level_2));

    monitor(_decay_level_1);
    monitor(_decay_level_2);
}

void DecayAnalyzer::onFileOpen(const std::string &filename, const Input *)
{
}

void DecayAnalyzer::process(const Event *event)
{
    if (!event->gen_particles().size())
        return;

    genParticles(event->gen_particles());
}

const bsm::H2Ptr DecayAnalyzer::decay_level_1() const
{
    return _decay_level_1->histogram();
}

const bsm::H2Ptr DecayAnalyzer::decay_level_2() const
{
    return _decay_level_2->histogram();
}

uint32_t DecayAnalyzer::id() const
{
    return core::ID<DecayAnalyzer>::get();
}

DecayAnalyzer::ObjectPtr DecayAnalyzer::clone() const
{
    return ObjectPtr(new DecayAnalyzer(*this));
}

void DecayAnalyzer::print(std::ostream &out) const
{
    out << setw(15) << left << " [Decay L1]" << *decay_level_1() << endl;
    out << setw(15) << left << " [Decay L2]" << *decay_level_2();
}

// Privates
//
void DecayAnalyzer::genParticles(const GenParticles &particles,
        const GenParticle *parent,
        const uint32_t &level)
{
    for(GenParticles::const_iterator particle = particles.begin();
            particles.end() != particle;
            ++particle)
    {
        if (parent)
        {
            const H2Ptr histogram = (1 == level
                ? decay_level_1()
                : decay_level_2());

            histogram->fill(particle->id(), parent->id());
        }

        if (level < 2)
            genParticles(particle->children(), &*particle, level + 1);
    }
}
