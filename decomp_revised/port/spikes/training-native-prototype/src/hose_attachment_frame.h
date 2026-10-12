#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <vector>
#include "player_ash_transform.h"

// Source-level output of the 0044C840 Hose Spray attachment-frame commit.
// Training prop frame construction and the owner's +0x18 getter stay inputs.
namespace HoseAttachmentFrame {

struct State {
    int hoseMode = 2;
    int valveRoot = -1;
    int firstStreamPoseNode = -1;
    std::array<float, 3> hoseFramePosition{}; // Hose Spray +0xC70..+0xC78
    std::array<float, 3> hoseValveOffset{};    // negated resolved frame axis 0
    std::array<float, 3> streamPosePosition{}; // first node +8/+0xC/+0x10
};

struct Input {
    int valveRoot = -1;
    std::vector<PlayerAshTransformNode> valveFrameNodes;
    std::size_t valveFrameRoot = 0;
    // Return value of valve-root vtable +0x18; its +0x28 triplet is copied
    // into Hose Spray +0xC70 by 00485A80.
    std::array<float, 3> valveGetterPosition{};
    int firstStreamPoseNode = -1;
};

struct Result {
    std::array<std::string, 2> boundaryEvents{};
};

inline Result commit(State& state, const Input& input) {
    Result result;
    const PlayerAshTransform world = resolvePlayerAshTransform(
        input.valveFrameNodes, input.valveFrameRoot);

    state.hoseFramePosition = input.valveGetterPosition;
    // 00446EE0 copies the first frame axis at +0x04..+0x0C; the following
    // 0042D2B0 call negates that vector. This is not the frame translation.
    for (std::size_t i = 0; i < 3; ++i)
        state.hoseValveOffset[i] = -world.axis[0][i];

    state.valveRoot = input.valveRoot;
    if (state.hoseMode != 3) {
        state.hoseMode = 3;
        // 00463370 changes 2 -> 3 and calls HUD mode 5 with a null parameter.
        result.boundaryEvents[0] = "hud:1524:5:0";
    }

    state.firstStreamPoseNode = input.firstStreamPoseNode;
    state.streamPosePosition = state.hoseFramePosition;
    // The owner hides Player+0x4C0 after the stream-pose node writes.
    result.boundaryEvents[1] = "hide:1216:0";
    return result;
}

} // namespace HoseAttachmentFrame
