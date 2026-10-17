#pragma once
#include "extinguisher_contacts.h"
#include "extinguisher_pose.h"

// RNG and timer portion of the emitter request in 00461F80. The renderer
// consumes the returned velocity at the contacted fire's frame origin.
struct ExtinguisherEffects {
 struct SpawnRequest { V offset{}; bool requested=false; };

 // 00449A40 accepts the four-word message-30000 contact and calls the fire
 // suppression path with terminal=false. Route and generation validation
 // stay with the message-queue owner. Terminal mode and its visual-component
 // gate are handled when the source terminal branch is requested separately.
 template<class Fire>
 static bool deliverContact(uint32_t message,const void* payload,float dt,Fire& fire){
  if(message!=ExtinguisherContacts::messageId)return false;
  const auto* contact=static_cast<const ExtinguisherContacts::Contact*>(payload);
  fire.spray(dt,contact->strength,false,[]{},[]{});
  return true;
 }

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
  // 0047F1B0 returns the particle's +0x7c velocity vector. 0042D280
  // receives it as {last, height, first}; acceleration at +0x88 is zeroed.
  spawn.offset={last,height,first};
  spawn.requested=true;
  return true;
 }
};
