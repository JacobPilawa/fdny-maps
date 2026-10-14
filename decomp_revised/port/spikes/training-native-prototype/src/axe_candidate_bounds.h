#pragma once

#include <array>
#include <cstddef>

// Source-comparable point/AABB overlap from retail helper FUN_004816C0.
namespace AxeCandidateBounds {
using Vec3 = std::array<float, 3>;

inline bool overlaps(const Vec3& mins, const Vec3& maxs, const Vec3& point,
                     float radius) {
    for (std::size_t axis = 0; axis != 3; ++axis) {
        if (point[axis] + radius < mins[axis] ||
            point[axis] - radius > maxs[axis])
            return false;
    }
    return true;
}
} // namespace AxeCandidateBounds
