#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <stdexcept>
#include "random.h"

// Source-addressed state for the 100 Player ash children at Player+0x55c.
// This is not the complete 0x16c-byte engine object: calls outside the
// recovered scalar/vector path must be supplied as services below.
struct PlayerAshParticle {
    bool active = true;                    // +0x30, 0047f500
    bool animationEnabled = true;          // +0xad, 0047f500
    bool positionInitialized = true;       // +0x78, 0047f500
    bool vectorsDirty = false;             // +0xa0, 0047f500
    float activationScale = 1.0f;           // +0x1c, 0047d330
    float opacityScale = 1.0f;              // +0xa8, 0047f500
    uint32_t deadline = 0;                  // +0x168, 004463f0
    std::array<float,3> vector7c{};         // +0x7c, 0047f500 / 00446510
    std::array<float,3> vector88{};         // +0x88, 0047f500 / 00446510
    std::optional<std::array<float,3>> spawnPosition; // +0x5c is not set by 0047f500
};

struct PlayerAshDescriptor {
    uint32_t id;
    std::array<uint32_t,3> scalarBits;
    std::array<uint32_t,3> trailingBits;
};

struct PlayerAshPoolServices {
    // Keep opaque engine constructors explicit. These callbacks receive the
    // shared RNG in case later source recovery finds draws in their call trees.
    std::function<void(size_t container,uint32_t currentFlags,size_t record,
                       const PlayerAshDescriptor&,Random&)> addDescriptor;
    std::function<void(size_t particle,size_t descriptorContainer,
                       PlayerAshParticle&,Random&)> constructOpaqueParticle;
    // These original calls are outside the scalar/vector model. The density
    // scan can only be source-faithful when all three are supplied.
    std::function<std::array<float,3>(float angle,float distance,Random&)> spawnPoint;
    std::function<void(PlayerAshParticle&,Random&)> resetAnimation;
    std::function<void(PlayerAshParticle&,Random&)> present;
};

struct PlayerAshPool {
    static constexpr size_t capacity = 100;
    static constexpr uint32_t maximumLifetime = 99999999;
    static constexpr uint32_t descriptorFlagsDuringAdd = 0x35;
    static constexpr uint32_t descriptorFlagsAfterSetup = 0x3c;
    std::array<PlayerAshParticle,capacity> particles{};
    std::array<uint32_t,2> descriptorFlags{};
    float density = 1.0f; // child +0x54; 004465d0
    bool initialized = false;

    static constexpr std::array<std::array<uint32_t,6>,5> descriptorWords{{
        {{0x3f000000,0x3f800000,0x3e4ccccd,0x3f666666,0x3dcccccd,0}},
        {{0x3f000000,0x3f800000,0x3e4ccccd,0x3f666666,0x3f000000,0}},
        {{0x3f000000,0x3f800000,0x3e4ccccd,0x3f000000,0x3f000000,0x3f000000}},
        {{0x3f800000,0x3f800000,0x3e4ccccd,0,0,0}},
        {{0x3f800000,0,0x3e4ccccd,0,0,0}},
    }};

    static PlayerAshDescriptor descriptorFor(size_t container, size_t record) {
        const auto& words=descriptorWords.at(record);
        return {uint32_t(200+container),
            {words[0],words[1],words[2]}, {words[3],words[4],words[5]}};
    }

    static void requireInitializationServices(const PlayerAshPoolServices& s) {
        if (!s.addDescriptor || !s.constructOpaqueParticle)
            throw std::invalid_argument("ash pool initialization requires engine constructors");
    }

    void initialize(const PlayerAshPoolServices& s, Random& rng) {
        if (initialized) return;
        requireInitializationServices(s);
        for (size_t container=0;container<2;container++) {
            descriptorFlags[container]=descriptorFlagsDuringAdd;
            for (size_t record=0;record<descriptorWords.size();record++)
                s.addDescriptor(container,descriptorFlags[container],record,
                                descriptorFor(container,record),rng);
            descriptorFlags[container]=descriptorFlagsAfterSetup;
        }
        for (size_t i=0;i<particles.size();i++) {
            particles[i]=PlayerAshParticle{};
            s.constructOpaqueParticle(i,i&1,particles[i],rng);
        }
        density = 1.0f;
        initialized = true;
    }

    void setDensity(float value) {
        if (value > 1.0f) value = 1.0f;
        if (value < 0.0f) value = 0.0f;
        density = value;
    }

    // 00446510, including the calls through 0047f1b0/0047f1c0 and vector
    // stores in 0042d280. The odd component order is the original argument
    // order: first draw -> vector7c.z, second -> vector7c.y increment,
    // third -> vector7c.x, fourth -> vector88.y.
    static void update(PlayerAshParticle& p, uint32_t now, Random& rng) {
        if (!p.active || p.deadline > now) return;
        p.deadline = now + maximumLifetime; // original unsigned wrap
        float first = float(rng.range(-40.0f,40.0f));
        p.vectorsDirty = true; // 0047f1b0
        double incrementY = rng.range(0.0f,10.0f);
        float third = float(rng.range(-40.0f,40.0f));
        p.vectorsDirty = true; // second 0047f1b0
        p.vector7c = {third, float(incrementY + double(p.vector7c[1])), first};
        float fourth = float(rng.range(-30.0f,-5.0f));
        p.vectorsDirty = true; // 0047f1c0
        p.vector88 = {0.0f,fourth,0.0f};
    }

    // 004467e0 updates/presents every slot, then scans int(100*density).
    // Engine presentation and scene transforms are mandatory services so a
    // caller cannot silently replace unknown behavior with a no-op.
    void tick(uint32_t now, Random& rng, const PlayerAshPoolServices& s) {
        if (!initialized) throw std::logic_error("Player ash pool not initialized");
        if (!s.present || !s.spawnPoint || !s.resetAnimation)
            throw std::invalid_argument("ash pool tick requires scene and presentation services");
        for (auto& p : particles) {
            update(p,now,rng);
            s.present(p,rng); // 0047f960, after 00446510
        }
        int scan = static_cast<int>(double(capacity) * double(density));
        if (scan < 0) scan = 0;
        if (scan > int(capacity)) scan = int(capacity);
        for (int i=0;i<scan;i++) {
            auto& p=particles[size_t(i)];
            if (p.active) continue;
            float angle=float(rng.range(0.0f,6.2831855f));
            float distance=float(rng.range(10.0f,400.0f));
            p.spawnPosition=s.spawnPoint(angle,distance,rng);
            p.positionInitialized=true; // 0047f1a0
            s.resetAnimation(p,rng); // 0047d300
            p.activationScale=float(rng.range(0.5f,1.5f));
            p.active=true;
            p.opacityScale=float(rng.range(0.5f,1.5f));
            p.deadline=0;
            float velocityY=float(rng.range(-80.0f,-10.0f));
            p.vectorsDirty=true; // 0047f1b0
            p.vector7c={0.0f,velocityY,0.0f};
        }
    }
};
