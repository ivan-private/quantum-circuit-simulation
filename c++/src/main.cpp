#include <iostream>

#include <complex>

#include "SmallPolar.h"
#include "QuantumCircuit.h"
#include "Utils.h"


// example usecase 
int main()
{    
    static_assert(sizeof(SmallPolar<4, 4>) == 1);
    static_assert(sizeof(SmallPolar<8, 8>) == 2);

    const int n_qubits = 3;

    QuantumCircuit<std::complex<double>> qc_base(n_qubits);

    QuantumCircuit<SmallPolar<4, 4>> qc_polar(n_qubits);

    qc_base.x(1);
    qc_polar.x(1);

    qc_base.qft();
    qc_polar.qft();

    std::cout << "QuantumCircuit<std::complex<double>>\n";
    utils::print_statevector_as_prob(qc_base);

    std::cout << "QuantumCircuit<SmallPolar<4, 4>>\n";
    utils::print_statevector_as_prob(qc_polar);


    
}

