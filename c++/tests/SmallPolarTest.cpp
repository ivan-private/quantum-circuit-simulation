#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <complex>
#include <cmath>

#define private public // so private class members can be accessed and tested

#include "SmallPolar.h"


constexpr double PI = M_PI;
constexpr double TWO_PI = 2.0 * M_PI;

double phase_diff(double phase1, double phase2)
{
    if (phase1 < 0.0)
        phase1 += TWO_PI;

    if (phase2 < 0.0)
        phase2 += TWO_PI;

    return std::min(
        std::abs(phase1 - phase2),
        std::min(
            std::abs(phase1 + TWO_PI - phase2),
            std::abs(phase2 + TWO_PI - phase1)
        )
    );
}


inline double convert_phase_range(double theta)
{
    return theta < 0.0 ? theta + TWO_PI : theta;
}


inline uint8_t phase_int(double theta_degrees, uint16_t num_representable_phases)
{
    return uint16_t((theta_degrees / 360.0) * num_representable_phases) % (num_representable_phases - 1);
}


inline double degrees_to_radians(double degrees)
{
    return (PI / 180.0) * degrees;
}

inline double radians_to_degrees(double radians)
{
    return (180.0 / PI) * radians;
}


template <uint8_t RBits, uint8_t ThetaBits>
struct SmallPolarConfig 
{
    static constexpr uint8_t r_bits = RBits;
    static constexpr uint8_t theta_bits = ThetaBits;
    using SmallPolarType = SmallPolar<RBits, ThetaBits>;
};


template <typename Config>
class SmallPolarTest : public testing::Test 
{
public:
    using SmallPolarType = Config::SmallPolarType;
    static constexpr uint8_t r_bits = Config::r_bits;
    static constexpr uint8_t theta_bits = Config::theta_bits;

    static constexpr double EPSILON_R = 1.0 / (double)((1 << r_bits));
    static constexpr double EPSILON_THETA = TWO_PI / (double)((1 << theta_bits));
};


using SmallPolarTestTypes = testing::Types<
    SmallPolarConfig<4, 4>,
    SmallPolarConfig<8, 8>
>;
TYPED_TEST_SUITE(SmallPolarTest, SmallPolarTestTypes);


TYPED_TEST(SmallPolarTest, SizeOfStruct) 
{
    using SmallPolarType = TestFixture::SmallPolarType;

    const uint8_t r_bits = TestFixture::r_bits;
    const uint8_t theta_bits = TestFixture::theta_bits;

    const uint8_t expected_size_bytes = (r_bits + theta_bits) / 8;

    EXPECT_EQ(sizeof(SmallPolarType), expected_size_bytes);
}


TYPED_TEST(SmallPolarTest, DefaultConstructor) 
{
    using SmallPolarType = TestFixture::SmallPolarType;
    SmallPolarType x;
    EXPECT_EQ(x.r, 0);
    EXPECT_EQ(x.theta, 0);
}


