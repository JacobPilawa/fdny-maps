#pragma once
// Authored VictimRescue `Rescue` resources from Data/ResDefs/victims.srf.
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <string>

// Fatman keeps the ManRescue behavior/audio record even when its renderer
// model is selected from a different actor variant.
inline std::string victimRescueSoundName(const std::string& type) {
 std::string key=type;
 std::transform(key.begin(),key.end(),key.begin(),[](unsigned char ch){return char(std::tolower(ch));});
 if(key=="woman")return "WomanRescue";
 if(key=="man"||key=="fatman")return "ManRescue";
 if(key=="girl")return "GirlRescue";
 if(key=="dog")return "DogBark1";
 if(key=="herodown")return "FighterRescue";
 // boy, boy1, and the VictimRescue resource fallback use the first profile.
 return "BoyRescue";
}

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
