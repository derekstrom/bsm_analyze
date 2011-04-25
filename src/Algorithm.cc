// Common Algorithm Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#include <TLorentzVector.h>

#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "interface/Algorithm.h"
#include "interface/Utility.h"

using bsm::algorithm::ClosestJet;

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

    utility::set(_lepton_p4.get(), &electron->physics_object().p4());

    return operator()(jets);
}

const bsm::Jet *ClosestJet::operator()(const Jets &jets, const Muon &muon)
{
    if (!jets.size())
        return 0;

    utility::set(_lepton_p4.get(), &muon->physics_object().p4());

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

        if (delta_r < min_delta_r)
        {
            min_delta_r = delta_r;

            closest_jet = &*jet;
        }
    }

    return closest_jet;
}
