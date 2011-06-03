// Common Algorithm Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_ALGORITHM
#define BSM_ALGORITHM

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

                NeutrinoReconstruct(const double &mass_a,
                        const double &mass_b);
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
                        const double &pz);

                void setSolution(P4Ptr &,
                        const double &,
                        const double &,
                        const double &,
                        const double &);

                double _mass_a;
                double _mass_b;

                uint32_t _solutions;

                P4Ptr _solution_one;
                P4Ptr _solution_two;
        };
    }

    using algorithm::ClosestJet;
    using algorithm::NeutrinoReconstruct;
}

#endif
