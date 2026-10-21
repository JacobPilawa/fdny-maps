#pragma once
// Authored VictimRescue `Beckon`/`Rescue` resources from Data/ResDefs/victims.srf.
#include <array>
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <string>
#include "random.h"

struct VictimSoundProfile {
 std::array<std::string,3> beckonNames{};
 int beckonCount=0;
 std::string rescueName;
 float minBeckonDelay=0.f,maxBeckonDelay=0.f;
};

// The ordered names and delays follow Data/ResDefs/victims.srf. Fatman keeps
// the Man behavior/audio profile even when its renderer model is randomized.
inline VictimSoundProfile victimSoundProfile(const std::string& type) {
 std::string key=type;
 std::transform(key.begin(),key.end(),key.begin(),[](unsigned char ch){return char(std::tolower(ch));});
 VictimSoundProfile profile;
 if(key=="woman") {
  profile.beckonNames={"WomanBeckon1","WomanBeckon2","WomanBeckon3"};
  profile.beckonCount=3;profile.rescueName="WomanRescue";
  profile.minBeckonDelay=15.f;profile.maxBeckonDelay=30.f;return profile;
 }
 if(key=="man"||key=="fatman") {
  profile.beckonNames={"ManBeckon1","ManBeckon2","ManBeckon3"};
  profile.beckonCount=3;profile.rescueName="ManRescue";
  profile.minBeckonDelay=5.f;profile.maxBeckonDelay=10.f;return profile;
 }
 if(key=="girl") {
  profile.beckonNames={"GirlBeckon1","GirlBeckon2","GirlBeckon3"};
  profile.beckonCount=3;profile.rescueName="GirlRescue";
  profile.minBeckonDelay=5.f;profile.maxBeckonDelay=10.f;return profile;
 }
 if(key=="dog") {
  profile.beckonNames={"DogBark1","DogBark2","DogBark3"};
  profile.beckonCount=3;profile.rescueName="DogBark1";
  profile.minBeckonDelay=5.f;profile.maxBeckonDelay=10.f;return profile;
 }
 if(key=="herodown") {
  profile.beckonNames[0]="FighterBeckon";
  profile.beckonCount=1;profile.rescueName="FighterRescue";
  profile.minBeckonDelay=1.5f;profile.maxBeckonDelay=1.5f;return profile;
 }
 // boy, boy1, and the current host's unknown-type fallback use the first row.
 profile.beckonNames={"BoyBeckon1","BoyBeckon2","BoyBeckon3"};
 profile.beckonCount=3;profile.rescueName="BoyRescue";
 profile.minBeckonDelay=5.f;profile.maxBeckonDelay=10.f;return profile;
}

inline std::string victimRescueSoundName(const std::string& type) {
 return victimSoundProfile(type).rescueName;
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
