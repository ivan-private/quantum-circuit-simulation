#include <iostream>
#include <format>
#include <complex>
#include <vector>
#include <cmath>

#include "Traits.h"

#include "SmallPolar.h"
#include "Polar.h"

#include "QuantumCircuit.h"

#include "Utils.h"



void print_pc(traits::PolarCoordinate auto pc)
{
    std::cout << "Polar coordinate: (" << pc.get_magnitude() << ", " << pc.get_phase_degrees() << ")\n";
}


int
main(int argc, char** argv)
{    
    static_assert(sizeof(OneBytePolar) == 1);
    static_assert(sizeof(TwoBytePolar) == 2);

    std::complex<double> z(1.0, 0.0);


    const uint8_t n_qubits = 3;
    QuantumCircuit<std::complex<double>> qc_base(n_qubits);
    //QuantumCircuit<Polar<double>> qc_polar(n_qubits);
    
    QuantumCircuit<TwoBytePolar> qc_polar(n_qubits);

    qc_base.x(1);
    qc_polar.x(1);

    qc_base.qft();
    qc_polar.qft();

    std::cout << "BaseQC<double> statevector:";
    utils::print_statevector_as_prob(qc_base);

    std::cout << "\nPolarQC<Polar<double>> statevector:";
    utils::print_statevector_as_prob(qc_polar);


}


