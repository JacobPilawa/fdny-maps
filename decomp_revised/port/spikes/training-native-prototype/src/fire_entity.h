#pragma once
#include <algorithm>
// Original placed, unheaded fire path from 00448D60. World floor placement,
// named loop playback and Think services remain explicit host callbacks.
struct FireEntity {
 FireVisual visual;FireSuppression suppression;FireUpdate update;
 // UpdateFireRescueActor reads actor-subobject +0x104, which aliases root
 // +0x128 suppression strength, before passing this alpha input to +0xD0.
 V origin{};int soundChoice=-1;float alpha=1,glowAlpha=1;
 std::array<float,3> rgbTint{1.f,1.f,1.f};bool rgbOverride=false;
 void initialize(Random& shared,uint32_t now,int& dividerCycle,V authored,
  float radius,float height,float sprayTime,bool named,bool smoke,
  const std::function<void(FireVisual&)>& floor,
  const std::function<void(int)>& playLoop){
  dividerCycle++;if(dividerCycle>3)dividerCycle=0;update.divider=uint8_t(dividerCycle);
  visual.initialize(shared,smoke,now);
  rgbTint={1.f,1.f,1.f};rgbOverride=false;
  soundChoice=named?int(shared.range(0,2.999f)):-1;
  suppression.strength=1;suppression.sprayed=false;suppression.hasSound=named;
  suppression.visualComponentEnabled=1;suppression.sprayTime=sprayTime;
  visual.position=authored;visual.sizes.add({float(double(radius)*.75),float(double(height)*.25),float(double(radius)*.75)},now,now);
  origin={authored.x,float(double(authored.y)+4),authored.z};
  if(floor)floor(visual);if(named)playLoop(soundChoice);
  // 00448D60 replaces near-zero K_SprayTime with 1 + 3 * the clamped
  // 00447770 radius fraction over [24,288]. Host double math can differ at
  // the original x87 extended-precision edges.
  if(double(suppression.sprayTime)<double(1e-28f)&&double(suppression.sprayTime)>double(-1e-28f)){
   double factor=(double(radius)-24.0)/(288.0-24.0);factor=std::clamp(factor,0.0,1.0);suppression.sprayTime=float(factor*3.0+1.0);
  }
 }
 void setInitialStrength(float value){
  suppression.strength=std::clamp(value,0.f,1.f);
  const float opacity=suppression.strength>=.5f?1.f:suppression.strength*2.f;
  alpha=std::min(opacity,.999f);glowAlpha=std::min(float(double(alpha)*1.5),1.f);
  tintForStrength(suppression.strength);
 }
 void tintForStrength(float strength){
  constexpr float threshold=.8f; // pinned retail _DAT_004E1680
  if(strength>=threshold){rgbTint={1.f,1.f,1.f};rgbOverride=false;return;}
  const float t=strength/threshold,remaining=1.f-t;
  rgbTint={float(double(.9f)*remaining+double(t)),
           float(double(.3f)*remaining+double(t)),
           float(double(.1f)*remaining+double(t))};
  rgbOverride=true;
 }
 void think(uint32_t now,float dt,const std::function<void()>& light,
  const std::function<void()>& bookkeeping,const std::function<void()>& advanceRoute){
  update.think(suppression,dt,light,bookkeeping,advanceRoute,[&]{visual.sizeUpdate(now);},
   [&](float scale){visual.sizes.scale={1,scale,1};},
   [&](float input){alpha=std::min(input,.999f);glowAlpha=std::min(float(double(alpha)*1.5),1.f);},
   [&](float strength){tintForStrength(strength);});
 }
 void spray(float dt,float influence,bool terminal,const std::function<void()>& countDoused,const std::function<void()>& stopSound){
  suppression.spray(dt,influence,terminal,countDoused,stopSound);
  // Root +0x1C8 aliases the visual component's +0x9C enabled byte. The retail
  // visual updater returns while disabled, so stop future child updates/draws.
  visual.enabled=suppression.visualComponentEnabled!=0;
 }
};