TYPED_TEST(SmallPolarTest, ComplexNumberConstructor) 
{
    using SmallPolarType = TestFixture::SmallPolarType;

    const std::complex<double> i(0.0, 1.0);// complex number i

    SmallPolarType x(i); // x = i

    EXPECT_EQ(x.r, (uint8_t) std::round(1.0 * SmallPolarType::MAX_VAL_R)) << "x = i"; 
    EXPECT_EQ(x.theta, phase_int(90, SmallPolarType::NUM_REPRESENTABLE_THETA)) << "x = i"; 



    std::complex<double> y = 0.5 * i * i; // -0.5
    EXPECT_DOUBLE_EQ(y.real(), -0.5);
    EXPECT_DOUBLE_EQ(y.imag(), 0.0);

    x = y; 
    EXPECT_EQ(x.r, (uint8_t) std::round(0.5 * SmallPolarType::MAX_VAL_R)) << "x = -0.5"; 
    EXPECT_EQ(x.theta, phase_int(180, SmallPolarType::NUM_REPRESENTABLE_THETA)) << "x = -0.5";


    x = 0.5 + 0.5*i; 
    EXPECT_EQ(x.r, (uint8_t) std::round(1.0 / std::sqrt(2.0) * SmallPolarType::MAX_VAL_R)) << "x = 0.5 + 0.5i"; 
    EXPECT_EQ(x.theta, phase_int(45, SmallPolarType::NUM_REPRESENTABLE_THETA)) << "x = 0.5 + 0.5i"; 


    x = (std::complex<float>)0.3;
    EXPECT_EQ(x.r, (uint8_t) std::round(0.3 * SmallPolarType::MAX_VAL_R)) << "x = 0.3"; 
    EXPECT_EQ(x.theta, phase_int(0, SmallPolarType::NUM_REPRESENTABLE_THETA)) << "x = 0.3"; 


    x = std::polar<double>(0.25, -PI / 2.0); // 90 degrees
    EXPECT_EQ(x.r, (uint8_t) std::round(0.25 * SmallPolarType::MAX_VAL_R)) << "x = (r, theta) = (0.25, 90)"; 
    EXPECT_EQ(x.theta, phase_int(360-90, SmallPolarType::NUM_REPRESENTABLE_THETA)) << "x = (r, theta) = (0.25, 90)"; 


    // my polar type only accepts r in range [0, 1]
    EXPECT_THROW(SmallPolarType(std::polar<double>(1.1, 0.0)), std::invalid_argument) << "x = (r, theta) = (1.1, 0)";

}


TYPED_TEST(SmallPolarTest, CopyConstructor) 
{
    using SmallPolarType = TestFixture::SmallPolarType;

    SmallPolarType x;
    SmallPolarType y = x;

    EXPECT_EQ(x.r, y.r);
    EXPECT_EQ(x.theta, y.theta);
    EXPECT_NE(&x, &y);
}


TYPED_TEST(SmallPolarTest, EncodeDecodeR)
{   
    using SmallPolarType = TestFixture::SmallPolarType;

    const double r_resolution = 1.0 / double(SmallPolarType::MAX_VAL_R);

    EXPECT_THROW(SmallPolarType::encode_r(1.1), std::invalid_argument);
    EXPECT_THROW(SmallPolarType::encode_r(-0.1), std::invalid_argument);

    // EXPECT_EQ(
    //     SmallPolarType::interpolate_r(0.0), 
    //     0
    // ) << "r: " << 0;


    // EXPECT_EQ(
    //     SmallPolarType::interpolate_r(r_resolution), 
    //     1
    // ) << "r: " << r_resolution;



    // EXPECT_EQ(
    //     SmallPolarType::interpolate_r(0.999), 
    //     SmallPolarType::MAX_VAL_R
    // ) << "r: " << 0.999;

} 


TYPED_TEST(SmallPolarTest, EncodeDecodeTheta)
{   
    using SmallPolarType = TestFixture::SmallPolarType;

    EXPECT_THROW(SmallPolarType::encode_theta(degrees_to_radians(400)), std::invalid_argument);

    // const double degrees_resolution = 360.0 / double(SmallPolarType::NUM_REPRESENTABLE_THETA);

    // uint8_t phase_int = 0;
    // for (double degrees = 0; degrees < 360; degrees += degrees_resolution)
    // {   
    //     ASSERT_EQ(
    //         SmallPolarType::encode_theta(degrees_to_radians(degrees)), 
    //         phase_int++
    //     ) << "degrees: " << degrees;
    // }
    
    // ASSERT_EQ(
    //         SmallPolarType::encode_theta(degrees_to_radians(360)), 
    //         0
    //     ) << "degrees: " << 360;
} 


TYPED_TEST(SmallPolarTest, GetPhase)
{   
    using SmallPolarType = TestFixture::SmallPolarType;

    const double rounding_epsilon = 1e-3;

    const double degrees_resolution = 360.0 / double(SmallPolarType::NUM_REPRESENTABLE_THETA);

    SmallPolarType x;
    
    for (double degrees = 0; degrees < 360; degrees += degrees_resolution)
    {  
        // exact representations of angles
        ASSERT_DOUBLE_EQ(x.get_phase_degrees(), degrees) 
            << "degrees: " << degrees;

        ASSERT_DOUBLE_EQ(x.get_phase(), degrees_to_radians(degrees)) 
            << "radians: " << degrees_to_radians(degrees);
        
        x.theta++;
    }
} 


