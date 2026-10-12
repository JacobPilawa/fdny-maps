#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include "random.h"
#include "victim_rescue.h"

// State shared with pickup. passoutPosition is Player +0x3400..+0x3408;
// position is Player +0x4c..+0x54. The counter is Player +0x3428.
struct VictimPassoutPlayerState : VictimPickupPlayerState {
    uint32_t passoutCount = 0;
    std::array<float, 3> position{};
    std::array<float, 3> passoutPosition{};
};

struct VictimPassoutActorState : VictimPickupActorState {
    uint32_t motionCount = 0; // victim +0x180
};

enum class VictimPassoutResult { noCarriedVictim, dropped };

// Models the carried-victim transaction in 0044FF10 -> 0045C1A0. The
// independent mode-gated herodown spawn and later Player cleanup branches
// remain outside this component. Required Ops callbacks:
//   setMotion(victim, boundedDraw, speed), copyFirstPlayerFrameAxis,
//   copySecondPlayerFrameAxis, rebuildVictimFrameBasis, copyPlayerFramePosition,
//   placeOnFloor, setWorldActorFlags(victim, true), dispatchPassout,
//   resetPlayerRescue(), readPassoutCorrection(), correctDroppedFrameX(value).
// Frame and engine internals remain delegated. The two reads of the source
// correction value stay in caller order around distinct Player/frame X writes.
template<class Ops>
VictimPassoutResult applyVictimPassout(VictimPassoutPlayerState& player,
                                       VictimPassoutActorState& victim,
                                       Random& random, Ops&& ops) {
    ++player.passoutCount;
    player.position = player.passoutPosition;
    if (!player.carriedVictimEntity) return VictimPassoutResult::noCarriedVictim;
    // Integration invariant: caller resolves Player +0x3534 to this actor.
    assert(*player.carriedVictimEntity == victim.entity);

    victim.state = 4;
    const bool sixMotions = victim.motionCount == 6;
    const uint32_t motion = random.bounded(sixMotions ? 3u : victim.motionCount);
    ops.setMotion(victim, motion, sixMotions ? 1.0f : 0.2f);
    if (!sixMotions) victim.state = 4;

    ops.copyFirstPlayerFrameAxis(victim, player);
    ops.copySecondPlayerFrameAxis(victim, player);
    ops.rebuildVictimFrameBasis(victim);
    ops.copyPlayerFramePosition(victim, player);
    ops.placeOnFloor(victim);
    victim.worldActorFlags = 0x400; // 00481140(actor, true) replaces flags word.
    ops.setWorldActorFlags(victim, true);
    ops.dispatchPassout(victim);

    ops.resetPlayerRescue(); // 0044FCE0(0)
    player.position[0] += ops.readPassoutCorrection();
    ops.correctDroppedFrameX(ops.readPassoutCorrection());
    player.carriedVictimEntity.reset();
    return VictimPassoutResult::dropped;
}
