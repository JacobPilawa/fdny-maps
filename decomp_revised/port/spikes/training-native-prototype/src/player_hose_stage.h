#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
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
    // Retail initializes the WindowFire timer to zero when this target is
    // selected, ramps it toward one at 2*dt, then subtracts dt/SprayTime on
    // contact. Frame ordering and the authored duration's wall-clock units
    // remain provisional in the host.
    float timer = 0.f;
    bool rampActive = false;
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

struct PlayerHoseRailSample {
    float distance = 0.f;
    V position{};
    V lookAt{};
};

struct PlayerHoseCameraPose {
    bool valid = false;
    V railPosition{};
    V lookAt{};
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
    std::vector<size_t> railNodeIndices;
    std::vector<PlayerHoseRailSample> railSamples;
    std::vector<float> railAnchorDistances;
    std::vector<size_t> railAnchorNodeIndices;
    float railLength = 0.f;
    float railProgress = 0.f;
    int cachedRailAnchorIndex = -1;
    std::string expectedHotspot;
    int activeHotspot = -1;
    int activeTarget = -1;
    int cyclesRemaining = 0;
    int hotspotOrdinal = 0;
    int modeIndex = 2;
    double modeSpotTimeMultiplier = 1.0;
    float countdown = 0.f;
    // The shared stream component moves its output scalar 20% toward the
    // current hotspot's M_MaxPressure while held, and 20% toward zero when
    // released. The exact rendered-water transfer remains unresolved.
    float streamPressureTarget = 0.f;
    float streamPressure = 0.f;
    bool timeWarningSent = false;
    Result result = Result::Running;
    std::string transition;

    static V lerp(V a, V b, float t) {
        return {a.x + (b.x - a.x) * t,
                a.y + (b.y - a.y) * t,
                a.z + (b.z - a.z) * t};
    }

    static V catmullRom(V p0, V p1, V m0, V m1, float t) {
        const float t2 = t * t;
        const float t3 = t2 * t;
        const float h00 = 2.f * t3 - 3.f * t2 + 1.f;
        const float h10 = t3 - 2.f * t2 + t;
        const float h01 = -2.f * t3 + 3.f * t2;
        const float h11 = t3 - t2;
        return {h00 * p0.x + h10 * m0.x + h01 * p1.x + h11 * m1.x,
                h00 * p0.y + h10 * m0.y + h01 * p1.y + h11 * m1.y,
                h00 * p0.z + h10 * m0.z + h01 * p1.z + h11 * m1.z};
    }

    void buildCameraRail() {
        railNodeIndices.clear();
        railSamples.clear();
        railAnchorDistances.clear();
        railAnchorNodeIndices.clear();
        railLength = 0.f;
        railProgress = 0.f;
        cachedRailAnchorIndex = -1;
        auto current = byName.find(firstPathPoint);
        for (int count = 0; current != byName.end() && count < 32; ++count) {
            const size_t index = current->second;
            if (std::find(railNodeIndices.begin(), railNodeIndices.end(), index) !=
                railNodeIndices.end()) break;
            railNodeIndices.push_back(index);
            std::string next = pathNodes[index].nextPath;
            if (next.empty() || next == "<null>") next = firstPathPoint;
            if (next == firstPathPoint) break;
            current = byName.find(next);
        }
        if (railNodeIndices.size() < 2) return;

        const size_t nodeCount = railNodeIndices.size();
        std::vector<V> points(nodeCount), lookAts(nodeCount);
        V previousLookAt{};
        for (size_t i = 0; i < nodeCount; ++i) {
            const auto& node = pathNodes[railNodeIndices[i]];
            // Retail passes authored X/Z and a zero third control coordinate
            // to the curve builder. Vertical camera placement comes from the
            // later world-floor trace.
            points[i] = {node.position.x, 0.f, node.position.z};
            auto target = byName.find(node.lookAtPath);
            if (target != byName.end())
                previousLookAt = pathNodes[target->second].position;
            lookAts[i] = previousLookAt;
        }

        struct SegmentRange { size_t first, last, fromNode, toNode; };
        std::vector<SegmentRange> segments;
        constexpr int subdivisions = 10;
        for (size_t segment = 0; segment < nodeCount; ++segment) {
            const size_t previous = (segment + nodeCount - 1) % nodeCount;
            const size_t next = (segment + 1) % nodeCount;
            const size_t afterNext = (segment + 2) % nodeCount;
            const V tangent0 = (points[next] - points[previous]) * .5f;
            const V tangent1 = (points[afterNext] - points[segment]) * .5f;
            const size_t first = railSamples.empty() ? 0 : railSamples.size() - 1;
            for (int step = railSamples.empty() ? 0 : 1; step <= subdivisions; ++step) {
                const float t = float(step) / float(subdivisions);
                PlayerHoseRailSample sample;
                sample.position = catmullRom(points[segment], points[next], tangent0, tangent1, t);
                railSamples.push_back(sample);
            }
            segments.push_back({first, railSamples.size() - 1, segment, next});
        }

        for (size_t i = 1; i < railSamples.size(); ++i) {
            const V delta = railSamples[i].position - railSamples[i - 1].position;
            railLength += std::sqrt(delta.x * delta.x + delta.z * delta.z);
            railSamples[i].distance = railLength;
        }
        if (railLength <= 1e-5f) {
            railSamples.clear();
            railLength = 0.f;
            return;
        }
        for (const auto& segment : segments) {
            railAnchorDistances.push_back(railSamples[segment.first].distance);
            railAnchorNodeIndices.push_back(railNodeIndices[segment.fromNode]);
        }
        for (const auto& segment : segments) {
            const float firstDistance = railSamples[segment.first].distance;
            const float segmentDistance = railSamples[segment.last].distance - firstDistance;
            for (size_t i = segment.first; i <= segment.last; ++i) {
                const float ratio = segmentDistance > 1e-5f
                    ? (railSamples[i].distance - firstDistance) / segmentDistance : 0.f;
                railSamples[i].lookAt = lerp(
                    lookAts[segment.fromNode], lookAts[segment.toNode], ratio);
            }
        }
    }

