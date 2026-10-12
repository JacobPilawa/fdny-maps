#pragma once

#include <cstdint>

#include "player_fire_exposure.h"
#include "player_pain_deadline.h"
#include "player_passout.h"

struct PlayerFireMessageResult {
    bool ignoredFailedPlayer = false;
    bool painDeadlineUpdated = false;
    PlayerFireExposureResult exposure{};
};

// Source-ordered body of the Player 0x7531 handler at 00451640. `Ops` keeps
// engine-owned facing, clock/frame reads, chatter/stat work, and the
// 00450FB0 passout dispatch injectable. The fire producer and message cadence
// remain outside this transaction.
//
// Required Ops methods:
//   turnTowardFire()
//   readClock() -> uint32_t
//   triggerPain(Random&)
//   addSeriousBurns()
//   readFrameDelta() -> float
//   triggerPassout(PlayerPassoutState&, int)
template<class Ops>
inline PlayerFireMessageResult handlePlayerFireMessage7531(
    PlayerPassoutState& player, uint32_t& painDeadline, float& exposure,
    float influence, Random& random, Ops&& ops) {
    if (player.state == 6)
        return {true, false, {}}; // 0045165B..00451663

    ops.turnTowardFire(); // vector-facing helper and 0044BB00

    const uint32_t now = ops.readClock(); // [00502AFC], after facing
    const bool deadlineUpdated = updatePlayerPainDeadline(
        painDeadline, now, random,
        [&](Random& shared) { ops.triggerPain(shared); },
        [&] { ops.addSeriousBurns(); });

    const float frameDelta = ops.readFrameDelta(); // [00502B04], after pain/stat
    const auto accumulated = accumulatePlayerFireExposure(
        exposure, frameDelta, influence,
        [&] { ops.triggerPassout(player, 1); }); // 00450FB0(this, 1)

    return {false, deadlineUpdated, accumulated};
}
