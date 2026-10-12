#pragma once

#include <array>
#include <stdexcept>
#include <string>
#include <vector>

#include "hose_attachment_frame.h"
#include "hose_item_selection.h"
#include "hose_spray_pose_list.h"
#include "hose_valve_callback.h"
#include "use_manager.h"

// Connected Training path for PropModel candidates -> USE callback -> Hose
// inventory/frame commit -> pose-list head. Engine script dispatch remains at
// the caller so it can preserve ScriptActor queue ownership and timing.
namespace TrainingHoseInteraction {

struct Valve {
    UseManager::Candidate target;
    HoseAttachmentFrame::Input attachment;
    int activeState = 1;
};

struct UseResult {
    int targetEntity = -1;
    bool ownerCalled = false;
    bool forwardUseEvent = true;
    std::vector<std::string> orderedOwnerEvents;
};

inline Valve trainingValve(int entity, int model, float heading,
                           const std::array<float, 3>& position, float radius) {
    if (model != 5)
        throw std::invalid_argument("Training makevalve target does not use model 5");

    Valve valve;
    valve.target = {entity, V{position[0], position[1], position[2]}, radius};
    valve.attachment.valveRoot = entity;
    valve.attachment.valveGetterPosition = position;

    PlayerAshTransformNode root;
    root.value.position = position;
    valve.attachment.valveFrameNodes.push_back(root);

    PlayerAshTransformNode modelFrame;
    modelFrame.parent = 0;
    // The shipped valve.act has an identity root attachment. Training's
    // 00443860 angle conversion maps heading 9 to -360 degrees and heading 3
    // to -180 degrees. These residual words are the saved Ghidra emulator
    // outputs; its Java Math trig substitution means the last basis bits are
    // provisional until an x87-capable original-runtime capture is available.
    if (heading == 9.0f) {
        modelFrame.value.axis = {{{1.0f, 0.0f, -4.867185765533577e-8f},
                                  {0.0f, 1.0f, 0.0f},
                                  {4.867185765533577e-8f, -0.0f, 1.0f}}};
    } else if (heading == 3.0f) {
        modelFrame.value.axis = {{{-1.0f, 0.0f, 2.4335928827667885e-8f},
                                  {0.0f, 1.0f, 0.0f},
                                  {-2.4335928827667885e-8f, 0.0f, -1.0f}}};
    } else {
        throw std::invalid_argument("Training valve heading has no recovered model frame");
    }
    valve.attachment.valveFrameNodes.push_back(modelFrame);
    valve.attachment.valveFrameRoot = 1;
    return valve;
}

inline std::vector<UseManager::Candidate> candidates(const std::vector<Valve>& valves) {
    std::vector<UseManager::Candidate> result;
    result.reserve(valves.size());
    for (const auto& valve : valves)
        result.push_back(valve.target);
    return result;
}

inline UseResult use(const Valve& valve, int action,
                     HoseItemSelection::State& inventory,
                     HoseSprayPoseList::State& poseList,
                     HoseAttachmentFrame::State& attachmentFrame,
                     int& activeValve) {
    UseResult result;
    result.targetEntity = valve.target.entity;
    const auto callback = HoseValveCallback::invoke(valve.activeState, action);
    result.ownerCalled = callback.callOwnerAction;
    result.forwardUseEvent = callback.forwardUseEvent;
    if (!callback.callOwnerAction)
        return result;

    if (action == 1001) {
        const auto owner = HoseItemSelection::attach(inventory);
        result.orderedOwnerEvents = owner.events;
        if (!owner.reachedFrameTransform)
            return result;

        attachmentFrame.hoseMode = inventory.hoseMode;
        auto frameInput = valve.attachment;
        frameInput.valveRoot = valve.target.entity;
        frameInput.firstStreamPoseNode = poseList.head;
        const auto frame = HoseAttachmentFrame::commit(attachmentFrame, frameInput);
        inventory.hoseMode = attachmentFrame.hoseMode;
        activeValve = valve.target.entity;
        HoseSprayPoseList::attach(poseList, attachmentFrame.hoseFramePosition);
        result.orderedOwnerEvents.insert(result.orderedOwnerEvents.end(),
                                         frame.boundaryEvents.begin(),
                                         frame.boundaryEvents.end());
    } else if (action == 1002) {
        const auto owner = HoseItemSelection::detach(inventory);
        result.orderedOwnerEvents = owner.events;
        attachmentFrame.hoseMode = inventory.hoseMode;
        activeValve = -1;
    }
    return result;
}

} // namespace TrainingHoseInteraction
