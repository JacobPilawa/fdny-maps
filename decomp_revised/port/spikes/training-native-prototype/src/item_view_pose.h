#pragma once
#include "extended.h"
#include <cmath>

// The base item update 0045FEC0 advances its animation clock and writes the
// view/aim offsets consumed by 0044F340. Actor and bone animation are separate.
struct ItemViewPose {
 using E=Extended;
 struct Result {
  float currentTime=0,previousTime=0;
  uint32_t animationFlag=0;
  V aimOffset{},viewOffset{};
 };

 static int compare(E a,E b){E difference=a-b;if(!difference.m)return 0;return difference.negative?-1:1;}
 static float multiply(float a,float b){
  if(a==0||b==0)return std::signbit(a)!=std::signbit(b)?-0.0f:0.0f;
  return (E::from(a)*E::from(b)).asFloat();
 }

 static Result update(float frameDt,float advance,float currentTime,uint32_t animationFlag){
  // 0045FEC0 performs x87 multiply/add and stores the candidate time to float
  // before its strict comparison against the double 1.0 constant.
  E candidate=E::from(frameDt)*E::from(advance)+E::from(currentTime);
  float next=candidate.asFloat();
  uint32_t nextFlag=animationFlag;
  if(compare(E::from(next),E::from(1.0))>0){next=0;nextFlag=0;}

  E phase=E::from(next)*E::from(3.0f)*E::from(0x1.2d97c7f3321d2p+2);
  long double angle=std::ldexp((long double)phase.m,phase.e);
  if(phase.negative)angle=-angle;
  float amplitude=float(std::sin(angle)); // x87 FSIN result stored as float
  E threePi=E::from(0x1.2d97c7f3321d2p+3);
  E twoPi=E::from(0x1.921fb54442d18p+2);
  E pi=E::from(0x1.921fb54442d18p+1);
  if(compare(phase,threePi)>0){
   amplitude=0;
  }else if(compare(phase,twoPi)>0||compare(phase,pi)<=0){
   amplitude=(E::from(amplitude)*E::from(0x1.999999999999ap-3)).asFloat();
  }
  Result result;result.currentTime=next;result.previousTime=currentTime;result.animationFlag=nextFlag;
  result.aimOffset={multiply(25.0f,amplitude),multiply(-100.0f,amplitude),multiply(0.0f,amplitude)};
  result.viewOffset={multiply(12.5f,amplitude),multiply(-7.5f,amplitude),multiply(10.0f,amplitude)};
  return result;
 }
};
