#pragma once
// Original placed, unheaded fire path from 00448D60. World floor placement,
// named loop playback and Think services remain explicit host callbacks.
struct FireEntity {
 FireVisual visual;FireSuppression suppression;FireUpdate update;
 V origin{};int soundChoice=-1;float alpha=1,glowAlpha=1;
 void initialize(Random& shared,uint32_t now,int& dividerCycle,V authored,
  float radius,float height,float sprayTime,bool named,bool smoke,
  const std::function<void(FireVisual&)>& floor,
  const std::function<void(int)>& playLoop){
  dividerCycle++;if(dividerCycle>3)dividerCycle=0;update.divider=uint8_t(dividerCycle);
  visual.initialize(shared,smoke,now);
  soundChoice=named?int(shared.range(0,2.999f)):-1;
  suppression.strength=1;suppression.sprayed=false;suppression.hasSound=named;suppression.sprayTime=sprayTime;
  visual.position=authored;visual.sizes.add({float(double(radius)*.75),float(double(height)*.25),float(double(radius)*.75)},now,now);
  origin={authored.x,float(double(authored.y)+4),authored.z};
  if(floor)floor(visual);if(named)playLoop(soundChoice);
  if(double(suppression.sprayTime)<double(1e-28f)&&double(suppression.sprayTime)>double(-1e-28f)){
   double factor=(double(radius)-24.0)/(288.0-24.0);factor=std::clamp(factor,0.0,1.0);suppression.sprayTime=float(factor*3.0+1.0);
  }
 }
 void think(uint32_t now,float dt,const std::function<void()>& light,
  const std::function<void()>& bookkeeping,const std::function<void()>& advanceRoute){
  update.think(suppression,dt,light,bookkeeping,advanceRoute,[&]{visual.sizeUpdate(now);},
   [&](float scale){visual.sizes.scale={1,scale,1};},[&](float input){alpha=std::min(input,.999f);glowAlpha=std::min(float(double(alpha)*1.5),1.f);});
 }
 void spray(float dt,float influence,bool terminal,const std::function<void()>& countDoused,const std::function<void()>& stopSound){
  suppression.spray(dt,influence,terminal,countDoused,stopSound);
  if(!suppression.flameEnabled){visual.enabled=false;for(auto& a:visual.animations)a.active=false;}
 }
};
