#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>

// Source-comparable wrapper for Axe strike helper FUN_00492CE0. The Genesis
// geWorld_Collision implementation is deliberately an injected engine service.
namespace AxeWorldCollision {
using Vec3 = std::array<float, 3>;

struct State {
    uint32_t receiver{};       // __thiscall ECX, Axe/entity-like object
    uint32_t worldAtPlus8{};   // *(receiver + 0x8 + 0x78), inactive mode
    uint32_t worldAtPlusB4{};  // *(receiver + 0xb4 + 0x78), active mode
    uint8_t useB4{};           // *(uint8_t *)(receiver + 0x160)
};

struct Inputs {
    uint32_t param1Address{};  // start position, float[3]
    uint32_t param2Address{};  // mins, float[3]
    uint32_t param3Address{};  // maxs, float[3]
    uint32_t param4Address{};  // displacement, float[3]
    uint32_t param5Address{};  // hit position output, float[3]
    uint32_t param6Address{};  // hit normal/output vector, float[3]
    Vec3 param1{};
    Vec3 param2{};
    Vec3 param3{};
    Vec3 param4{};
};

struct EngineRequest {
    uint32_t world{};
    Vec3 mins{};
    Vec3 maxs{};
    Vec3 start{};
    Vec3 end{};
    uint32_t contentsMask{0x43};
    uint32_t collisionType{2};
    uint32_t flags0{};
    uint32_t flags1{};
    uint32_t flags2{};
};
struct EngineResult {
    Vec3 position{}; // trace record +0x0c
    Vec3 normal{};   // trace record +0x1c
};
struct Result {
    bool hit{};
    bool outputsWritten{};
    uint32_t positionOutputAddress{}; // param_5; written only on a hit
    uint32_t normalOutputAddress{};   // param_6; written only on a hit
    Vec3 position{};
    Vec3 normal{};
};
using Collision = std::function<bool(const EngineRequest&, EngineResult&)>;

inline Result invoke(const State& state, const Inputs& in, const Collision& collision) {
    Result out;
    const uint32_t world = state.useB4 ? state.worldAtPlusB4 : state.worldAtPlus8;
    if (world == 0) return out;

    EngineRequest request;
    request.world = world;
    request.mins = in.param2;
    request.maxs = in.param3;
    request.start = in.param1;
    // Original x87 FADD followed by FSTP stores to binary32 before dispatch.
    for (size_t i = 0; i != 3; ++i)
        request.end[i] = static_cast<float>(in.param1[i] + in.param4[i]);

    EngineResult trace;
    out.hit = collision(request, trace);
    if (out.hit) {
        out.outputsWritten = true;
        out.positionOutputAddress = in.param5Address;
        out.normalOutputAddress = in.param6Address;
        out.position = trace.position;
        out.normal = trace.normal;
    }
    return out;
}
} // namespace AxeWorldCollision
