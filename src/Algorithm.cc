// Common Algorithm Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#include <TLorentzVector.h>

#include "bsm_input/interface/Algebra.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/Physics.pb.h"
#include "interface/Algorithm.h"
#include "interface/Utility.h"

using bsm::algorithm::ClosestJet;
using bsm::algorithm::MissingEnergyCorrection;

ClosestJet::ClosestJet()
{
    _lepton_p4.reset(new TLorentzVector());
    _jet_p4.reset(new TLorentzVector());
}

const bsm::Jet *ClosestJet::operator()(const Jets &jets,
                                       const Electron &electron)
{
    if (!jets.size())
        return 0;

    utility::set(_lepton_p4.get(), &electron.physics_object().p4());

    return operator()(jets);
}

const bsm::Jet *ClosestJet::operator()(const Jets &jets, const Muon &muon)
{
    if (!jets.size())
        return 0;

    utility::set(_lepton_p4.get(), &muon.physics_object().p4());

    return operator()(jets);
}

// Privates
//
const bsm::Jet *ClosestJet::operator()(const Jets &jets)
{
    double min_delta_r = -1;
    const Jet *closest_jet = 0;
    for(Jets::const_iterator jet = jets.begin();
            jets.end() != jet;
            ++jet)
    { 
        utility::set(_jet_p4.get(), &jet->physics_object().p4());

        double delta_r = _lepton_p4->DeltaR(*_jet_p4);

        if (-1 == min_delta_r
                || delta_r < min_delta_r)
        {
            min_delta_r = delta_r;

            closest_jet = &*jet;
        }
    }

    return closest_jet;
}



// Missing Energy Correct
//
MissingEnergyCorrection::MissingEnergyCorrection(const double &mass_a):
    _mass_a(mass_a),
    _solutions(0)
{
    _solution_one.reset(new LorentzVector());
    _solution_two.reset(new LorentzVector());
}

uint32_t MissingEnergyCorrection::operator()(const LorentzVector &p4,
        const LorentzVector &met)
{
    _solutions = 0;

    double a = _mass_a * _mass_a
        - p4 * p4
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
        addSolution(_solution_one, met, -B / A);
    else if(0 == discriminant)
    {
        _solutions = 1;

        addSolution(_solution_one, met, -B / A);
    }
    else
    {
        discriminant = sqrt(discriminant);

        addSolution(_solution_one, met, (-B - discriminant) / A);
        addSolution(_solution_two, met, (-B + discriminant) / A);

        _solutions = 2;
    }

    return _solutions;
}

MissingEnergyCorrection::P4Ptr
    MissingEnergyCorrection::solution(const uint32_t &solution) const
{
    if (!_solutions
            || !solution)
        return _solution_one;

    return _solution_two;
}

// Privates
//
void MissingEnergyCorrection::addSolution(P4Ptr &solution,
        const LorentzVector &p4,
        const double &pz)
{
    solution->set_e(p4.e());
    solution->set_px(p4.px());
    solution->set_py(p4.py());
    solution->set_pz(pz);
}
