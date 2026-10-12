#pragma once
#include <map>
#include <functional>
#include "victim_rescue.h"
#include "victim_release.h"
#include "victim_pickup_effects.h"
#include "victim_frame_ops.h"
#include "victim_rescue_statistics.h"
#include "player_rescue_deadline.h"
#include "zone_membership.h"
#include "victim_exit.h"
#include "actor_animation.h"

struct VictimRuntimeActor : TrainingVictimReleaseActor {
 FrameTransform frame;
 ActorAnimation animation;
};
struct VictimTransactionState {
 VictimPickupPlayerState player;
 VictimPickupEffectState pickupEffects;
 RescueExitState exits;
 TrainingZoneMembership membership;
 std::map<int,VictimRuntimeActor> actors;
 // Source global-list order and verified exit binding/bounds inputs.
 std::vector<RescueExitEntity> entities;
 uint32_t deadline3420=0;
};

// Required source service boundaries; absent services fail explicitly rather
// than silently pretending that an engine/UI action is implemented.
struct VictimTransactionServices {
 std::function<const FrameTransform&()> playerFrame;
 VictimFrameOps::Trace traceFloor;
 std::function<void()> reportFloorFailure;
 std::function<uint32_t()> readClock;
 std::function<int()> readPlayerField18C,readGlobalMode;
 std::function<float()> readPlayerField343C;
 std::function<void(int,uint32_t)> setActorFlags;
 std::function<void(int,const char*)> dispatchVictimEvent;
 std::function<void(bool)> setEntityCarryMode;
 std::function<void(int)> updateVictimStatus; // source status actions 1/2
 std::function<void(const char*,uint32_t)> addStatistic,updateStatistic;
 std::function<void(uint32_t)> setRescueStatisticTotal;
 std::function<void(int)> reportUnzonedVictim,reportUnzonedExit,enableExitTrigger;
 std::function<void()> refreshSelectedItem;
};

// Composes already-delivered transactions. This object has no tick method:
// the caller must establish the original producer/dispatcher separately.
struct VictimTransactions {
 VictimTransactionState& state;
 VictimTransactionServices& services;
 const TrainingZones& zones;
 Random& random;

 void refreshMembership(){
  std::vector<ZoneMembershipActor> actors;
  for(const auto& entity:state.entities){
   uint32_t actorState=entity.type==2?state.actors.at(entity.entity).state:0;
   actors.push_back({entity.entity,entity.type,actorState});
  }
  rebuildTrainingZoneMembership(zones,actors,state.membership,
   [&](int entity){return state.actors.at(entity).frame.position;},
   [&](int entity){services.reportUnzonedVictim(entity);});
 }

 VictimPickupMessageResult pickup(int entity){
  if(state.exits.messageGate)return VictimPickupMessageResult::playerMessageGateClosed;
  if(state.player.carriedVictimEntity)return VictimPickupMessageResult::playerAlreadyCarrying;
  auto& actor=state.actors.at(entity);
  return deliverVictimPickupMessage(0x7532,true,state.player,actor,
   [&](int id,uint32_t flags){services.setActorFlags(id,flags);},
   [&](int id){services.dispatchVictimEvent(id,"pickup");},
   [&]{
    applyVictimPickupEffectsWithFieldReader([&]{return services.readPlayerField18C();},state.pickupEffects,
     [&](bool mode){services.setEntityCarryMode(mode);},
     [&]{services.updateVictimStatus(1);},
     [&]{refreshMembership();},
     [&]{return services.readClock();});
   });
 }

 struct StatisticOps {
  VictimTransactions& runtime;
  uint32_t readPickupClock(){return runtime.state.pickupEffects.pickupClock;}
  uint32_t readClock(){return runtime.services.readClock();}
  void addStatistic(const char* name,uint32_t value){runtime.services.addStatistic(name,value);}
  void updateStatistic(const char* name,uint32_t value){runtime.services.updateStatistic(name,value);}
 };
 struct ExitOps {
  VictimTransactions& runtime;
  void refreshMembership(){runtime.refreshMembership();}
  void setRescueStatisticTotal(uint32_t value){runtime.services.setRescueStatisticTotal(value);}
  void reportExitWithoutZone(int entity){runtime.services.reportUnzonedExit(entity);}
  void refreshSelectedItem(){runtime.services.refreshSelectedItem();}
  void enableExitTrigger(int entity){runtime.services.enableExitTrigger(entity);}
 };
 struct ReleaseOps : VictimFrameOps {
  VictimTransactions& runtime;
  VictimRuntimeActor& actor;
  ReleaseOps(VictimTransactions& owner,VictimRuntimeActor& target)
   :VictimFrameOps(target.frame,owner.services.playerFrame,owner.services.traceFloor,
                   owner.services.reportFloorFailure),runtime(owner),actor(target){}
  void recordRescueStatistics(TrainingVictimReleaseState&){
   StatisticOps ops{runtime};recordTrainingRescueStatistics(runtime.state.exits.rescues,ops);
  }
  void setWorldActorFlags(TrainingVictimReleaseActor& target,uint32_t flags){runtime.services.setActorFlags(target.entity,flags);}
  void dispatchRescue(TrainingVictimReleaseActor& target){runtime.services.dispatchVictimEvent(target.entity,"rescue");}
  uint32_t readClock(){return runtime.services.readClock();}
  void resetActorSubobject(TrainingVictimReleaseActor&){actor.animation.playing=false;} // 00480D40
  void updateVictPickPath(TrainingVictimReleaseState&){runtime.services.updateVictimStatus(2);}
  void playerRescuePostEffect(TrainingVictimReleaseState&){
   float field=runtime.services.readPlayerField343C();
   int mode=runtime.services.readGlobalMode();
   uint32_t now=runtime.services.readClock();
   setPlayerRescueDeadline(runtime.state.deadline3420,field,mode,now,runtime.random);
  }
  void resetPlayerRescue(){runtime.services.setEntityCarryMode(false);}
  void checkExits(TrainingVictimReleaseState&){
   ExitOps ops{runtime};processRescueExits(runtime.state.exits,runtime.state.entities,runtime.zones,ops);
  }
 };
 bool escape(){
  if(!state.player.carriedVictimEntity)return false;
  auto& actor=state.actors.at(*state.player.carriedVictimEntity);
  ReleaseOps ops(*this,actor);
  return trainingEscapeRelease(state.player,actor,ops);
 }
};
