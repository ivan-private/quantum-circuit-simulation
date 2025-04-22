#pragma once

#include <concepts>
#include <complex>

#include "Polar.h"

namespace traits 
{
    template <typename T>
    concept PolarCoordinate =   std::regular<T> &&
                                std::convertible_to<std::complex<double>, T> &&
                                std::constructible_from<T, std::complex<double>> &&
                                std::constructible_from<T, double, double> &&
                                requires(T x) 
                                { 
                                    { x.is_zero() } -> std::same_as<bool>;

                                    { x.get_phase() } -> std::floating_point;

                                    { x.get_phase_degrees() } -> std::floating_point;

                                    { x.get_magnitude() } -> std::floating_point;

                                    {x * x} -> std::same_as<T>;
                                    {x *= x} -> std::same_as<T>;

                                    {x + x} -> std::same_as<T>;
                                    {x += x} -> std::same_as<T>;

                                    {x - x} -> std::same_as<T>;
                                    {x -= x} -> std::same_as<T>;

                                };


    template <typename T>
    concept StdComplex =    std::same_as<T, std::complex<long double>> ||
                            std::same_as<T, std::complex<double>> ||
                            std::same_as<T, std::complex<float>>;


    template <typename T>
    concept ComplexNumber = StdComplex<T> || PolarCoordinate<T>;


    template<typename T>
    concept QuantumCircuit = requires(T x)
    {
        x.h( int{} );
        x.x( int{} );
        x.cx( int{}, int{} );
        x.cp( double{}, int{}, int{} );
        x.swap( int{}, int{} );
        x.qft();
        x.ghz();


        { x.get_statevector() } -> std::convertible_to<std::vector<Polar>>; 
        x.reset_statevector();

        { x.num_qubits() } -> std::convertible_to<int>;
    };

} // namespace traits