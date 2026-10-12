#pragma once
// Original ActorAnimator selection 00480EA0 and pose time 00480D50.
struct ActorAnimation {
 int motion=-1;float start=0,duration=0,speed=1;uint32_t started=0,period=0;bool playing=false;
 void select(int id,float begin,float end,float rate,uint32_t now){
  motion=id;start=begin;duration=float(double(end)-begin);speed=rate;started=now;
  period=uint32_t(uint64_t((double(end)-begin)/double(rate)*1000.0));playing=true;
 }
 float poseTime(uint32_t now)const{
  if(!playing)return 0;
  if(!period)throw std::runtime_error("Original actor animation has a zero millisecond period");
  uint32_t remainder=(now-started)%period;int32_t signedRemainder;std::memcpy(&signedRemainder,&remainder,4);
  return float(double(signedRemainder)*double(speed)*.001+double(start));
 }
};
