#include <iostream>
#include <format>
#include <complex>
#include <vector>

#include "SmallPolar.h"

constexpr double PI = M_PI;
constexpr double TWO_PI = 2.0 * M_PI;

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
                << "expected: (" << std::abs(z1) << ", " << radians_to_degrees(std::arg(z1)) << ")\n";
}


int
main(int argc, char** argv)
{    

    multiply_print(std::polar(0.5, degrees_to_radians(0)), std::polar(0.5, degrees_to_radians(90)));

    multiply_print(std::polar(1.0, degrees_to_radians(45)), std::polar(0.5, degrees_to_radians(45)));

    multiply_print(std::polar(1.0, degrees_to_radians(360)), std::polar(1.0, degrees_to_radians(90)));

    SmallPolar<4,4> x;
    x = std::polar<double>(1.0, degrees_to_radians(360 - 20));

    std::cout << "\nx = " << x << "\n";
}


