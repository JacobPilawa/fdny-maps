#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <vector>

// Source-derived model of the Hose Spray frame's linked stream-pose list.
// The native component models the fields that the reviewed routines touch;
// it is not a raw 32-bit allocation mirror.
namespace HoseSprayPoseList {

constexpr int kDefaultNodeCount = 12;       // 00482850: 240 / 20
constexpr int kOriginalNodeBytes = 0x30;    // operator_new(0x30) in 00485D20

struct Node {
    int32_t next = -1;                       // target node +0x00
    int32_t previous = -1;                   // target node +0x04
    std::array<float, 3> position{};         // target node +0x08
    std::array<float, 3> vector14{};         // target node +0x14
    std::array<float, 3> vector20{};         // target node +0x20
    // Target +0x2C is the final dword in each 0x30-byte allocation. The
    // constructor does not initialize it, so this component deliberately
    // leaves it unmodeled and never supplies an invented default.
};

struct State {
    std::array<float, 3> posePosition{};       // pose object +0x00..+0x08
    std::array<float, 3> auxiliaryVector24{};  // pose object +0x24..+0x2C
    std::vector<Node> nodes;
    int32_t head = -1;                        // Hose +0xCB8 (host index)
    int32_t tail = -1;                        // Hose +0xCBC (host index)
    int32_t count = 0;                        // Hose +0xCC0
    int32_t current = -1;                     // Hose +0xCD0 (host index)
};

// 00485D20 frees and rebuilds the entire list when the requested count
// differs. Equal-count calls leave both node contents and the cache intact.
inline bool resize(State& state, int requestedCount) {
    if (requestedCount == state.count)
        return false;

    state.nodes.clear();
    state.head = state.tail = state.current = -1;
    state.count = 0;
    if (requestedCount <= 0)
        return true;

    state.nodes.reserve(static_cast<std::size_t>(requestedCount));
    for (int32_t i = 0; i < requestedCount; ++i) {
        Node node;
        node.position = state.posePosition;
        // 00485D20 writes zero to target dwords +0x14..+0x28. It does not
        // write +0x2C. The vector's default member initializers model those
        // six known zero words.
        node.previous = i == 0 ? -1 : i - 1;
        node.next = i + 1 == requestedCount ? -1 : i + 1;
        state.nodes.push_back(node);
    }

    state.head = 0;
    state.tail = requestedCount - 1;
    state.count = requestedCount;
    state.current = state.head;
    return true;
}

inline void construct(State& state,
                      const std::array<float, 3>& initialPosition = {0.0f, 0.0f, 0.0f}) {
    state = State{};
    state.posePosition = initialPosition;
    // 00482850 calls 00485AC0 with (0,-5000,0) at pose-object +0x24;
    // this is separate from the origin copied into each new list node.
    state.auxiliaryVector24 = {0.0f, -5000.0f, 0.0f};
    resize(state, kDefaultNodeCount);
}

// Training's touchhose 25 path supplies the scaled value 25 * 24 / 20 to
// 00482780; 004B9400 truncates it before 00485D20 receives the node count.
inline int trainingInitializerCount(int touchhoseArgument) {
    const float scaled = (static_cast<float>(touchhoseArgument) * 24.0f) / 20.0f;
    return static_cast<int>(std::trunc(scaled));
}

inline bool initializeTraining(State& state, int touchhoseArgument) {
    construct(state);
    return resize(state, trainingInitializerCount(touchhoseArgument));
}

// Attachment writes the valve getter's position into the Hose frame first;
// after saving the valve and changing mode, 0044C840 caches the head node and
// copies that same position to its +0x08 triplet.
inline void setPosePosition(State& state, const std::array<float, 3>& position) {
    state.posePosition = position;
}

inline void cacheAndWriteHeadPosition(State& state,
                                      const std::array<float, 3>& position) {
    if (state.head < 0 || state.head >= state.count)
        throw std::logic_error("Hose attachment requires an initialized stream-pose head");
    state.current = state.head;
    state.nodes[static_cast<std::size_t>(state.head)].position = position;
}

inline void attach(State& state, const std::array<float, 3>& position) {
    setPosePosition(state, position);
    cacheAndWriteHeadPosition(state, position);
}

} // namespace HoseSprayPoseList
