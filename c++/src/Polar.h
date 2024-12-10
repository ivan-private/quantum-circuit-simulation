#pragma once

#include <complex>
#include <concepts>


template<std::floating_point Float_t>
class Polar
{
public:

    //TODO: maybe add noexcept to everything?

    Polar() : r(0.0), theta(0.0) {}

    Polar(Float_t r, Float_t theta) : r(r), theta(theta) {}

    Polar(const Polar<Float_t>& other) : r(other.r), theta(other.theta) {}

    template<std::floating_point T>
    Polar(const std::complex<T>& z) : r(std::abs(z)), theta(std::arg(z)) {}

    template<std::floating_point T>
    explicit operator std::complex<T>() const
    {
        return std::polar<T>(static_cast<T>(r), static_cast<T>(theta));
    }


    bool 
    is_zero() const noexcept
    {
        return r < EPSILON;
    }


    bool
    operator == (const Polar<Float_t>& other) const noexcept
    {
        return r == other.r && theta == other.theta;
    }


    bool
    operator != (const Polar<Float_t>& other) const noexcept
    {
        return !(*this == other);
    }


    Polar<Float_t>
    operator * (const Polar<Float_t>& other) const 
    {
        return static_cast<Polar<Float_t>>(
            static_cast<std::complex<double>>(*this) * static_cast<std::complex<double>>(other)
        );
    }   


    Polar<Float_t>
    operator *= (const Polar<Float_t>& other) 
    {
        *this = (*this) * other;
        return *this;
    }   


    Polar<Float_t>
    operator + (const Polar<Float_t>& other) const 
    {
        return static_cast<Polar<Float_t>>(
            static_cast<std::complex<double>>(*this) + static_cast<std::complex<double>>(other)
        );
    }   


    Polar<Float_t>
    operator += (const Polar<Float_t>& other) 
    {
        *this = (*this) + other;
        return *this;
    }   


    Polar<Float_t>
    operator - (const Polar<Float_t>& other) const 
    {
        return static_cast<Polar<Float_t>>(
            static_cast<std::complex<double>>(*this) - static_cast<std::complex<double>>(other)
        );
    }   


    Polar<Float_t>
    operator -= (const Polar<Float_t>& other) 
    {
        *this = (*this) - other;
        return *this;
    }   


    double 
    get_phase_degrees() const noexcept
    {
        return static_cast<double>(theta) * 180.0 / M_PI;
    }


    double
    get_magnitude() const noexcept
    {   
        return static_cast<double>(r);
    }


    double 
    get_phase() const noexcept
    {   
        return static_cast<double>(theta);
    }


private:
    Float_t r;
    Float_t theta;


    static constexpr double EPSILON = 1e-9;

};




