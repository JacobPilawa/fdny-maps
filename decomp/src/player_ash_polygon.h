#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include "player_ash_presentation.h"

// Genesis vertex payload copied by geWorld_AddPolyOnce: XYZ, UV, then RGBA.
// The UV constants are written by the original 0047F850 constructor.
struct PlayerAshPolygonVertex {
    std::array<float,3> position{};
    std::array<float,2> uv{};
    std::array<float,4> rgba{};
};

struct PlayerAshPolygonPayload {
    std::array<PlayerAshPolygonVertex,4> vertices{};
};

inline PlayerAshPolygonPayload playerAshTrainingPolygonPayload(
    const PlayerAshParticle& particle,
    uint32_t presentationState,
    const AshPresentationInputs& inputs,
    const std::array<AshPresentationFrame,PlayerAshPool::descriptorWords.size()>& frames) {
    const auto rgb=playerAshPresentationColor(inputs,frames);
    const auto bytes=playerAshPresentationByteChannels(inputs,frames);
    const float uv[4][2]={{0.0f,0.0f},{0.99f,0.0f},{0.99f,0.99f},{0.0f,0.99f}};
    PlayerAshPolygonPayload payload{};
    for(size_t i=0;i<payload.vertices.size();i++) {
        auto& vertex=payload.vertices[i];
        // State 0 in 0047F960 copies the particle's base position into vertex
        // zero; all other cached positions come from the geometry buffer.
        vertex.position=(presentationState==0&&i==0)
            ? particle.spawnPosition : particle.geometryPositions[i];
        vertex.uv={uv[i][0],uv[i][1]};
        // 0047F960 multiplies the helper's binary32 RGB outputs by 255 in x87
        // precision, then spills each result to the vertex's RGBA words.
        for(size_t channel=0;channel<3;channel++)
            vertex.rgba[channel]=(Extended::from(rgb[channel])*Extended::from(255.0f)).asFloat();
        // Training constructor clears +0xD4, so the first D6D0 byte feeds A.
        vertex.rgba[3]=float(bytes[0]);
    }
    return payload;
}

inline std::array<uint32_t,36> playerAshPolygonWords(const PlayerAshPolygonPayload& payload) {
    std::array<uint32_t,36> words{};
    size_t out=0;
    for(const auto& vertex:payload.vertices) {
        auto append=[&](float value) { std::memcpy(&words[out++],&value,sizeof(value)); };
        for(float value:vertex.position) append(value);
        for(float value:vertex.uv) append(value);
        for(float value:vertex.rgba) append(value);
    }
    return words;
}
