#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cassert>
#include <complex>
#include <numbers>

#include "Traits.h"


template<traits::ComplexNumber ComplexType>
class QuantumCircuit
{
public:

    QuantumCircuit(uint8_t n_qubits) 
        : n_qubits(n_qubits), 
        n_states(1 << n_qubits),
        statevector(n_states)
    {
        statevector[0] = static_cast<ComplexType>(std::complex<double>(1.0, 0.0));
    }
    
    QuantumCircuit(const QuantumCircuit& other) = delete;

    QuantumCircuit(QuantumCircuit&& other) noexcept = default;

    void h(uint8_t qubits);

    void x(uint8_t qubit);

    void cx(uint8_t control_qubit, uint8_t target_qubit);

    void cp(double angle, uint8_t target_qubit, uint8_t control_qubit);

    void swap(uint8_t qubit1, uint8_t qubit2);

    void qft();

    void ghz();

    std::vector<std::complex<double>> get_statevector() const;

    inline uint8_t num_qubits() const noexcept { return n_qubits; }

    void reset_statevector();

private: 
    size_t n_states; // 2^{64} different states in the system, so max 64 qubits
    uint8_t n_qubits; // max 255 qubits in the system
    std::vector<ComplexType> statevector;
};


template<traits::ComplexNumber ComplexType>
void 
QuantumCircuit<ComplexType>::h(uint8_t qubit)  
{
    const ComplexType scale(std::complex<double>(1.0 / std::numbers::sqrt2, 0.0));
    
    const size_t target_mask = 1 << qubit;
    
    assert(statevector.size() == n_states);

    for (size_t state = 0; state < n_states; state++)
    {
        if (state & target_mask)
            continue;

        size_t adjoint_state = state ^ target_mask;
        assert(adjoint_state < n_states);
        
        const auto amplitude = statevector[state];
        const auto adjoint_amplitude = statevector[adjoint_state];

        statevector[state] = scale * amplitude + scale * adjoint_amplitude;
        statevector[adjoint_state] = scale * amplitude - scale * adjoint_amplitude;
    }
}


template<traits::ComplexNumber ComplexType>
void
QuantumCircuit<ComplexType>::x(uint8_t qubit)
{
    const size_t mask = 1 << qubit;
    size_t adjoint_state;

    for (size_t state = 0; state < n_states; state++)
    {
        if ((state & mask) == 0)
        {
            adjoint_state = state ^ mask;
            std::swap(statevector[state], statevector[adjoint_state]);
        }
    }                
}


template<traits::ComplexNumber ComplexType>
void
QuantumCircuit<ComplexType>::cx(uint8_t control_qubit, uint8_t target_qubit)
{
    const size_t target_mask = 1 << target_qubit;
    const size_t control_mask = 1 << control_qubit;

    size_t adjoint_state;

    for (size_t state = 0; state < n_states; state++)
    {
        target_qubit = state & target_mask;
        control_qubit = state & control_mask;
        if (control_qubit && target_qubit)
        {
            adjoint_state = state ^ target_mask;
            std::swap(statevector[state], statevector[adjoint_state]);
        }
    }  
}


template<traits::ComplexNumber ComplexType>
void 
QuantumCircuit<ComplexType>::cp(double angle, uint8_t control_qubit, uint8_t target_qubit) 
{
    const size_t target_mask = 1 << target_qubit;
    const size_t control_mask = 1 << control_qubit;

    const ComplexType phase_factor(std::polar<double>(1.0, angle));

    constexpr ComplexType zero;

    for (size_t state = 0; state < n_states; state++)
    {
        target_qubit = state & target_mask;
        control_qubit = state & control_mask;

        if (control_qubit && target_qubit && statevector[state] != zero)
        {
            statevector[state] *= phase_factor;
        }

    }
}


template<traits::ComplexNumber ComplexType>
void 
QuantumCircuit<ComplexType>::swap(uint8_t qubit1, uint8_t qubit2) 
{
    uint8_t q1_state, q2_state, xor_result;
    size_t adjoint_state;

    for (size_t state = 0; state < n_states; state++)
    {
        q1_state = (state >> qubit1) & 1;
        q2_state = (state >> qubit2) & 1;
        if (!q1_state && q2_state)
        {
            xor_result = (q1_state ^ q2_state) & 1;
            adjoint_state = state ^ ((xor_result << qubit1) | (xor_result << qubit2));
            std::swap(statevector[state], statevector[adjoint_state]);
        }
    }
}


template<traits::ComplexNumber ComplexType>
void 
QuantumCircuit<ComplexType>::qft()
{
    constexpr double pi = std::numbers::pi;

    for (int i = n_qubits-1; i >= 0; i--) 
    {
        h(i);
        for (int j = i-1; j >= 0; j--)
        {   
            // TODO: double check if this is the correct order of target / control qubit
            cp(pi * std::pow(2, j-i), j, i); 
        }
    }

    for (int i = 0; i < n_qubits / 2; i++) 
    {
        swap(i, n_qubits - 1 - i);
    }
}


template<traits::ComplexNumber ComplexType>
void 
QuantumCircuit<ComplexType>::ghz()
{
    h(0);
    for (int i = 1; i < n_qubits; i++)
    {
        cx(i-1, i);
    }
}


template<traits::ComplexNumber ComplexType>
std::vector<std::complex<double>>
QuantumCircuit<ComplexType>::get_statevector() const
{
    assert(statevector.size() == n_states);
    std::vector<std::complex<double>> statevector_copy(n_states);
    std::transform(
        statevector.cbegin(), statevector.cend(), 
        statevector_copy.begin(), 
        [](const ComplexType& z){ return static_cast<std::complex<double>>(z); }
    );
    return statevector_copy;
}

template<traits::ComplexNumber ComplexType>
void 
QuantumCircuit<ComplexType>::reset_statevector() 
{
    std::fill(
        statevector.begin(), statevector.end(), 
        static_cast<ComplexType>(std::complex<double>(0.0, 0.0))
    );

    statevector[0] = static_cast<ComplexType>(std::complex<double>(1.0, 0.0));
}
