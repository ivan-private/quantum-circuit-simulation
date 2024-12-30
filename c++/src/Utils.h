#pragma once

#include <iostream>
#include <vector>
#include <complex>

#include "Traits.h"

namespace utils
{
    std::string
    to_string(const std::vector<std::complex<double>>& statevector);

    std::string
    to_string(const std::vector<double>& statevector);

    std::string
    to_prob_string(const std::vector<std::complex<double>>& statevector);

    void
    print_statevector_as_prob(const traits::QuantumCircuit auto& qc) 
    {
        std::cout << utils::to_prob_string(qc.get_statevector()) << '\n';
    }

    void print_polar_coordinate(traits::PolarCoordinate auto pc)
    {
        std::cout << "Polar coordinate: (" << pc.get_magnitude() << ", " << pc.get_phase_degrees() << " degrees)\n";
    }


}

