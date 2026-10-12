#include <cmath>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <cassert>
struct V{float x=0,y=0,z=0;float& operator[](int i){return i==0?x:i==1?y:z;}float operator[](int i)const{return i==0?x:i==1?y:z;}};
#include "../src/extended.h"
#include "../src/victim_exit.h"
struct Ops{
 int refreshes=0,selected=0;uint32_t total=0;
 std::vector<int> enabled,unzoned;
 void refreshMembership(){refreshes++;}
 void setRescueStatisticTotal(uint32_t value){total=value;}
 void refreshSelectedItem(){selected++;}
 void reportExitWithoutZone(int entity){unzoned.push_back(entity);}
 void enableExitTrigger(int entity){enabled.push_back(entity);}
};
int main(){
 TrainingZones zones(TrainingZones::LabelMode::CaseSensitive);
 zones.addTrigger("A",1,{-1,-1,-1},{1,1,1});zones.addTrigger("B",2,{2,-1,-1},{4,1,1});
 std::vector<RescueExitEntity> actors{{191,2},{194,2},{7,1},{10,8,true,{0,0,0}},{11,8,true,{0,0,0}},{12,8,true,{3,0,0}},{13,8,true,{100,0,0}}};
 RescueExitState state;Ops first;
 assert(processRescueExits(state,actors,zones,first)==RescueExitResult::survivorsRemaining);
 assert(state.total==2&&first.total==2&&first.refreshes==2&&first.enabled.empty());
 state.rescues=2;Ops done;
 assert(processRescueExits(state,actors,zones,done)==RescueExitResult::exitPrompt);
 assert(done.refreshes==1&&done.selected==0&&done.unzoned==std::vector<int>{13});
 assert(done.enabled==std::vector<int>({10,11,12,13}));
 assert(state.prompt=="All of the survivors have been rescued!  Exit through:\nA,\nor B");
 // Training's authored escape triggers have no finalexit binding.
 RescueExitState training;training.total=2;training.rescues=2;Ops noExits;
 assert(processRescueExits(training,{{191,2},{194,2},{106,8,false}},zones,noExits)==RescueExitResult::noExitZones);
 assert(training.messageGate==1&&noExits.selected==1);
 // Source TEST/JG operates on the wrapped subtraction result.
 RescueExitState wrapped;wrapped.total=0;wrapped.rescues=1;Ops negative;
 assert(processRescueExits(wrapped,{},zones,negative)==RescueExitResult::noExitZones);
 wrapped.total=0x80000000u;wrapped.rescues=0;Ops high;
 assert(processRescueExits(wrapped,{},zones,high)==RescueExitResult::noExitZones);
}
