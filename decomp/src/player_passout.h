#pragma once

#include <cstdint>

struct PlayerPassoutState {
    uint32_t failedAt = 0; // Player +0x342C
    int32_t state = 0;     // Player +0x18C
};

enum class PlayerPassoutResult { enteredFailureState, delegatedToHeroDown };

// Source-ordered core of 00450FB0. `playPassoutMotion` receives the animator
// subobject offset (Player+0x454 for parameter 0; +0x430 for parameter 1).
// Chatter, clock, active-object lookup, and the object's vtable +0x1C callback
// remain host boundaries. Parameter 2 takes the original early HeroDown path.
template<class Ops>
PlayerPassoutResult applyPlayerPassout(
    PlayerPassoutState& player, int parameter, Ops&& ops) {
    if (parameter == 0)
        ops.playPassoutMotion(0x454);
    else if (parameter == 1)
        ops.playPassoutMotion(0x430);

    ops.playChatterSet("RC_PassOut", true);
    if (parameter == 2) {
        ops.runHeroDown(1); // 0044FF10(this, 1); source returns immediately.
        return PlayerPassoutResult::delegatedToHeroDown;
    }

    const uint32_t now = ops.readClock();
    const uint32_t activeObjectIndex = ops.readActiveObjectIndex();
    player.failedAt = now;
    player.state = 6;
    if (activeObjectIndex != 0)
        ops.invokeActiveObjectSlot1C(activeObjectIndex);
    return PlayerPassoutResult::enteredFailureState;
}
