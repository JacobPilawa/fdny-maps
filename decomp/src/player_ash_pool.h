#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <stdexcept>
#include <vector>
#include "extended.h"
#include "random.h"
#include "sprite_animation.h"

// Source-addressed state for the 100 Player ash children at Player+0x55c.
// This is not the complete 0x16c-byte engine object: calls outside the
// recovered scalar/vector path must be supplied as services below.
struct PlayerAshParticle {
    bool active = true;                    // +0x30, 0047f500
    bool animationEnabled = true;          // +0xad, 0047f500
    bool geometryDirty = true;             // +0x78, 0047f500 / 0047f5a0
    bool vectorsDirty = true;              // +0xa0, 004463f0 -> 0047f1d0
    uint32_t animationFlags = 0;           // +0x18, copied from selected descriptor container
    float activationScale = 1.0f;           // +0x1c, 0047d330
    float opacityScale = 1.0f;              // +0xa8, 0047f500
    uint32_t animationIndex = 0;            // +0x04, 0047d300
    uint32_t nextAnimationIndex = 1;        // +0x08, 0047d300
    uint32_t animationTimestamp = 0;        // +0x14, 0047d300
    uint32_t deadline = 0;                  // +0x168, 004463f0
    std::array<float,3> vector94{};          // +0x94, 004463f0 -> 0047f1d0
    std::array<float,3> vector7c{};         // +0x7c, 0047f500 / 00446510
    std::array<float,3> vector88{};         // +0x88, 0047f500 / 00446510
    std::array<float,3> vectorB4{1.0f,1.0f,1.0f}; // +0xb4, 0047f500
    std::array<float,3> spawnPosition{};    // +0x5c, identity transform from 0042d2d0
    // Position triplets at +0xd8,+0xfc,+0x120,+0x144; valid when !geometryDirty.
    std::array<std::array<float,3>,4> geometryPositions{};
};

struct PlayerAshDescriptor {
    uint32_t id;
    std::array<uint32_t,3> scalarBits;
    std::array<uint32_t,3> trailingBits;
};

// The Player primary-vtable +0x18 getter returns Player+0x24.  The emitter
// reads position at +0x28/+0x2c/+0x30 and direction at +0x1c..+0x24.
struct PlayerAshFrame {
    std::array<float,3> position{};
    std::array<float,3> direction{};
};

// World transform layout consumed by 00495D20: three basis columns, then
// translation and scale. The host resolves any parent chain before passing it.
struct PlayerAshTransform {
    std::array<std::array<float,3>,3> axis{{{1,0,0},{0,1,0},{0,0,1}}};
    std::array<float,3> position{};
    std::array<float,3> scale{1.0f,1.0f,1.0f};
};

struct PlayerAshPoolServices {
    // The pool seeds source-mapped fields before the opaque engine/resource
    // constructor remainder. Keep that remainder explicit, including possible
    // shared-RNG use until its call tree is closed.
    std::function<void(size_t container,uint32_t currentFlags,size_t record,
                       const PlayerAshDescriptor&,Random&)> addDescriptor;
    std::function<void(size_t particle,size_t descriptorContainer,
                       PlayerAshParticle&,Random&)> constructOpaqueParticle;
    // 004467E0 calls Player vtable slot +0x18 four times, reading X, Z, Y,
    // then the direction vector. Keep each dispatch explicit: the computed
    // target can have engine/RNG effects even though the recovered leaf
    // getter 0049D760 returns Player+0x24 without drawing.
    std::function<PlayerAshFrame(Random&)> getPlayerFrame;
    // `present` is the remainder of 0047F960 after this component's exact
    // 0047D660 animation step. The density scan cannot silently substitute
    // either unresolved engine service.
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

