#pragma once

// Source path: Player bounds from 0044D550/00451870/0044BCE0 and the victim
// callback predicate in 0045A070. Player vtable slot +0x0c
// (0049DB90 -> 0049D790) returns a record beginning with world XYZ and
// followed by the PlayerRescue mins at +0x78 and maxs at +0x84. The fourth
// float at +0x74 is padding for this callback; 0045A070 reads the three mins
// and three maxs from indices [4..6] and [7..9].
struct VictimPickupBox {
 V origin, mins, maxs;
};

inline VictimPickupBox victimPickupBox(const Player& player,V worldOrigin){
 // 0044D550 initializes mins=(-30,-30,-30), maxs=(30,85,30). CROUCH
 // writes max Y=25 at root +0x88; UNCROUCH restores the authored 85.
 return {worldOrigin,{-30,-30,-30},player.maxs()};
}

// `radius` is the already-resolved callback radius. Training's two victims
// both supply J_Radius=60. The generic invalid-radius comparator/fallback is
// kept separate because 00459B40 delegates to the bit-level float helper
// 0043B120; callers should not silently substitute a simpler test for it.
inline bool sourceVictimPickupAabbOverlap(const VictimPickupBox& player,
                                          V victimCenter,float radius){
 const double r=static_cast<double>(radius);
 const auto axisOverlaps=[&](float origin,float low,float high,float center){
  const double playerLow=static_cast<double>(origin)+static_cast<double>(low);
  const double playerHigh=static_cast<double>(origin)+static_cast<double>(high);
  const double victimLow=static_cast<double>(center)-r;
  const double victimHigh=static_cast<double>(center)+r;
  return playerLow<=victimHigh&&victimLow<=playerHigh;
 };
 // Original callback tests X, then Z, then Y; all six bounds comparisons
 // accept equality. Binary64 preserves the relevant sums of finite binary32
 // world coordinates while avoiding an extra binary32 rounding step.
 return axisOverlaps(player.origin.x,player.mins.x,player.maxs.x,victimCenter.x)&&
        axisOverlaps(player.origin.z,player.mins.z,player.maxs.z,victimCenter.z)&&
        axisOverlaps(player.origin.y,player.mins.y,player.maxs.y,victimCenter.y);
}

inline bool sourceVictimPickupCandidate(int victimState,int otherEntityType,
                                        bool playerWorldBoxAvailable,
                                        const VictimPickupBox& player,
                                        V victimCenter,float radius){
 if(victimState==5||victimState==6||otherEntityType!=1||!playerWorldBoxAvailable)
  return false;
 return sourceVictimPickupAabbOverlap(player,victimCenter,radius);
}
