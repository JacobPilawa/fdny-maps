#pragma once
#include "tool_follow.h"
#include <array>
// Position/state part of 00483720; caller supplies the original world target.
struct Follower {
 V position{},previous{},threshold{20,1000,20},settling{.05f,0,.05f},smoothing{.006f,.009f,.006f};
 std::array<V,3> basis{{{1,0,0},{0,1,0},{0,0,1}}};
 unsigned flags=0x70;bool held=false,allow=false,resume=false;
 static double distance(float a,float b){return std::abs(double(a)-double(b));}
 bool beyond(V target)const{for(int a=0;a<3;a++)if((flags&(2u<<a))&&distance(target[a],position[a])>threshold[a])return true;return false;}
 void update(V target,float dt){
  if(!held){
   for(int a=0;a<3;a++)position[a]=(flags&(0x10u<<a))?ToolFollow::step(position[a],target[a],smoothing[a],previous[a],dt):target[a];
   bool ready=allow;for(int a=0;a<3;a++)ready=ready&&(flags&(0x10u<<a))&&distance(target[a],position[a])<.001;
   if(ready){held=true;resume=false;}
  }else if(resume||beyond(target)){
   for(int a=0;a<3;a++)position[a]=(flags&(2u<<a))?ToolFollow::step(position[a],target[a],settling[a],previous[a],dt):target[a];
   resume=beyond(target);
  }else position=target;
  previous=target;
 }
};
