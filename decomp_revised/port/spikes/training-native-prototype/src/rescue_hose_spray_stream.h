#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "player_hose_stage.h"

// The rescue PlayerRescue HoseSpray uses the same stream component as the
// dedicated PlayerHose stage, with 15 strip sections rather than 25. The
// nozzle frame and action input are supplied by the shared host and remain
// provisional until a retail attachment/input capture is available.
struct RescueHoseSprayStream {
    float pressure = 0.f;       // shared component +0x3B0
    float targetPressure = 1.f; // HoseSpray setup initializes +0x3B4 to 1
    float clockSeconds = 0.f;
    std::vector<PlayerHoseStreamSample> history;
    bool contactGate = false;   // shared component +0x1C
    V contactPoint{};           // shared component +0x0C..+0x14
    float contactPadding = 10.f; // shared component +0x18

    void clearContact() { contactGate = false; }

    void reset() {
        pressure = 0.f;
        clockSeconds = 0.f;
        history.clear();
        clearContact();
        contactPoint = {};
        contactPadding = 10.f;
    }

    void updatePressure(bool actionActive) {
        const float target = actionActive ? targetPressure : 0.f;
        pressure += (target - pressure) * 0.2f;
    }

    float collisionPadding() const {
        return 40.f * pressure + (1.f - pressure) * 10.f;
    }

    void appendSample(V source, V direction) {
        PlayerHoseStreamSample sample;
        sample.time = clockSeconds;
        sample.source = source;
        sample.velocity = direction * (4000.f * pressure);
        sample.shape = 0.8f + 2.2f * pressure;
        if (history.empty()) {
            history.assign(8, sample);
            return;
        }
        if (sample.time - history.back().time < 0.1f) return;
        history.push_back(sample);
        if (history.size() > 8) history.erase(history.begin());
    }

    PlayerHoseStreamSample sampleAt(float time) const {
        if (history.empty()) return {};
        if (time <= history.front().time) return history.front();
        for (size_t i = 1; i < history.size(); ++i) {
            const auto& before = history[i - 1];
            const auto& after = history[i];
            if (time > after.time) continue;
            const float span = after.time - before.time;
            if (span <= 1e-6f) return after;
            const float amount = std::clamp((time - before.time) / span, 0.f, 1.f);
            return {time,
                    PlayerHoseStage::lerp(before.source, after.source, amount),
                    PlayerHoseStage::lerp(before.velocity, after.velocity, amount),
                    before.shape + (after.shape - before.shape) * amount};
        }
        return history.back();
    }

    std::vector<PlayerHoseStreamPoint> update(float dt, V source, V direction) {
        clockSeconds += std::max(0.f, dt);
        appendSample(source, direction);
        constexpr int sections = 15; // HoseSpray setup at 00463700.
        constexpr float historySeconds = 0.75f;
        const V gravity{0.f, -2000.f, 0.f};
        const V secondaryVelocityOffset{0.f, -500.f, 0.f};
        std::vector<PlayerHoseStreamPoint> points;
        points.reserve(sections);
        for (int i = 0; i < sections; ++i) {
            const float u = float(i) / float(sections - 1);
            const float historyFraction = 0.5f * u + 0.5f * u * u;
            const float age = historyFraction * historySeconds;
            const PlayerHoseStreamSample sample = sampleAt(clockSeconds - age);
            const V position = sample.source + sample.velocity * age +
                gravity * (0.5f * age * age);
            const float secondaryAge = age * 0.6f;
            const V secondaryPosition = sample.source +
                (sample.velocity + secondaryVelocityOffset) * secondaryAge +
                gravity * (0.5f * secondaryAge * secondaryAge);
            points.push_back({position, secondaryPosition, sample.shape});
        }
        return points;
    }
};
