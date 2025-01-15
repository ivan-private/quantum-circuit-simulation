#pragma once

#include <iostream>
#include <vector>
#include <complex>

#include "Traits.h"
#include "Polar.h"

namespace utils
{
    std::string
    to_prob_string(size_t state_number, const Polar& state)
    {
        std::stringstream ss;

        const double r = state.get_magnitude();
        const double phase = state.get_phase_degrees();
        const double prob = r*r * 100;

        ss << "|" << state_number << ">\tprob: " << prob << "%,\tphase: " << phase << " degrees\n";

        return ss.str();
    }


    std::string
    to_prob_string(const std::vector<Polar>& statevector)
    {
        std::stringstream ss;

        for (uint64_t i = 0; i < statevector.size(); i++)
        {
            const auto& state = statevector[i];
            const double r = state.get_magnitude();
            const double phase = state.get_phase_degrees();

            const double prob = r*r * 100;


            ss << "|" << i << ">\tprob: " << prob << "%\t\tphase: " << phase << " degrees\n";
        }

        return ss.str();
    }


    

    void 
    print_state_as_prob(const traits::QuantumCircuit auto& qc, size_t state)
    {
        std::cout << to_prob_string(state, qc.get_statevector()[state]);
    }

    void
    print_statevector_as_prob(const traits::QuantumCircuit auto& qc) 
    {
        std::cout << to_prob_string(qc.get_statevector()) << '\n';
    }

    void 
    print_polar_coordinate(traits::PolarCoordinate auto pc)
    {
        std::cout << "Polar coordinate: (" << pc.get_magnitude() << ", " << pc.get_phase_degrees() << " degrees)\n";
    }


}

