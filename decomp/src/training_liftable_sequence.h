#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include "bmodel_use.h"
#include "throw_scalar.h"

// Ordered Training liftable interaction effects recovered from FDNYFirefighter.exe.
// This is an explicit transaction core, not a scheduler or a live scene adapter.
// The caller supplies the unresolved engine operations (frames, model transforms,
// statistics, ScriptActor dispatch, and timer/list cleanup).
namespace TrainingLiftableSequence {

struct State {
    void* heldObject = nullptr;        // Player root +0x3440
    int playerState = 3;               // Player root +0x18C
    int bmodelState = 0;               // BModel root +0x25C
    int useMode = 0;                   // BModel root +0x258
    int modelCount = 0;                // BModel root +0x1C4
    std::array<float, 3> start{};       // BModel root +0x1F0..+0x1F8
    std::array<float, 3> end{};         // BModel root +0x1FC..+0x204
    uint32_t throwStartTick = 0;        // BModel root +0x208
    float throwDuration = 0;           // BModel root +0x20C
    float throwAcceleration = 0;       // BModel root +0x210
    float throwInitialVelocity = 0;    // BModel root +0x214
    float throwInitialMiddle = 0;      // BModel root +0x218
    bool reverseEndpoint = false;      // BModel root +0x220
};

struct Services {
    BModelUse::ActionSubobjectCall playerActionStatus;
    std::function<void(void*, const char*)> dispatchBmodelEvent;
    // Player carry pose construction and BModel correction are engine/frame
    // operations. This callback represents their source-ordered boundary.
    std::function<void(void*)> updateHeldObjectPose;
    // Returns the source helper 00497AA0 angular separation for the two vectors
    // built at 00453964. Their semantic labels remain unresolved.
    std::function<float(void*)> heldObjectThrowAngleDegrees;
    std::function<void(void*, const std::array<float, 3>&)> applyBmodelPosition;
    std::function<void()> incrementObjectsThrown;
    std::function<void(void*, const char*)> dispatchDropEvent;
    std::function<void(void*)> clearBmodelTimer;
};

inline bool validForThrowUpdate(const State& state) {
    // 0043DC60 requires a model and nonzero useMode; 0043D330 requires mode 2.
    return state.modelCount >= 1 && state.useMode != 0 && state.bmodelState == 2;
}

// 0044BC40 then the ScriptActor `use` event in 0043D2B0. The action callback
// preserves the original 004571A0(Player+0x5F4, 3, nullptr) boundary.
inline bool pickUp(State& state, int selectedActionCode, void* bmodelRoot,
                   void* playerActionSubobject, const Services& services) {
    if (selectedActionCode != BModelUse::kLiftAction || state.useMode != 1 ||
        !bmodelRoot || !services.playerActionStatus ||
        !services.dispatchBmodelEvent) return false;
    BModelUse::LiftTransition transition{state.heldObject, state.bmodelState,
                                         state.playerState};
    const BModelUse::ActionSubobjectCall orderedActionCall =
        [&](void* receiver, std::uintptr_t argument, const void* payload) {
            // Mirror the root stores before forwarding 004571A0. The helper
            // writes state 7 only after this callback returns.
            state.heldObject = transition.playerHeldObject;
            state.bmodelState = transition.bmodelObjectState;
            state.playerState = transition.playerState;
            services.playerActionStatus(receiver, argument, payload);
        };
    if (!BModelUse::enterLiftMode(state.useMode, bmodelRoot,
                                  playerActionSubobject, transition,
                                  orderedActionCall)) return false;
    state.heldObject = transition.playerHeldObject;
    state.bmodelState = transition.bmodelObjectState;
    state.playerState = transition.playerState;
    if (services.dispatchBmodelEvent)
        services.dispatchBmodelEvent(bmodelRoot, "use");
    return true;
}

// Source-derived action gate in 0044C520, kept next to the transaction so an
// adapter cannot start pickup merely because a candidate was geometrically
// selected. Candidate registration/query order is still supplied by the host.
inline int pickupAction(bool hoseSlot4Present, int hoseMode,
                        std::uint8_t playerByte12C, const State& state) {
    const auto candidate = BModelUse::candidate(
        state.useMode, hoseSlot4Present, hoseMode, playerByte12C,
        state.heldObject == nullptr, state.bmodelState);
    return candidate.action;
}

// Called only from an externally driven Player update. The host pose callback
// corresponds to the held-position write and 0043BD90 correction that precede
// the angle branch in 00452570. No recurring cadence is implied here.
inline bool updateCarryAndMaybeRelease(State& state, uint32_t tick,
                                       void* playerActionSubobject,
                                       const Services& services) {
    (void)tick; // Sampled by the throw setup only after the strict angle gate.
    void* object = state.heldObject;
    if (!object) return false;
    if (!services.updateHeldObjectPose ||
        !services.heldObjectThrowAngleDegrees) return false;
    services.updateHeldObjectPose(object);
    const float angle = services.heldObjectThrowAngleDegrees(object);
    if (!(angle < 10.0f)) return false; // 00452570: equality follows side selection.
    if (!services.playerActionStatus || !services.incrementObjectsThrown)
        return false;

    // 0043B580 initializes mode 2 before Player action status 4 and carry clear.
    const auto setup = BModelThrowScalar::setup(state.start, state.end, tick);
    state.throwStartTick = setup.tick;
    state.throwDuration = setup.duration;
    state.throwAcceleration = setup.acceleration;
    state.throwInitialVelocity = setup.initialVerticalVelocity;
    state.throwInitialMiddle = setup.initialMiddle;
    state.bmodelState = int(setup.mode);
    if (services.playerActionStatus)
        services.playerActionStatus(playerActionSubobject, 4, nullptr);
    state.heldObject = nullptr;
    state.playerState = 3;
    if (services.incrementObjectsThrown) services.incrementObjectsThrown();
    return true;
}

// Represents one externally scheduled BModel subobject update. On an active
// tick, 0043D330 sends its interpolated point through 0043BAA0. On completion,
// source order is endpoint mutation, endpoint flip, `drop`, then timer clear.
inline bool updateThrow(State& state, uint32_t tick, void* bmodelRoot,
                        const Services& services) {
    if (!bmodelRoot || !validForThrowUpdate(state)) return false;
    if (!services.applyBmodelPosition) return false;
    const auto update = BModelThrowScalar::update(
        state.start, state.end, state.throwStartTick, tick,
        state.throwDuration, state.throwAcceleration,
        state.throwInitialVelocity, state.reverseEndpoint);
    if (!update.completed) {
        services.applyBmodelPosition(bmodelRoot, update.point);
        return true;
    }
    if (!services.dispatchDropEvent || !services.clearBmodelTimer) return false;

    state.bmodelState = 0;
    services.applyBmodelPosition(bmodelRoot, update.point);
    state.reverseEndpoint = !state.reverseEndpoint;
    services.dispatchDropEvent(bmodelRoot, "drop");
    services.clearBmodelTimer(bmodelRoot);
    return true;
}

} // namespace TrainingLiftableSequence
