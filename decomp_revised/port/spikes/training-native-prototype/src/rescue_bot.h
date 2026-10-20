#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
#include <optional>
#include <vector>
#include "core.h"
#include "player_rescue_deadline.h"

struct RescueBotCandidate {
 int entity=-1;
 uint32_t state=0;
 bool excluded=false;
 V position{};
};

struct RescueBotState {
 std::optional<int> activeVictim;
 uint32_t activeDeadline=0;
 uint32_t nextMessageDeadline=0;
 uint32_t nextAttemptDeadline=0;
 bool schedulesInitialized=false;
};

// 00455230 scans VictimRescue entries in manager order, accepts state 4/7,
// skips the embedded norescuebot byte, requires an obstructed segment, then
// accepts distance >= the binary64 constant 600.0. Manager order is supplied
// by the caller; this helper does not infer actor registration order.
template<class IsWorldSegmentBlocked>
inline std::optional<int> selectRescueBotCandidate(
 const std::vector<RescueBotCandidate>& candidates,V playerPosition,
 IsWorldSegmentBlocked&& isWorldSegmentBlocked){
 for(const auto& candidate:candidates){
  if(candidate.excluded||(candidate.state!=4&&candidate.state!=7))continue;
  if(!isWorldSegmentBlocked(candidate.position,playerPosition))continue;
  const double dx=double(candidate.position.x)-double(playerPosition.x);
  const double dy=double(candidate.position.y)-double(playerPosition.y);
  const double dz=double(candidate.position.z)-double(playerPosition.z);
  const double distance=std::sqrt(dx*dx+dy*dy+dz*dz);
  if(distance>=600.0)return candidate.entity;
 }
 return std::nullopt;
}

// 00455580 computes floor(totalVictims * modeFactor + 0.5), then converts
// toward zero with 004B9400. The mode factors are 0.4, 0.25, and 0.0 for
// global modes 0, 1, and other respectively; DAT_004DE6F8 is 0.5.
inline int32_t rescueBotMaximum(uint32_t totalVictims,int globalMode){
 int32_t signedTotal;
 static_assert(sizeof(signedTotal)==sizeof(totalVictims),"32-bit count required");
 std::memcpy(&signedTotal,&totalVictims,sizeof(signedTotal));
 const double factor=globalMode==0?0.4:(globalMode==1?0.25:0.0);
 return int32_t((double(signedTotal)*factor+0.5));
}

inline bool rescueBotDeadlineDue(uint32_t now,uint32_t deadline){
 // The retail callback uses unsigned CMP/JBE, so equality is not due.
 return now>deadline;
}

inline uint32_t nextRescueBotMessageDeadline(uint32_t now,Random& random){
 // 00454100: current clock + 120,000 + Random(120,000).
 return now+120000u+random.bounded(120000u);
}

inline void initializeRescueBotSchedules(RescueBotState& state,uint32_t now,
 float playerField343C,int globalMode,Random& random){
 // 00454BA0 sets the message deadline before calling 004541A0 for the next
 // selection deadline. The recurring callback object's constructor/default
 // state is still unresolved, so the viewer uses this source initializer only
 // after an explicit setrescuebot 1 and labels its cadence provisional.
 state.nextMessageDeadline=nextRescueBotMessageDeadline(now,random);
 setPlayerRescueDeadline(state.nextAttemptDeadline,playerField343C,
                         globalMode,now,random);
 state.schedulesInitialized=true;
}

inline uint32_t startRescueBotCarryDeadline(uint32_t now,Random& random){
 // 00454750: current clock + 4,000 + Random(4,000).
 return now+4000u+random.bounded(4000u);
}

inline void postponeRescueBotAttemptIfFuture(RescueBotState& state,uint32_t now){
 // 00454130 adds 10,000 only when the stored unsigned deadline is future.
 if(now<state.nextAttemptDeadline)state.nextAttemptDeadline+=10000u;
}