    bool hasCameraRail() const { return railSamples.size() > 1 && railLength > 0.f; }

    void advanceCameraRail(float dt) {
        if (!hasCameraRail() || result != Result::Running) return;
        // The retail code multiplies its normalized progress by +500 and a
        // frame-delta global. Interpreting that global as host seconds remains
        // provisional until its retail calibration is captured.
        railProgress += std::max(0.f, dt) * 500.f / railLength;
        railProgress -= std::floor(railProgress);
        processRailAnchor();
    }

    void processRailAnchor() {
        if (railAnchorDistances.empty() || railAnchorDistances.size() != railAnchorNodeIndices.size())
            return;
        const float distance = railProgress * railLength;
        const auto upper = std::upper_bound(railAnchorDistances.begin(), railAnchorDistances.end(), distance);
        const int selected = upper == railAnchorDistances.begin()
            ? 0 : int(std::distance(railAnchorDistances.begin(), upper) - 1);
        if (selected == cachedRailAnchorIndex) return;
        cachedRailAnchorIndex = selected;

        const size_t nodeIndex = railAnchorNodeIndices[size_t(selected)];
        if (nodeIndex >= pathNodes.size()) return;
        auto& node = pathNodes[nodeIndex];
        const bool expectedNext = !expectedHotspot.empty() && node.name == expectedHotspot;
        if (!node.isHotspot || (!node.isFirstHotspot && !expectedNext)) return;
        if (!enterHotspot(nodeIndex)) {
            result = Result::Lost;
            transition = "Hose stage stopped: invalid hotspot anchor " + node.name;
        }
    }

    PlayerHoseCameraPose cameraPose() const {
        PlayerHoseCameraPose pose;
        if (!hasCameraRail()) return pose;
        const float distance = std::clamp(railProgress, 0.f, 1.f) * railLength;
        auto upper = std::upper_bound(railSamples.begin(), railSamples.end(), distance,
            [](float value, const PlayerHoseRailSample& sample) {
                return value < sample.distance;
            });
        if (upper == railSamples.begin()) upper = railSamples.begin() + 1;
        if (upper == railSamples.end()) {
            pose.valid = true;
            pose.railPosition = railSamples.back().position;
            pose.lookAt = railSamples.back().lookAt;
            return pose;
        }
        const auto& b = *upper;
        const auto& a = *(upper - 1);
        const float span = b.distance - a.distance;
        const float t = span > 1e-5f ? std::clamp((distance - a.distance) / span, 0.f, 1.f) : 0.f;
        pose.valid = true;
        pose.railPosition = lerp(a.position, b.position, t);
        pose.lookAt = lerp(a.lookAt, b.lookAt, t);
        return pose;
    }

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

    static void beginTarget(PlayerHoseTarget& target) {
        target.timer = 0.f;
        target.rampActive = true;
        target.complete = false;
    }

