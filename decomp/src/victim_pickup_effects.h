#pragma once
#include <cstdint>

// Player+0x198 fields written by 00490E60. Names describe storage only;
// the downstream subsystem's gameplay interpretation remains unverified.
struct VictimPickupEffectState {
 float value=0;
 float clearedValue=0;
 uint8_t enabled=0;
 uint32_t pickupClock=0; // Player+0x5C4
};

// The post-event portion of 00450440. Call this from the post-pickup hook
// in victim_rescue.h, after the synchronous pickup event. Entity, status,
// and zone adapters remain explicit until their runtime integration is ready.
// readClock is evaluated last: earlier callbacks may change the global clock.
template<class ReadPlayerField,class SetEntityMode,class UpdateStatus,class RefreshZones,class ReadClock>
void applyVictimPickupEffectsWithFieldReader(ReadPlayerField&& readPlayerField,VictimPickupEffectState& state,
 SetEntityMode&& setEntityMode,UpdateStatus&& updateStatus,
 RefreshZones&& refreshZones,ReadClock&& readClock){
 setEntityMode(true); // 0044FCE0(1)
 state.value=readPlayerField()==4?20.0f:50.0f;
 state.clearedValue=0;
 state.enabled=1;
 updateStatus(); // 004571A0(Player+0x5F4,1,null)
 refreshZones(); // 00484260 excludes victim states 5 and 6
 state.pickupClock=readClock();
}

template<class SetEntityMode,class UpdateStatus,class RefreshZones,class ReadClock>
void applyVictimPickupEffects(int playerField18C,VictimPickupEffectState& state,
 SetEntityMode&& setEntityMode,UpdateStatus&& updateStatus,
 RefreshZones&& refreshZones,ReadClock&& readClock){
 applyVictimPickupEffectsWithFieldReader([&]{return playerField18C;},state,
  setEntityMode,updateStatus,refreshZones,readClock);
}

inline bool sourceVictimZoneMember(int entityType,uint32_t victimState){
 return entityType==2&&victimState!=5&&victimState!=6;
}
