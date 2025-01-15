#include <cassert>
#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <numbers>
#include <complex>
#include <algorithm>
#include <numeric>

#include "QuantumCircuit.h"
#include "Traits.h"
#include "Polar.h"
#include "SmallPolar.h"




template<traits::ComplexNumber ComplexType>
double 
fidelity(const QuantumCircuit<std::complex<double>>& actual_qc, const QuantumCircuit<ComplexType>& qc) 
{
    std::complex<double> inner_product = 0.0;
    auto sv1 = actual_qc.get_statevector();
    auto sv2 = qc.get_statevector();

    assert (sv1.size() == sv2.size());
    
    for (size_t i = 0; i < sv1.size(); ++i) 
    {
        std::complex<double> z1 = static_cast<std::complex<double>>(sv1[i]);
        std::complex<double> z2 = static_cast<std::complex<double>>(sv2[i]);
        inner_product += z1 * std::conj(z2);
    }

    return std::norm(inner_product); 
}



int main()
{    
    static_assert(sizeof(SmallPolar<4, 4>) == 1);
    static_assert(sizeof(SmallPolar<8, 8>) == 2);

    // using PolarToTest = SmallPolar<4, 4>;
    using PolarToTest = SmallPolar<8, 8>;

    for (int n : {3, 5, 10, 15, 20, 25})
    {
        std::cout << "\n======================\n";
        std::cout <<"Qubits: " << n << '\n';

        QuantumCircuit<PolarToTest> qc(n);
        QuantumCircuit<std::complex<double>> actual_qc(n);

        std::mt19937 rng;
        rng.seed(1);

        std::uniform_int_distribution<int> random_qubit(0, n-1);

        std::uniform_int_distribution<int> random_gate(0, 4);

        const double phase = M_PI / 2;

        auto random_qubit_not_same = [&random_qubit, &rng] (int control_qubit) 
        {
            int q;
            do { q = random_qubit(rng); } while (q == control_qubit);
            return q;
        };

        for (int num_random_gates : {n, 2*n, n*n})
        {
            int qubit, qubit1, qubit2, control_qubit, target_qubit;
            

            std::cout << "\nGate\t\tcontrol\t\ttarget\t\tphase (degrees)\n";

            for (int i = 0; i < num_random_gates; i++)
            {
                switch (random_gate(rng))
                {
                    case 0:
                        qubit = random_qubit(rng);
                        std::cout << "H\t\t-\t\t" << qubit << '\n';
                        actual_qc.h(qubit);
                        qc.h(qubit);
                        break;

                    case 1:
                        qubit = random_qubit(rng);
                        std::cout << "X\t\t-\t\t" << qubit << '\n';
                        actual_qc.x(qubit);
                        qc.x(qubit);
                        break;

                    case 2:
                        control_qubit = random_qubit(rng);
                        target_qubit = random_qubit_not_same(control_qubit);
                        std::cout << "CX\t\t" << control_qubit << "\t\t" << target_qubit << '\n';
                        actual_qc.cx(control_qubit, target_qubit);
                        qc.cx(control_qubit, target_qubit);
                        break;

                    case 3:
                        control_qubit = random_qubit(rng);
                        target_qubit = random_qubit_not_same(control_qubit);
                        std::cout << "CP\t\t" << control_qubit << "\t\t" << target_qubit << "\t\t" << phase * 180.0/M_PI << '\n';
                        actual_qc.cp(phase, control_qubit, target_qubit);
                        qc.cp(phase, control_qubit, target_qubit);
                        break;

                    case 4:
                        qubit1 = random_qubit(rng);
                        qubit2 = random_qubit_not_same(control_qubit);
                        std::cout << "SWAP\t\t" << qubit1 << "\t\t" << qubit2 << '\n';
                        actual_qc.swap(qubit1, qubit2);
                        qc.swap(qubit1, qubit2);
                        break;
                    
                    // default:
                    //     std::cout << "random_gate generated a number which does not correspond to a gate";
                    //     exit(1);
                }

                actual_qc.reset_statevector();
                qc.reset_statevector();
            }

            std::cout   << "Random gates: " << num_random_gates
                        << ", Fidelity: " << fidelity(actual_qc, qc) << "\n";
        }


        actual_qc.x(1);
        qc.x(1);

        actual_qc.qft();
        qc.qft();

        std::cout << "\nQFT, " << "Fidelity: " << fidelity(actual_qc, qc) << "\n";

        actual_qc.reset_statevector();
        qc.reset_statevector();



        actual_qc.ghz();
        qc.ghz();

        std::cout << "\nGHZ, " << "Fidelity: " << fidelity(actual_qc, qc) << "\n";

        actual_qc.reset_statevector();
        qc.reset_statevector();


        std::cout << "\n======================\n";
    }
}

