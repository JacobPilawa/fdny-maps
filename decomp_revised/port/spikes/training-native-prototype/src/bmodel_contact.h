#pragma once

#include <cmath>
#include "core.h"

namespace BModelContact {
// Source-mapped from FDNYFirefighter.exe 0043B620 -> 004817A0. The original
// compares actor mins/maxs against each BModel bound expanded by E_Reach.
inline bool overlapsExpanded(V modelMin, V modelMax, V actorMin, V actorMax, float reach) {
 for(int axis=0;axis<3;axis++)
  if(!(actorMax[axis]>=modelMin[axis]-reach&&actorMin[axis]<=reach+modelMax[axis]))return false;
 return true;
}

// 0043AFF0 returns true for motion end times outside the open interval
// (-1e-28, +1e-28), including unordered (NaN) input. The BModel constructor
// initializes its aggregate end time to zero and retains the greatest child
// motion end time. 0043B620 substitutes 48 only for a strict reach < 48.
inline bool hitpointReachGate(int hitPoints, int liftable, float motionEnd) {
 return hitPoints!=0&&liftable==0&&
  (std::isnan(motionEnd)||motionEnd<=-1.0e-28f||motionEnd>=1.0e-28f);
}
inline float effectiveReach(float authoredReach, int hitPoints, int liftable, float motionEnd) {
 return authoredReach<48.f&&hitpointReachGate(hitPoints,liftable,motionEnd)?48.f:authoredReach;
}
}
