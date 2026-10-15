#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>

#if defined(__SIZEOF_INT128__) && !defined(FDNY_FORCE_PORTABLE_WIDE_INTEGER)
using WideUInt128 = __uint128_t;
#else
// Two 64-bit limbs provide the wrapping operations used by the recovered
// x87-significand and frame-clock calculations on compilers without __int128.
struct WideUInt128 {
    uint64_t high = 0;
    uint64_t low = 0;

    constexpr WideUInt128() = default;
    constexpr WideUInt128(uint64_t highWord, uint64_t lowWord)
        : high(highWord), low(lowWord) {}

    template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    constexpr WideUInt128(T value) : low(static_cast<uint64_t>(value)) {
        if constexpr (std::is_signed<T>::value) {
            if (value < 0) high = std::numeric_limits<uint64_t>::max();
        }
    }

    template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>,
              typename = void>
    explicit WideUInt128(T value) {
        if (!std::isfinite(value) || value <= T(0)) return;
        int exponent = 0;
        const T fraction = std::frexp(value, &exponent);
        constexpr int digits = std::numeric_limits<T>::digits;
        if (exponent > 128) throw std::overflow_error("128-bit conversion overflow");
        const T significand = std::ldexp(fraction, digits);
        uint64_t highWord = 0;
        uint64_t lowWord = 0;
        if (significand >= std::ldexp(T(1), 64)) {
            highWord = static_cast<uint64_t>(std::ldexp(significand, -64));
            lowWord = static_cast<uint64_t>(significand - std::ldexp(T(highWord), 64));
        } else {
            lowWord = static_cast<uint64_t>(significand);
        }
        *this = WideUInt128(highWord, lowWord);
        const int shift = exponent - digits;
        if (shift >= 0) *this <<= shift;
        else *this >>= -shift;
    }

    explicit constexpr operator bool() const { return high != 0 || low != 0; }
    explicit constexpr operator uint64_t() const { return low; }
    explicit operator double() const;
    explicit operator int64_t() const {
        if (low <= uint64_t(std::numeric_limits<int64_t>::max())) return int64_t(low);
        return -1 - int64_t(~low);
    }

    friend constexpr bool operator==(WideUInt128 a, WideUInt128 b) {
        return a.high == b.high && a.low == b.low;
    }
    friend constexpr bool operator!=(WideUInt128 a, WideUInt128 b) { return !(a == b); }
    friend constexpr bool operator<(WideUInt128 a, WideUInt128 b) {
        return a.high == b.high ? a.low < b.low : a.high < b.high;
    }
    friend constexpr bool operator>(WideUInt128 a, WideUInt128 b) { return b < a; }
    friend constexpr bool operator<=(WideUInt128 a, WideUInt128 b) { return !(b < a); }
    friend constexpr bool operator>=(WideUInt128 a, WideUInt128 b) { return !(a < b); }

    friend constexpr WideUInt128 operator+(WideUInt128 a, WideUInt128 b) {
        WideUInt128 result;
        result.low = a.low + b.low;
        result.high = a.high + b.high + (result.low < a.low ? 1u : 0u);
        return result;
    }
    friend constexpr WideUInt128 operator-(WideUInt128 a, WideUInt128 b) {
        WideUInt128 result;
        result.low = a.low - b.low;
        result.high = a.high - b.high - (a.low < b.low ? 1u : 0u);
        return result;
    }

    static constexpr std::pair<uint64_t, uint64_t> multiply64(uint64_t a, uint64_t b) {
        constexpr uint64_t mask = 0xffffffffULL;
        const uint64_t a0 = a & mask, a1 = a >> 32;
        const uint64_t b0 = b & mask, b1 = b >> 32;
        uint64_t t = a0 * b0;
        const uint64_t word0 = t & mask;
        uint64_t carry = t >> 32;
        t = a1 * b0 + carry;
        const uint64_t word1 = t & mask;
        const uint64_t word2 = t >> 32;
        t = a0 * b1 + word1;
        const uint64_t high = a1 * b1 + word2 + (t >> 32);
        const uint64_t low = (t << 32) | word0;
        return {high, low};
    }

