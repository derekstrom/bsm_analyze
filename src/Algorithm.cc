// Common Algorithm Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#include <TLorentzVector.h>

#include <boost/pointer_cast.hpp>

#include "bsm_core/interface/ID.h"
#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "bsm_input/interface/Utility.h"
#include "interface/Algorithm.h"
#include "interface/Utility.h"

using bsm::algorithm::ClosestJet;
using bsm::algorithm::NeutrinoReconstruct;
using bsm::algorithm::HadronicDecay;
using bsm::algorithm::LeptonicDecay;

using bsm::core::ID;

ClosestJet::ClosestJet()
{
    _p4.reset(new TLorentzVector());
}

ClosestJet::ClosestJet(const ClosestJet &object)
{
    _p4.reset(new TLorentzVector());
}

const bsm::Jet *ClosestJet::find(const Jets &jets,
                                       const Electron &electron)
{
    if (!jets.size())
        return 0;

    utility::set(_p4.get(), &electron.physics_object().p4());

    return find(jets);
}

const bsm::Jet *ClosestJet::find(const Jets &jets, const Muon &muon)
{
    if (!jets.size())
        return 0;

    utility::set(_p4.get(), &muon.physics_object().p4());

    return find(jets);
}

uint32_t ClosestJet::id() const
{
    return ID<ClosestJet>::get();
}

ClosestJet::ObjectPtr ClosestJet::clone() const
{
    return ObjectPtr(new ClosestJet(*this));
}

void ClosestJet::print(std::ostream &out) const
{
    out << "nothing to print for the closest jet";
}

// Privates
//
const bsm::Jet *ClosestJet::find(const Jets &jets)
{
    TLorentzVector lepton_p4(*_p4);

    double min_delta_r = -1;
    const Jet *closest_jet = 0;
    for(Jets::const_iterator jet = jets.begin();
            jets.end() != jet;
            ++jet)
    { 
        utility::set(_p4.get(), &jet->physics_object().p4());

        double delta_r = _p4->DeltaR(lepton_p4);

        if (-1 == min_delta_r
                || delta_r < min_delta_r)
        {
            min_delta_r = delta_r;

            closest_jet = &*jet;
        }
    }

    return closest_jet;
}



// Neutrino Momenturm Reconstructor
//
NeutrinoReconstruct::NeutrinoReconstruct(const double &mass_a,
        const double &mass_b):
    _mass_a(mass_a),
    _mass_b(mass_b),
    _solutions(0)
{
    _solution_one.reset(new LorentzVector());
    _solution_two.reset(new LorentzVector());
}

NeutrinoReconstruct::NeutrinoReconstruct(const NeutrinoReconstruct &obj):
    _mass_a(obj._mass_a),
    _mass_b(obj._mass_b),
    _solutions(obj._solutions)
{
    _solution_one.reset(new LorentzVector());
    _solution_two.reset(new LorentzVector());

    *_solution_one = *obj._solution_one;
    *_solution_two = *obj._solution_two;
}

uint32_t NeutrinoReconstruct::apply(const LorentzVector &p4,
        const LorentzVector &met)
{
    reset();

    double a = _mass_a * _mass_a
        - _mass_b * _mass_b
        + 2 * p4.px() * met.px()
        + 2 * p4.py() * met.py();

    // The final equation is:
    //
    //  4 * ( E_B^2 - pz_B^2) * x^2
    //  - 4 * a * pz_B * x
    //  + [4 E_B^2 * (px_nu^2 + py_nu^2) - a^2] = 0
    //
    //  with x being the pz_nu. OR:
    //
    //      A x^2 + 2B x + C = 0
    //
    double A = 4 * (p4.e() * p4.e() - p4.pz() * p4.pz());
    double B = -2 * a * p4.pz();
    double C = 4 * p4.e() * p4.e() * ( met.px() * met.px()
            + met.py() * met.py())
        - a * a;

    double discriminant = B * B - A * C;

    if (0 > discriminant)
    {
        // Take only real part of the solution
        _solutions = 0;

        addSolution(_solution_one, met, -B / A);
    }
    else if(0 == discriminant)
    {
        _solutions = 1;

        addSolution(_solution_one, met, -B / A);
    }
    else
    {
        _solutions = 2;

        discriminant = sqrt(discriminant);

        addSolution(_solution_one, met, (-B - discriminant) / A);
        addSolution(_solution_two, met, (-B + discriminant) / A);
    }

    return _solutions;
}

NeutrinoReconstruct::P4Ptr
    NeutrinoReconstruct::solution(const uint32_t &solution) const
{
    if (!_solutions
            || !solution)
        return _solution_one;

    return _solution_two;
}

void NeutrinoReconstruct::reset()
{
    _solutions = 0;

    setSolution(_solution_one, 0, 0, 0, 0);
    setSolution(_solution_two, 0, 0, 0, 0);
}

uint32_t NeutrinoReconstruct::id() const
{
    return ID<NeutrinoReconstruct>::get();
}

NeutrinoReconstruct::ObjectPtr NeutrinoReconstruct::clone() const
{
    return ObjectPtr(new NeutrinoReconstruct(*this));
}

