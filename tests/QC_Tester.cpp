#include <cassert>
#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <numbers>
#include <complex>
#include <algorithm>
#include <numeric>
#include <functional>

#include "QuantumCircuit.h"
#include "Traits.h"
#include "Polar.h"
#include "SmallPolar.h"
#include "Utils.h"


template<traits::PolarCoordinate PolarToTest>
std::string type_str()
{
    if (std::is_same_v<PolarToTest, OneBytePolar>)
        return "OneBytePolar";
    
    else if (std::is_same_v<PolarToTest, TwoBytePolar>)
        return "TwoBytePolar";
    
    else 
        throw std::logic_error("Test for given type not implemented.");
}


template<traits::PolarCoordinate PolarToTest>
std::string statevector_filename(int qubits, const std::string& circuit)
{
    return "results/data/" + type_str<PolarToTest>() + "/statevectors/" + std::to_string(qubits) + "_qubits_" + circuit + ".csv";
}


template<traits::PolarCoordinate PolarToTest>
std::string statevector_error_filename(const std::string& circuit)
{
    return "results/data/" + type_str<PolarToTest>() + "/" + circuit + "_statevector_error.csv";
}


template<traits::PolarCoordinate PolarToTest>
void save_results(const std::string& circuit, const QuantumCircuit<std::complex<double>>& actual_qc, const QuantumCircuit<PolarToTest>& qc)
{
    std::string filename = statevector_filename<PolarToTest>(actual_qc.num_qubits(), circuit);
    utils::statevector_comparision_as_csv(filename, actual_qc, qc);

    filename = statevector_error_filename<PolarToTest>(circuit);
    utils::fidelity_and_error_as_csv(filename, actual_qc, qc);
}


template<traits::PolarCoordinate PolarToTest>
void run_test()
{
    std::mt19937 rng;
    rng.seed(7);

    std::uniform_int_distribution<int> random_gate(0, 4);

    const double phase = M_PI / 2;

    // 5, 10, 15, 20, 25
    for (int n : {3, 4, 5})
    {
        std::cout << "\n======================\n";
        std::cout <<"Qubits: " << n << '\n';

        QuantumCircuit<PolarToTest> qc(n);
        QuantumCircuit<std::complex<double>> actual_qc(n);

        std::uniform_int_distribution<int> random_qubit(0, n-1);


        auto random_qubit_not_same = [&random_qubit, &rng] (int control_qubit) 
        {
            int q;
            do { q = random_qubit(rng); } while (q == control_qubit);
            return q;
        };

        for (int num_random_gates : {100})
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
            }

            save_results<PolarToTest>(std::to_string(num_random_gates) + "_random_gates", actual_qc, qc);

            actual_qc.reset_statevector();
            qc.reset_statevector();
            std::cout   << "Random gates: " << num_random_gates
                        << ", Fidelity: " << utils::fidelity(actual_qc, qc) << "\n";
        }


        actual_qc.x(1);
        qc.x(1);

        actual_qc.qft();
        qc.qft();

        save_results<PolarToTest>("QFT", actual_qc, qc);

        std::cout << "\nQFT, " << "Fidelity: " << utils::fidelity(actual_qc, qc) << "\n";

        actual_qc.reset_statevector();
        qc.reset_statevector();



        actual_qc.ghz();
        qc.ghz();

        save_results<PolarToTest>("GHZ", actual_qc, qc);

        std::cout << "\nGHZ, " << "Fidelity: " << utils::fidelity(actual_qc, qc) << "\n";

        actual_qc.reset_statevector();
        qc.reset_statevector();


        std::cout << "\n======================\n";
    }
}



int main()
{    
    static_assert(sizeof(SmallPolar<4, 4>) == 1);
    static_assert(sizeof(SmallPolar<8, 8>) == 2);
    static_assert(sizeof(OneBytePolar) == 1);
    static_assert(sizeof(TwoBytePolar) == 2);

    run_test<OneBytePolar>();
    run_test<TwoBytePolar>();

}