    friend constexpr WideUInt128 operator*(WideUInt128 a, WideUInt128 b) {
        const auto lowProduct = multiply64(a.low, b.low);
        const auto highLowProduct = multiply64(a.high, b.low);
        const auto lowHighProduct = multiply64(a.low, b.high);
        return {lowProduct.first + highLowProduct.second + lowHighProduct.second,
                lowProduct.second};
    }

    friend constexpr WideUInt128 operator&(WideUInt128 a, WideUInt128 b) {
        return {a.high & b.high, a.low & b.low};
    }
    friend constexpr WideUInt128 operator|(WideUInt128 a, WideUInt128 b) {
        return {a.high | b.high, a.low | b.low};
    }

    friend constexpr WideUInt128 operator<<(WideUInt128 value, int shift) {
        if (shift <= 0) return value;
        if (shift >= 128) return {};
        if (shift >= 64) return {value.low << (shift - 64), 0};
        return {(value.high << shift) | (value.low >> (64 - shift)),
                value.low << shift};
    }
    friend constexpr WideUInt128 operator>>(WideUInt128 value, int shift) {
        if (shift <= 0) return value;
        if (shift >= 128) return {};
        if (shift >= 64) return {0, value.high >> (shift - 64)};
        return {value.high >> shift,
                (value.low >> shift) | (value.high << (64 - shift))};
    }

    static std::pair<WideUInt128, WideUInt128> divide(WideUInt128 numerator,
                                                       WideUInt128 denominator) {
        if (!denominator) throw std::domain_error("128-bit division by zero");
        WideUInt128 quotient, remainder;
        for (int bit = 127; bit >= 0; --bit) {
            const bool overflow = (remainder.high >> 63) != 0;
            remainder = remainder << 1;
            const uint64_t incoming = bit >= 64
                ? ((numerator.high >> (bit - 64)) & 1u)
                : ((numerator.low >> bit) & 1u);
            remainder.low |= incoming;
            if (overflow || remainder >= denominator) {
                remainder = remainder - denominator;
                if (bit >= 64) quotient.high |= uint64_t(1) << (bit - 64);
                else quotient.low |= uint64_t(1) << bit;
            }
        }
        return {quotient, remainder};
    }
    friend WideUInt128 operator/(WideUInt128 a, WideUInt128 b) {
        return divide(a, b).first;
    }
    friend WideUInt128 operator%(WideUInt128 a, WideUInt128 b) {
        return divide(a, b).second;
    }

    WideUInt128& operator>>=(int shift) { return *this = *this >> shift; }
    WideUInt128& operator<<=(int shift) { return *this = *this << shift; }
    WideUInt128& operator+=(WideUInt128 value) { return *this = *this + value; }
    WideUInt128& operator-=(WideUInt128 value) { return *this = *this - value; }
    WideUInt128& operator++() { return *this += WideUInt128(1); }
    WideUInt128 operator++(int) { WideUInt128 prior = *this; ++*this; return prior; }
};

inline WideUInt128 wideUIntFromWords(uint64_t high, uint64_t low) {
    return {high, low};
}
inline uint64_t wideUIntHigh(WideUInt128 value) { return value.high; }
inline uint64_t wideUIntLow(WideUInt128 value) { return value.low; }
#endif

#if defined(__SIZEOF_INT128__) && !defined(FDNY_FORCE_PORTABLE_WIDE_INTEGER)
inline WideUInt128 wideUIntFromWords(uint64_t high, uint64_t low) {
    return (WideUInt128(high) << 64) | WideUInt128(low);
}
inline uint64_t wideUIntHigh(WideUInt128 value) { return uint64_t(value >> 64); }
inline uint64_t wideUIntLow(WideUInt128 value) { return uint64_t(value); }
#endif

inline double wideUIntToDouble(WideUInt128 value) {
    if (!value) return 0.0;
    int bits = 0;
    for (WideUInt128 remaining = value; remaining; remaining >>= 1) ++bits;
    const int shift = std::max(0, bits - 53);
    WideUInt128 quotient = value >> shift;
    if (shift > 0) {
        const WideUInt128 remainder = value - (quotient << shift);
        const WideUInt128 halfway = WideUInt128(1) << (shift - 1);
        if (remainder > halfway ||
            (remainder == halfway && (quotient & WideUInt128(1)))) ++quotient;
    }
    return std::ldexp(double(static_cast<uint64_t>(quotient)), shift);
}

