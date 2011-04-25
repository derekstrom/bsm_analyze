// Monitor PrimaryVertices
//
// Generate monitoring plots given primary vertices collection
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#include <TH1I.h>
#include <TH1F.h>

#include "interface/PrimaryVertexMonitor.h"
#include "interface/Utility.h"

using bsm::PrimaryVertexMonitor;

PrimaryVertexMonitor::PrimaryVertexMonitor()
{
    utility::SupressTHistAddDirectory supressor;

    _multiplicity.reset(new TH1I("primary_vertex_multiplicity", "PrimaryVertices Multiplicity",
                20, 0, 20));
    _multiplicity->GetXaxis()->SetTitle("N_{primary_vertices}");

    _x.reset(new TH1F("primary_vertex_x", "PrimaryVertex X", 200, -.1, .1));
    _x->GetXaxis()->SetTitle("X [cm]");

    _y.reset(new TH1F("primary_vertex_y", "PrimaryVertex Y", 200, -.1, .1));
    _y->GetXaxis()->SetTitle("Y [cm]");

    _z.reset(new TH1F("primary_vertex_z", "PrimaryVertex Z", 100, -50, 50));
    _z->GetXaxis()->SetTitle("Z [cm]");
}

PrimaryVertexMonitor::~PrimaryVertexMonitor()
{
}

void PrimaryVertexMonitor::fill(const PrimaryVertices &primary_vertices)
{
    _multiplicity->Fill(primary_vertices.size());

    for(PrimaryVertices::const_iterator primary_vertex = primary_vertices.begin();
            primary_vertices.end() != primary_vertex;
            ++primary_vertex)
    {
        _x->Fill(primary_vertex->vertex().x());
        _y->Fill(primary_vertex->vertex().y());
        _z->Fill(primary_vertex->vertex().z());
    }
}

const PrimaryVertexMonitor::H1Ptr PrimaryVertexMonitor::multiplicity() const
{
    return _multiplicity;
}

const PrimaryVertexMonitor::H1Ptr PrimaryVertexMonitor::x() const
{
    return _x;
}

const PrimaryVertexMonitor::H1Ptr PrimaryVertexMonitor::y() const
{
    return _y;
}

const PrimaryVertexMonitor::H1Ptr PrimaryVertexMonitor::z() const
{
    return _z;
}
