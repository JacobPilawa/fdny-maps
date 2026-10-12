#pragma once

#include <cmath>
#include <cstdint>
#include <functional>
#include <vector>

// Training Axe child vtable +0x20 body (004610E0). This deliberately models
// one explicit invocation only: the native host has not established the
// original callback cadence. Strike, recovery/scale, player getter, and model
// update internals are named boundaries.
namespace AxeChildThink {

struct Vec3 { float x{}, y{}, z{}; };

struct ModelTimer {
 uint32_t modelAddress=0; // original model pointer carried by child +0x324 array
 bool hasFrame=false;       // model +0x168 != 0
 float frameTime=0.0f;      // *(float*)(frame +0x1D8)
 float modelTime=0.0f;      // model +0x16C
 uint8_t enabled=1;         // model +0x30, cleared by the timer test
};

struct State {
 bool active=false;         // child +0x158
 uint8_t player345c=0;      // Player +0x345C
 uint8_t actorFlag80=0x7f;  // *(child+0xEC)+0x80, seeded fixture initial value
 uint32_t actionState=0;    // child +0x368
 Vec3 childOrigin{};        // child +0x39C
 Vec3 playerOrigin{};       // getter result +0x28
 float distanceThreshold=0.0f; // DAT_004E1604
 double modelTimerThreshold=0.0; // DAT_004DFC78
 uint32_t modelUpdateArgument=0; // *(uint32_t*)(DAT_00503A78+4)
 std::vector<ModelTimer> models; // child +0x324 array, +0x328 count
};

struct Services {
 std::function<void(State&)> strike00460b90;
 std::function<void(State&,float)> recovery0045fa60;
 std::function<void(State&,float)> visualScale0045fec0;
 std::function<Vec3(State&)> playerGetterVtable18;
 std::function<void(size_t,uint32_t,uint32_t,ModelTimer&)> modelUpdate0047f960;
};

struct Result {
 bool entered=false;
 bool state1StrikeDispatched=false;
 bool state2RecoveryDispatched=false;
 bool state3ScaleDispatched=false;
 std::vector<size_t> modelUpdates;
 std::vector<size_t> modelTimersCleared;
};

inline Result think(State& state, const Services& services) {
 Result result;
 result.entered=true;
 if(!state.active)return result;

 state.player345c=0;
 state.actorFlag80=1;
 switch(state.actionState){
  case 0:
   break;
  case 1:
   result.state1StrikeDispatched=true;
   if(services.strike00460b90)services.strike00460b90(state);
   break;
  case 2: {
   state.actorFlag80=0;
   if(services.playerGetterVtable18)state.playerOrigin=services.playerGetterVtable18(state);
   // 0043B2D0 materializes the vector difference to float storage before
   // 004610E0 performs its x87 norm calculation.
   const float dx=state.childOrigin.x-state.playerOrigin.x;
   const float dy=state.childOrigin.y-state.playerOrigin.y;
   const float dz=state.childOrigin.z-state.playerOrigin.z;
   const long double distance=std::sqrt(static_cast<long double>(dx)*dx+static_cast<long double>(dy)*dy+static_cast<long double>(dz)*dz);
   if(distance>static_cast<float>(state.distanceThreshold)){
    result.state2RecoveryDispatched=true;
    if(services.recovery0045fa60)services.recovery0045fa60(state,0.75f);
   }
   break;
  }
  case 3:
   result.state3ScaleDispatched=true;
   if(services.visualScale0045fec0)services.visualScale0045fec0(state,1.0f);
   break;
  default:
   break;
 }

 // The source makes this pass for every active state, including state 0.
 for(size_t i=0;i<state.models.size();++i){
  if(services.modelUpdate0047f960)services.modelUpdate0047f960(i,state.models[i].modelAddress,state.modelUpdateArgument,state.models[i]);
  result.modelUpdates.push_back(i);
  auto& model=state.models[i];
  if(model.hasFrame&&std::fabs(static_cast<double>(model.frameTime)-model.modelTime)>state.modelTimerThreshold){
   model.enabled=0;
   result.modelTimersCleared.push_back(i);
  }
 }
 return result;
}

} // namespace AxeChildThink
