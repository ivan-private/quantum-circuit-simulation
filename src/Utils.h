#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <cassert>
#include <iomanip>
#include <filesystem>

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



    double 
    fidelity(const traits::QuantumCircuit auto& qc1, const traits::QuantumCircuit auto& qc2) 
    {
        const auto sv1 = qc1.get_statevector();
        const auto sv2 = qc2.get_statevector();

        assert (sv1.size() == sv2.size());

        std::complex<double> inner_product = 0.0;
        
        for (size_t i = 0; i < sv1.size(); ++i) 
        {
            std::complex<double> z1 = static_cast<std::complex<double>>(sv1[i]);
            std::complex<double> z2 = static_cast<std::complex<double>>(sv2[i]);
            inner_product += z1 * std::conj(z2);
        }

        return std::norm(inner_product); 
    }


    double 
    l2_norm_complex(const traits::QuantumCircuit auto& qc1, const traits::QuantumCircuit auto& qc2)
    {
        
        const auto sv1 = qc1.get_statevector();
        const auto sv2 = qc2.get_statevector();

        assert (sv1.size() == sv2.size());

        double inner_product = 0.0;
        
        for (size_t i = 0; i < sv1.size(); ++i) 
        {
            std::complex<double> z1 = static_cast<std::complex<double>>(sv1[i]);
            std::complex<double> z2 = static_cast<std::complex<double>>(sv2[i]);
            inner_product += std::norm(z1-z2);
        }

        return std::sqrt(inner_product); 
    }


    double 
    l2_norm_r(const traits::QuantumCircuit auto& qc1, const traits::QuantumCircuit auto& qc2)
    {
        const auto sv1 = qc1.get_statevector();
        const auto sv2 = qc2.get_statevector();

        assert (sv1.size() == sv2.size());

        double inner_product = 0.0;

        for (size_t i = 0; i < sv1.size(); ++i) 
        {
            double r1 = sv1[i].get_magnitude();
            double r2 = sv2[i].get_magnitude();
            inner_product += std::abs(r1-r2);
        }

        return std::sqrt(inner_product);
    }


    double
    max_norm_complex(const traits::QuantumCircuit auto& qc1, const traits::QuantumCircuit auto& qc2)
    {
                
        const auto sv1 = qc1.get_statevector();
        const auto sv2 = qc2.get_statevector();

        assert (sv1.size() == sv2.size());

        double max_norm = 0.0;
        
        for (size_t i = 0; i < sv1.size(); ++i) 
        {
            std::complex<double> z1 = static_cast<std::complex<double>>(sv1[i]);
            std::complex<double> z2 = static_cast<std::complex<double>>(sv2[i]);
            max_norm = std::max(max_norm, std::abs(z1-z2));
        }

        return max_norm;
    }


    double
    max_norm_r(const traits::QuantumCircuit auto& qc1, const traits::QuantumCircuit auto& qc2)
    {
                
        const auto sv1 = qc1.get_statevector();
        const auto sv2 = qc2.get_statevector();

        assert (sv1.size() == sv2.size());

        double max_norm = 0.0;
        
        for (size_t i = 0; i < sv1.size(); ++i) 
        {
            double r1 = sv1[i].get_magnitude();
            double r2 = sv2[i].get_magnitude();
            max_norm = std::max(max_norm, std::abs(r1-r2));
        }

        return max_norm;
    }


    void 
    statevector_comparision_as_csv(const std::string& filename, 
        const traits::QuantumCircuit auto& qc1, const traits::QuantumCircuit auto& qc2)
    {
        assert(qc1.num_qubits() == qc2.num_qubits());
        std::string n_qubits = std::to_string(qc1.num_qubits());

        std::ofstream f(filename);

        const std::vector<Polar>& sv1 = qc1.get_statevector();
        const std::vector<Polar>& sv2 = qc2.get_statevector();

        assert(sv1.size() == sv2.size());

        f << "state, r1, r2, theta1, theta2, r_abs_diff, theta_abs_diff\n";

        for (size_t i = 0; i < sv1.size(); i++)
        {
            double r_diff = std::abs(sv1[i].get_magnitude() - sv2[i].get_magnitude());
            double theta_diff = std::abs(sv1[i].get_phase() - sv2[i].get_phase());

            f   << i << ", " 
                << sv1[i].get_magnitude() << ", "
                << sv2[i].get_magnitude() << ", "
                << sv1[i].get_phase() << ", "
                << sv2[i].get_phase() << ", "
                << r_diff << ", "
                << theta_diff << "\n";
        }

    }


    void 
    fidelity_and_error_as_csv(const std::string& filename, 
        const traits::QuantumCircuit auto& qc1, const traits::QuantumCircuit auto& qc2)
    {
        assert(qc1.num_qubits() == qc2.num_qubits());
        std::string n_qubits = std::to_string(qc1.num_qubits());

        bool write_header = !std::filesystem::exists(filename);

        std::ofstream f(filename, std::ios::app);

        const std::vector<Polar>& sv1 = qc1.get_statevector();
        const std::vector<Polar>& sv2 = qc2.get_statevector();

        assert(sv1.size() == sv2.size());

        if (write_header)
        {
            f << "n_qubits, n_states, fidelity, l2_norm_complex, l2_norm_r, max_norm_complex, max_norm_r\n";
        }
        

        f   << qc1.num_qubits() 
            << ", " << sv1.size() 
            << ", " << fidelity(qc1, qc2)
            << ", " << l2_norm_complex(qc1, qc2)
            << ", " << l2_norm_r(qc1, qc2)
            << ", " << max_norm_complex(qc1, qc2)
            << ", " << max_norm_r(qc1, qc2)
            << "\n";
    }

}