TYPED_TEST(SmallPolarTest, GetMagnitude)
{   
    using SmallPolarType = TestFixture::SmallPolarType;

    const double ROUNDING_ERROR = 1e-9;

    const double r_resolution = 1.0 / double(SmallPolarType::MAX_VAL_R);

    SmallPolarType x;
    
    // exact representations of magnitudes
    for (double r = 0; r <= 1.0; r += r_resolution)
    {  
        ASSERT_NEAR(x.get_magnitude(), r, ROUNDING_ERROR) 
            << "r: " << r;
        x.r++;
    }
    
} 


TYPED_TEST(SmallPolarTest, CastingToComplexNumber) 
{
    using SmallPolarType = TestFixture::SmallPolarType;

    const double theta_resolution = TWO_PI / double(SmallPolarType::NUM_REPRESENTABLE_THETA);
    const double r_resolution = 1.0 / double(SmallPolarType::MAX_VAL_R);

    const double num_test_values = 16;

    SmallPolarType x;
    std::complex<double> x_complex;
    std::complex<double> y;

    
    for (double r = 0; r <= 1.0; r += 1.0 / num_test_values)
    {
        for (double theta = -PI; theta < PI; theta += TWO_PI / num_test_values)
        {
            y = std::polar(r, theta);
            x = y; // conversion constructor
            x_complex = static_cast<std::complex<double>>(x);  
            
            ASSERT_NEAR(std::abs(x_complex), std::abs(y), r_resolution) 
                << "expected r = " << r << "\n"
                << "actual r = " << std::abs(x_complex)
                << "r tol = " << r_resolution;


            ASSERT_LE(phase_diff(std::arg(x_complex), std::arg(y)), theta_resolution)
                << "expected theta = " << radians_to_degrees(theta) << "\n"
                << "actual theta = " << radians_to_degrees(std::arg(x_complex)) << "\n"
                << "diff = " << radians_to_degrees(phase_diff(std::arg(x_complex), std::arg(y))) << "\n"
                << "theta tol = " << radians_to_degrees(theta_resolution);
                
        }
    }

}


TYPED_TEST(SmallPolarTest, Multiplication) 
{
    using SmallPolarType = TestFixture::SmallPolarType;

    const double rounding_grace = 1e-9;
    const double theta_resolution = rounding_grace + TWO_PI / double(SmallPolarType::NUM_REPRESENTABLE_THETA);
    const double r_resolution = rounding_grace + 1.0 / double(SmallPolarType::MAX_VAL_R);

    const double num_test_values = 16;

    std::complex<double> z1, z2, z3;
    SmallPolarType a, b, c;


    for (double r1 = 1.0 / num_test_values; r1 <= 1.0; r1 += 1.0 / num_test_values)
    {
        for (double r2 = 1.0 / num_test_values; r2 <= 1.0; r2 += 1.0 / num_test_values)
        {
            for (double theta1 = -PI; theta1 < PI; theta1 += TWO_PI / num_test_values)
            {
                for (double theta2 = -PI; theta2 < PI; theta2 += TWO_PI / num_test_values)
                {
                    z1 = std::polar(r1, theta1);
                    z2 = std::polar(r2, theta2);
                    z3 = z1 * z2;

                    a = z1;
                    b = z2;
                    c = a * b;

                    ASSERT_NEAR(c.get_magnitude(), std::abs(z3), 1.5 * r_resolution);

                    ASSERT_LE(phase_diff(c.get_phase(), std::arg(z3)), theta_resolution)
                        << "expected theta = " << radians_to_degrees(std::arg(z3)) << "\n"
                        << "actual theta = " << radians_to_degrees(c.get_phase()) << "\n"
                        << "diff = " << radians_to_degrees(phase_diff(c.get_phase(), std::arg(z3))) << "\n"
                        << "theta tol = " << radians_to_degrees(theta_resolution) << "\n"
                        << "theta1 = " << theta1 << "\n"
                        << "theta2 = " << theta2 << "\n";
                }
            }
        }
    }


}