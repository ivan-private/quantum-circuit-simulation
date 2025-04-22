#include <iostream>
#include <complex>

#include "SmallPolar.h"
#include "Polar.h"
#include "QuantumCircuit.h"
#include "Utils.h"


// example usecase 
int main()
{    
    static_assert(sizeof(SmallPolar<4, 4>) == 1);
    static_assert(sizeof(SmallPolar<8, 8>) == 2);
    static_assert(sizeof(OneBytePolar) == 1);
    static_assert(sizeof(TwoBytePolar) == 2);

    const int n_qubits = 3;

    QuantumCircuit<std::complex<double>> qc_base(n_qubits);

    QuantumCircuit<OneBytePolar> qc_polar(n_qubits);

    qc_base.x(1);
    qc_polar.x(1);

    qc_base.qft();
    qc_polar.qft();

    std::cout << "QC Base:\n";
    utils::print_statevector_as_prob(qc_base);

    std::cout << "\nQC Polar:\n";
    utils::print_statevector_as_prob(qc_polar);    
}

