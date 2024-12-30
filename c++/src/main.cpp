#include <iostream>

#include <cmath>
#include <numbers>
#include <complex>

//#include "Traits.h"
#include "SmallPolar.h"
#include "Polar.h"
#include "QuantumCircuit.h"
#include "Utils.h"

#include <vector>
#include <algorithm>

#include <thread>

// working prototype
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


// example usecase 
int
main()
{    
    static_assert(sizeof(SmallPolar<4, 4>) == 1);
    static_assert(sizeof(SmallPolar<8, 8>) == 2);

    for (int i = 0; i < 20; i++)
    {
        const double r = std::pow(1 / std::numbers::sqrt2, i);
        TwoBytePolar x(r, 0.0);

        std::cout << "\n\nr = (1 / sqrt(2))^" << i << '\n' << x;
    }
    

    QuantumCircuit<SmallPolar<7,1>> qc(4);
    qc.x(1);
    qc.qft();
    //qc.ghz();

    utils::print_statevector_as_prob(qc);
}

