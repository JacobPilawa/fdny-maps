#pragma once

#include <cstdint>

// Player oxygen fields used by 00451560 and 00458400. Host frame cadence and
// the renderer's warning artwork remain separate engine/UI boundaries.
struct PlayerOxygenState {
    float resource = 1.f;                  // Player +0x3450
    float displayedResource = 1.f;         // status +0x28b0, smoothed by 00458400
    uint32_t nextDrainAt = 0;              // Player +0x3454
    uint32_t drainEnabledAt = 0x7fffffffu; // Player +0x3458
    bool lowResource = false;              // Player +0x52c
    bool lowWarningPosted = false;         // status +0x2b34
};

// The Player status child at +0x2160 animates rescue.rrf resource 702 while
// oxygen is enabled or disabled (00455AB0 / 00455F60).
struct PlayerOxygenVisorEffect {
    uint32_t startAt = 0;
    uint32_t endAt = 0;
    int32_t startY = 0;
    int32_t endY = 0;
    bool active = false;
    bool lockOnCompletion = false;
};

inline void startPlayerOxygenVisorEffect(
    PlayerOxygenVisorEffect& effect, uint32_t now, bool oxygenOn) {
    effect.startAt = now - 10u; // 00455AB0
    effect.endAt = now + (oxygenOn ? 1000u : 500u); // arg * 1000 at 00455AB0
    effect.startY = oxygenOn ? -256 : 736;
    effect.endY = oxygenOn ? 736 : -256;
    effect.active = true;
    effect.lockOnCompletion = oxygenOn;
}

// Returns the source's integer-truncated top edge only inside the strict
// start/end interval used by 00455F60. The child rectangle is 640 by 256.
inline bool samplePlayerOxygenVisorEffect(
    const PlayerOxygenVisorEffect& effect, uint32_t now, int32_t& topY) {
    if (!effect.active || !(effect.startAt < now && now < effect.endAt)) return false;
    const uint32_t elapsed = now - effect.startAt;
    const uint32_t duration = effect.endAt - effect.startAt;
    const float progress = float(elapsed) / float(duration);
    topY = static_cast<int32_t>(float(effect.startY) +
        progress * float(effect.endY - effect.startY)); // 00455F60 -> 004B9400
    return true;
}

enum class PlayerOxygenVisorEffectCompletion {
    none,
    oxygenOn,
    oxygenOff
};

inline constexpr float playerOxygenVisorGroup2Gain = 0.3f; // 004DF7B0

// The retail update performs the end action once after the interval. Only the
// oxygen-on direction plays VisorLock; the off direction selects manager state 5.
inline PlayerOxygenVisorEffectCompletion completePlayerOxygenVisorEffect(
    PlayerOxygenVisorEffect& effect, uint32_t now) {
    if (!effect.active || now < effect.endAt) return PlayerOxygenVisorEffectCompletion::none;
    effect.active = false;
    const bool lock = effect.lockOnCompletion;
    effect.lockOnCompletion = false;
    return lock ? PlayerOxygenVisorEffectCompletion::oxygenOn
                : PlayerOxygenVisorEffectCompletion::oxygenOff;
}

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

template<class PlayBreathe, class PlayOxygenRelease, class Passout>
inline PlayerOxygenUpdateResult updatePlayerOxygen(
    PlayerOxygenState& state, uint32_t now, float frameDelta,
    PlayBreathe&& playBreathe, PlayOxygenRelease&& playOxygenRelease,
    Passout&& passout) {
    if (state.drainEnabledAt < now) {
        if (state.nextDrainAt <= now) {
            playBreathe(); // 00451560 requests Player+0x3c4 before changing oxygen.
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
            if (state.lowResource)
                playOxygenRelease(); // 004A11A0(Player+0x40c, null), configured as OxyRelease.
            state.lowResource = false;
            state.resource = 1.f;
        }
        return PlayerOxygenUpdateResult::refilled;
    }
    return PlayerOxygenUpdateResult::unchanged;
}

// 00458400 eases the HUD value 25% toward Player +0x3450 once per status
// update. The retail status updater is called in the Player frame path.
inline void updatePlayerOxygenDisplay(PlayerOxygenState& state) {
    state.displayedResource += (state.resource - state.displayedResource) * 0.25f;
}

// 00458400 clears its one-shot warning gate at 0.5 and raises RC_OxygenLow
// once below 0.25. The status widget's visual response is not part of this
// state helper.
inline bool updatePlayerOxygenWarning(PlayerOxygenState& state) {
    if (state.displayedResource >= 0.5f) {
        state.lowWarningPosted = false;
        return false;
    }
    if (state.displayedResource < 0.25f && !state.lowWarningPosted) {
        state.lowWarningPosted = true;
        return true;
    }
    return false;
}

// 00458400 enables the oxygen face's orange second layer below 0.25 and writes
// a controller scalar from 1.0 at 0.25 down to 0.1 at zero. The native
// renderer maps that range to a 0..0.1 alpha ramp as an approximation; retail
// blend interpretation is still unknown.
inline float playerOxygenWarningOverlayAmount(float displayedResource) {
    if (displayedResource >= 0.25f) return 0.f;
    return (0.25f - displayedResource) * 4.f * 0.1f;
}
