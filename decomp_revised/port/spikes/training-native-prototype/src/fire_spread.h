#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <optional>

// Hash-pinned 00447A90 spread placement and shape generation. The caller owns
// proximity bookkeeping and defers actor creation until after its contact pass.
struct FireSpreadSpawn {
    V position{};
    float heading = 0.f;
    V peakSize{};
    uint32_t growAt = 0;
    uint32_t collapseAt = 0;
};

struct FireSpreadAttempt {
    enum class Result { skipped, rejectedByChance, missedWorld, spawned } result = Result::skipped;
    std::optional<FireSpreadSpawn> child;
};

struct FireSpread {
    using Sweep = std::function<bool(V start, V delta, V mins, V maxs, V& point)>;

    static int proximityDistance(V fireOrigin, V candidateOrigin) {
        const float largest = std::max({std::abs(candidateOrigin.x - fireOrigin.x),
                                        std::abs(candidateOrigin.y - fireOrigin.y),
                                        std::abs(candidateOrigin.z - fireOrigin.z)});
        return int(largest); // 004B9400 truncates the positive max-axis value.
    }

    static FireSpreadAttempt attempt(V currentSize, V fireOrigin, float intensity,
                                     uint32_t now, uint32_t& nextAttempt,
                                     int nearbyFireCount, Random& random,
                                     const Sweep& sweep) {
        if (currentSize.x < .01f || now < nextAttempt)
            return {};

        // Native comparison accepts U == intensity.
        if (random.unit() > intensity) {
            nextAttempt = now + 500;
            return {FireSpreadAttempt::Result::rejectedByChance, std::nullopt};
        }

        const float v = float(random.range(0.f, 1.f) * double(intensity));
        const float minimumHorizontal = std::min(currentSize.x, currentSize.z);
        const float radius = float(double(minimumHorizontal) * .5 * (1.0 + double(v)));
        const float angle = float(random.range(0.f, 6.2831855f));
        const V candidate{float(double(fireOrigin.x) + double(radius) * std::cos(double(angle))),
                          float(double(fireOrigin.y) + 20.0),
                          float(double(fireOrigin.z) + double(radius) * std::sin(double(angle)))};

        V surface{};
        if (!sweep(candidate, {0.f, -200.f, 0.f}, {-1.f, -1.f, -1.f},
                   {1.f, 1.f, 1.f}, surface) ||
            std::abs(surface.y - candidate.y) < .001f ||
            std::abs(surface.x - candidate.x) > .1f ||
            std::abs(surface.z - candidate.z) > .1f) {
            // Retail leaves the old deadline untouched on a geometry miss.
            return {FireSpreadAttempt::Result::missedWorld, std::nullopt};
        }

        // 00447A90: b=(10+90*(1-v))*U(.9,1.1), then independent X/Y factors.
        const float b = float((10.0 + 90.0 * (1.0 - double(v))) * random.range(.9f, 1.1f));
        const float x = float(double(b) * (.25 + .75 * double(v)) * random.range(.9f, 1.1f));
        const float y = float((10.0 + 30.0 * double(v)) * random.range(.8f, 1.2f));
        const uint32_t growDelay = uint32_t(random.range(750.f, 3000.f));
        const uint32_t tailDelay = uint32_t(random.range(1000.f, 6000.f));
        const uint32_t collapseDelay = growDelay + tailDelay;

        // Retail uses max(1, 12 / nearby-count) as the interval divisor. A
        // successful callback is already inside the <750 gate, so count is
        // normally positive; retain a bounded fallback for a host-list mismatch.
        const double intervalDivisor = nearbyFireCount > 0
            ? std::max(1.0, 12.0 / double(nearbyFireCount)) : 1.0;
        nextAttempt = uint32_t(std::trunc(double(now) + double(collapseDelay) / intervalDivisor));

        return {FireSpreadAttempt::Result::spawned,
                FireSpreadSpawn{candidate, angle,
                                {x, y, b}, now + growDelay, now + collapseDelay}};
    }
};
