// Monitor Jets
//
// Generate monitoring plots given jets collection
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#include <iomanip>
#include <ostream>

#include <TLorentzVector.h>

#include "bsm_stat/interface/H1.h"

#include "interface/JetMonitor.h"
#include "interface/Utility.h"

using std::endl;

using bsm::JetMonitor;

JetMonitor::JetMonitor()
{
    using bsm::stat::H1;

    _multiplicity.reset(new H1(10, 0, 10));
    _pt.reset(new H1(100, 0, 100));
    _leading_jet_pt.reset(new H1(100, 0, 100));

    _p4.reset(new TLorentzVector());
}

JetMonitor::~JetMonitor()
{
}

void JetMonitor::fill(const Jets &jets)
{
    _multiplicity->fill(jets.size());

    double leading_jet_pt = 0;
    double pt = 0;
    for(Jets::const_iterator jet = jets.begin();
            jets.end() != jet;
            ++jet)
    {
        _p4->SetPxPyPzE(jet->physics_object().p4().px(),
                jet->physics_object().p4().py(),
                jet->physics_object().p4().pz(),
                jet->physics_object().p4().e());

        pt = _p4->Pt();

        if (leading_jet_pt < pt)
            leading_jet_pt = pt;

        _pt->fill(pt);
    }

    if (leading_jet_pt)
        _leading_jet_pt->fill(leading_jet_pt);
}

const JetMonitor::H1Ptr JetMonitor::multiplicity() const
{
    return _multiplicity;
}

const JetMonitor::H1Ptr JetMonitor::pt() const
{
    return _pt;
}

const JetMonitor::H1Ptr JetMonitor::leading_jet_pt() const
{
    return _leading_jet_pt;
}

void JetMonitor::print(std::ostream &out) const
{
    using std::setw;
    using std::left;

    out << "JetMonitor" << endl;
    out << setw(20) << left << " [multiplicity]" << *_multiplicity << endl;
    out << setw(20) << left << " [pt]" << *_pt << endl;
    out << setw(20) << left << " [leading jet pt] " << *_leading_jet_pt << endl;
}



// Helpers
//
std::ostream &bsm::operator<<(std::ostream &out, const JetMonitor &monitor)
{
    monitor.print(out);

    return out;
}
