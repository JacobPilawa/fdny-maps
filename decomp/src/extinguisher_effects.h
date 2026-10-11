#pragma once
#include "extinguisher_pose.h"

// RNG and timer portion of the emitter request in 00461F80. Particle drawing
// is still a renderer task; this keeps its shared-stream draws in source order.
struct ExtinguisherEffects {
 struct SpawnRequest { V offset{}; };

 static Extended range(Random& random,float low,float high){
  return (Extended::from(high)-Extended::from(low))*Extended::from(random.unit())
       + Extended::from(low);
 }

 // Called after 00481A60 appends an accepted contact. The source pool is
 // configured with limit -1 by 0047FE70, so a normal successful allocation
 // always yields an emitter; a capped/busy pool can still return null.
 static bool afterContact(Random& random,uint32_t gameTime,float fireStrength,
                          uint32_t& nextEffectTime,bool emitterAvailable,
                          SpawnRequest& spawn){
  if(double(fireStrength)<0.05 || gameTime<nextEffectTime)return false;

  Extended deadline=Extended::integer(gameTime)+range(random,10.f,250.f);
  nextEffectTime=uint32_t(deadline.truncate());
  if(!emitterAvailable)return true;

  float first=range(random,-50.f,50.f).asFloat();
  float height=range(random,100.f,400.f).asFloat();
  float last=range(random,-50.f,50.f).asFloat();
  // 0042D280 receives the emitter offset as {last, height, first}.
  spawn.offset={last,height,first};
  return true;
 }
};
