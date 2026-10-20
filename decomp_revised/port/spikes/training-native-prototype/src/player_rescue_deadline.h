#pragma once
#include <algorithm>
#include <cmath>
#include <optional>
#include "random.h"
#include "extended.h"

// Player initializer 0044D550, fragment 0044DE07..0044DE90. The level word is catalog time: token times 60 (00423160).
// Null follows the source fallback; the record constructor default is separate.
inline float initializePlayerDeadlineInput(std::optional<float> levelField10,int mode){
 float value=levelField10.value_or(600.0f);
 double multiplier=(mode==0||mode==1)?1.5:1.0;
 return (Extended::from(value)*Extended::from(multiplier)).asFloat();
}

// 004541A0, reached by escape via thunk 00454300 after status call 2.
// Storage target is (Player+0x340C)+0x14 = Player+0x3420.
// Its Player+0x343C argument is the live remaining-seconds countdown: 0044D550
// initializes it from the catalog, and 00452570 subtracts frame delta.
struct PlayerRescueDeadlineResult {
 uint32_t threshold=0,base=0,bound=0;
 bool beforeThreshold=false;
};
inline PlayerRescueDeadlineResult setPlayerRescueDeadline(
 uint32_t& deadline,float playerField343C,int mode,uint32_t now,Random& random){
 using E=Extended;
 float value=std::clamp(playerField343C,180.0f,7200.0f);
 double factor=mode==0?0.6:mode==1?0.75:1.0;
 uint32_t threshold=uint32_t((E::from(value)*E::from(1000.0f)*E::from(factor)).truncate());
 if(now<threshold){
  deadline=threshold+random.bounded(30000);
  return {threshold,threshold,30000,true};
 }
 // Original modes select literal doubles 1, 2, and 1000 for this branch.
 double delayFactor=mode==0?1.0:mode==1?2.0:1000.0;
 uint32_t base=uint32_t((E::integer(now)+E::from(delayFactor)*E::from(60000.0f)).truncate());
 deadline=base; // This source store precedes the RNG call.
 uint32_t draw=random.bounded(60000);
 deadline+=draw;
 return {threshold,base,60000,false};
}
