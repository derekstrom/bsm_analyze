// Common Algorithm Classes/functions
//
// Useful tools that are heavily used in the analysis
//
// Created by Samvel Khalatyan, Apr 25, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_ALGORITHM
#define BSM_ALGORITHM

#include <boost/shared_ptr.hpp>

#include "bsm_input/interface/Jet.pb.h"

class TLorentzVector;

namespace bsm
{
    class Electron;
    class LorentzVector;
    class Muon;

    namespace algorithm
    {
        // Search for closest jet to electron, muon
        //
        class ClosestJet
        {
            public:
                typedef ::google::protobuf::RepeatedPtrField<Jet>
                    Jets;

                ClosestJet();

                const Jet *operator()(const Jets &, const Electron &);
                const Jet *operator()(const Jets &, const Muon &);

            private:
                typedef boost::shared_ptr<TLorentzVector> P4;

                const Jet *operator()(const Jets &);

                P4 _lepton_p4;
                P4 _jet_p4;
        };

        // Given the Decay: A -> B + Neutrino
        // correct Neutrino pZ component of the Lorentz Vector
        //
        // Calculation is carried with formula:
        //
        //      P4A^2 = (P4B + P4Neutrino) ^ 2
        //
        // and
        //      P4Neutrino^2 = 0 (within the SM: m_neutrino = 0)
        //
        class MissingEnergyCorrection
        {
            public:
                typedef boost::shared_ptr<LorentzVector> P4Ptr;

                MissingEnergyCorrection(const double &mass_a);

                // return number of solutions found
                //  0   imaginary solution
                //  1   only one solution (discriminator = 0)
                //  2   two solutions are found
                //
                uint32_t operator()(const LorentzVector &p4_b,
                        const LorentzVector &missing_energy);

                P4Ptr solution(const uint32_t &) const;

            private:
                void addSolution(P4Ptr &,
                        const LorentzVector &,
                        const double &pz);

                const double _mass_a;

                uint32_t _solutions;
                P4Ptr _solution_one;
                P4Ptr _solution_two;
        };
    }

    using algorithm::ClosestJet;
    using algorithm::MissingEnergyCorrection;
}

#endif