void NeutrinoReconstruct::merge(const ObjectPtr &object_pointer)
{
    if (id() != object_pointer->id())
        return;

    const boost::shared_ptr<NeutrinoReconstruct> object =
        boost::dynamic_pointer_cast<NeutrinoReconstruct>(object_pointer);

    if (!object)
        return;

    _mass_a = object->_mass_a;
    _mass_b = object->_mass_b;
    _solutions = object->_solutions;

    *_solution_one = *object->_solution_one;
    *_solution_two = *object->_solution_two;
}

void NeutrinoReconstruct::print(std::ostream &out) const
{
    out << _solutions << " solution(s): "
        << *_solution_one << " " << *_solution_two;
}

// Privates
//
void NeutrinoReconstruct::addSolution(P4Ptr &solution,
        const LorentzVector &p4,
        const double &pz)
{
    setSolution(solution, p4.e(), p4.px(), p4.py(), pz);
}

void NeutrinoReconstruct::setSolution(P4Ptr &solution,
        const double &e,
        const double &px,
        const double &py,
        const double &pz)
{
    solution->set_e(e);
    solution->set_px(px);
    solution->set_py(py);
    solution->set_pz(pz);
}



// Hadronic Decay
//
HadronicDecay::HadronicDecay():
    _dr(0),
    _dr_w_top(0),
    _dr_b_top(0)
{
    _top.reset(new LorentzVector());
}

HadronicDecay::HadronicDecay(const HadronicDecay &object):
    _dr(object._dr),
    _dr_w_top(object._dr_w_top),
    _dr_b_top(object._dr_b_top)
{
    _top.reset(new LorentzVector());

    *_top = *object._top;
}

double HadronicDecay::apply(const LorentzVector &w, const LorentzVector &b)
{
    _top->Clear();

    *_top += w;
    *_top += b;

    _dr_w_top = bsm::dr(w, *_top);
    _dr_b_top = bsm::dr(b, *_top);
    _dr = _dr_w_top + _dr_b_top;

    return _dr;
}

double HadronicDecay::dr() const
{
    return _dr;
}

double HadronicDecay::dr_w_top() const
{
    return _dr_w_top;
}

double HadronicDecay::dr_b_top() const
{
    return _dr_b_top;
}

uint32_t HadronicDecay::id() const
{
    return core::ID<HadronicDecay>::get();
}

HadronicDecay::ObjectPtr HadronicDecay::clone() const
{
    return ObjectPtr(new HadronicDecay(*this));
}

void HadronicDecay::merge(const ObjectPtr &object_pointer)
{
    if (id() != object_pointer->id())
        return;

    const boost::shared_ptr<HadronicDecay> object =
        boost::dynamic_pointer_cast<HadronicDecay>(object_pointer);

    if (!object)
        return;

    _dr = object->dr();
    _dr_w_top = object->dr_w_top();
    _dr_b_top = object->dr_b_top();

    *_top = *object->_top;
}

void HadronicDecay::print(std::ostream &out) const
{
    out << "dr: " << dr()
        << " dr(w,t): " << dr_w_top()
        << " dr(b,t): " << dr_b_top();
}



// Leptonic Decay
//
LeptonicDecay::LeptonicDecay():
    _dr(0),
    _dr_l_top(0),
    _dr_nu_top(0),
    _dr_b_top(0)
{
    _top.reset(new LorentzVector());
}

LeptonicDecay::LeptonicDecay(const LeptonicDecay &object):
    _dr(object._dr),
    _dr_l_top(object._dr_l_top),
    _dr_nu_top(object._dr_nu_top),
    _dr_b_top(object._dr_b_top)
{
    _top.reset(new LorentzVector());

    *_top = *object._top;
}

double LeptonicDecay::apply(const LorentzVector &l,
        const LorentzVector &nu,
        const LorentzVector &b)
{
    _top->Clear();

    *_top += l;
    *_top += nu;
    *_top += b;

    _dr_l_top = bsm::dr(l, *_top);
    _dr_nu_top = bsm::dr(nu, *_top);
    _dr_b_top = bsm::dr(b, *_top);
    _dr = _dr_l_top + _dr_nu_top + _dr_b_top;

    return _dr;
}

double LeptonicDecay::dr() const
{
    return _dr;
}

double LeptonicDecay::dr_l_top() const
{
    return _dr_l_top;
}

double LeptonicDecay::dr_nu_top() const
{
    return _dr_nu_top;
}

double LeptonicDecay::dr_b_top() const
{
    return _dr_b_top;
}

uint32_t LeptonicDecay::id() const
{
    return core::ID<LeptonicDecay>::get();
}

LeptonicDecay::ObjectPtr LeptonicDecay::clone() const
{
    return ObjectPtr(new LeptonicDecay(*this));
}

void LeptonicDecay::merge(const ObjectPtr &object_pointer)
{
    if (id() != object_pointer->id())
        return;

    const boost::shared_ptr<LeptonicDecay> object =
        boost::dynamic_pointer_cast<LeptonicDecay>(object_pointer);

    if (!object)
        return;

    _dr = object->dr();
    _dr_l_top = object->dr_l_top();
    _dr_nu_top = object->dr_nu_top();
    _dr_b_top = object->dr_b_top();

    *_top = *object->_top;
}

void LeptonicDecay::print(std::ostream &out) const
{
    out << "dr: " << dr()
        << " dr(l,t): " << dr_l_top()
        << " dr(nu,t): " << dr_nu_top()
        << " dr(b,t): " << dr_b_top();
}
