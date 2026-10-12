#pragma once
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
#include "zones.h"

struct RescueExitState {
 uint32_t total=0xffffffffu; // Player+0x3528, lazy unknown sentinel
 uint32_t field3530=0,rescues=0; // +0x3530 and +0x352C
 uint32_t messageGate=0,field5CC=0; // +0x3438 and +0x5CC
 std::string prompt; // +0x5D0
};
struct RescueExitEntity {
 int entity=-1,type=0;
 bool matchesFinalExit=false; // Result of source 0046ED70 binding tests.
 V boundsCenter{}; // 0044BE90, supplied by the source bounds adapter.
};
enum class RescueExitResult { survivorsRemaining,noExitZones,exitPrompt };

// 004504C0 and lazy count helper 0044FBA0. Entity order is source global-list
// order; binding classification and bounds-center calculation remain inputs.
// No meaning beyond source storage is assigned to messageGate or item refresh.
template<class Ops>
RescueExitResult processRescueExits(RescueExitState& state,
 const std::vector<RescueExitEntity>& entities,const TrainingZones& zones,Ops&& ops){
 ops.refreshMembership();
 if(state.total==0xffffffffu){
  state.total=0;
  for(const auto& entity:entities)if(entity.type==2)state.total++;
  ops.setRescueStatisticTotal(state.total); // 00486920
  ops.refreshMembership();
 }
 // Two SUBs then TEST/JG: positive wrapped int32 remainder returns.
 uint32_t remainder=state.total-state.field3530-state.rescues;
 if(remainder!=0&&(remainder&0x80000000u)==0)return RescueExitResult::survivorsRemaining;
 std::vector<int> triggers;
 std::vector<const TrainingZone*> exitZones;
 for(const auto& entity:entities){
  if(entity.type!=8||!entity.matchesFinalExit)continue;
  triggers.push_back(entity.entity); // Kept even when its zone is absent.
  const auto* zone=zones.lookup(entity.boundsCenter);
  if(!zone){ops.reportExitWithoutZone(entity.entity);continue;}
  if(std::find(exitZones.begin(),exitZones.end(),zone)==exitZones.end())exitZones.push_back(zone);
 }
 if(exitZones.empty()){
  state.messageGate=1;
  ops.refreshSelectedItem(); // 0044C360 is item selection, not a proven outro.
  return RescueExitResult::noExitZones;
 }
 state.prompt="All of the survivors have been rescued!  Exit through:\n";
 for(size_t i=0;i<exitZones.size();i++){
  state.prompt+=exitZones[i]->label;
  if(i+1<exitZones.size()){state.prompt+=",\n";if(i+2==exitZones.size())state.prompt+="or ";}
 }
 state.field5CC=0;
 for(int trigger:triggers)ops.enableExitTrigger(trigger); // 00440F50(true)
 return RescueExitResult::exitPrompt;
}
