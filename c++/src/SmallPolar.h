#pragma once

#include <complex>
#include <concepts>


template<uint8_t r_bits=8, uint8_t theta_bits=8>
class SmallPolar
{   
public:
    SmallPolar() : r(0), theta(0) {}


    SmallPolar(double r, double theta)
        : r(interpolate_r(r)), theta(interpolate_theta(theta)) {}


    SmallPolar(const SmallPolar<r_bits, theta_bits>& other) 
        : r(other.r), theta(other.theta) {}


    template<std::floating_point T>
    SmallPolar(const std::complex<T>& z) 
        : r(interpolate_r(std::abs(z))), theta(interpolate_theta(std::arg(z))) {}


    template<std::floating_point T>
    explicit operator std::complex<T>() const
    {
        return std::polar<T>(get_magnitude(), get_phase());
    }


    bool 
    is_zero() const noexcept
    {
        return r == 0;
    }


    bool
    operator == (const SmallPolar<r_bits, theta_bits>& other) const noexcept
    {
        return r == other.r && theta == other.theta;
    }


    bool
    operator != (const SmallPolar<r_bits, theta_bits>& other) const noexcept
    {
        return !(*this == other);
    }


    SmallPolar<r_bits, theta_bits>
    operator * (SmallPolar<r_bits, theta_bits> other) const 
    {
        uint16_t product_r = static_cast<uint16_t>(this->r) * static_cast<uint16_t>(other.r);
        uint8_t r = static_cast<uint8_t>(product_r / MAX_VAL_R);

        // adds to theta even if r is 0
        // could be good if r is small so it rounds to 0
        uint8_t theta = this->theta + other.theta;

        return SmallPolar<r_bits, theta_bits>(r, theta);
    }   


    SmallPolar<r_bits, theta_bits>
    operator *= (SmallPolar<r_bits, theta_bits> other) 
    {
        *this = (*this) * other;
        return *this;
    }   


    SmallPolar<r_bits, theta_bits>
    operator + (SmallPolar<r_bits, theta_bits> other) const 
    {
        auto z = static_cast<std::complex<double>>(*this);
        z += static_cast<std::complex<double>>(other);
        if (std::abs(z) > 1.0)
            z /= std::abs(z);
        return static_cast<SmallPolar<r_bits, theta_bits>>(z);
    }


    SmallPolar<r_bits, theta_bits>
    operator += (SmallPolar<r_bits, theta_bits> other) 
    {
        *this = (*this) + other;
        return *this;
    }


    SmallPolar<r_bits, theta_bits>
    operator - (SmallPolar<r_bits, theta_bits> other) const 
    {
        auto z = static_cast<std::complex<double>>(*this);
        z -= static_cast<std::complex<double>>(other);
        if (std::abs(z) > 1.0)
            z /= std::abs(z);
        return static_cast<SmallPolar<r_bits, theta_bits>>(z);
    }   


    SmallPolar<r_bits, theta_bits>
    operator -= (SmallPolar<r_bits, theta_bits> other) 
    {
        *this = (*this) - other;
        return *this;
    }   


    double
    get_magnitude() const noexcept
    {   
        return static_cast<double>(r) / static_cast<double>(MAX_VAL_R);
    }


    double 
    get_phase() const noexcept
    {   
        // radians in the range [0, 2pi)
        return 2.0 * M_PI * (static_cast<double>(theta) / static_cast<double>(NUM_REPRESENTABLE_THETA));
    }


    double 
    get_phase_degrees() const noexcept
    {
        return 360.0 * (static_cast<double>(theta) / static_cast<double>(NUM_REPRESENTABLE_THETA));
    }


private:

    explicit SmallPolar(uint8_t r, uint8_t theta) : r(r), theta(theta) {}

    static uint8_t
    interpolate_r(double r)
    {
        if ( !(0.0 <= r && r <= 1.0) )
            throw std::invalid_argument("r must be between 0 and 1: r=" + std::to_string(r));

        return static_cast<uint8_t>(std::round(r * static_cast<double>(MAX_VAL_R)));
    }


    static uint8_t
    interpolate_theta(double theta)
    {           
        // std::complex::arg gives theta in the range [-pi, pi]
        // I also want it to work with angles in the range [0, 2pi]
        if ( !(-M_PI <= theta && theta <= 2.0 * M_PI) )
            throw std::invalid_argument("theta must be between -pi and 2pi: theta=" + std::to_string(theta));

        if (theta < 0.0)
            theta += 2.0 * M_PI; // theta in range [0, 2pi]

        theta /= 2.0 * M_PI; // theta in range [0, 1]

        if (theta > 1.0)
            theta = 1.0;

        return static_cast<uint16_t>(static_cast<uint16_t>(std::round(theta * static_cast<double>(NUM_REPRESENTABLE_THETA))) % NUM_REPRESENTABLE_THETA);
    }



private:
    uint8_t r: r_bits;
    uint8_t theta: theta_bits;

    static constexpr uint8_t MIN_VAL = 0;
    static constexpr uint8_t MAX_VAL_R = (1 << r_bits) - 1;
    static constexpr uint8_t MAX_VAL_THETA = (1 << theta_bits) - 1;

    static constexpr uint16_t NUM_REPRESENTABLE_R = (1 << r_bits);
    static constexpr uint16_t NUM_REPRESENTABLE_THETA = (1 << theta_bits);

};


using OneBytePolar = SmallPolar<4, 4>;
using TwoBytePolar = SmallPolar<8, 8>;
