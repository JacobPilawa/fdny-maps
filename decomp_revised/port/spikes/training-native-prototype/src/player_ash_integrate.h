#pragma once

#include <array>
#include <cstdint>
#include "player_ash_pool.h"

// Isolated scalar/vector body of original 0047F2A0. The caller owns the
// timestep: the executable stores it at 00502b04, but its scheduler is outside
// this component. Each arithmetic result is rounded through binary32 storage,
// matching the source's x87 temporary vectors and component stores.
struct PlayerAshIntegrateState {
    std::array<float, 3> position{};       // particle +0x7c
    std::array<float, 3> velocity{};       // particle +0x88
    std::array<float, 3> acceleration{};   // particle +0x94
    std::array<float, 3> origin{};         // particle +0x5c
};

inline float playerAshRound(float x) { volatile float stored = x; return stored; }

inline void playerAshIntegrate0047F2A0(PlayerAshIntegrateState& s,
                                       float globalTimestep,
                                       double limit = 1.0e-5,
                                       float zeroSentinel = 0.0f) {
    std::array<float, 3> velocityStep{}, positionStep{}, accelerationStep{};
    for (int i = 0; i != 3; ++i) {
        velocityStep[i] = playerAshRound(s.velocity[i] * globalTimestep);
        s.position[i] = playerAshRound(s.position[i] + velocityStep[i]);
    }
    for (int i = 0; i != 3; ++i) {
        positionStep[i] = playerAshRound(s.position[i] * globalTimestep);
        accelerationStep[i] = playerAshRound(s.velocity[i] * globalTimestep);
    }
    std::array<float, 3> twiceAcceleration{};
    for (int i = 0; i != 3; ++i)
        twiceAcceleration[i] = playerAshRound(accelerationStep[i] * globalTimestep);
    // 0042d050 is a component-wise vector sum. This integrates the origin
    // with the updated position*dt plus velocity*dt^2.
    for (int i = 0; i != 3; ++i) {
        const float originStep = playerAshRound(positionStep[i] + twiceAcceleration[i]);
        s.origin[i] = playerAshRound(s.origin[i] + originStep);
    }

    for (int i = 0; i != 3; ++i) {
        float& p = s.position[i];
        const float displacement = playerAshRound(globalTimestep * s.acceleration[i]);
        if (p <= limit) {
            if (p < limit) {
                const double candidate = double(displacement) + double(p);
                if (candidate > zeroSentinel) p = 0.0f;
                else p = playerAshRound(float(candidate));
            }
        } else if (double(p) - double(displacement) < double(zeroSentinel)) {
            p = 0.0f;
        } else {
            p = playerAshRound(float(double(p) - double(displacement)));
        }
    }
}

// Map the isolated routine onto the recovered particle fields without
// choosing when the unresolved Player schedule calls it.
inline void playerAshIntegrateParticle(PlayerAshParticle& particle,
                                       float globalTimestep) {
    PlayerAshIntegrateState state{
        particle.vector7c, particle.vector88,
        particle.vector94, particle.spawnPosition
    };
    playerAshIntegrate0047F2A0(state, globalTimestep);
    particle.vector7c = state.position;
    particle.spawnPosition = state.origin;
}
