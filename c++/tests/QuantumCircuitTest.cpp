#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>
#include <cmath>
#include <complex>
#include <algorithm>
#include <concepts>
#include <cassert>

#include "Traits.h"

#define private public // so private class members can be accessed and tested

#include "QuantumCircuit.h"

using testing::Each;
using testing::DoubleEq;
using testing::ElementsAreArray;
using testing::Pointwise;
using testing::Le;
using testing::DoubleNear;


void complex_values_diff(const std::vector<std::complex<double>>& a, 
    const std::vector<std::complex<double>>& b,
    std::vector<double>& diff)
{
    assert(a.size() == b.size() && a.size() == diff.size());

    for (size_t i = 0; i < a.size(); i++)
    {
        diff[i] = std::abs(a[i]-b[i]);
    }
}


template <traits::ComplexNumber T>
struct TypeWrapper 
{
    using ComplexType = T;
};


// Parameterized test fixture
template <typename T>
class QuantumCircuitTest : public testing::Test 
{
public: 
    using ComplexType = T; 
};


using TestTypes = testing::Types<
    std::complex<double>,
    std::complex<float>
>;
TYPED_TEST_SUITE(QuantumCircuitTest, TestTypes);


TYPED_TEST(QuantumCircuitTest, Constructor) 
{
    using ComplexType = TestFixture::ComplexType;

    for (int n_qubits = 1; n_qubits <= 5; n_qubits++)
    {
        uint64_t n_states = (1 << n_qubits);

        QuantumCircuit<ComplexType> qc(n_qubits);

        EXPECT_EQ(qc.n_qubits, n_qubits);
        EXPECT_EQ(qc.n_states, n_states);

        const auto& statevector = qc.get_statevector();

        EXPECT_EQ(statevector.size(), n_states);

        EXPECT_DOUBLE_EQ(std::abs(statevector[0]), 1.0);
        EXPECT_DOUBLE_EQ(std::arg(statevector[0]), 0.0);


        std::vector<double> magnitudes;
        magnitudes.reserve(n_states);

        std::transform(statevector.cbegin()+1, statevector.cend(), 
            magnitudes.begin(), [](const auto& state){ return std::abs(state); });

        EXPECT_THAT(magnitudes, Each(DoubleEq(0.0)));
        

        std::vector<double> phases;
        phases.reserve(n_states);

        std::transform(statevector.cbegin()+1, statevector.cend(), 
            phases.begin(), [](const auto& state){ return std::arg(state); });
    }
    
}


TYPED_TEST(QuantumCircuitTest, MoveConstructor)
{
    using ComplexType = TestFixture::ComplexType;
    const uint8_t n_qubits = 3;
    const uint64_t n_states = (1 << n_qubits);

    QuantumCircuit<ComplexType> qc_throwaway(n_qubits);
    QuantumCircuit<ComplexType> qc = std::move(qc_throwaway);

    EXPECT_EQ(qc.n_qubits, n_qubits);
    EXPECT_EQ(qc.n_states, n_states);

    const auto& statevector = qc.get_statevector();

    EXPECT_EQ(statevector.size(), n_states);

    EXPECT_DOUBLE_EQ(std::abs(statevector[0]), 1.0);
    EXPECT_DOUBLE_EQ(std::arg(statevector[0]), 0.0);


    std::vector<double> magnitudes(n_states);

    std::transform(statevector.cbegin()+1, statevector.cend(), 
        magnitudes.begin(), [](const auto& state){ return std::abs(state); });

    EXPECT_THAT(magnitudes, Each(DoubleEq(0.0)));
    

    std::vector<double> phases(n_states);

    std::transform(statevector.cbegin()+1, statevector.cend(), 
        phases.begin(), [](const auto& state){ return std::arg(state); });

} 


TYPED_TEST(QuantumCircuitTest, XGate) 
{
    using ComplexType = TestFixture::ComplexType;

    constexpr double EPSILON_COMPLEX_DIFF = 1e-6;

    const uint8_t n_qubits = 3;
    const uint64_t n_states = (1 << n_qubits);

    QuantumCircuit<ComplexType> qc(n_qubits);
    std::vector<std::complex<double>> correct_statevector(n_states);
    std::vector<double> diff(n_states);


    qc.x(0);
    correct_statevector = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, 
                            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    EXPECT_THAT(qc.get_statevector(), ElementsAreArray(correct_statevector));


    qc.x(0);
    correct_statevector = {{1.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, 
                            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    EXPECT_THAT(qc.get_statevector(), ElementsAreArray(correct_statevector));


    qc.x(0);
    correct_statevector = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, 
                            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    EXPECT_THAT(qc.get_statevector(), ElementsAreArray(correct_statevector));


    qc.x(1);
    correct_statevector = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {1.0, 0.0}, 
                            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    EXPECT_THAT(qc.get_statevector(), ElementsAreArray(correct_statevector));


    qc.x(2);
    correct_statevector = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, 
                            {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {1.0, 0.0}};
    EXPECT_THAT(qc.get_statevector(), ElementsAreArray(correct_statevector));


    // Reset QC
    qc.reset_statevector();

    qc.h(0);
    qc.x(1);
    correct_statevector = {{0.0, 0.0}, {0.0, 0.0}, {0.7071067811865476, 0.0}, {0.7071067811865475, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
    complex_values_diff(qc.get_statevector(), correct_statevector, diff);
    EXPECT_THAT(diff, Each(Le(EPSILON_COMPLEX_DIFF)));

}

