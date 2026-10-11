#pragma once
// 0048D8F0: unsigned eligibility check, delay draw, sound choice, optional
// repeat-neighbor draw. Delay is in seconds; the routine converts to
// milliseconds and adds the unsigned original game time before truncating.
struct VictimSoundChoices{
 uint32_t deadline=0;int previous=-1;
 // 0048E080: the first deadline is 0..5000 milliseconds from the game
 // clock's origin, with no current-time addition. Previous sound starts at 0.
 void initialize(Random& shared){deadline=uint32_t(shared.range(0,5000));previous=0;}
 int update(Random& shared,uint32_t now,int count,float low,float high){
  if(deadline>now)return -1;
  deadline=uint32_t(uint64_t(shared.range(low,high)*1000.0+double(now)));
  previous=shared.soundIndex(count,previous);
  return previous>=0&&previous<count?previous:-1;
 }
};
