#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include "extended.h"

// Scalar-only transcription of BModel throw setup/update (0043B580/0043D330).
// This does not model the 0043DC60 caller, model transforms, events, or cadence.
struct BModelThrowScalar {
 using E=Extended;
 struct Setup {
  float duration=0,acceleration=0,initialVerticalVelocity=0,initialMiddle=0;
  uint32_t tick=0,mode=0;
 };
 struct Update { bool completed=false; std::array<float,3> point{}; };

 static int compare(E a,E b){
  if(!a.m&&!b.m)return 0;
  if(a.negative!=b.negative)return a.negative?-1:1;
  int result=0;
  if(!a.m)result=-1;
  else if(!b.m)result=1;
  else if(a.e!=b.e)result=a.e<b.e?-1:1;
  else if(a.m!=b.m)result=a.m<b.m?-1:1;
  return a.negative?-result:result;
 }
 static E squareRoot(E value){
  if(value.negative)throw std::runtime_error("Negative BModel throw distance square");
  if(!value.m)return {};
  bool odd=(value.e&1)!=0;
  int shift=odd?63:64;
  int exponent=odd?(value.e-63)/2:(value.e-64)/2;
  E::U radicand=E::U(value.m)<<shift;
  uint64_t low=uint64_t(1)<<63,high=UINT64_MAX;
  while(low<high){uint64_t mid=low+((high-low+1)>>1);if(E::U(mid)<=radicand/mid)low=mid;else high=mid-1;}
  E::U root=low,remainder=radicand-root*root;
  // For integer radicands, comparing the remainder with root rounds sqrt to
  // nearest; the exact midpoint is root + 1/2, whose square has a .25 term.
  if(remainder>root)root++;
  return E::pack(root,exponent);
 }
 static uint32_t bits(float value){uint32_t result;std::memcpy(&result,&value,4);return result;}
 static float fromBits(uint32_t value){float result;std::memcpy(&result,&value,4);return result;}

 static Setup setup(const std::array<float,3>& start,const std::array<float,3>& end,uint32_t tick){
  // 0043B2D0 stores each start-end difference to a float local before 0043B580
  // loads and squares it. The listing sums x²+y², then adds z² before FSQRT.
  std::array<float,3> delta{};
  for(int axis=0;axis<3;axis++)delta[axis]=(E::from(start[axis])-E::from(end[axis])).asFloat();
  E x=E::from(delta[0]),y=E::from(delta[1]),z=E::from(delta[2]);
  E distance=squareRoot(x*x+y*y+z*z);
  const float durationScale=fromBits(0x3a888889u); // 004E0BB8, loaded as binary32.
  E durationExtended=distance*E::from(durationScale);
  const float acceleration=fromBits(0xc3c00000u); // immediate store at 0043B5BC.
  // 0043B5F2 stores duration as float but leaves the extended value on x87
  // for the following velocity calculation at 0043B5F8..0043B612.
  E deltaMiddle=E::from(end[1])-E::from(start[1]);
  E velocity=(deltaMiddle-(durationExtended*durationExtended)*E::from(acceleration))/durationExtended;
  return {durationExtended.asFloat(),acceleration,velocity.asFloat(),start[1],tick,2};
 }

 static Update update(const std::array<float,3>& start,const std::array<float,3>& end,
                      uint32_t startTick,uint32_t tick,float duration,float acceleration,
                      float initialVerticalVelocity,bool reverse){
  // SUB at 0043D400 is modulo-32-bit; TEST reads the resulting signed sign bit.
  uint32_t rawDelta=tick-startTick;
  int32_t signedDelta;std::memcpy(&signedDelta,&rawDelta,4);
  E elapsed=E::from(static_cast<double>(signedDelta)); // FILD dword at 0043D40C.
  if(signedDelta<0)elapsed=elapsed+E::from(4294967296.0); // 004DFC90.
  elapsed=elapsed*E::from(0.001); // binary64 constant 004DFA28; no float store here.
  E durationExtended=E::from(duration);
  if(compare(durationExtended,elapsed)<0){
   Update result;result.completed=true;
   // 0043D45D / 0043D466 select end for forward, start for reverse.
   result.point=reverse?start:end;
   return result;
  }
  E t=reverse?durationExtended-elapsed:elapsed;
  // 0043D4BD stores this quotient in a float stack local before reloading it.
  float ratio=(t/durationExtended).asFloat();
  E oneMinusRatio=E::from(1.0f)-E::from(ratio);
  Update result;
  result.point[0]=(oneMinusRatio*E::from(start[0])+E::from(ratio)*E::from(end[0])).asFloat();
  result.point[2]=(oneMinusRatio*E::from(start[2])+E::from(ratio)*E::from(end[2])).asFloat();
  result.point[1]=((t*E::from(acceleration)+E::from(initialVerticalVelocity))*t
                   +E::from(start[1])).asFloat();
  return result;
 }
};
