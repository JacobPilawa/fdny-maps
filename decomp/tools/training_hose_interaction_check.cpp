#include <json-c/json.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using J = json_object*;

static J field(J object, const char* name) {
    J value = nullptr;
    if (!json_object_object_get_ex(object, name, &value))
        throw std::runtime_error(std::string("Missing Training entity field: ") + name);
    return value;
}

static J at(J object, int index) { return json_object_array_get_idx(object, index); }
static int count(J object) { return json_object_array_length(object); }
static float num(J value) { return float(json_object_get_double(value)); }
static int integer(J value) { return json_object_get_int(value); }
static std::string str(J value) { return json_object_get_string(value); }

#include "../src/core.h"
#include "../src/training_hose_interaction.h"

static std::string string(J value) { return str(value); }

static std::array<float, 3> location(const std::string& value) {
    std::array<float, 3> result{};
    if (std::sscanf(value.c_str(), "%f %f %f", &result[0], &result[1], &result[2]) != 3)
        throw std::runtime_error("Malformed Training valve location");
    return result;
}

static void require(bool condition, const char* message) {
    if (!condition)
        throw std::runtime_error(message);
}

static bool close(float left, float right, float epsilon = 1e-7f) {
    return std::fabs(left - right) <= epsilon;
}

static V rayStart(const UseManager::Candidate& target) {
    return {target.center.x, target.center.y, target.center.z + 100.0f};
}

static constexpr V kRayDirection{0.0f, 0.0f, -1.0f};

