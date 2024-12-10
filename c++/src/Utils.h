#pragma once

#include <iostream>
#include <vector>
#include <format>
#include <complex>

#include "Polar.h"
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

    // std::string
    // to_string(const LowPrecisionNumber number);

    // void
    // test_FixedPointNumber8_t();


}

