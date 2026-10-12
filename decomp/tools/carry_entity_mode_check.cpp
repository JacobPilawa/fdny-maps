#include "../src/carry_entity_mode.h"
#include <cassert>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

struct RecordingOps{
 std::vector<std::string> calls;
 std::vector<std::optional<int>> models;
 std::vector<uint8_t> reportObservedModes;
 std::vector<CarryEntityModeEntity>* entities=nullptr;
 void reportMissingWorldModel(int entity){
  assert(entity==14&&entities);
  reportObservedModes.push_back(entities->at(4).type8Mode);
  calls.push_back("missing-model:14");
 }
 void setWorldModelFlags(std::optional<int> model,uint32_t flags){
  assert(flags==0&&entities);
  if(!model)assert(entities->at(4).type8Mode==0); // +0x165 store preceded the API call
  calls.push_back(model?"model-flags:"+std::to_string(*model):"model-flags:null");models.push_back(model);
 }
};

int main(){
 std::vector<CarryEntityModeEntity> entities{
  {10,3,0,0,0,0,0,17},
  {11,8,0,0,0,1,0,22},
  {12,8,0,0,0,0,7,23}, // type-8 gate closed
  {13,4,3,0,0,1,8,24}, // unrelated type
  {14,8,0,0,0,1,9,std::nullopt}, // source warning, but still mode/model call
  {15,3,7,0,9,0,0,25}
 };
 RecordingOps disableOps{{},{},{},&entities};
 auto disabled=applyCarryEntityMode(entities,uint8_t(0),disableOps);
 assert((disabled.type8EntitiesMissingModel==std::vector<int>{14}));
 assert((disableOps.calls==std::vector<std::string>{"model-flags:22","missing-model:14","model-flags:null"}));
 assert((disableOps.reportObservedModes==std::vector<uint8_t>{9})); // diagnostic precedes +0x165 store
 assert(entities[0].type3Mode==0&&entities[0].type3ObjectInitialized==1&&entities[0].type3ObjectMode==0);
 assert(entities[1].type8Mode==0&&entities[2].type8Mode==7);
 assert(entities[3].type3Mode==3&&entities[3].type8Mode==8); // irrelevant type untouched
 assert(entities[4].type8Mode==0&&entities[5].type3Mode==0&&entities[5].type3ObjectMode==0);

 // Enabled mode updates type-3 and gated type-8 fields in input list order;
 // the false-only world-model API is not called.
 entities[0].type3Mode=4;entities[0].type3ObjectMode=4;
 entities[1].type8Mode=0;entities[2].type8Mode=6;entities[4].type8Mode=0;
 RecordingOps enableOps{{},{},{},&entities};
 auto enabled=applyCarryEntityMode(entities,uint8_t(1),enableOps);
 assert(enabled.type8EntitiesMissingModel==std::vector<int>{14});
 assert((enableOps.calls==std::vector<std::string>{"missing-model:14"}));
 assert((enableOps.reportObservedModes==std::vector<uint8_t>{0}));
 assert(entities[0].type3Mode==1&&entities[0].type3ObjectInitialized==1&&entities[0].type3ObjectMode==1);
 assert(entities[1].type8Mode==1&&entities[2].type8Mode==6); // closed gate preserved
 assert(entities[4].type8Mode==1&&entities[5].type3Mode==1);
}
