#pragma once

#include <complex>
#include <concepts>

#include "SmallPolar.h"

class Polar
{
public:

    //TODO: maybe add noexcept to everything?

    constexpr Polar() : r(0.0), theta(0.0) {}

    constexpr Polar(double r, double theta) : r(r), theta(theta) {}

    Polar(const std::complex<double>& z) : r(std::abs(z)), theta(std::arg(z)) {}

    template<uint8_t r_bits, uint8_t theta_bits>
    Polar(const SmallPolar<r_bits, theta_bits>& sp) : r(sp.get_magnitude()), theta(sp.get_phase()) {}


    constexpr Polar(const Polar& other) = default;
    constexpr Polar(Polar&& other) noexcept = default;
    constexpr ~Polar() = default;
    constexpr Polar& operator = (const Polar& other) = default;
    constexpr Polar& operator = (Polar&& other) noexcept = default;


    explicit operator std::complex<double>() const
    {
        return std::polar(r, theta);
    }


    bool 
    is_zero() const noexcept
    {
        return r < EPSILON;
    }


    bool
    operator == (const Polar& other) const noexcept
    {
        return r == other.r && theta == other.theta;
    }


    bool
    operator != (const Polar& other) const noexcept
    {
        return !(*this == other);
    }


    Polar
    operator * (const Polar& other) const 
    {
        return static_cast<Polar>(
            static_cast<std::complex<double>>(*this) * static_cast<std::complex<double>>(other)
        );
    }   


    Polar
    operator *= (const Polar& other) 
    {
        *this = (*this) * other;
        return *this;
    }   


    Polar
    operator + (const Polar& other) const 
    {
        return static_cast<Polar>(
            static_cast<std::complex<double>>(*this) + static_cast<std::complex<double>>(other)
        );
    }   


    Polar
    operator += (const Polar& other) 
    {
        *this = (*this) + other;
        return *this;
    }   


    Polar
    operator - (const Polar& other) const 
    {
        return static_cast<Polar>(
            static_cast<std::complex<double>>(*this) - static_cast<std::complex<double>>(other)
        );
    }   


    Polar
    operator -= (const Polar& other) 
    {
        *this = (*this) - other;
        return *this;
    }   


    double 
    get_phase_degrees() const noexcept
    {
        return theta * 180.0 / M_PI;
    }


    double
    get_magnitude() const noexcept
    {   
        return r;
    }


    double 
    get_phase() const noexcept
    {   
        return theta;
    }


private:
    double r;
    double theta;


    static constexpr double EPSILON = 1e-9;

};




