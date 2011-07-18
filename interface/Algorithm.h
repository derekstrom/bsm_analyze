// Common Algorithm Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_ALGORITHM
#define BSM_ALGORITHM

#include <vector>

#include <boost/shared_ptr.hpp>

#include "bsm_core/interface/Object.h"
#include "bsm_input/interface/bsm_input_fwd.h"
#include "bsm_input/interface/Jet.pb.h"

class TLorentzVector;

namespace bsm
{
    namespace algorithm
    {
        // Search for closest jet to electron, muon
        //
        class ClosestJet : public core::Object
        {
            public:
                typedef ::google::protobuf::RepeatedPtrField<Jet>
                    Jets;

                ClosestJet();
                ClosestJet(const ClosestJet &);

                const Jet *find(const Jets &, const Electron &);
                const Jet *find(const Jets &, const Muon &);

                // Object interface
                //
                virtual uint32_t id() const;

                virtual ObjectPtr clone() const;
                using Object::merge;

                virtual void print(std::ostream &) const;

            private:
                // Prevent copying
                //
                ClosestJet &operator =(const ClosestJet &);

                typedef boost::shared_ptr<TLorentzVector> P4;

                const Jet *find(const Jets &);

                P4 _p4;
        };

        // Given the Decay:
        //
        //      A -> B + Neutrino
        //
        // reconstruct the Neutrino pZ component of the Lorentz Vector if
        // it's Energy, px, and py are known.
        //
        // Calculation is carried with formula:
        //
        //      P4A^2 = (P4B + P4Neutrino)^2
        //
        // and
        //
        //      P4Neutrino^2 = 0
        //
        //  (within the SM: m_neutrino = 0)
        //
        class NeutrinoReconstruct : public core::Object
        {
            public:
                typedef boost::shared_ptr<LorentzVector> P4Ptr;

                NeutrinoReconstruct(const float &mass_a,
                        const float &mass_b);
                NeutrinoReconstruct(const NeutrinoReconstruct &);

                // return number of solutions found
                //  0   imaginary solution
                //  1   only one solution (discriminator = 0)
                //  2   two solutions are found
                //
                uint32_t apply(const LorentzVector &p4_b,
                        const LorentzVector &missing_energy);

                // Solutions are counted from 0
                //
                P4Ptr solution(const uint32_t &) const;

                void reset();

                // Object interface
                //
                virtual uint32_t id() const;

                virtual ObjectPtr clone() const;
                virtual void merge(const ObjectPtr &);

                virtual void print(std::ostream &) const;

            private:
                // Prevent copying
                //
                NeutrinoReconstruct &operator =(const NeutrinoReconstruct &);

                void addSolution(P4Ptr &,
                        const LorentzVector &,
                        const float &pz);

                void setSolution(P4Ptr &,
                        const float &,
                        const float &,
                        const float &,
                        const float &);

                float _mass_a;
                float _mass_b;

                uint32_t _solutions;

                P4Ptr _solution_one;
                P4Ptr _solution_two;
        };

        // Hadronic decay of the t-quark:
        //
        //      t -> W + b
        //
        // Given w-tagged jet and b-jet:
        //
        //      1. Reconstruct top P4
        //      2. Calculate DeltaR(Wjet, top)
        //      4.      "    DeltaR(bjet, top)
        //      5.      "    Combo of 2 and 4
        //
        class HadronicDecay : public core::Object
        {
            public:
                typedef boost::shared_ptr<LorentzVector> P4Ptr;

                HadronicDecay();
                HadronicDecay(const HadronicDecay &);

                const P4Ptr top() const;

                float dr() const;
                float dr_w_top() const;
                float dr_b_top() const;

                float apply(const LorentzVector &w, const LorentzVector &b);

                void reset();

                // Object interface
                //
                virtual uint32_t id() const;

                virtual ObjectPtr clone() const;
                virtual void merge(const ObjectPtr &);

                virtual void print(std::ostream &) const;

            private:
                // Prevent copying
                //
                HadronicDecay &operator =(const HadronicDecay &);

                P4Ptr _top;

                float _dr;
                float _dr_w_top;
                float _dr_b_top;
        };

        // Leptonic decay of the t-quark:
        //
        //      t -> l + neutrino + b
        //
        // Given lepton, neutrino and b-jet:
        //
        //      1. Reconstruct the t-quark P4
        //      2. Calculate DeltaR(l, top)
        //      3.      "    DeltaR(nu, top)
        //      4.      "    DeltaR(b, top)
        //      5. Combine 2) - 4)
        //
        class LeptonicDecay : public core::Object
        {
            public:
                typedef boost::shared_ptr<LorentzVector> P4Ptr;

                LeptonicDecay();
                LeptonicDecay(const LeptonicDecay &);

                const P4Ptr top() const;

                float dr() const;
                float dr_l_top() const;
                float dr_nu_top() const;
                float dr_b_top() const;

                float apply(const LorentzVector &l,
                        const LorentzVector &nu,
                        const LorentzVector &b);

                void reset();

                // Object interface
                //
                virtual uint32_t id() const;

                virtual ObjectPtr clone() const;
                virtual void merge(const ObjectPtr &);

                virtual void print(std::ostream &) const;

            private:
                // Prevent copying
                //
                LeptonicDecay &operator =(const LeptonicDecay &);

                P4Ptr _top;

                float _dr;
                float _dr_l_top;
                float _dr_nu_top;
                float _dr_b_top;
        };

        class TTbarDeltaRReconstruct : public core::Object
        {
            public:
                typedef std::vector<const Jet *> Jets;
                typedef boost::shared_ptr<HadronicDecay> HadronicPtr;
                typedef boost::shared_ptr<LeptonicDecay> LeptonicPtr;

                TTbarDeltaRReconstruct();
                TTbarDeltaRReconstruct(const TTbarDeltaRReconstruct &);

                float dr() const;

                HadronicPtr hadronicDecay() const;
                LeptonicPtr leptonicDecay() const;

                // Function will return combined DeltaR:
                //
                //  DR = DR_leptonic + DR_hadronic
                //
                float apply(const Jets &,
                        const LorentzVector &lepton,
                        const LorentzVector &missing_energy,
                        const LorentzVector &wjet);

                void reset();

                // Object interface
                //
                virtual uint32_t id() const;

                virtual ObjectPtr clone() const;
                virtual void merge(const ObjectPtr &);

                virtual void print(std::ostream &) const;

            private:
                // Prevent copying
                //
                TTbarDeltaRReconstruct &operator =(const TTbarDeltaRReconstruct &);

                float minimize(const Jets &,
                        const Jets::const_iterator &jet1,
                        const Jets::const_iterator &jet2,
                        const LorentzVector &lepton,
                        const LorentzVector &missing_energy,
                        const LorentzVector &wjet);

                float _dr;
                
                HadronicPtr _hadronic;
                LeptonicPtr _leptonic;
        };
    }

    using algorithm::ClosestJet;
    using algorithm::NeutrinoReconstruct;
    using algorithm::HadronicDecay;
    using algorithm::LeptonicDecay;
    using algorithm::TTbarDeltaRReconstruct;
}

#endif
