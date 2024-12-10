#include "Utils.h"

#include "SmallPolar.h"

std::string
utils::to_string(const std::vector<std::complex<double>>& statevector)
{
    std::stringstream ss;
    ss << "\n";

    for (uint64_t i = 0; i < statevector.size(); i++)
    {
        const auto& state = statevector[i];
        ss << "|" << i << ">\t" << state << "\n";
    }

    return ss.str();
}


std::string
utils::to_string(const std::vector<double>& statevector)
{
    std::stringstream ss;
    ss << "\n";

    for (uint64_t i = 0; i < statevector.size(); i++)
    {
        const auto& state = statevector[i];
        ss << "|" << i << ">\t" << state << "\n";
    }

    return ss.str();
}


std::string
utils::to_prob_string(const std::vector<std::complex<double>>& statevector)
{
    std::stringstream ss;
    ss << "\n";

    for (uint64_t i = 0; i < statevector.size(); i++)
    {
        const auto& state = statevector[i];

        const auto prob = std::norm(state) * 100;
        const auto phase = std::arg(state) * 180.0 / M_PI;

        ss << "|" << i << ">\tprob: " << prob << "%,\tphase: " << phase << " degrees\n";
    }

    return ss.str();
}



// std::string
// utils::to_string(const FixedPointNumber8_t number)
// {    
//     std::stringstream ss;

//     ss << double(number);
    

//     return ss.str();
// }




// void
// utils::test_FixedPointNumber8_t()
// {
//     FixedPointNumber8_t n;
//     std::cout << "\nBase constructor: " << utils::to_string(n) << '\n';

//     n = 0.5;
//     std::cout << "\ndouble 0.5 copy assignment: " << utils::to_string(n) << '\n';

//     n = 1.0;
//     std::cout << "\ndouble 1.0 copy assignment: " << utils::to_string(n) << '\n';

//     n = 0.0;
//     std::cout << "\ndouble 0.0 copy assignment: " << utils::to_string(n) << '\n';

//     n = 0.5f;
//     std::cout << "\nfloat 0.5 copy assignment: " << utils::to_string(n) << '\n';

//     n = uint8_t(200);
//     std::cout << "\nuint8_t 200 copy assignment: " << utils::to_string(n) << '\n';

//     n = 1.5;
//     std::cout << "\ndouble 1.5 copy assignment, should clamp: " << utils::to_string(n) << '\n';

    

//     n = 0.5;
//     FixedPointNumber8_t x(0.25);

//     std::cout << "\nStart value n = " << utils::to_string(n) << ",\nx = " << utils::to_string(x) << '\n';

//     // n = 0.5
//     std::cout << "\n+ operator, n+x: " << utils::to_string(n+x) << '\n';

//     n = n+x; // n = 0.75
//     std::cout << "\n+ operator, n=n+x: " << utils::to_string(n) << '\n';

//     n = n + x + x; // n = 0.25
//     std::cout << "\n+ operator, n=n+x+x, should overflow: " << utils::to_string(n) << '\n';

//     n = n + 0.25; // n = 0.5
//     std::cout << "\n+ operator, n=n+0.25: " << utils::to_string(n) << '\n';

//     n = n - 0.25; // n = 0.25
//     std::cout << "\n- operator, n=n-0.25: " << utils::to_string(n) << '\n';

//     n = n - x; // n = 0 or n = 1 (same for phase but really different for amplitudes)
//     std::cout << "\n- operator, n=n-x: " << utils::to_string(n) << '\n';

//     n = n - x; // n = 0.75
//     std::cout << "\n- operator, n=n-x: " << utils::to_string(n) << '\n';

//     n += x; // n = 1 or n = 0
//     std::cout << "\n+= operator, n+=x: " << utils::to_string(n) << '\n';

//     n += 0.25; // n = 0.25
//     std::cout << "\n+= operator, n+=0.25: " << utils::to_string(n) << '\n';

//     n -= 0.5; // n = 0.75
//     std::cout << "\n-= operator, n-=0.5: " << utils::to_string(n) << '\n';

//     n -= x; // n = 0.5
//     std::cout << "\n-= operator, n-=x: " << utils::to_string(n) << '\n';


//     n = 0.5;
//     x = 0.5;
//     std::cout << "\nStart value n = " << utils::to_string(n) << ",\nx = " << utils::to_string(x) << '\n';

//     n = n * x; // n = 0.25
//     std::cout << "\n* operator, n = n*x: " << utils::to_string(n) << '\n';

//     n = n * 0.5; // n = 0.125
//     std::cout << "\n* operator, n = n*0.25: " << utils::to_string(n) << '\n';

//     n *= x; // n = 0.0625
//     std::cout << "\n*= operator, n*=x: " << utils::to_string(n) << '\n';

//     n *= 0.0; // n = 0.0
//     std::cout << "\n*= operator, n*=0.0: " << utils::to_string(n) << '\n';


//     n = 0.5;
//     x = 0.5;
//     std::cout << "\nStart value n = " << utils::to_string(n) << ",\nx = " << utils::to_string(x) << '\n';

//     n = n / x; // n = 1
//     std::cout << "\n/ operator, n = n/x: " << utils::to_string(n) << '\n';

//     n = n / 2.0; // n = 0.5
//     std::cout << "\n/ operator, n = n/2.0: " << utils::to_string(n) << '\n';

//     n /= 2.0; // n = 0.25
//     std::cout << "\n/= operator, n /= 2: " << utils::to_string(n) << '\n';

//     n /= x; // n = 0.5
//     std::cout << "\n/= operator, n /= x: " << utils::to_string(n) << '\n';

//     std::cout << "\n" << '\n';
// }

