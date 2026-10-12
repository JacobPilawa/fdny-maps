#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <stdexcept>
struct V{float x=0,y=0,z=0;float& operator[](int i){return i==0?x:i==1?y:z;}float operator[](int i)const{return i==0?x:i==1?y:z;}};
#include <utility>
#include "../src/extended.h"
#include "../src/zone_membership.h"
#include <cassert>
#include <map>
#include <string>
#include <vector>

int main(){
 TrainingZones zones(TrainingZones::LabelMode::CaseSensitive);
 zones.addTrigger("A broad",1,{-10,-10,-10},{10,10,10});
 zones.addTrigger("B narrow",2,{-2,-2,-2},{2,2,2});
 zones.addTrigger("Alpha tie",3,{-1,-1,-1},{1,1,1});
 zones.addTrigger("Beta tie",4,{-1,-1,-1},{1,1,1});
 zones.addTrigger("Empty",5,{20,20,20},{22,22,22});
 // Equal volume uses the later case-sensitive tree entry (Beta tie).
 assert(zones.lookup({0,0,0})->label=="Beta tie");
 // The smallest summed member-box volume wins before tie breaking.
 assert(zones.lookup({1.5f,0,0})->label=="B narrow");
 // Box edges are included.
 assert(zones.lookup({2,0,0})->label=="B narrow");

 const std::vector<ZoneMembershipActor> actors{
  {10,2,4},{99,1,4},{11,2,5},{12,2,6},{13,2,4},{14,2,4},{15,2,4}
 };
 const std::map<int,V> positions{{10,{0,0,0}},{13,{0,0,0}},{14,{1.5f,0,0}},{15,{100,0,0}}};
 std::vector<int> queried,reported;
 TrainingZoneMembership result;
 result.actorsByZone["Empty"]={777};result.actorsWithoutZone={888};
 rebuildTrainingZoneMembership(zones,actors,result,[&](int id){queried.push_back(id);return positions.at(id);},[&](int id){reported.push_back(id);});
 assert((queried==std::vector<int>{10,13,14,15}));
 assert((reported==std::vector<int>{15}));
 assert((result.actorsWithoutZone==std::vector<int>{15}));
 assert((result.actorsByZone.at("Beta tie")==std::vector<int>{10,13}));
 assert((result.actorsByZone.at("B narrow")==std::vector<int>{14}));
 assert(result.actorsByZone.at("Empty").empty());

 // Rebuilding clears previous contents, retains empty zones, and reports an
 // eligible actor outside all zones without attempting a membership append.
 TrainingZoneMembership noZone;
 noZone.actorsByZone["stale"]={1};noZone.actorsWithoutZone={2};
 std::vector<int> noZoneReports;
 rebuildTrainingZoneMembership(zones,{{20,2,4}},noZone,[](int){return V{100,100,100};},[&](int id){noZoneReports.push_back(id);});
 assert(noZone.actorsByZone.count("stale")==0&&noZone.actorsByZone.at("Empty").empty());
 assert((noZone.actorsWithoutZone==std::vector<int>{20}));
 assert((noZoneReports==std::vector<int>{20}));
}
