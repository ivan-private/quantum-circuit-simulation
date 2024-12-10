#include <benchmark/benchmark.h>
#include <complex>

#include "Traits.h"
#include "Polar.h"
#include "SmallPolar.h"
#include "QuantumCircuit.h"


constexpr int MIN_QUBITS = 19;
constexpr int MAX_QUBITS = 19;


template<traits::ComplexNumber ComplexType>
void complex_multiplication(benchmark::State& state) 
{
    ComplexType z(std::complex<double>(1.0, 0.0));

    ComplexType i(std::complex<double>(0.0, 1.0));

    for (auto _ : state) 
    {
        benchmark::DoNotOptimize(z *= i);
    }
}


template<traits::ComplexNumber ComplexType>
void complex_addition(benchmark::State& state) 
{
    ComplexType z(std::complex<double>(1.0, 0.0));

    ComplexType i(std::complex<double>(0.0, 1.0));

    for (auto _ : state) 
    {
        benchmark::DoNotOptimize(z += i);
    }
}


template<traits::ComplexNumber ComplexType>
void x_gate(benchmark::State& state)
{
    const int n_qubits = state.range(0);

    QuantumCircuit<ComplexType> qc(n_qubits);

    for (auto _ : state) 
    {
        for (int i = 0; i < n_qubits; i++) 
        {
            qc.x(i);
        }
    }
}


template<traits::ComplexNumber ComplexType>
void cp_gate(benchmark::State& state)
{
    const int n_qubits = state.range(0);
    constexpr double theta = M_PI_4; 

    QuantumCircuit<ComplexType> qc(n_qubits);

    for (int i = 0; i < n_qubits; i++) 
    {
        qc.x(i);
    }


    for (auto _ : state) 
    {
        for (int i = 0; i < n_qubits; i++) 
        {
            for (int j = i+1; j < n_qubits; j++) 
            {
                qc.cp(theta, i, j);
            }
            
        }
    }
}


template<traits::ComplexNumber ComplexType>
void h_gate(benchmark::State& state)
{
    const int n_qubits = state.range(0);

    QuantumCircuit<ComplexType> qc(n_qubits);

    // for (int i = 0; i < n_qubits; i++) 
    // {
    //     qc.x(i);
    // }
    

    for (auto _ : state) 
    {
        for (int i = 0; i < n_qubits; i++) 
        {
            qc.h(i);
        }
    }
}



template<traits::ComplexNumber ComplexType>
void qft(benchmark::State& state)
{
    const int n_qubits = state.range(0);

    QuantumCircuit<ComplexType> qc(n_qubits);

    qc.x(1);
    
    for (auto _ : state) 
    {
        for (int i = 0; i < n_qubits; i++) 
        {
            qc.qft();
        }
    }
}



BENCHMARK(complex_multiplication< std::complex<double> >);
BENCHMARK(complex_multiplication< std::complex<float> >);
BENCHMARK(complex_multiplication< Polar<double> >);
BENCHMARK(complex_multiplication< Polar<float> >);
BENCHMARK(complex_multiplication< TwoBytePolar >);
BENCHMARK(complex_multiplication< OneBytePolar >);


BENCHMARK(complex_addition< std::complex<double> >);
BENCHMARK(complex_addition< std::complex<float> >);
BENCHMARK(complex_addition< Polar<double> >);
BENCHMARK(complex_addition< Polar<float> >);
BENCHMARK(complex_addition< TwoBytePolar >);
BENCHMARK(complex_addition< OneBytePolar >);


BENCHMARK(x_gate< std::complex<double> >)   ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(x_gate< std::complex<float> >)    ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(x_gate< Polar<double> >)          ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(x_gate< Polar<float> >)           ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(x_gate< TwoBytePolar >)           ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(x_gate< OneBytePolar >)           ->DenseRange(MIN_QUBITS, MAX_QUBITS);


BENCHMARK(cp_gate< std::complex<double> >)  ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(cp_gate< std::complex<float> >)   ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(cp_gate< Polar<double> >)         ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(cp_gate< Polar<float> >)          ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(cp_gate< TwoBytePolar >)          ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(cp_gate< OneBytePolar >)          ->DenseRange(MIN_QUBITS, MAX_QUBITS);


BENCHMARK(h_gate< std::complex<double> >)  ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(h_gate< std::complex<float> >)   ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(h_gate< Polar<double> >)         ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(h_gate< Polar<float> >)          ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(h_gate< TwoBytePolar >)          ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(h_gate< OneBytePolar >)          ->DenseRange(MIN_QUBITS, MAX_QUBITS);


BENCHMARK(qft< std::complex<double> >)     ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(qft< std::complex<float> >)      ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(qft< Polar<double> >)            ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(qft< Polar<float> >)             ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(qft< TwoBytePolar >)             ->DenseRange(MIN_QUBITS, MAX_QUBITS);
BENCHMARK(qft< OneBytePolar >)             ->DenseRange(MIN_QUBITS, MAX_QUBITS);

BENCHMARK_MAIN();
