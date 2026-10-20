#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <map>
#include <string>
#include <vector>

// Dedicated PlayerHose/WindowFire stage state. This stays separate from
// PlayerRescue's HoseSpray item and FireRescue suppression transactions.
struct PlayerHoseTarget {
    int slot = -1;
    int model = -1;
    std::string modelName;
    float sprayTime = 0.f;
    // The retail WindowFire blends this value while burning and subtracts
    // deltaTime/SprayTime while receiving DouseHose contact. A unit starting
    // level is a host gameplay proxy; its exact retail initialization is open.
    float remaining = 1.f;
    bool complete = false;
};

struct PlayerHoseHotspot {
    int entity = -1;
    std::string name;
    std::string nextPath;
    std::string lookAtPath;
    std::string nextHotspot;
    V position{};
    bool isHotspot = false;
    bool isFirstHotspot = false;
    float maxPressure = 0.f;
    float pitchSpread = 45.f;
    float spotTime = 0.f;
    int numCycles = 0;
    std::array<std::string, 9> fireModelSlots{};
    std::array<float, 9> sprayTimeSlots{};
    std::vector<PlayerHoseTarget> targets;
};

struct PlayerHoseStage {
    enum class Result { Running, Won, Lost };

    static double modeTimeMultiplier(int modeIndex) {
        if (modeIndex == 0) return 1.8;
        if (modeIndex == 1) return 1.3;
        return 1.0;
    }

    bool present = false;
    int playerEntity = -1;
    V spawn{};
    float spawnYaw = 0.f;
    std::string firstPathPoint;
    std::vector<PlayerHoseHotspot> pathNodes;
    std::map<std::string, size_t> byName;
    int activeHotspot = -1;
    int activeTarget = -1;
    int cyclesRemaining = 0;
    int hotspotOrdinal = 0;
    int modeIndex = 2;
    double modeSpotTimeMultiplier = 1.0;
    float countdown = 0.f;
    bool timeWarningSent = false;
    Result result = Result::Running;
    std::string transition;

    const PlayerHoseHotspot* hotspot() const {
        if (activeHotspot < 0 || size_t(activeHotspot) >= pathNodes.size()) return nullptr;
        return &pathNodes[size_t(activeHotspot)];
    }

    const PlayerHoseTarget* target() const {
        const auto* current = hotspot();
        if (!current || activeTarget < 0 || size_t(activeTarget) >= current->targets.size())
            return nullptr;
        return &current->targets[size_t(activeTarget)];
    }

    bool enterHotspot(size_t index) {
        if (index >= pathNodes.size()) return false;
        auto& next = pathNodes[index];
        if (!next.isHotspot || next.targets.empty()) return false;
        activeHotspot = int(index);
        activeTarget = 0;
        cyclesRemaining = std::max(0, next.numCycles);
        // Retail multiplies the authored float by a mode-selected double,
        // then stores the result back to float before entering the timer state.
        countdown = static_cast<float>(double(next.spotTime) * modeSpotTimeMultiplier);
        timeWarningSent = false;
        ++hotspotOrdinal;
        for (auto& target : next.targets) {
            target.remaining = 1.f;
            target.complete = false;
        }
        transition = "Hotspot " + std::to_string(hotspotOrdinal) + " started";
        return true;
    }

    bool start(int selectedModeIndex = 2) {
        transition.clear();
        if (!present || firstPathPoint.empty()) return false;
        modeIndex = selectedModeIndex;
        modeSpotTimeMultiplier = modeTimeMultiplier(modeIndex);
        auto current = byName.find(firstPathPoint);
        std::vector<size_t> visited;
        while (current != byName.end() &&
               std::find(visited.begin(), visited.end(), current->second) == visited.end()) {
            const size_t index = current->second;
            visited.push_back(index);
            const auto& node = pathNodes[index];
            if (node.isHotspot) return enterHotspot(index);
            current = byName.find(node.nextPath);
        }
        transition = "No reachable PlayerHose hotspot from " + firstPathPoint;
        return false;
    }

    void update(float dt, bool waterHeld, int hitModel) {
        if (result != Result::Running || activeHotspot < 0) return;
        const float elapsed = std::max(0.f, dt);
        const float previousCountdown = countdown;
        countdown -= elapsed;
        if (!timeWarningSent && std::floor(previousCountdown) == 6.f &&
            std::floor(countdown) == 5.f) {
            timeWarningSent = true;
            transition = "Hotspot time warning";
        }
        // Retail state 4 checks timeout before updating input and WindowFire.
        if (countdown < 0.f) {
            result = Result::Lost;
            transition = "Hose stage lost: hotspot time expired";
            return;
        }

        auto* currentHotspot = &pathNodes[size_t(activeHotspot)];
        if (!waterHeld || activeTarget < 0 || size_t(activeTarget) >= currentHotspot->targets.size())
            return;
        auto& currentTarget = currentHotspot->targets[size_t(activeTarget)];
        if (currentTarget.complete || currentTarget.model != hitModel) return;

        // 00468680 subtracts the frame delta divided by the paired authored
        // SprayTime and clears WindowFire's selected byte after the value drops
        // below zero. A zero value is source-authored in city_h; avoid a host
        // divide-by-zero and treat it as a one-frame completion proxy.
        if (currentTarget.sprayTime <= 0.f) {
            if (elapsed <= 0.f) return;
            currentTarget.remaining = 0.f;
        } else {
            currentTarget.remaining -= elapsed / currentTarget.sprayTime;
        }
        if (currentTarget.remaining <= 0.f) {
            currentTarget.remaining = 0.f;
            currentTarget.complete = true;
            transition = "Doused " + currentTarget.modelName;
            advanceTarget();
        }
    }

    void advanceTarget() {
        if (activeHotspot < 0) return;
        auto& current = pathNodes[size_t(activeHotspot)];
        ++activeTarget;
        if (size_t(activeTarget) < current.targets.size()) return;
        --cyclesRemaining;
        if (cyclesRemaining > 0) {
            activeTarget = 0;
            for (auto& target : current.targets) {
                target.remaining = 1.f;
                target.complete = false;
            }
            transition = "WindowFire cycle " + std::to_string(current.numCycles - cyclesRemaining + 1) +
                         " of " + std::to_string(current.numCycles);
            return;
        }

        if (current.nextHotspot.empty()) {
            result = Result::Won;
            transition = "Hose stage won";
            return;
        }
        auto next = byName.find(current.nextHotspot);
        if (next == byName.end() || !enterHotspot(next->second)) {
            result = Result::Lost;
            transition = "Hose stage stopped: unresolved next hotspot " + current.nextHotspot;
            return;
        }
    }
};
