#pragma once
#include <cstdint>

// Hose Spray ACTION callbacks 004632D0/00463300.
// Sound dispatch is a separate engine service and is not modeled here.
struct HoseSprayAction {
 struct State {
  bool active=false;
  uint32_t effectDeadline=0;
 };

 static void begin(State& state,uint32_t now){
  state.active=true;
  state.effectDeadline=now+100u;
 }

 static void end(State& state){
  if(state.active)state.active=false;
 }
};
