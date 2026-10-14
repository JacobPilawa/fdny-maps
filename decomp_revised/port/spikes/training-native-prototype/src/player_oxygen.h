#pragma once

#include <cstdint>

// Player oxygen fields used by 00451560 and 00458400. Host frame cadence and
// the renderer's oxygen gauge remain separate engine/UI boundaries.
struct PlayerOxygenState {
    float resource = 1.f;                  // Player +0x3450
    uint32_t nextDrainAt = 0;              // Player +0x3454
    uint32_t drainEnabledAt = 0x7fffffffu; // Player +0x3458
    bool lowResource = false;              // Player +0x52c
    bool lowWarningPosted = false;         // status +0x2b34
};

enum class PlayerOxygenUpdateResult {
    unchanged,
    drained,
    refilled,
    depleted
};

inline void startPlayerOxygenDrain(PlayerOxygenState& state, uint32_t now) {
    state.drainEnabledAt = now + 700u; // 0044BD80
}

inline void stopPlayerOxygenDrain(PlayerOxygenState& state) {
    state.drainEnabledAt = 0x7fffffffu; // 0044BDB0
}

template<class Passout>
inline PlayerOxygenUpdateResult updatePlayerOxygen(
    PlayerOxygenState& state, uint32_t now, float frameDelta, Passout&& passout) {
    if (state.drainEnabledAt < now) {
        if (state.nextDrainAt <= now) {
            state.nextDrainAt = now + 3000u;
            state.resource -= 0.05f;
            if (state.resource < 0.f) {
                state.resource = 0.f;
                passout(); // 00450FB0(Player, 0)
                if (state.resource < 0.8f) state.lowResource = true;
                return PlayerOxygenUpdateResult::depleted;
            }
            if (state.resource < 0.8f) state.lowResource = true;
            return PlayerOxygenUpdateResult::drained;
        }
    } else {
        state.nextDrainAt = now;
        state.resource += frameDelta * 0.1f;
        if (state.resource >= 1.f) {
            // 004A11A0(Player+0x40c, null) remains an unresolved recovery effect.
            state.lowResource = false;
            state.resource = 1.f;
        }
        return PlayerOxygenUpdateResult::refilled;
    }
    return PlayerOxygenUpdateResult::unchanged;
}

// 00458400 clears its one-shot warning gate at 0.5 and raises RC_OxygenLow
// once below 0.25. The status widget's visual response is not part of this
// state helper.
inline bool updatePlayerOxygenWarning(PlayerOxygenState& state) {
    if (state.resource >= 0.5f) {
        state.lowWarningPosted = false;
        return false;
    }
    if (state.resource < 0.25f && !state.lowWarningPosted) {
        state.lowWarningPosted = true;
        return true;
    }
    return false;
}