    void updateStreamPressure(bool waterHeld) {
        const float target = waterHeld ? streamPressureTarget : 0.f;
        streamPressure += (target - streamPressure) * 0.2f;
    }

    float streamContactRadius() const {
        // 0046C5A0 interpolates the shared component's +0x18 padding between
        // constructor values +0x38C (10) and +0x3AC (40) using current +0x3B0.
        return 40.f * streamPressure + (1.f - streamPressure) * 10.f;
    }

    bool enterHotspot(size_t index) {
        if (index >= pathNodes.size()) return false;
        auto& next = pathNodes[index];
        if (!next.isHotspot || next.targets.empty()) return false;
        activeHotspot = int(index);
        activeTarget = 0;
        cyclesRemaining = std::max(0, next.numCycles);
        streamPressureTarget = next.maxPressure;
        // Retail multiplies the authored float by a mode-selected double,
        // then stores the result back to float before entering the timer state.
        countdown = static_cast<float>(double(next.spotTime) * modeSpotTimeMultiplier);
        timeWarningSent = false;
        ++hotspotOrdinal;
        for (auto& target : next.targets) {
            target.timer = 0.f;
            target.rampActive = false;
            target.complete = false;
        }
        beginTarget(next.targets[size_t(activeTarget)]);
        transition = "Hotspot " + std::to_string(hotspotOrdinal) + " started";
        expectedHotspot = next.nextHotspot;
        next.isFirstHotspot = false;
        return true;
    }

    bool start(int selectedModeIndex = 2) {
        transition.clear();
        if (!present || firstPathPoint.empty()) return false;
        buildCameraRail();
        if (!hasCameraRail()) {
            transition = "Hose stage stopped: no usable authored camera rail";
            return false;
        }
        modeIndex = selectedModeIndex;
        modeSpotTimeMultiplier = modeTimeMultiplier(modeIndex);
        activeHotspot = -1;
        activeTarget = -1;
        cyclesRemaining = 0;
        hotspotOrdinal = 0;
        countdown = 0.f;
        streamPressureTarget = 0.f;
        streamPressure = 0.f;
        expectedHotspot.clear();
        transition = "Waiting for first PlayerHose hotspot anchor";
        return true;
    }

    void update(float dt, bool waterHeld, bool targetInContactVolume) {
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
        if (activeTarget < 0 || size_t(activeTarget) >= currentHotspot->targets.size())
            return;
        auto& currentTarget = currentHotspot->targets[size_t(activeTarget)];
        if (currentTarget.complete) return;

        // 00468870 advances the selected WindowFire timer by 2*delta until it
        // reaches one, then stops the ramp. The contact message's separate
        // call into 00468680 subtracts delta/SprayTime afterward in this host
        // update; retail message-versus-entity scheduling remains unresolved.
        if (currentTarget.rampActive) {
            const float ramped = currentTarget.timer + elapsed * 2.f;
            if (ramped <= 1.f) {
                currentTarget.timer = ramped;
            } else {
                currentTarget.timer = 1.f;
                currentTarget.rampActive = false;
            }
        }

        if (!waterHeld || !targetInContactVolume) return;

        // 00468680 subtracts delta/SprayTime and completes only below the
        // source's zero threshold. Preserve the authored zero/negative divisor
        // arithmetic rather than replacing it with a separate host rule.
        currentTarget.timer -= elapsed / currentTarget.sprayTime;
        if (currentTarget.timer < 0.f) {
            currentTarget.timer = 0.f;
            currentTarget.complete = true;
            transition = "Doused " + currentTarget.modelName;
            advanceTarget();
        }
    }

    void advanceTarget() {
        if (activeHotspot < 0) return;
        auto& current = pathNodes[size_t(activeHotspot)];
        ++activeTarget;
        if (size_t(activeTarget) < current.targets.size()) {
            beginTarget(current.targets[size_t(activeTarget)]);
            return;
        }
        --cyclesRemaining;
        if (cyclesRemaining > 0) {
            activeTarget = 0;
            for (auto& target : current.targets) {
                target.timer = 0.f;
                target.rampActive = false;
                target.complete = false;
            }
            beginTarget(current.targets[size_t(activeTarget)]);
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
        if (next == byName.end() || !pathNodes[next->second].isHotspot ||
            pathNodes[next->second].targets.empty()) {
            result = Result::Lost;
            transition = "Hose stage stopped: unresolved next hotspot " + current.nextHotspot;
            return;
        }
        activeHotspot = -1;
        activeTarget = -1;
        transition = "Hotspot complete; waiting for anchor " + current.nextHotspot;
    }
};
