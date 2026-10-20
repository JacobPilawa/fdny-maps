#pragma once

#include "extinguisher_effects.h"

// HoseSpray's contact effect path in 00463EF0 has its own 0.01 residual-fire
// gate, separate from the extinguisher's 0.05 effect gate.
struct RescueHoseSprayEffects {
    using SpawnRequest = ExtinguisherEffects::SpawnRequest;

    // HoseSpray's initial size getter returns current template key 0's scalar.
    // The OpenGL preview treats that scalar as quad side length; this mapping
    // to geWorld_AddPolyOnce geometry remains provisional.
    static constexpr float initialParticleSizeScalar = 3.f;

    static bool afterContact(Random& random, uint32_t gameClockMs,
                             float remainingFireStrength,
                             uint32_t& nextEffectClockMs,
                             SpawnRequest& spawn) {
        // The retail branch compares the unsigned elapsed clock directly with
        // the unsigned item deadline. Preserve that comparison at DWORD wrap.
        if (remainingFireStrength < 0.01f || gameClockMs < nextEffectClockMs)
            return false;

        // 00463EF0 advances the shared effect deadline before allocating the
        // child, then consumes three more random values for its velocity.
        const Extended deadline = Extended::integer(gameClockMs) +
            ExtinguisherEffects::range(random, 10.f, 250.f);
        nextEffectClockMs = static_cast<uint32_t>(deadline.truncate());

        const float first = ExtinguisherEffects::range(random, -50.f, 50.f).asFloat();
        const float height = ExtinguisherEffects::range(random, 100.f, 400.f).asFloat();
        const float last = ExtinguisherEffects::range(random, -50.f, 50.f).asFloat();
        // 0042D280 receives the x87 outputs in {last,height,first} order.
        spawn.offset = {last, height, first};
        spawn.requested = true;
        return true;
    }
};
