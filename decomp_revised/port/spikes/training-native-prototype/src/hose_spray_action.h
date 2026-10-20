#pragma once
#include <cstdint>

// Hose Spray ACTION callbacks 004632D0/00463300. `now` is the shared
// elapsed game clock in milliseconds, not a rendered-frame counter.
struct HoseSprayAction {
 struct State {
  bool active=false;
  uint32_t effectDeadline=0;
 };

 static void begin(State& state,uint32_t nowMs){
  state.active=true;
  state.effectDeadline=nowMs+100u;
 }

 static void end(State& state){
  if(state.active)state.active=false;
 }
};
