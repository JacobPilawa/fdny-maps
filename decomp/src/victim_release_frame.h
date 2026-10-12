#pragma once
#include "view_update.h"

// Frame prefix of 0045C000, before floor placement at 0045C0EE.
// 00446EE0/0049E770 read X/Z; 0042D2B0 negates both vectors.
// The source reads the caller-supplied Player frame directly, not world().
inline FrameTransform sourceVictimReleaseFrame(const FrameTransform& player,
                                               FrameTransform victim){
 victim.axis[0]={-player.axis[0].x,-player.axis[0].y,-player.axis[0].z};
 victim.axis[2]={-player.axis[2].x,-player.axis[2].y,-player.axis[2].z};
 ViewUpdate::setUp(victim,{0,1,0}); // 0042BD50 → 00496210
 // Two binary32 stores in two distinct 00446390 calls; collapsing to ×120
 // would change intermediate rounding for some inputs.
 V offset=ViewUpdate::scale(victim.axis[2],5.0f);
 offset=ViewUpdate::scale(offset,24.0f);
 victim.position=ViewUpdate::subtract(player.position,offset); // 0043B2D0
 return victim;
}
