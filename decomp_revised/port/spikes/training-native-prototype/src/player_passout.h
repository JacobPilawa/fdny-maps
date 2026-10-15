#pragma once

#include <cstdint>

struct PlayerPassoutState {
    uint32_t failedAt = 0; // Player +0x342C
    int32_t state = 0;     // Player +0x18C
};

// Player state-6 updater 00451080 enters 0044FF10 only after unsigned
// elapsed time is strictly greater than 1000 ms. In particular, 1000 is
// still the last interpolation frame; recovery is due at 1001 ms.
inline bool playerPassoutRecoveryDue(const PlayerPassoutState& player, uint32_t now) {
    return player.state == 6 && uint32_t(now - player.failedAt) > 1000u;
}

// 00451080 calls 0044F340 with this fraction through elapsed=1000, then
// delegates recovery once the elapsed time becomes 1001 ms.
inline float playerPassoutViewFactor(const PlayerPassoutState& player, uint32_t now) {
    return 1.f - float(uint32_t(now - player.failedAt)) * .001f;
}

enum class PlayerPassoutResult { enteredFailureState, delegatedToHeroDown };

// Source-ordered core of 00450FB0. `requestPassoutSoundInstance` receives the
// configured Player sound-instance offset (+0x454 for parameter 0; +0x430 for
// parameter 1). Chatter, clock, selected-item lookup, and the selected item's
// vtable +0x1C ACTION-release callback remain host boundaries. Parameter 2
// takes the original early HeroDown path.
template<class Ops>
PlayerPassoutResult applyPlayerPassout(
    PlayerPassoutState& player, int parameter, Ops&& ops) {
    if (parameter == 0)
        ops.requestPassoutSoundInstance(0x454);
    else if (parameter == 1)
        ops.requestPassoutSoundInstance(0x430);

    ops.playChatterSet("RC_PassOut", true);
    if (parameter == 2) {
        ops.runHeroDown(1); // 0044FF10(this, 1); source returns immediately.
        return PlayerPassoutResult::delegatedToHeroDown;
    }

    const uint32_t now = ops.readClock();
    const uint32_t selectedItemIndex = ops.readSelectedItemIndex();
    player.failedAt = now;
    player.state = 6;
    if (selectedItemIndex != 0)
        ops.releaseSelectedItem(selectedItemIndex);
    return PlayerPassoutResult::enteredFailureState;
}
