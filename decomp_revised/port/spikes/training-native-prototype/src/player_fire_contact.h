#pragma once

#include <algorithm>
#include <cmath>

// Retail 004483A0 exposure candidate math, using the live shared FireVisual
// size as the two volume extents. Retail's fourth candidate value remains
// unresolved; the viewer currently supplies its 30-unit horizontal player
// collision radius as a provisional padding value. Emitter scale also remains
// explicit because its FireEmitter mapping is not yet verified.
struct PlayerFireContact {
    int maxAxisDistance = 0;
    float spatialFalloff = 0.f;
    float influence = 0.f;

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
        result.spatialFalloff = float(double(emitterScale) / radius *
            (radius - std::sqrt(weightedDistanceSquared)));

        const double intensityWeight = double(fireIntensity) +
            (1.0 - double(fireIntensity)) * .25;
        double influence = double(result.spatialFalloff) * intensityWeight;
        if (blocked) influence *= .1; // FireRescue's world-visibility failure scale.
        result.influence = float(influence);
        return result;
    }
};
