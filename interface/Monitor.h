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

#include "bsm_core/interface/Object.h"
#include "bsm_input/interface/bsm_input_fwd.h"
#include "bsm_input/interface/Electron.pb.h"
#include "bsm_input/interface/Jet.pb.h"
#include "bsm_input/interface/Muon.pb.h"
#include "bsm_input/interface/PrimaryVertex.pb.h"
#include "bsm_stat/interface/bsm_stat_fwd.h"
#include "interface/bsm_fwd.h"

class TLorentzVector;

namespace bsm
{
    typedef boost::shared_ptr<H1Proxy> H1ProxyPtr;
    typedef boost::shared_ptr<H2Proxy> H2ProxyPtr;

    typedef boost::shared_ptr<stat::H1> H1Ptr;
    typedef boost::shared_ptr<stat::H2> H2Ptr;

    class DeltaMonitor : public core::Object
    {
        public:
            DeltaMonitor();
            DeltaMonitor(const DeltaMonitor &);

            void fill(const LorentzVector &, const LorentzVector &);

            const H1Ptr r() const;
            const H1Ptr eta() const;
            const H1Ptr phi() const;
            const H1Ptr ptrel() const;
            const H2Ptr ptrel_vs_r() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            DeltaMonitor &operator =(const DeltaMonitor &);

            typedef boost::shared_ptr<TLorentzVector> P4;

            H1ProxyPtr _r;
            H1ProxyPtr _eta;
            H1ProxyPtr _phi;
            H1ProxyPtr _ptrel;
            H2ProxyPtr _ptrel_vs_r;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4_1;
            P4 _p4_2;
    };

    class ElectronsMonitor : public core::Object
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Electron> Electrons;

            ElectronsMonitor();
            ElectronsMonitor(const ElectronsMonitor &);

            void fill(const Electrons &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_pt() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            ElectronsMonitor &operator =(const ElectronsMonitor &monitor);

            H1ProxyPtr _multiplicity;
            H1ProxyPtr _pt;
            H1ProxyPtr _leading_pt;
    };

    class GenParticleMonitor : public core::Object
    {
        public:
            typedef boost::shared_ptr<stat::H1> H1Ptr;

            GenParticleMonitor();
            GenParticleMonitor(const GenParticleMonitor &);
            
            void fill(const GenParticle &);

            const H1Ptr pdgid() const;
            const H1Ptr status() const;
            const H1Ptr pt() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            GenParticleMonitor &operator =(const GenParticleMonitor &);

            H1ProxyPtr _pdg_id;
            H1ProxyPtr _status;
            H1ProxyPtr _pt;
    };

    class JetsMonitor : public core::Object
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Jet> Jets;

            JetsMonitor();
            JetsMonitor(const JetsMonitor &);
            
            void fill(const Jets &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_pt() const;
            const H1Ptr children() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            JetsMonitor &operator =(const JetsMonitor &);

            H1ProxyPtr _multiplicity;
            H1ProxyPtr _pt;
            H1ProxyPtr _leading_pt;
            H1ProxyPtr _children;
    };

    class LorentzVectorMonitor : public core::Object
    {
        public:
            LorentzVectorMonitor();
            LorentzVectorMonitor(const LorentzVectorMonitor &);

            void fill(const LorentzVector &);

            const H1Ptr energy() const;
            const H1Ptr px() const;
            const H1Ptr py() const;
            const H1Ptr pz() const;

            const H1Ptr pt() const;
            const H1Ptr eta() const;
            const H1Ptr phi() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            LorentzVectorMonitor &operator =(const LorentzVectorMonitor &);

            typedef boost::shared_ptr<TLorentzVector> P4;

            H1ProxyPtr _energy;
            H1ProxyPtr _px;
            H1ProxyPtr _py;
            H1ProxyPtr _pz;

            H1ProxyPtr _pt;
            H1ProxyPtr _eta;
            H1ProxyPtr _phi;

            // Convert bsm::LorentzVector to TLorentzVector
            P4 _p4;
    };

    class MissingEnergyMonitor : public core::Object
    {
        public:
            MissingEnergyMonitor();
            MissingEnergyMonitor(const MissingEnergyMonitor &);
            
            void fill(const MissingEnergy &);

            const H1Ptr pt() const;
            const H1Ptr x() const;
            const H1Ptr y() const;
            const H1Ptr z() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            MissingEnergyMonitor &operator =(const MissingEnergyMonitor &);

            typedef boost::shared_ptr<TLorentzVector> P4;

            H1ProxyPtr _pt;
            H1ProxyPtr _x;
            H1ProxyPtr _y;
            H1ProxyPtr _z;
    };

    class MuonsMonitor : public core::Object
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<Muon> Muons;

            MuonsMonitor();
            MuonsMonitor(const MuonsMonitor &);
            
            void fill(const Muons &);

            const H1Ptr multiplicity() const;
            const H1Ptr pt() const;
            const H1Ptr leading_pt() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            MuonsMonitor &operator =(const MuonsMonitor &);

            H1ProxyPtr _multiplicity;
            H1ProxyPtr _pt;
            H1ProxyPtr _leading_pt;
    };

    class PrimaryVerticesMonitor : public core::Object
    {
        public:
            typedef ::google::protobuf::RepeatedPtrField<PrimaryVertex>
                PrimaryVertices;

            PrimaryVerticesMonitor();
            PrimaryVerticesMonitor(const PrimaryVerticesMonitor &);
            
            void fill(const PrimaryVertices &);

            const H1Ptr multiplicity() const;
            const H1Ptr x() const;
            const H1Ptr y() const;
            const H1Ptr z() const;

            // Object interface
            //
            virtual uint32_t id() const;

            virtual ObjectPtr clone() const;
            using Object::merge;

            virtual void print(std::ostream &) const;

        private:
            // Prevent copying
            //
            PrimaryVerticesMonitor &operator =(const PrimaryVerticesMonitor &);

            H1ProxyPtr _multiplicity;
            H1ProxyPtr _x;
            H1ProxyPtr _y;
            H1ProxyPtr _z;
    };
}

#endif