int main(int argc, char** argv) {
    try {
        if (argc != 2)
            throw std::runtime_error("Usage: training_hose_interaction_check entities.json");
        J entities = json_object_from_file(argv[1]);
        if (!entities)
            throw std::runtime_error("Cannot read Training entities.json");

        std::vector<TrainingHoseInteraction::Valve> valves;
        for (int i = 0; i < json_object_array_length(entities); ++i) {
            J entity = json_object_array_get_idx(entities, i);
            if (string(field(entity, "class")) != "RCS_PropModel")
                continue;
            J properties = field(entity, "properties");
            if (string(field(properties, "S_Script01")) != "makevalve")
                continue;
            const int entityId = integer(field(entity, "index"));
            const int model = std::stoi(string(field(properties, "D_Model")));
            const float heading = std::stof(string(field(properties, "E_Heading")));
            const float radius = std::stof(string(field(properties, "F_TouchRadius")));
            valves.push_back(TrainingHoseInteraction::trainingValve(
                entityId, model, heading,
                location(string(field(properties, "ZZ_location"))), radius));
        }
        require(valves.size() == 2, "Expected Training's two authored makevalve candidates");
        require(valves[0].target.entity == 159 && valves[1].target.entity == 164,
                "Training makevalve order changed; review PropModel registration order");

        HoseItemSelection::State inventory;
        inventory.selected = 1;
        inventory.slots[1].present = true; // Axe from the authored Training setup.
        inventory.slots[4].present = true; // Hose Spray after touchhose 25.
        inventory.hoseMode = 2;
        HoseSprayPoseList::State poseList;
        require(HoseSprayPoseList::initializeTraining(poseList, 25),
                "touchhose 25 should rebuild the Training pose list");
        require(poseList.count == 30 && poseList.head == 0,
                "Training Hose pose-list count/head changed");
        HoseAttachmentFrame::State attachmentFrame;
        int activeValve = -1;

        const auto candidates = TrainingHoseInteraction::candidates(valves);
        auto target = UseManager::select(rayStart(candidates[0]), kRayDirection,
                                         candidates, inventory.hoseMode, activeValve);
        require(target.index == 0 && target.action == 1001,
                "Training Valve3 should be selected for attach");
        auto attached = TrainingHoseInteraction::use(valves[size_t(target.index)], target.action,
                                                      inventory, poseList, attachmentFrame, activeValve);
        require(attached.ownerCalled && attached.forwardUseEvent && attached.targetEntity == 159,
                "Valve3 attach callback should update the owner then dispatch use");
        require(attached.orderedOwnerEvents == std::vector<std::string>{
                    "can:1:1", "active:1:0", "active:4:1", "hud:1524:5:0",
                    "hud:1524:5:0", "hide:1216:0"},
                "Valve3 attach item/frame service ordering changed");
        require(inventory.selected == 4 && inventory.hoseMode == 3 && activeValve == 159,
                "Valve3 attach should select Hose Spray and enter mode 3");
        require(attachmentFrame.valveRoot == 159 && poseList.current == poseList.head,
                "Valve3 attach should save its root and cache the stream-pose head");
        for (int axis = 0; axis < 3; ++axis) {
            require(poseList.posePosition[size_t(axis)] == candidates[0].center[axis],
                    "Valve3 pose position differs from its authored location");
            require(poseList.nodes[size_t(poseList.head)].position[size_t(axis)] == candidates[0].center[axis],
                    "Valve3 head-node position differs from its authored location");
        }
        require(close(attachmentFrame.hoseValveOffset[0], -1.0f) &&
                close(attachmentFrame.hoseValveOffset[2], 4.867185765533577e-8f),
                "Valve3 resolved frame offset changed");

        auto otherWhileAttached = UseManager::select(rayStart(candidates[1]), kRayDirection,
                                                      candidates, inventory.hoseMode, activeValve);
        require(otherWhileAttached.index < 0,
                "A different valve must not offer detach while Valve3 is attached");
        auto detachTarget = UseManager::select(rayStart(candidates[0]), kRayDirection,
                                                candidates, inventory.hoseMode, activeValve);
        require(detachTarget.index == 0 && detachTarget.action == 1002,
                "The attached Training valve should offer detach");
        auto detached = TrainingHoseInteraction::use(valves[size_t(detachTarget.index)], detachTarget.action,
                                                      inventory, poseList, attachmentFrame, activeValve);
        require(detached.ownerCalled && !detached.forwardUseEvent && detached.targetEntity == 159,
                "Valve detach should run owner state changes and suppress the use event");
        require(detached.orderedOwnerEvents == std::vector<std::string>{
                    "hud:1524:5:0", "active:4:0", "active:1:1", "hud:1524:5:0",
                    "hide:1216:0"},
                "Valve detach/reselection service ordering changed");
        require(inventory.hoseMode == 2 && inventory.selected == 1 && activeValve == -1,
                "Valve detach should return to mode 2, reselect Axe and clear the active link");

        auto secondTarget = UseManager::select(rayStart(candidates[1]), kRayDirection,
                                                candidates, inventory.hoseMode, activeValve);
        require(secondTarget.index == 1 && secondTarget.action == 1001,
                "Training Valve5 should be selected for the second attach");
        auto attachedSecond = TrainingHoseInteraction::use(valves[size_t(secondTarget.index)], secondTarget.action,
                                                           inventory, poseList, attachmentFrame, activeValve);
        require(attachedSecond.ownerCalled && attachedSecond.forwardUseEvent && activeValve == 164,
                "Valve5 attach should update the Hose root and dispatch use");
        require(attachmentFrame.valveRoot == 164 && inventory.hoseMode == 3,
                "Valve5 should replace the saved frame root and enter mode 3");
        require(close(attachmentFrame.hoseValveOffset[0], 1.0f) &&
                close(attachmentFrame.hoseValveOffset[2], -2.4335928827667885e-8f),
                "Valve5 resolved frame offset changed");
        for (int axis = 0; axis < 3; ++axis)
            require(poseList.nodes[size_t(poseList.head)].position[size_t(axis)] == candidates[1].center[axis],
                    "Valve5 head-node position differs from its authored location");

        std::cout << "Training hose interactions: Valve3 attach/detach, Valve5 attach, candidate gating, "
                     "inventory selection, frame commit and pose-list head; source-ordered scenario passed.\n";
        std::cout << "Note: Valve model basis residuals retain the saved Ghidra Java-Math trig approximation.\n";
        json_object_put(entities);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << "\n";
        return 1;
    }
}
