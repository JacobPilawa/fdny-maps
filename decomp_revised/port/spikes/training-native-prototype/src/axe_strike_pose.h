#pragma once

#include <array>
#include <cstdint>

// Pose/sweep prefix of Axe strike FUN_00460B90. Frame inputs are copied from
// the exact Player-frame offsets used by the original helpers; they are named
// by offset because their semantic axis labels have not been established.
// World overlap, candidate traversal, and post-hit engine effects are separate
// modules.
namespace AxeStrikePose {
using Vec3 = std::array<float, 3>;

struct Inputs {
    Vec3 playerFrame04{};       // 00446EE0(frame, out) copies frame +0x04
    Vec3 playerFrame10{};       // 00447830(frame, out) copies frame +0x10
    Vec3 playerFrame1c{};       // 0049E770(frame, out) copies frame +0x1c
    Vec3 playerAt34cc{};        // three float words at Player +0x34CC
    int8_t axeFlag1fc{};        // Axe +0x1FC; zero selects -1, nonzero +1
};

struct Query {
    Vec3 start{};               // 00492CE0 param_1
    Vec3 mins{};                // 00492CE0 param_2
    Vec3 maxs{};                // 00492CE0 param_3
    Vec3 displacement{};        // 00492CE0 param_4
    Vec3 hitPositionOutput{};   // 00492CE0 param_5, initialized to zero
    // 00492CE0 param_6 points to this original 12-float local array. Its
    // first vector is the output vector; the remaining words are input scratch.
    std::array<float, 12> scratch{};
};

inline float storeFloat(float value) {
    volatile float stored = value;
    return stored;
}

inline Vec3 scale00446390(const Vec3& value, float scale) {
    return {storeFloat(value[0] * scale), storeFloat(value[1] * scale),
            storeFloat(value[2] * scale)};
}

inline Vec3 scale00447710(float scale, const Vec3& value) {
    return {storeFloat(value[0] * scale), storeFloat(value[1] * scale),
            storeFloat(value[2] * scale)};
}

inline Vec3 add0042D050(const Vec3& first, const Vec3& second) {
    return {storeFloat(first[0] + second[0]), storeFloat(first[1] + second[1]),
            storeFloat(first[2] + second[2])};
}

inline Vec3 subtract0043B2D0(const Vec3& first, const Vec3& second) {
    return {storeFloat(first[0] - second[0]), storeFloat(first[1] - second[1]),
            storeFloat(first[2] - second[2])};
}

inline Query build(const Inputs& inputs) {
    // 00447830 -> 00446390(20) -> 0042D050(Player+0x34CC, scaled frame+0x10).
    const Vec3 frame10 = inputs.playerFrame10;
    const Vec3 forward20 = scale00446390(frame10, 20.0f);
    Vec3 start = add0042D050(inputs.playerAt34cc, forward20);

    // The source stores sign*axis to a distinct vector before scaling that
    // rounded vector by 10. Keep the two helper calls and float stores separate.
    const float sideSign = inputs.axeFlag1fc != 0 ? 1.0f : -1.0f;
    const Vec3 signedSide = scale00447710(sideSign, inputs.playerFrame04);
    const Vec3 side10 = scale00446390(signedSide, 10.0f);
    start = add0042D050(start, side10);

    // The swept displacement is (frame+0x1C)*100 - (frame+0x10)*60.
    const Vec3 forward60 = scale00446390(frame10, 60.0f);
    const Vec3 frame1c = inputs.playerFrame1c;
    const Vec3 reach100 = scale00446390(frame1c, 100.0f);
    const Vec3 displacement = subtract0043B2D0(reach100, forward60);

    Query query;
    query.start = start;
    query.mins = {-6.0f, -6.0f, -6.0f};
    query.maxs = {6.0f, 6.0f, 6.0f};
    query.displacement = displacement;
    // 00460B90 zeros local_f0 and local_cc[0..2], writes delta to both
    // local_cc[3..5] and local_cc[9..11], and puts extents at [6..8].
    query.scratch[3] = displacement[0];
    query.scratch[4] = displacement[1];
    query.scratch[5] = displacement[2];
    query.scratch[6] = 6.0f;
    query.scratch[7] = 6.0f;
    query.scratch[8] = 6.0f;
    query.scratch[9] = displacement[0];
    query.scratch[10] = displacement[1];
    query.scratch[11] = displacement[2];
    return query;
}
} // namespace AxeStrikePose
