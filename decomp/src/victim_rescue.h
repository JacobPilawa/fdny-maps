#pragma once
#include <cstdint>
#include <optional>

// State used by the already-delivered Player message 0x7532. This component
// models the handler transaction only; it does not schedule proximity checks
// or claim a recurring callback cadence.
struct VictimPickupPlayerState {
 std::optional<int> carriedVictimEntity;
};

struct VictimPickupActorState {
 int entity=-1;
 uint32_t state=4; // VictimRescueEnt::Init (0045B690) writes state 4.
};

enum class VictimPickupMessageResult {
 ignoredMessage,
 playerMessageGateClosed,
 playerAlreadyCarrying,
 accepted
};

// Source order: Player::HandleMessage (00453A70) rejects messages while
// Player+0x3438 is nonzero and routes 0x7532 to 00450440. The handler stores
// Player+0x3534 before calling 0045BEF0. That helper sets victim state 5,
// calls 00481140 with false (which sets the Genesis world actor flags word to
// 0, suppressing normal-pass render submission), then sends the synchronous
// "pickup" event through 004705B0. Player UI/state work in
// 00450440 follows that helper. The event callbacks below preserve this order.
// They deliberately do not model the caller's update schedule.
template<class SetActorFlags,class DispatchPickup,class ApplyPlayerPostPickup>
VictimPickupMessageResult deliverVictimPickupMessage(
    int message,
    bool playerMessageGateOpen,
    VictimPickupPlayerState& player,
    VictimPickupActorState& victim,
    SetActorFlags&& setActorFlags,
    DispatchPickup&& dispatchPickup,
    ApplyPlayerPostPickup&& applyPlayerPostPickup){
 if(!playerMessageGateOpen)return VictimPickupMessageResult::playerMessageGateClosed;
 if(message!=0x7532)return VictimPickupMessageResult::ignoredMessage;
 if(player.carriedVictimEntity)return VictimPickupMessageResult::playerAlreadyCarrying;

 player.carriedVictimEntity=victim.entity;
 victim.state=5;
 setActorFlags(victim.entity,uint32_t(0));
 dispatchPickup(victim.entity);
 applyPlayerPostPickup();
 return VictimPickupMessageResult::accepted;
}
