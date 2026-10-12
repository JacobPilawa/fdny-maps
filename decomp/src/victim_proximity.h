#pragma once

#include <cstdint>
#include <optional>
#include "entity_id_registry.h"
#include "fire_message_queue.h"
#include "victim_pickup.h"

enum class VictimProximityResult {
    rejectedVictimState, rejectedRecipientType, noPlayerWorldBox,
    outsidePickupBounds, queued, queueFull
};

// One invocation of the source victim primary callback 0045A070. This does
// not supply a tick, cadence, player frame, or radius policy. The radius
// resolver returns the already source-resolved Training value (60); it is a
// callback solely so it occurs after the Player world-box getter as in source.
// Recipient is the generation-bearing result of the existing route registry;
// payload must remain stable until the original queued-message drain.
template<class GetPlayerWorldBox, class ResolveRadius, class GetVictimCenter,
         class GetRecipientHandle, class ReportQueueFull>
VictimProximityResult publishVictimProximity(
    int victimState, int recipientEntityType,
    const void* stableVictimPayload,
    FireMessageQueue& queue, GetPlayerWorldBox&& getPlayerWorldBox,
    ResolveRadius&& resolveTrainingRadius, GetVictimCenter&& getVictimCenter,
    GetRecipientHandle&& getRecipientHandle,
    ReportQueueFull&& reportQueueFull) {
    if (victimState == 5 || victimState == 6)
        return VictimProximityResult::rejectedVictimState;
    if (recipientEntityType != 1)
        return VictimProximityResult::rejectedRecipientType;

    const std::optional<VictimPickupBox> player = getPlayerWorldBox();
    if (!player) return VictimProximityResult::noPlayerWorldBox;

    const float radius = resolveTrainingRadius(); // 00459B40; caller owns exact resolver
    const V center = getVictimCenter();
    if (!sourceVictimPickupAabbOverlap(*player, center, radius))
        return VictimProximityResult::outsidePickupBounds;

    // Only after overlap, source 0049D350 supplies recipient route/generation;
    // 0049D4E0 routes the record into mapper 0049B4C0. Append flag is 0.
    const EntityIdRegistry::Handle recipient = getRecipientHandle();
    if (!queue.enqueue(recipient.route, recipient.generation, 0x7532,
                       stableVictimPayload, 0)) {
        reportQueueFull(); // native mapper logs "No free messages!" and returns
        return VictimProximityResult::queueFull;
    }
    return VictimProximityResult::queued;
}
