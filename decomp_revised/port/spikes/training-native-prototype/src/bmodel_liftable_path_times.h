#pragma once

#include <cmath>

// Call-specific port of retail 0043BE00 -> 0043B120(time, endpoint, 2).
// Zero is the helper's sentinel and uses its strict +/-1e-28 window. For the
// one-second endpoint, mode 2 accepts a binary32 difference below 2^-21.
// The retail comparator has other special cases; this intentionally covers
// only the two endpoint comparisons used by the liftable initializer.
namespace BModelLiftablePathTimes {

inline bool matchesZero(float time) {
    const double value = double(time);
    return value > -1.0e-28 && value < 1.0e-28;
}

inline bool matchesOne(float time) {
    const float roundedDifference = time - 1.0f;
    return std::fabs(roundedDifference) < 0x1p-21f;
}

} // namespace BModelLiftablePathTimes
