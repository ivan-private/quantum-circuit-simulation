#include <iostream>
#include <format>
#include <complex>
#include <vector>
#include <cmath>

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

inline double degrees_to_radians(double degrees)
{
    return (PI / 180.0) * degrees;
}

inline double radians_to_degrees(double radians)
{
    return (180.0 / PI) * radians;
}

void 
print_all_magnitudes()
{
    using P4 = SmallPolar<4, 4>;

    P4 x;

    for (uint8_t r_int = 0; r_int <= 0xF; r_int++)
    {
        std::cout << "\nx.get_magnitude() = " << x.get_magnitude() << "\n"
                    << "r as int = " << (int) x.r << " / " << (int) 0xF << "\n";
        x.r++;
    }
}

void
multiply_print(std::complex<double> z1, std::complex<double> z2)
{
    using P4 = SmallPolar<4,4>;
    P4 a, b, c;

    a = z1;
    b = z2;

    std::cout   << "\na: " << a << "\n"
                << "b: " << b << "\n";

    a *= b;

    z1 *= z2;
    
    std::cout   << "a *= b: " << a << "\n"
                << "expected: (" << std::abs(z1) << ", " << std::arg(z1) << ")\n"
                << "theta diff: " << phase_diff(std::arg(z1), std::arg(static_cast<std::complex<double>>(a))) << "\n"
                << "theta resolution: " << TWO_PI / static_cast<double>(1 << 4) << "\n";
}


int
main(int argc, char** argv)
{    

    multiply_print(std::polar(1.0, degrees_to_radians(-157.31999999999979)), std::polar(1.0, degrees_to_radians(0)));

    multiply_print(std::polar(0.0, degrees_to_radians(180)), std::polar(0.0, degrees_to_radians(0)));
    
    std::complex<double> z1, z2, z3;
    z1 = std::polar(0.0, PI);
    z2 = std::polar(0.0, 0.0);
    z3 = z1*z2;
    std::cout << "z1: " << z1 << "\nz2: " << z2 << "\n";
    std::cout << "z3: " << std::abs(z3) << ", " << std::arg(z3) << "\n";
}


