// A collection of Monitors
//
// Monitors are used to easily check different quantities, physics objects,
// jets, etc.
//
// Created by Samvel Khalatyan, Apr 22, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_MONITOR
#define BSM_MONITOR

#include <iosfwd>

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Jet.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/PrimaryVertex.pb.h"
#include "bsm_stat/interface/statfwd.h"

class TLorentzVector;

namespace bsm
{
    class GenParticle;
    class LorentzVector;
    class MissingEnergy;

    class JetMonitor
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            JetMonitor();
            ~JetMonitor();
            
            void fill(const Jets &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_pt() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _multiplicity;
            H1Ptr _pt;
            H1Ptr _leading_pt;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };

    class DeltaMonitor
    {
        public:
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            DeltaMonitor();

            void fill(const LorentzVector &, const LorentzVector &);

            const H1Ptr r() const;
            const H1Ptr eta() const;
            const H1Ptr phi() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _r;
            H1Ptr _eta;
            H1Ptr _phi;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4_1;
            P4 _p4_2;
    };

    class ElectronMonitor
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            ElectronMonitor();
            ~ElectronMonitor();
            
            void fill(const Electrons &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_pt() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _multiplicity;
            H1Ptr _pt;
            H1Ptr _leading_pt;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };

    class GenParticleMonitor
    {
        public:
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            GenParticleMonitor();
            ~GenParticleMonitor();
            
            void fill(const GenParticle &);

            const H1Ptr id() const;
            const H1Ptr status() const;
            const H1Ptr pt() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _id;
            H1Ptr _status;
            H1Ptr _pt;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };

    class LorentzVectorMonitor
    {
        public:
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            LorentzVectorMonitor();

            void fill(const LorentzVector &);

            const H1Ptr energy() const;
            const H1Ptr px() const;
            const H1Ptr py() const;
            const H1Ptr pz() const;

            const H1Ptr pt() const;
            const H1Ptr eta() const;
            const H1Ptr phi() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _energy;
            H1Ptr _px;
            H1Ptr _py;
            H1Ptr _pz;

            H1Ptr _pt;
            H1Ptr _eta;
            H1Ptr _phi;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };

    class MissingEnergyMonitor
    {
        public:
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            MissingEnergyMonitor();
            ~MissingEnergyMonitor();
            
            void fill(const MissingEnergy &);

            const H1Ptr pt() const;
            const H1Ptr x() const;
            const H1Ptr y() const;
            const H1Ptr z() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _pt;
            H1Ptr _x;
            H1Ptr _y;
            H1Ptr _z;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };

    class MuonMonitor
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            MuonMonitor();
            ~MuonMonitor();
            
            void fill(const Muons &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_pt() const;

        private:
            typedef boost::shared_ptr<TLorentzVector> P4;

            H1Ptr _multiplicity;
            H1Ptr _pt;
            H1Ptr _leading_pt;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };

    class PrimaryVertexMonitor
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<PrimaryVertex>
                PrimaryVertices;
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            PrimaryVertexMonitor();
            ~PrimaryVertexMonitor();
            
            void fill(const PrimaryVertices &);

            const H1Ptr multiplicity() const;
            const H1Ptr x() const;
            const H1Ptr y() const;
            const H1Ptr z() const;

        private:
            H1Ptr _multiplicity;
            H1Ptr _x;
            H1Ptr _y;
            H1Ptr _z;
    };

    // Helpers
    //
    std::ostream &operator<<(std::ostream &, const ElectronMonitor &);
    std::ostream &operator<<(std::ostream &, const DeltaMonitor &);
    std::ostream &operator<<(std::ostream &, const GenParticleMonitor &);
    std::ostream &operator<<(std::ostream &, const JetMonitor &);
    std::ostream &operator<<(std::ostream &, const LorentzVectorMonitor &);
    std::ostream &operator<<(std::ostream &, const MissingEnergyMonitor &);
    std::ostream &operator<<(std::ostream &, const MuonMonitor &);
    std::ostream &operator<<(std::ostream &, const PrimaryVertexMonitor &);

    void merge(ElectronMonitor &, const ElectronMonitor &);
    void merge(JetMonitor &, const JetMonitor &);
    void merge(MissingEnergyMonitor &, const MissingEnergyMonitor &);
    void merge(MuonMonitor &, const MuonMonitor &);
    void merge(PrimaryVertexMonitor &, const PrimaryVertexMonitor &);
}

#endif
