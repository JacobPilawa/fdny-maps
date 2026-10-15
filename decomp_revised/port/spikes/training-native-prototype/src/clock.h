#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>
#include "wide_integer.h"
// Integer arithmetic preserves the original x87 64-bit significand on ARM,
// where long double has the same precision as double. A double intermediate
// rounded 702.999999999999994 to 703 before the original truncating conversion.
using ClockWide=WideInt128;
inline ClockWide clockRound(ClockWide value,int precision=64){bool negative=value.isNegative();WideUInt128 n=value.magnitude();int bits=0;for(auto k=n;k;k>>=1)bits++;int shift=bits-precision;if(shift<=0)return value;auto quotient=n>>shift,remainder=n-(quotient<<shift),half=(WideUInt128(1)<<(shift-1));if(remainder>half||(remainder==half&&(quotient&1)))quotient++;return ClockWide(quotient<<shift,negative);}
inline ClockWide clockFixed(double value){return ClockWide(std::ldexp(value,60));}
inline float clockFloat(ClockWide value,int scale=60){return float(std::ldexp(double(clockRound(value,24)),-scale));}
inline ClockWide clockReciprocal(ClockWide divisor){WideUInt128 numerator=WideUInt128(1)<<120;WideUInt128 denominator=divisor.magnitude();WideUInt128 q=numerator/denominator;int bits=0;for(auto n=q;n;n>>=1)bits++;int shift=std::max(0,bits-64);WideUInt128 den=denominator<<shift;WideUInt128 result=numerator/den,remainder=numerator%den;if(remainder*2>den||(remainder*2==den&&(result&1)))result++;return ClockWide(result<<shift);}
// Original clock update 0042BF70, including its stall correction sign.
struct GameClock {
 uint32_t previous=0,current=0,origin=0,reserved=0;
 float dt=0,fps=0,filteredFps=0;bool paused=false,waiting=false;
 bool tick(uint32_t now){
  if(paused)return false;
  if(!waiting)previous=current;
  current=now-origin;uint32_t elapsed=current-previous;
  ClockWide seconds=clockRound(ClockWide(elapsed)*clockFixed(.001));dt=clockFloat(seconds);
  if(seconds<=clockFixed(1./60)){waiting=true;return false;}waiting=false;
  if(double(dt)>.1){ClockWide adjustment=clockRound(clockRound(clockFixed(.1)-clockFixed(dt))*1000);current=uint32_t(clockRound((ClockWide(current)<<60)-adjustment)/(ClockWide(1)<<60));origin=uint32_t(clockRound((ClockWide(origin)<<60)+adjustment)/(ClockWide(1)<<60));dt=.1f;}
  ClockWide rate=clockReciprocal(clockFixed(dt));fps=clockFloat(rate);
  ClockWide previousTerm=clockRound(clockFixed(filteredFps)*clockFixed(.95));ClockWide newTerm=clockRound(rate*clockFixed(.05));filteredFps=clockFloat(clockRound(previousTerm+newTerm),120);return true;
 }
};
