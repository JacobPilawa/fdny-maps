#pragma once
#include <cstdint>
#include <optional>
#include <vector>

// Raw fields used by the type-3 and type-8 branches of Player carry mode
// helper 0044FCE0. Inputs must follow the source global entity list order.
struct CarryEntityModeEntity {
 int entity=-1;
 int type=0; // entity +0x18

 uint8_t type3Mode=0; // entity +0x12D
 uint8_t type3ObjectInitialized=0; // entity model object +0x44
 uint8_t type3ObjectMode=0; // entity model object +0x45

 uint8_t type8ModeGate=0; // entity +0x164
 uint8_t type8Mode=0; // entity +0x165
 std::optional<int> type8WorldModel; // BoundsTrigger UserData model pointer; null models are source-reported
};

struct CarryEntityModeResult {
 std::vector<int> type8EntitiesMissingModel;
};

// 0044FCE0 walks entities in list order. Type 3 always receives 00446EC0,
// which writes +0x12D then calls 00497DE0 (+0x45=mode, +0x44=1). Type 8
// reaches 00440F50, which is gated by +0x164. Ops provides the missing-model
// diagnostic and the unported geWorld_ModelSetFlags(model,0) API.
template<class Ops>
CarryEntityModeResult applyCarryEntityMode(
 std::vector<CarryEntityModeEntity>& entities,uint8_t mode,Ops&& ops){
 CarryEntityModeResult result;
 for(auto& entity:entities){
  if(entity.type==3){
   entity.type3Mode=mode;
   entity.type3ObjectMode=mode;
   entity.type3ObjectInitialized=1;
  }else if(entity.type==8&&entity.type8ModeGate!=0){
   if(!entity.type8WorldModel){
    result.type8EntitiesMissingModel.push_back(entity.entity);
    ops.reportMissingWorldModel(entity.entity);
   }
   entity.type8Mode=mode;
   if(mode==0)ops.setWorldModelFlags(entity.type8WorldModel,uint32_t(0));
  }
 }
 return result;
}
