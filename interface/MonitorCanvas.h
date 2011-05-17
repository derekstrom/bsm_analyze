// Monitors Visualizers
//
// Generate standartized Canvases for monitors
//
// Created by Samvel Khalatyan, May 17, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR_CANVAS
#define BSM_MONITOR_CANVAS

#include <string>

#include <boost/shared_ptr.hpp>

#include "bsm_core/interface/bsm_core_fwd.h"
#include "bsm_stat/interface/bsm_stat_fwd.h"

namespace bsm
{
    class DeltaMonitor;
    class ElectronMonitor;
    class JetMonitor;
    class LorentzVectorMonitor;
    class MissingEnergyMonitor;
    class MuonMonitor;
    class PrimaryVertexMonitor;

    class DeltaCanvas
    {
        public:
            DeltaCanvas(const std::string &title = "");

            void draw(const DeltaMonitor &);

        private:
            typedef boost::shared_ptr<core::IDCounter> IDPtr;
            typedef boost::shared_ptr<TCanvas> CanvasPtr;

            typedef stat::TH1Ptr TH1Ptr;
            typedef stat::TH2Ptr TH2Ptr;

            static IDPtr _id;
            CanvasPtr _canvas;

            TH1Ptr _r;
            TH1Ptr _ptrel;
            TH2Ptr _ptrel_vs_r;
            TH1Ptr _eta;
            TH1Ptr _phi;
    };

    class ElectronCanvas
    {
        public:
            ElectronCanvas(const std::string &title = "");

            void draw(const ElectronMonitor &);

        private:
            typedef boost::shared_ptr<core::IDCounter> IDPtr;
            typedef boost::shared_ptr<TCanvas> CanvasPtr;

            typedef stat::TH1Ptr TH1Ptr;

            static IDPtr _id;
            CanvasPtr _canvas;

            TH1Ptr _multiplicity;
            TH1Ptr _leading_pt;
            TH1Ptr _pt;
    };

    class JetCanvas
    {
        public:
            JetCanvas(const std::string &title = "");

            void draw(const JetMonitor &);

        private:
            typedef boost::shared_ptr<core::IDCounter> IDPtr;
            typedef boost::shared_ptr<TCanvas> CanvasPtr;

            typedef stat::TH1Ptr TH1Ptr;

            static IDPtr _id;
            CanvasPtr _canvas;

            TH1Ptr _multiplicity;
            TH1Ptr _leading_pt;
            TH1Ptr _pt;
    };

    class LorentzVectorCanvas
    {
        public:
            LorentzVectorCanvas(const std::string &title = "");

            void draw(const LorentzVectorMonitor &);

        private:
            typedef boost::shared_ptr<core::IDCounter> IDPtr;
            typedef boost::shared_ptr<TCanvas> CanvasPtr;

            typedef stat::TH1Ptr TH1Ptr;

            static IDPtr _id;
            CanvasPtr _canvas;

            TH1Ptr _energy;
            TH1Ptr _px;
            TH1Ptr _py;
            TH1Ptr _pz;

            TH1Ptr _pt;
            TH1Ptr _eta;
            TH1Ptr _phi;
    };

    class MissingEnergyCanvas
    {
        public:
            MissingEnergyCanvas(const std::string &title = "");

            void draw(const MissingEnergyMonitor &);

        private:
            typedef boost::shared_ptr<core::IDCounter> IDPtr;
            typedef boost::shared_ptr<TCanvas> CanvasPtr;

            typedef stat::TH1Ptr TH1Ptr;

            static IDPtr _id;
            CanvasPtr _canvas;

            TH1Ptr _pt;
            TH1Ptr _x;
            TH1Ptr _y;
            TH1Ptr _z;
    };

    class MuonCanvas
    {
        public:
            MuonCanvas(const std::string &title = "");

            void draw(const MuonMonitor &);

        private:
            typedef boost::shared_ptr<core::IDCounter> IDPtr;
            typedef boost::shared_ptr<TCanvas> CanvasPtr;

            typedef stat::TH1Ptr TH1Ptr;

            static IDPtr _id;
            CanvasPtr _canvas;

            TH1Ptr _multiplicity;
            TH1Ptr _leading_pt;
            TH1Ptr _pt;
    };

    class PrimaryVertexCanvas
    {
        public:
            PrimaryVertexCanvas(const std::string &title = "");

            void draw(const PrimaryVertexMonitor &);

        private:
            typedef boost::shared_ptr<core::IDCounter> IDPtr;
            typedef boost::shared_ptr<TCanvas> CanvasPtr;

            typedef stat::TH1Ptr TH1Ptr;

            static IDPtr _id;
            CanvasPtr _canvas;

            TH1Ptr _multiplicity;
            TH1Ptr _x;
            TH1Ptr _y;
            TH1Ptr _z;
    };
}

#endif
