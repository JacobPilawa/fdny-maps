#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

// 00462E80 rebuilds the goggles target list from actor kinds 6 and 2.
// Filter and squared range come from the pinned executable. The shared host
// stores live target positions; GogglesRenderer consumes them for the traced
// resource-703 effect window.
struct GogglesScanCandidate {
    int entity = -1;
    int kind = 0;
    uint32_t state = 0;
    V position{};
};

struct GogglesScanTarget {
    int entity = -1;
    int kind = 0;
    float distance = 0.f;
    V position{};
};

struct GogglesScan {
    static constexpr float maximumDistanceSquared = 4000000.f; // 004E1108
    static constexpr uint32_t effectWindowMs = 8500; // 00462730 scales the remaining 0x2134 ticks by 1/8500.

    std::vector<GogglesScanTarget> targets;
    uint32_t scannedAt = 0; // goggles +0x250
    bool effectsActive = false;

    void scan(V playerPosition, const std::vector<GogglesScanCandidate>& candidates,
              uint32_t now) {
        targets.clear();
        for (const auto& candidate : candidates) {
            if (candidate.kind != 6 && candidate.kind != 2) continue;
            if (candidate.kind == 2 && (candidate.state == 5 || candidate.state == 6))
                continue;
            const float dx = candidate.position.x - playerPosition.x;
            const float dy = candidate.position.y - playerPosition.y;
            const float dz = candidate.position.z - playerPosition.z;
            const float distanceSquared = dx * dx + dy * dy + dz * dz;
            if (maximumDistanceSquared < distanceSquared) continue;
            targets.push_back({candidate.entity, candidate.kind, std::sqrt(distanceSquared), candidate.position});
        }
        scannedAt = now;
        effectsActive = true;
    }

    void followTargets(const std::vector<GogglesScanCandidate>& candidates) {
        for (auto& target : targets) {
            auto current = std::find_if(candidates.begin(), candidates.end(), [&](const auto& candidate) {
                return candidate.entity == target.entity;
            });
            if (current != candidates.end()) target.position = current->position;
        }
    }

    bool effectWindowOpen(uint32_t now) const {
        return effectsActive && uint32_t(now - scannedAt) <= effectWindowMs;
    }

    void deactivate() { effectsActive = false; }
};
