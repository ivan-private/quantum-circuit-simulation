#include "Utils.h"


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