    void initialize(const PlayerAshPoolServices& s, uint32_t now, Random& rng) {
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
            particles[i].animationFlags=descriptorFlags[i&1];
            particles[i].animationIndex=0;
            particles[i].nextAnimationIndex=1;
            particles[i].animationTimestamp=now;
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

    // 00446470. Its 0047d300 reset selects entries 0 and 1 and stores the
    // current clock. Both indices are in range for the known five-entry
    // Training descriptor, so 0047d2b0 leaves them unchanged.
    static void activate(PlayerAshParticle& p, const std::array<float,3>& position,
                         uint32_t now, Random& rng) {
        p.spawnPosition=position;             // 0047f1a0 destination +0x5c
        p.geometryDirty=true;
        p.animationIndex=0;
        p.nextAnimationIndex=1;
        p.animationTimestamp=now;
        p.activationScale=float(rng.range(0.5f,1.5f));
        p.active=true;
        p.opacityScale=float(rng.range(0.5f,1.5f));
        p.deadline=0;
        float velocityY=float(rng.range(-80.0f,-10.0f));
        p.vectorsDirty=true;                  // 0047f1b0
        p.vector7c={0.0f,velocityY,0.0f};
    }

    // 004467E0 after its angle/distance draws. Inputs are rounded to float by
    // the caller, matching the original FSTP locals. Position + radial offset
    // is rounded once to float; the scaled direction is separately stored as
    // float by 00446390, then added and stored as float by the caller.
    static std::array<float,3> spawnPosition(float angle,float distance,
                                              Random& rng,
                                              const PlayerAshPoolServices& s) {
        if (!s.getPlayerFrame)
            throw std::invalid_argument("ash spawn requires Player frame service");
        using E=Extended;
        const PlayerAshFrame frameX=s.getPlayerFrame(rng);
        const E cosine=E::from(std::cos(double(angle)));
        const float xRadial=(cosine*E::from(distance)+E::from(frameX.position[0])).asFloat();

        const PlayerAshFrame frameZ=s.getPlayerFrame(rng);
        const E sine=E::from(std::sin(double(angle)));
        const float zRadial=(sine*E::from(distance)+E::from(frameZ.position[2])).asFloat();

        const PlayerAshFrame frameY=s.getPlayerFrame(rng);
        const float yRaised=(E::from(frameY.position[1])+E::from(150.0f)).asFloat();

        const PlayerAshFrame frameDirection=s.getPlayerFrame(rng);
        std::array<float,3> directionScaled{};
        for (size_t axis=0;axis<3;axis++)
            directionScaled[axis]=frameDirection.direction[axis]*75.0f;
        return {
            (E::from(xRadial)+E::from(directionScaled[0])).asFloat(),
            (E::from(yRaised)+E::from(directionScaled[1])).asFloat(),
            (E::from(zRadial)+E::from(directionScaled[2])).asFloat()
        };
    }

    // 0047F5A0: build the four cached quad positions through the world
    // transform. The local template is a unit square in XY; source explicitly
    // overwrites its Z components with zero. Transform products/sums retain
    // x87 precision until each vertex component is stored to float. Parent
    // composition belongs to the caller's transform service.
    static bool buildCachedGeometry(PlayerAshParticle& p,const PlayerAshTransform& frame) {
        if (!p.geometryDirty) return false;
        const float halfX=p.vectorB4[0]*0.5f;
        const float halfY=p.vectorB4[1]*0.5f;
        const float negativeHalfX=(-p.vectorB4[0])*0.5f;
        const float negativeHalfY=(-p.vectorB4[1])*0.5f;
        const std::array<std::array<float,3>,4> local{{
            {{halfX*(-1.0f),halfY,0.0f}},
            {{halfX,halfY,0.0f}},
            {{negativeHalfX*(-1.0f),negativeHalfY,0.0f}},
            {{negativeHalfX,negativeHalfY,0.0f}}
        }};
        using E=Extended;
        for(size_t vertex=0;vertex<local.size();vertex++){
            std::array<E,3> scaled{};
            for(size_t axis=0;axis<3;axis++)
                scaled[axis]=E::from(frame.scale[axis])*E::from(local[vertex][axis]);
            for(size_t component=0;component<3;component++){
                // 00495D20 schedules the last two matrix products into one
                // x87 sum before adding the first, spills that linear result
                // to float, then reloads it to add translation.
                E first=E::from(frame.axis[0][component])*scaled[0];
                E second=E::from(frame.axis[1][component])*scaled[1];
                E third=E::from(frame.axis[2][component])*scaled[2];
                float linear=(first+(second+third)).asFloat();
                E value=E::from(linear)+E::from(frame.position[component]);
                p.geometryPositions[vertex][component]=value.asFloat();
            }
        }
        p.geometryDirty=false;
        return true;
    }

    static const std::vector<float>& animationDurations() {
        static const std::vector<float> durations=[] {
            std::vector<float> values;
            values.reserve(descriptorWords.size());
            for (const auto& record : descriptorWords) {
                float duration;
                std::memcpy(&duration,&record[0],sizeof(duration));
                values.push_back(duration);
            }
            return values;
        }();
        return durations;
    }

    // `0047F960` calls `0047D660` before its vector/presentation gates when
    // +0xad is set. The selected container is installed at particle +0x0c by
    // `0047D370`; its five record durations are descriptor word +4.
    static void updateAnimation(PlayerAshParticle& p, uint32_t now) {
        if (!p.animationEnabled) return;
        SpriteAnimation animation;
        animation.frame=static_cast<int>(p.animationIndex);
        animation.nextFrame=static_cast<int>(p.nextAnimationIndex);
        animation.flags=static_cast<int>(p.animationFlags);
        animation.started=p.animationTimestamp;
        animation.rate=p.activationScale;
        animation.active=p.active;
        animation.update(now,animationDurations());
        p.animationIndex=static_cast<uint32_t>(animation.frame);
        p.nextAnimationIndex=static_cast<uint32_t>(animation.nextFrame);
        p.animationTimestamp=animation.started;
        p.active=animation.active;
    }

    // 004467e0 updates/presents every slot, then scans int(100*density).
    // Engine presentation and scene transforms are mandatory services so a
    // caller cannot silently replace unknown behavior with a no-op.
    void tick(uint32_t now, Random& rng, const PlayerAshPoolServices& s) {
        if (!initialized) throw std::logic_error("Player ash pool not initialized");
        if (!s.present || !s.getPlayerFrame)
            throw std::invalid_argument("ash pool tick requires scene and presentation services");
        for (auto& p : particles) {
            update(p,now,rng);
            updateAnimation(p,now); // 0047f960 -> 0047d660, before its other gates
            s.present(p,rng);       // remaining 0047f960 work
        }
        int scan = static_cast<int>(double(capacity) * double(density));
        if (scan < 0) scan = 0;
        if (scan > int(capacity)) scan = int(capacity);
        for (int i=0;i<scan;i++) {
            auto& p=particles[size_t(i)];
            if (p.active) continue;
            float angle=float(rng.range(0.0f,6.2831855f));
            float distance=float(rng.range(10.0f,400.0f));
            auto position=spawnPosition(angle,distance,rng,s);
            activate(p,position,now,rng);
        }
    }
};
