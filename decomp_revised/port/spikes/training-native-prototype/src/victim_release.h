#pragma once

#include <cstdint>
#include <cassert>
#include <limits>
#include "victim_rescue.h"

// Training Player::escape (00451160), its release helper (0045C000), and the
// separate VictimRescue state-6 Think consumer (004599B0).
using TrainingVictimReleaseState = VictimPickupPlayerState;

struct TrainingVictimReleaseActor : VictimPickupActorState {
    uint32_t rescueTimer = 0; // +0x1F8
    uint32_t rescueStart = 0; // +0x1F4
    uint32_t rescueEnd = 0;   // +0x1F0
    uint8_t rescueActive = 0; // +0x1FC
};

struct VictimReleaseThinkStep {
    bool due = false;
    uint32_t worldActorFlags = 0;
};

// VictimRescue state 6's retail Think handler (004599B0). The host supplies
// one call per represented owner Think pass; this helper intentionally does
// not turn the per-call 5 ms counter decrement into elapsed-time arithmetic.
inline VictimReleaseThinkStep advanceVictimReleaseThink(
    TrainingVictimReleaseActor& victim, uint32_t now) {
    if (victim.state != 6 ||
        victim.rescueEnd == std::numeric_limits<uint32_t>::max() ||
        !(victim.rescueStart < now && victim.rescueEnd < now)) {
        return {};
    }

    // Retail also requests a stop for a non-null VictimRescue sound instance
    // at +0x154 on this first expired deadline. That instance is not retained
    // by the current host actor model, so audio remains an adapter gap.
    victim.rescueStart = 0;
    victim.rescueTimer -= 5u;
    if (victim.rescueTimer == 0) {
        victim.rescueActive = 0;
        victim.rescueEnd = std::numeric_limits<uint32_t>::max();
    } else {
        victim.rescueEnd = now + victim.rescueTimer;
        victim.rescueActive = victim.rescueActive == 0 ? 1 : 0;
    }
    return {true, victim.rescueActive != 0 ? 0x400u : 0u};
}

// Ops provide the native frame/math and neighboring Player effects. The call
// order below follows the source. Each frame callback maps to one call group
// in 0045C000; the two release-offset multiplies are intentionally separate.
// Required callbacks:
//   recordRescueStatistics(player), copyFirstPlayerFrameAxis(victim, player),
//   copySecondPlayerFrameAxis(victim, player), setVictimFrameUp(victim,0,1,0),
//   releaseOffsetTimes5(victim), releaseOffsetTimes24(victim),
//   setReleasePositionFromPlayerFrame(victim,player), placeOnFloor(victim),
//   setWorldActorFlags(victim,0x400), dispatchRescue(victim),
//   readClock(), resetActorSubobject(victim), updateVictPickPath(player),
//   playerRescuePostEffect(player), resetPlayerRescue(), checkExits(player).
// `releaseOffsetTimes5/24` receive the frame-derived vector as the preceding
// source call's output through the implementation's opaque native state.
template<class Ops>
bool trainingEscapeRelease(TrainingVictimReleaseState& player,
                           TrainingVictimReleaseActor& victim,
                           Ops&& ops) {
    if (!player.carriedVictimEntity) return false;
    // Native adapter invariant: resolve the carried ID before calling. This
    // assertion is not an extra source gameplay gate.
    assert(*player.carriedVictimEntity == victim.entity);

    ops.recordRescueStatistics(player);

    victim.state = 6;
    ops.copyFirstPlayerFrameAxis(victim, player);
    ops.copySecondPlayerFrameAxis(victim, player);
    ops.setVictimFrameUp(victim, 0.0f, 1.0f, 0.0f);
    ops.releaseOffsetTimes5(victim);
    ops.releaseOffsetTimes24(victim);
    ops.setReleasePositionFromPlayerFrame(victim, player);
    ops.placeOnFloor(victim);
    victim.worldActorFlags = 0x400;
    ops.setWorldActorFlags(victim, 0x400);
    ops.dispatchRescue(victim);
    victim.rescueTimer = 150;
    const uint32_t now = ops.readClock();
    victim.rescueStart = now + 2000u;
    victim.rescueEnd = now + 2150u;
    victim.rescueActive = 1;
    ops.resetActorSubobject(victim);

    ops.updateVictPickPath(player);
    ops.playerRescuePostEffect(player);
    player.carriedVictimEntity.reset();
    ops.resetPlayerRescue();
    ops.checkExits(player);
    return true;
}
