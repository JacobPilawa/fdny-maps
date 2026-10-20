#pragma once
#include "core.h"
#include "extended.h"
#include "random.h"
#include <cstdint>

// CameraRescue quake state from 0049C890/0049CD80. The retail class writes
// two smoothed offsets at +0x4c/+0x50; their connection to geCamera's rendered
// transform is not yet mapped, so the viewer applies them as a provisional
// camera-local translation when drawing the scene.
struct RescueQuake {
    uint32_t startTick = 0;
    uint32_t nextSampleTick = 0x540be3ffu;
    float strength = 0.f;
    float durationSeconds = 0.f;
    float offsetX = 0.f;
    float offsetY = 0.f;
    float targetX = 0.f;
    float targetY = 0.f;
    bool active = false;

    static bool less(Extended left, Extended right) {
        const Extended delta = left - right;
        return delta.m != 0 && delta.negative;
    }

    static float lerpSource(float current, float target, Extended fraction) {
        return ((Extended::from(target) - Extended::from(current)) * fraction +
                Extended::from(current)).asFloat();
    }

    static Extended randomRange(float low, float high, Random& random) {
        const Extended unit = Extended::from(random.unit());
        return (Extended::from(high) - Extended::from(low)) * unit +
               Extended::from(low);
    }

    void trigger(uint32_t now, float nextStrength, float nextDurationSeconds) {
        // 0049C890 replaces the active event's clock/parameters and makes the
        // next random sample immediately due. Existing offsets/targets remain.
        startTick = now;
        strength = nextStrength;
        durationSeconds = nextDurationSeconds;
        nextSampleTick = 0;
        active = true;
    }

    V update(uint32_t now, float deltaSeconds, Random& random) {
        if (!active) return {};

        // Retail stores arg2 * 1000 as a binary32 duration, then converts its
        // unsigned millisecond clock to x87 extended precision for comparison.
        const float durationMs = (Extended::from(durationSeconds) *
                                  Extended::from(1000.f)).asFloat();
        const Extended nowValue = Extended::integer(now);
        const Extended endValue = Extended::integer(startTick) +
                                  Extended::from(durationMs);
        // The source comparison is inclusive at the deadline: expire only
        // when now is strictly greater than start + duration.
        if (startTick > now || less(endValue, nowValue)) {
            active = false;
            offsetX = 0.f;
            offsetY = 0.f;
            targetX = 0.f;
            targetY = 0.f;
            nextSampleTick = 0x540be3ffu;
            return {};
        }

        Extended blend = Extended::from(deltaSeconds) * Extended::from(20.0);
        if (less(Extended::from(1.f), blend)) blend = Extended::from(1.f);
        offsetX = lerpSource(offsetX, targetX, blend);
        offsetY = lerpSource(offsetY, targetY, blend);

        // 0049CD80 updates the current offset first, then chooses a new target
        // at 50 ms intervals. The first command update draws X, then Y.
        if (now >= nextSampleTick) {
            nextSampleTick = now + 50u;
            const uint32_t elapsedMs = now - startTick;
            Extended currentStrength = Extended::from(strength);
            const Extended quarter = Extended::from(0.25);
            const Extended half = Extended::from(0.5);
            // A zero-duration event can reach this sample when its start and
            // update share a tick. Retail's float division yields NaN there,
            // making both envelope comparisons false and leaving strength as
            // supplied for that one sample.
            if (durationMs != 0.f) {
                const Extended progress = Extended::integer(elapsedMs) /
                                          Extended::from(durationMs);
                if (less(progress, quarter)) {
                    currentStrength = half * progress * currentStrength;
                } else if (less(half, progress)) {
                    currentStrength = (Extended::from(1.0) -
                                       (progress - half) * Extended::from(2.0)) * currentStrength;
                }
            }

            const float sampleStrength = currentStrength.asFloat();
            targetX = randomRange(-sampleStrength, sampleStrength, random).asFloat();
            targetY = (randomRange(-sampleStrength, sampleStrength, random) *
                       Extended::from(0.5)).asFloat();
        }

        return {offsetX, offsetY, 0.f};
    }
};
