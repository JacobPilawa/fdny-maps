#pragma once

#include <cmath>

// Shared movement math used by retail VictimRescue's 0045B220 state-7 update.
// Keep this after core.h, which defines V and its vector operators.
inline double victimRoamDistanceSquared(V delta) {
    return double(delta.x) * delta.x +
           double(delta.y) * delta.y +
           double(delta.z) * delta.z;
}

inline bool victimRoamStepReachesTarget(V delta, float stepLength) {
    const double step = double(stepLength);
    return stepLength > 0.f && victimRoamDistanceSquared(delta) < step * step;
}

inline V advanceVictimRoamPosition(V current, V target, float stepLength) {
    const V delta = target - current;
    if (victimRoamStepReachesTarget(delta, stepLength)) return target;

    const double distance = std::sqrt(victimRoamDistanceSquared(delta));
    if (distance == 0.0) return current;
    const V direction{
        float(double(delta.x) / distance),
        float(double(delta.y) / distance),
        float(double(delta.z) / distance),
    };
    return current + direction * stepLength;
}

// 00482AA0 uses pow(0.5, frameDelta / 0.3) as the old-heading weight.
// 0045B220 first adjusts the desired heading to the nearest equivalent angle.
inline float blendVictimRoamHeading(float oldHeading, float desiredHeading,
                                    float frameDelta) {
    float difference = desiredHeading - oldHeading;
    while (difference > 180.f) difference -= 360.f;
    while (difference < -180.f) difference += 360.f;
    const double adjustedDesired = double(oldHeading) + difference;
    const double oldWeight = std::pow(0.5, double(frameDelta) / double(0.3f));
    return float(double(oldHeading) * oldWeight +
                 adjustedDesired * (1.0 - oldWeight));
}