#if !defined(__SIZEOF_INT128__) || defined(FDNY_FORCE_PORTABLE_WIDE_INTEGER)
inline WideUInt128::operator double() const { return wideUIntToDouble(*this); }
#endif

class WideInt128 {
public:
    WideInt128() = default;

    template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    WideInt128(T value) {
        if constexpr (std::is_signed<T>::value) {
            if (value < 0) {
                negative_ = true;
                const int64_t signedValue = static_cast<int64_t>(value);
                magnitude_ = WideUInt128(uint64_t(-(signedValue + 1)) + 1);
            } else {
                magnitude_ = WideUInt128(static_cast<uint64_t>(value));
            }
        } else {
            magnitude_ = WideUInt128(static_cast<uint64_t>(value));
        }
    }

    explicit WideInt128(double value) {
        if (!std::isfinite(value)) throw std::domain_error("nonfinite 128-bit conversion");
        negative_ = std::signbit(value);
        double magnitude = std::fabs(value);
        if (magnitude == 0) {
            negative_ = false;
            return;
        }
        int exponent = 0;
        const double fraction = std::frexp(magnitude, &exponent);
        if (exponent > 128) throw std::overflow_error("128-bit conversion overflow");
        const uint64_t significand = static_cast<uint64_t>(std::ldexp(fraction, 53));
        const int shift = exponent - 53;
        magnitude_ = WideUInt128(significand);
        if (shift >= 0) magnitude_ <<= shift;
        else magnitude_ >>= -shift;
        normalize();
    }

    explicit WideInt128(WideUInt128 magnitude, bool negative = false)
        : magnitude_(magnitude), negative_(negative) { normalize(); }

    WideUInt128 magnitude() const { return magnitude_; }
    bool isNegative() const { return negative_; }

    explicit operator double() const {
        const double value = wideUIntToDouble(magnitude_);
        return negative_ ? -value : value;
    }
    explicit operator uint32_t() const {
        const uint32_t low = uint32_t(wideUIntLow(magnitude_));
        return negative_ ? uint32_t(0u - low) : low;
    }

    friend bool operator==(WideInt128 a, WideInt128 b) {
        return a.negative_ == b.negative_ && a.magnitude_ == b.magnitude_;
    }
    friend bool operator!=(WideInt128 a, WideInt128 b) { return !(a == b); }
    friend bool operator<(WideInt128 a, WideInt128 b) {
        if (a.negative_ != b.negative_) return a.negative_;
        return a.negative_ ? a.magnitude_ > b.magnitude_ : a.magnitude_ < b.magnitude_;
    }
    friend bool operator>(WideInt128 a, WideInt128 b) { return b < a; }
    friend bool operator<=(WideInt128 a, WideInt128 b) { return !(b < a); }
    friend bool operator>=(WideInt128 a, WideInt128 b) { return !(a < b); }

    friend WideInt128 operator-(WideInt128 value) {
        value.negative_ = !value.negative_;
        value.normalize();
        return value;
    }
    friend WideInt128 operator+(WideInt128 a, WideInt128 b) {
        if (a.negative_ == b.negative_)
            return WideInt128(a.magnitude_ + b.magnitude_, a.negative_);
        if (a.magnitude_ >= b.magnitude_)
            return WideInt128(a.magnitude_ - b.magnitude_, a.negative_);
        return WideInt128(b.magnitude_ - a.magnitude_, b.negative_);
    }
    friend WideInt128 operator-(WideInt128 a, WideInt128 b) { return a + (-b); }
    friend WideInt128 operator*(WideInt128 a, WideInt128 b) {
        return WideInt128(a.magnitude_ * b.magnitude_, a.negative_ != b.negative_);
    }
    friend WideInt128 operator/(WideInt128 a, WideInt128 b) {
        if (!b.magnitude_) throw std::domain_error("128-bit division by zero");
        return WideInt128(a.magnitude_ / b.magnitude_, a.negative_ != b.negative_);
    }
    friend WideInt128 operator<<(WideInt128 value, int shift) {
        return WideInt128(value.magnitude_ << shift, value.negative_);
    }

private:
    void normalize() { if (!magnitude_) negative_ = false; }
    WideUInt128 magnitude_{};
    bool negative_ = false;
};
