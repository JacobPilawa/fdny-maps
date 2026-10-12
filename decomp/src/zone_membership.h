#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "zones.h"

struct ZoneMembershipActor {
 int entity=-1;
 int type=0; // source actor +0x18
 uint32_t state=0; // victim state +0x120 when type==2
};

struct TrainingZoneMembership {
 // Keys are the exact, case-sensitive Training zone labels. Actor IDs in each
 // value retain the order in the source global actor list.
 std::map<std::string,std::vector<int>> actorsByZone;
 std::vector<int> actorsWithoutZone;
};

// Isolated port of TMapZoneManager::RebuildActorLists (00484260). The caller
// supplies actors in source global-list order and adapters for the source
// virtual position getter and unzoned-victim diagnostic.
template<class PositionGetter,class ReportUnzoned>
void rebuildTrainingZoneMembership(const TrainingZones& zones,
 const std::vector<ZoneMembershipActor>& actors,
 TrainingZoneMembership& membership,
 PositionGetter&& getActorPosition,
 ReportUnzoned&& reportUnzoned){
 membership.actorsByZone.clear();
 for(const auto& zone:zones.groups)membership.actorsByZone.emplace(zone.label,std::vector<int>{});
 membership.actorsWithoutZone.clear();

 for(const auto& actor:actors){
  if(actor.type!=2||actor.state==5||actor.state==6)continue;
  const V position=getActorPosition(actor.entity); // vtable slot +0x18
  const TrainingZone* zone=zones.lookup(position); // 00483E10
  if(!zone){
   membership.actorsWithoutZone.push_back(actor.entity);
   reportUnzoned(actor.entity);
   continue;
  }
  membership.actorsByZone.at(zone->label).push_back(actor.entity);
 }
}
