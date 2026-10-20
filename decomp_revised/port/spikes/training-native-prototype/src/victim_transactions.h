#pragma once
#include <map>
#include <functional>
#include <stdexcept>
#include "victim_rescue.h"
#include "victim_release.h"
#include "victim_pickup_effects.h"
#include "victim_frame_ops.h"
#include "victim_rescue_statistics.h"
#include "player_rescue_deadline.h"
#include "player_herodown_tail.h"
#include "zone_membership.h"
#include "victim_exit.h"
#include "actor_animation.h"

struct VictimRuntimeActor : TrainingVictimReleaseActor {
 uint32_t motionCount=0;
 FrameTransform frame;
 ActorAnimation animation;
};
struct VictimTransactionState {
 VictimPassoutPlayerState player;
 VictimPickupEffectState pickupEffects;
 RescueExitState exits;
 TrainingZoneMembership membership;
 std::map<int,VictimRuntimeActor> actors;
 // Source global-list order and verified exit binding/bounds inputs.
 std::vector<RescueExitEntity> entities;
 uint32_t deadline3420=0;
};

// Passout reaches several host-owned objects which are deliberately not
// inferred from Player's cached fields. Position +0x3400 and cached +0x4c
// remain distinct; the correction receiver is resolved again after callbacks.
struct VictimPassoutHostServices {
 std::function<V()> readPlayerPosition3400;
 std::function<void(V)> writePlayerPosition4c;
 std::function<void(int,uint32_t,float)> setVictimMotion;
 std::function<std::vector<PlayerHeroDownEntity>()> readCurrentEntities;
 std::function<void(const PlayerHeroDownSpawn&)> spawnHeroDown;
 std::function<void(int)> logFirefightersDown;
 std::function<uint32_t()> readCleanupState,readCleanupCollectionCount;
 std::function<void()> reportCleanupArrayBounds,refreshSelection,
  transitionCleanupSubobjectTo2,resetPlayerView,finishRescueStatus;
 std::function<int()> cleanupSubobjectMode;
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
 VictimPassoutHostServices passout;
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

