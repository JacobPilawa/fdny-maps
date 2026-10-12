#pragma once

#include <cstdint>
#include <cassert>
#include "victim_rescue.h"

// Training Player::escape (00451160) and its rescue-release helper (0045C000).
// This is an explicit transaction only; it does not imply a frame schedule.
using TrainingVictimReleaseState = VictimPickupPlayerState;

struct TrainingVictimReleaseActor : VictimPickupActorState {
    uint32_t rescueTimer = 0; // +0x1F8
    uint32_t rescueStart = 0; // +0x1F4
    uint32_t rescueEnd = 0;   // +0x1F0
    uint8_t rescueActive = 0; // +0x1FC
};

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
