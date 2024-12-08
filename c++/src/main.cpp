#include <iostream>
#include <format>
#include <complex>
#include <vector>

#include "SmallPolar.h"


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
print(double r)
{
    using P4 = SmallPolar<4, 4>;

    P4 x = std::polar<double>(r);

    std::cout << "\nx.get_magnitude() = " << x.get_magnitude() << "\n"
                << "r as int = " << (int) x.r << " / " << (int) 0xF << "\n"
                << "actual r = " << r << "\n";
    
}


int
main(int argc, char** argv)
{    
    print_all_magnitudes();

    std::cout << "\n\n";

    print(0.93);
    print(0.94);
    print(0.967);
}


