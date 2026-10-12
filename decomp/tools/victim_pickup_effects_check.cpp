#include "../src/victim_pickup_effects.h"
#include "../src/victim_rescue.h"
#include <iostream>
#include <vector>
int main(){
 int failures=0,cases=0;
 for(int field:{-1,0,3,4,5,2147483647}){
  ++cases;VictimPickupEffectState effects{99,88,0,77};
  VictimPickupPlayerState player;VictimPickupActorState victim{191,4,0x400};
  std::vector<int> order;uint32_t clock=0xfffffffeu;
  auto result=deliverVictimPickupMessage(0x7532,true,player,victim,
   [&](int,uint32_t){order.push_back(0);},
   [&](int){order.push_back(1);if(effects.value!=99)++failures;},
   [&]{applyVictimPickupEffects(field,effects,
    [&](bool enabled){order.push_back(2);if(!enabled||victim.state!=5||effects.value!=99)++failures;},
    [&]{order.push_back(3);if(effects.value!=(field==4?20:50)||effects.clearedValue!=0||effects.enabled!=1)++failures;},
    [&]{order.push_back(4);if(sourceVictimZoneMember(2,victim.state))++failures;clock=1;},
    [&]{order.push_back(5);return clock;});});
  if(result!=VictimPickupMessageResult::accepted||effects.pickupClock!=1||order!=std::vector<int>{0,1,2,3,4,5})++failures;
  auto before=order;
  deliverVictimPickupMessage(0x7532,true,player,victim,[&](int,uint32_t){++failures;},[&](int){++failures;},[&]{++failures;});
  if(order!=before)++failures;
 }
 for(int type:{0,2,3})for(uint32_t state:{0u,4u,5u,6u,7u,0xffffffffu}){
  ++cases;bool expected=type==2&&(state==0||state==4||state==7||state==0xffffffffu);
  if(sourceVictimZoneMember(type,state)!=expected)++failures;
 }
 std::cout<<"Victim pickup effects: "<<cases<<" cases, mismatches="<<failures<<'\n';return failures?1:0;
}
