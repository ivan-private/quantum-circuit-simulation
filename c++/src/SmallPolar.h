#pragma once

#include <complex>
#include <concepts>

#include <cmath>

#include <iostream>


template<uint8_t r_bits=8, uint8_t theta_bits=8>
class SmallPolar
{   
public:
    constexpr SmallPolar() : r(MAX_VAL_R), theta(0) {}

    constexpr ~SmallPolar() = default;
    constexpr SmallPolar(const SmallPolar& other) = default;
    constexpr SmallPolar& operator=(const SmallPolar& other) = default;
    constexpr SmallPolar(SmallPolar&& other) noexcept = default;
    constexpr SmallPolar& operator=(SmallPolar&& other) noexcept = default;



    constexpr SmallPolar(double r, double theta)
        : r(encode_r(r)), theta(encode_theta(theta)) {}



    template<std::floating_point T>
    SmallPolar(const std::complex<T>& z) 
        : r(encode_r(std::abs(z))), theta(encode_theta(std::arg(z))) {}


    template<std::floating_point T>
    explicit 
    operator std::complex<T>() const noexcept
    {
        return std::polar<T>(get_magnitude(), get_phase());
    }


    inline bool 
    is_zero() const noexcept
    {
        return r == MIN_VAL_R;
    }


    constexpr bool
    operator == (const SmallPolar<r_bits, theta_bits>& other) const noexcept
    {
        return r == other.r && theta == other.theta;
    }


    constexpr bool
    operator != (const SmallPolar<r_bits, theta_bits>& other) const noexcept
    {
        return !(*this == other);
    }


    SmallPolar<r_bits, theta_bits>
    operator * (SmallPolar<r_bits, theta_bits> other) const 
    {
        // return static_cast<SmallPolar<r_bits, theta_bits>>(
        //     static_cast<std::complex<double>>(*this) *
        //     static_cast<std::complex<double>>(other)
        // );        
        const uint8_t r_result = encode_r( decode_r(r) * decode_r(other.r) );
        const uint8_t theta_result = this->theta + other.theta;
        return SmallPolar<r_bits, theta_bits>(r_result, theta_result);
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
        return decode_r(r);
    }


    inline double 
    get_phase() const noexcept
    {   
        return decode_theta(theta);
    }


    inline double 
    get_phase_degrees() const noexcept
    {
        return decode_theta(theta) * 180.0 / M_PI;
    }


    // template<uint8_t r_bits=8, uint8_t theta_bits=8>
    friend std::ostream& 
    operator << (std::ostream& os, const SmallPolar<r_bits, theta_bits>& obj) 
    {
        os << "SmallPolar<" << static_cast<int>(r_bits) << ","
        << static_cast<int>(theta_bits) << ">: ("
        << static_cast<int>(obj.r) << ", " << static_cast<int>(obj.theta) << ") // ("
        << obj.get_magnitude() << ", " << obj.get_phase_degrees() << ")";
        //<< "\nMIN_VAL_R = " << static_cast<int>(MIN_VAL_R)
        //<< "\nMAX_VAL_R = "<< static_cast<int>(MAX_VAL_R);
        return os;
    }


private:

    explicit SmallPolar(uint8_t r, uint8_t theta) : r(r), theta(theta) {}

    static uint8_t
    encode_r(double r)
    {
        if (!(0.0 <= r && r <= 1.0))
            throw std::out_of_range("Input r must be in the range [0, 1].");

        if (r == 0.0) return MAX_VAL_R; 

        return static_cast<uint8_t>( std::round(-std::log2(r) / std::log2(EXP_BASE_R)) );
    }


    static double
    decode_r(uint8_t r)
    {
        if (r == MAX_VAL_R) return 0.0;

        return std::pow(EXP_BASE_R, -static_cast<double>(r)); 
    }


    static uint8_t
    encode_theta(double theta)
    {           
        // std::complex::arg gives theta in the range [-pi, pi]
        // I also want it to work with angles in the range [0, 2pi]
        if ( !(-M_PI <= theta && theta <= 2.0 * M_PI) )
            throw std::out_of_range("Input theta must be in the range [-pi, 2pi].");

        if (theta < 0.0)
            theta += 2.0 * M_PI; // theta in range [0, 2pi]

        theta /= 2.0 * M_PI; // theta in range [0, 1]

        if (theta > 1.0)
            theta = 1.0;

        return static_cast<uint8_t>(
            static_cast<uint16_t>(std::round(theta * static_cast<double>(NUM_REPRESENTABLE_THETA))) % NUM_REPRESENTABLE_THETA
        );
    }


    inline static double 
    decode_theta(uint8_t theta)
    {
        return 2.0 * M_PI * (static_cast<double>(theta) / static_cast<double>(NUM_REPRESENTABLE_THETA));
    }



public:
    uint8_t r: r_bits;
    uint8_t theta: theta_bits;

    static constexpr double EXP_BASE_R = std::numbers::sqrt2;
    static constexpr uint8_t MIN_VAL_R = 0; 
    static constexpr uint8_t MAX_VAL_R = (1 << r_bits) - 1; 
    static constexpr uint16_t NUM_REPRESENTABLE_R = (1 << r_bits); 
    

    static constexpr uint8_t MIN_VAL_THETA = 0; 
    static constexpr uint8_t MAX_VAL_THETA = (1 << theta_bits) - 1;
    static constexpr uint16_t NUM_REPRESENTABLE_THETA = (1 << theta_bits);

};


using OneBytePolar = SmallPolar<4, 4>;
using TwoBytePolar = SmallPolar<8, 8>;
