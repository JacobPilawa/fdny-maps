#pragma once

#include <algorithm>
#include <cmath>

// Retail 004483A0 compares the extended falloff result against the binary64
// constant at 004E16B8 (0.0001) before storing the message influence as float.
inline constexpr double kPlayerFireExposureInfluenceCutoff = 0.0001;

// Retail 004483A0 exposure candidate math, using the live shared FireVisual
// size as the two volume extents. PlayerRescue's constructor initializes the
// fourth float after its returned XYZ buffer to zero (+0x74); the viewer passes
// that source-derived padding. Emitter scale remains explicit because its
// FireEmitter mapping is not yet verified.
struct PlayerFireContact {
    int maxAxisDistance = 0;
    float spatialFalloff = 0.f;
    float influence = 0.f;
    double preVisibilityInfluence = 0.0;

    static PlayerFireContact evaluate(V fireOrigin, float volumeWidth, float volumeDepth,
                                      float candidatePadding, float emitterScale,
                                      float fireIntensity, V candidate, bool blocked) {
        const double dx = double(candidate.x) - fireOrigin.x;
        const double dy = double(candidate.y) - fireOrigin.y;
        const double dz = double(candidate.z) - fireOrigin.z;
        const double axisDistance = std::max({std::abs(dx), std::abs(dy), std::abs(dz)});
        PlayerFireContact result;
        result.maxAxisDistance = int(std::trunc(axisDistance)); // 004B9400 conversion in 004483A0.
        if (result.maxAxisDistance > 500) return result;

        const double unpaddedRadius = (double(volumeWidth) + volumeDepth) * .5;
        if (unpaddedRadius < .01) return result;
        const double radius = unpaddedRadius + candidatePadding;
        const double weightedDistanceSquared = dx * dx + (2.0 * dy) * (2.0 * dy) + dz * dz;
        if (radius <= 0.0 || weightedDistanceSquared > radius * radius) return result;
        const double spatialFalloff = double(emitterScale) / radius *
            (radius - std::sqrt(weightedDistanceSquared));
        result.spatialFalloff = float(spatialFalloff);

        // Retail stores this weight to binary32 before multiplying it by the
        // x87 falloff result. Keep the cutoff value separate from the float
        // message value so the host does not round before its threshold check.
        const float intensityWeight = float(double(fireIntensity) +
            (1.0 - double(fireIntensity)) * .25);
        result.preVisibilityInfluence = spatialFalloff * double(intensityWeight);
        result.influence = float(result.preVisibilityInfluence);
        if (blocked) {
            // Retail reloads the stored float and attenuates that value.
            result.influence = float(double(result.influence) * .1);
        }
        return result;
    }
};
