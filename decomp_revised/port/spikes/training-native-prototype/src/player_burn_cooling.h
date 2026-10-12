#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include "extended.h"

struct PlayerBurnCoolingState {
 float accumulator=0;
 std::array<uint8_t,4> directionalIndicators{};
 uint8_t statusMarker=0;
 std::array<uint32_t,3> copySource{};
 std::array<uint32_t,3> copyTarget{};
};

struct PlayerBurnCoolingResult {
 bool cooled=false;
 bool sideCopyCalled=false;
 bool clamped=false;
 float beforeClamp=0;
};

// Extended values produced by Extended::from/operation are normalized to a
// 64-bit significand, so exponent then significand gives an exact x87 compare.
inline int comparePlayerBurnExtended(Extended a,Extended b){
 if(!a.m&&!b.m)return 0;
 if(a.negative!=b.negative)return a.negative?-1:1;
 int order=0;
 if(!a.m)order=-1;
 else if(!b.m)order=1;
 else if(a.e!=b.e)order=a.e<b.e?-1:1;
 else if(a.m!=b.m)order=a.m<b.m?-1:1;
 return a.negative?-order:order;
}

// Player movement updater 00452570, normal branch after status helper
// 004559D0. State-6 dispatch to 00451080 is handled by the caller. The copy
// hook represents the 00441E90 boundary (three DWORDs: Player+0x4C to +0x3400).
// It runs after the binary32 store and before the negative-value clamp.
template<class CopyThreeWords>
inline PlayerBurnCoolingResult updatePlayerBurnCooling(
    PlayerBurnCoolingState& state,float frameDelta,bool sideFlag,
    float sideThreshold,CopyThreeWords&& copyThreeWords){
 using E=Extended;
 PlayerBurnCoolingResult result;
 bool indicatorsClear=state.directionalIndicators[0]==0;
 state.statusMarker=0; // 004529AA, after testing the first directional byte
 for(std::size_t i=1;i<state.directionalIndicators.size();i++)indicatorsClear&=(state.directionalIndicators[i]==0);
 if(indicatorsClear){
  // 004529D1..004529E3: FLD 0.1f; FMUL dt; FSUBR accumulator; FST
  // (non-pop). The following FCOMP observes this still-extended value.
  const E cooled=E::from(state.accumulator)-E::from(0.1f)*E::from(frameDelta);
  state.accumulator=cooled.asFloat();
  result.cooled=true;
  if(comparePlayerBurnExtended(cooled,E::from(0.1))<0&&sideFlag&&sideThreshold>0.5f){
   copyThreeWords(state.copySource,state.copyTarget);
   result.sideCopyCalled=true;
  }
 }
 result.beforeClamp=state.accumulator;
 if(state.accumulator<0.0f){state.accumulator=0.0f;result.clamped=true;}
 state.directionalIndicators.fill(0);
 return result;
}
