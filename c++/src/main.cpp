#include <iostream>

#include <cmath>
#include <numbers>
#include <complex>

#include "Traits.h"
#include "SmallPolar.h"
#include "Polar.h"
#include "QuantumCircuit.h"
#include "Utils.h"


Polar<double>
add(const Polar<double> a, const Polar<double> b)
{
    double r1 = a.get_magnitude();
    double r2 = b.get_magnitude();
    double theta1 = a.get_phase();
    double theta2 = b.get_phase();

    double r = std::sqrt( r1*r1 + r2*r2 + 2.0 * r1 * r2 * std::cos(theta2 - theta1) );
    double theta = theta1 + std::atan2(r2 * std::sin(theta2 - theta1), r1 + r2 * std::cos(theta2 - theta1));

    return Polar<double>(r, theta);
}


int
main(int argc, char** argv)
{    
    static_assert(sizeof(SmallPolar<4, 4>) == 1);
    static_assert(sizeof(SmallPolar<8, 8>) == 2);

    constexpr double PI = std::numbers::pi;

    std::complex<double> z1 = std::polar(1.0 / std::sqrt(2), 0.0);
    std::complex<double> z2 = std::polar(1.0 / std::sqrt(2), PI / 2.0);
    std::complex<double> z3 = z1 + z2;

    Polar<double> z1_polar(z1);
    Polar<double> z2_polar(z2);
    Polar<double> z3_polar = add(z1_polar, z2_polar);

    std::cout << "z3 = z1 + z2 = " << z3 << '\n'
            << "z3_polar = z1_polar + z2_polar = " << static_cast<std::complex<double>>(z3_polar) << '\n';

    const uint8_t n_qubits = 3;

    QuantumCircuit<std::complex<double>> qc_base(n_qubits);

    QuantumCircuit<SmallPolar<4, 4>> qc_polar(n_qubits);

    qc_base.x(1);
    qc_polar.x(1);

    qc_base.qft();
    qc_polar.qft();

    std::cout << "\nQuantumCircuit<std::complex<double>> statevector:";
    utils::print_statevector_as_prob(qc_base);

    std::cout << "\nQuantumCircuit<SmallPolar<4, 4>> statevector:";
    utils::print_statevector_as_prob(qc_polar);

}