 // Decoder for the stable victim pointer payload of source message 0x7532.
 // Handler gates precede payload access, matching the original Player route.
 VictimPickupMessageResult receivePickupPayload(const void* payload){
  if(state.exits.messageGate)return VictimPickupMessageResult::playerMessageGateClosed;
  if(state.player.carriedVictimEntity)return VictimPickupMessageResult::playerAlreadyCarrying;
  return pickup(static_cast<const VictimRuntimeActor*>(payload)->entity);
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
 RescueExitResult refreshFinalExitAvailability(){
  ExitOps ops{*this};
  return processRescueExits(state.exits,state.entities,zones,ops);
 }
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
   runtime.refreshFinalExitAvailability();
  }
 };
 bool escape(){
  if(!state.player.carriedVictimEntity)return false;
  auto& actor=state.actors.at(*state.player.carriedVictimEntity);
  ReleaseOps ops(*this,actor);
  return trainingEscapeRelease(state.player,actor,ops);
 }

 struct PassoutOps : VictimFrameOps {
  VictimTransactions& runtime;
  PassoutOps(VictimTransactions& owner,VictimRuntimeActor& target)
   :VictimFrameOps(target.frame,owner.services.playerFrame,owner.services.traceFloor,
                   owner.services.reportFloorFailure),runtime(owner){}
  void setMotion(VictimRuntimeActor& actor,uint32_t draw,float speed){
   runtime.services.passout.setVictimMotion(actor.entity,draw,speed);
  }
  void writePlayerPosition(const std::array<float,3>& position){
   runtime.services.passout.writePlayerPosition4c(V{position[0],position[1],position[2]});
  }
  void writePlayerPositionX(float){
   const auto& position=runtime.state.player.position;
   runtime.services.passout.writePlayerPosition4c(V{position[0],position[1],position[2]});
  }
  void copyPlayerFramePosition(VictimRuntimeActor& actor,VictimPassoutPlayerState& player){
   VictimFrameOps::copyPlayerFramePosition(actor,player);
  }
  void rebuildVictimFrameBasis(VictimRuntimeActor& actor){VictimFrameOps::rebuildVictimFrameBasis(actor);}
  void placeOnFloor(VictimRuntimeActor& actor){VictimFrameOps::placeOnFloor(actor);}
  void setWorldActorFlags(VictimRuntimeActor& actor,bool enabled){
   runtime.services.setActorFlags(actor.entity,enabled?0x400u:0u);
  }
  void dispatchPassout(VictimRuntimeActor& actor){runtime.services.dispatchVictimEvent(actor.entity,"passout");}
  void resetPlayerRescue(){runtime.services.setEntityCarryMode(false);}
  float readPassoutCorrection(){return 30.0f;} // 004E1698, source binary32.
  void correctDroppedFrameX(float correction){
   // 0044FF73 rereads Player+0x3534 after callbacks, then gets that receiver's
   // frame. There is no second source null check.
   const int entity=*runtime.state.player.carriedVictimEntity;
   FrameTransform& frame=runtime.state.actors.at(entity).frame;
   frame.position.x=(Extended::from(frame.position.x)-Extended::from(correction)).asFloat();
  }
 };

 struct PassoutTailOps {
  VictimTransactions& runtime;
  void spawnHeroDown(const PlayerHeroDownSpawn& spawn){runtime.services.passout.spawnHeroDown(spawn);}
  void setRescueStatisticTotal(uint32_t total){runtime.services.setRescueStatisticTotal(total);}
  void logFirefightersDown(int count){runtime.services.passout.logFirefightersDown(count);}
  uint32_t readCleanupState(){return runtime.services.passout.readCleanupState();}
  uint32_t readCleanupCollectionCount(){return runtime.services.passout.readCleanupCollectionCount();}
  void reportCleanupArrayBounds(){runtime.services.passout.reportCleanupArrayBounds();}
  int cleanupSubobjectMode(){return runtime.services.passout.cleanupSubobjectMode();}
  void transitionCleanupSubobjectTo2(){runtime.services.passout.transitionCleanupSubobjectTo2();}
  void refreshSelection(){runtime.services.passout.refreshSelection();}
  void resetPassoutPlayerView(){runtime.services.passout.resetPlayerView();}
  void finishPlayerRescueStatus(){runtime.services.passout.finishRescueStatus();}
 };

 VictimPassoutResult passout(uint8_t helperArgument){
  auto& host=services.passout;
  if(!host.readPlayerPosition3400||!host.writePlayerPosition4c||!host.setVictimMotion||
     !host.readCurrentEntities||!host.spawnHeroDown||!host.logFirefightersDown||
     !host.readCleanupState||!host.readCleanupCollectionCount||!host.reportCleanupArrayBounds||
     !host.cleanupSubobjectMode||!host.transitionCleanupSubobjectTo2||!host.refreshSelection||
     !host.resetPlayerView||!host.finishRescueStatus||!services.playerFrame||!services.traceFloor||
     !services.reportFloorFailure||!services.setActorFlags||!services.dispatchVictimEvent||
     !services.setEntityCarryMode||!services.readGlobalMode||!services.setRescueStatisticTotal)
   throw std::runtime_error("Missing source service for Training passout transaction");

  const V sourcePosition=host.readPlayerPosition3400();
  state.player.passoutPosition={sourcePosition.x,sourcePosition.y,sourcePosition.z};
  VictimRuntimeActor emptyActor;
  VictimRuntimeActor& victim=state.player.carriedVictimEntity
   ?state.actors.at(*state.player.carriedVictimEntity):emptyActor;
  PassoutOps ops(*this,victim);
  const auto result=applyVictimPassout(state.player,victim,random,ops);

  // Source checks mode/argument before enumerating the live entity list.
  const int mode=services.readGlobalMode();
  std::vector<PlayerHeroDownEntity> entities;
  if(mode==2&&helperArgument==0)entities=host.readCurrentEntities();
  continuePlayerHeroDownTail(state.player,state.exits,mode,char(helperArgument),entities,
   PassoutTailOps{*this});
  return result;
 }
};
