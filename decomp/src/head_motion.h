#pragma once
#include "extended.h"
#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>

// Head bob and transition state from 00491460/00491140. The five records
// below are the exact defaults written by the original constructor.
struct HeadMotion {
 using E=Extended;
 struct Mode {
  float xAmplitude,yAmplitude,xRate,yRate;
  float xPositive,yPositive,xNegative,yNegative;
  int xCurve,yCurve;
  float transitionRate;
 };
 struct Output {float x=0,y=0,z=0;};
 std::array<Mode,5> modes{{
  {0,2,0,2,1,1,1,1,0,1,4},
  {8,10,8,16,1,1,1,.5f,0,1,16},
  {0,0,0,0,0,0,0,0,0,1,10},
  {10,10,12,16,1,1,1,.5f,0,1,16},
  {4,7,8,15,1,1,1,.5f,0,1,6}
 }};
 float phaseX=8,phaseY=15,phaseZ=4;
 int currentMode=0;
 bool shakeActive=false,upwardTrend=false;
 float shakeAmplitude=0,shakePhase=0,shakeDuration=.1f;
 std::array<float,3> previousY{};

 static E constant(double value){return E::from(value);}
 static int compare(E a,E b){E difference=a-b;if(!difference.m)return 0;return difference.negative?-1:1;}
 static E absolute(E value){value.negative=false;return value;}
 static E fromLongDouble(long double value){
  if(value==0)return {};
  bool negative=std::signbit(value);int exponent=0;
  long double fraction=std::frexp(std::abs(value),&exponent);
  constexpr int digits=std::numeric_limits<long double>::digits;
  auto mantissa=(E::U)std::ldexp(fraction,digits);
  return E::pack(mantissa,exponent-digits,negative);
 }
 static float trigProduct(float phase,float amplitude,bool sine){
  long double wave=sine?std::sin((long double)phase):std::cos((long double)phase);
  if(amplitude==0)return std::signbit(wave)?-0.0f:0.0f;
  E product=fromLongDouble(wave)*E::from(amplitude);
  return product.asFloat();
 }
 static float multiply(float a,float b){return (E::from(a)*E::from(b)).asFloat();}
 static float add(float a,float b){return (E::from(a)+E::from(b)).asFloat();}

 // 0044F340 selects one of 00491140's five records from player state,
 // the normal-view-active byte, and the movement flag supplied by 00452570.
 static int viewMode(int playerState,bool active,bool moving){
  if(playerState==5)return moving?4:0;
  if(!active)return 2;
  if(!moving)return 0;
  return playerState==4?3:1;
 }

 // Port of 00490E80. Its phase interval is [-2π, 2π], its target choice is
 // based on the current phase, and crossing a target clamps but completes on
 // the following call after the 0.0001-radian test.
 static bool transitionAngle(float& angle,float step,int curve){
  constexpr double tau=0x1.921fb54442d18p+2;
  constexpr double minusTau=-0x1.921fb54442d18p+2;
  constexpr double halfPi=0x1.921fb54442d18p+0;
  constexpr double pi=0x1.921fb54442d18p+1;
  constexpr double threeHalfPi=0x1.2d97c7f3321d2p+2;
  constexpr double epsilon=.0001;
  E phase=E::from(angle),tauE=constant(tau),minusTauE=constant(minusTau);
  if(compare(tauE,phase)<0){while(compare(tauE,phase)<0)phase=phase-tauE;}
  if(compare(phase,minusTauE)<0){while(compare(phase,minusTauE)<0)phase=phase+tauE;}
  angle=phase.asFloat();
  E p=E::from(angle),half=constant(halfPi),piE=constant(pi),three=constant(threeHalfPi),eps=constant(epsilon);
  auto near=[&](E target){return compare(absolute(p-target),eps)<0;};
  auto advance=[&](float delta){angle=(E::from(angle)+E::from(delta)).asFloat();};
  auto retreat=[&](float delta){angle=(E::from(angle)-E::from(delta)).asFloat();};
  if(curve==0){
   if(near(three)||near(half))return true;
   if(compare(p,half)<0){advance(step);if(compare(E::from(angle),half)>=0)angle=1.57079637050628662109375f;}
   else if(compare(p,piE)<0){retreat(step);if(compare(E::from(angle),half)<0)angle=1.57079637050628662109375f;}
   else if(compare(p,three)<0){advance(step);if(compare(E::from(angle),three)>=0)angle=4.7123889923095703125f;}
   else {retreat(step);if(compare(E::from(angle),three)<0)angle=4.7123889923095703125f;}
   return false;
  }
  if(curve!=1)throw std::runtime_error("Unknown original head-motion curve mode");
  if(near(piE)||near(constant(0)))return true;
  if(compare(p,half)<0){retreat(step);if(compare(E::from(angle),constant(0))<0)angle=0;}
  else if(compare(p,piE)<0){advance(step);if(compare(E::from(angle),piE)>0)angle=3.1415927410125732421875f;}
  else if(compare(p,three)<0){retreat(step);if(compare(E::from(angle),piE)<0)angle=3.1415927410125732421875f;}
  else {advance(step);if(compare(E::from(angle),tauE)>0)angle=0;}
  return false;
 }

 void triggerShake(float amplitude){shakeAmplitude=amplitude;shakePhase=0;shakeActive=true;}
 Output update(int requested,float dt){
  if(requested<0||requested>=int(modes.size()))throw std::runtime_error("Invalid original head-motion mode");
  if(currentMode==requested){
   const auto& mode=modes[currentMode];
   phaseX=(E::from(phaseX)+E::from(dt)*E::from(mode.xRate)).asFloat();
   phaseY=(E::from(phaseY)+E::from(dt)*E::from(mode.yRate)).asFloat();
  }else{
   const auto& target=modes[requested];const auto& old=modes[currentMode];
   float step=(E::from(dt)*E::from(target.transitionRate)).asFloat();
   bool xDone=transitionAngle(phaseX,step,old.xCurve);
   bool yDone=transitionAngle(phaseY,step,old.yCurve);
   if(xDone&&yDone)currentMode=requested;
  }
  const auto& mode=modes[currentMode];
  float x=trigProduct(phaseX,mode.xAmplitude,mode.xCurve!=0);
  float y=trigProduct(phaseY,mode.yAmplitude,mode.yCurve!=0);
  if(x<0)x=multiply(mode.xNegative,x);else if(x>0)x=multiply(mode.xPositive,x);
  if(y<0)y=multiply(mode.yNegative,y);else if(y>0)y=multiply(mode.yPositive,y);
  if(shakeActive){
   float shakeStep=(E::from(dt)/E::from(shakeDuration)+E::from(shakePhase)).asFloat();
   long double wave=std::sin((long double)shakeStep);
   E shake=fromLongDouble(wave)*E::from(shakeAmplitude);
   shakePhase=shakeStep;
   if(shake.negative)shake=shake*constant(.25);
   if(compare(E::from(shakeStep),constant(0x1.921fb54442d18p+2))>=0){shakeActive=false;shakePhase=0;}
   else y=(E::from(y)-shake).asFloat();
  }
  previousY[2]=previousY[1];previousY[1]=previousY[0];previousY[0]=y;
  upwardTrend=previousY[1]<y&&previousY[1]<previousY[2];
  return {x,y,0};
 }
};
