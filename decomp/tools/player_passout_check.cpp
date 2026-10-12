#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <json-c/json.h>
#include "../src/player_passout.h"

static json_object* field(json_object* object,const char* name){
 json_object* value=nullptr;
 if(!json_object_object_get_ex(object,name,&value))throw std::runtime_error(std::string("Missing P-code field: ")+name);
 return value;
}
static uint64_t integer(json_object* value){return json_object_get_uint64(value);}

struct Ops {
 uint32_t now=0,activeIndex=0;
 std::vector<uint32_t> motionOffsets;
 std::vector<std::string> calls;
 int chatterCalls=0,heroDownCalls=0,activeCalls=0;
 uint32_t heroDownArgument=0,activeArgument=0;
 PlayerPassoutState* state=nullptr;
 void playPassoutMotion(uint32_t offset){calls.emplace_back("motion");motionOffsets.push_back(offset);}
 void playChatterSet(const char* key,bool shuffled){calls.emplace_back("chatter");chatterCalls++;if(std::string(key)!="RC_PassOut"||!shuffled)throw std::runtime_error("Bad RC_PassOut lookup arguments");}
 void runHeroDown(uint32_t arg){calls.emplace_back("herodown");heroDownCalls++;heroDownArgument=arg;}
 uint32_t readClock(){calls.emplace_back("clock");return now;}
 uint32_t readActiveObjectIndex(){calls.emplace_back("active-index");return activeIndex;}
 void invokeActiveObjectSlot1C(uint32_t index){calls.emplace_back("active-slot-1c");activeCalls++;activeArgument=index;if(state->state!=6||state->failedAt!=now)throw std::runtime_error("Active callback preceded Player failure stores");}
};

int main(int argc,char** argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: player_passout-check player-passout-pcode-oracle.json");
  json_object* fixture=json_object_from_file(argv[1]);
  if(!fixture)throw std::runtime_error("Cannot read original passout P-code fixture");
  auto* samples=field(fixture,"samples");const int sampleCount=int(json_object_array_length(samples));int failures=0;
  for(int i=0;i<sampleCount;i++){
   auto* sample=json_object_array_get_idx(samples,i);
   PlayerPassoutState player{uint32_t(integer(field(sample,"failed_at_before"))),int32_t(integer(field(sample,"state_before")))};
   Ops ops;ops.now=uint32_t(integer(field(sample,"clock")));ops.activeIndex=uint32_t(integer(field(sample,"active_object_index")));ops.state=&player;
   const auto result=applyPlayerPassout(player,int(integer(field(sample,"parameter"))),ops);
   std::vector<std::string> expectedOrder;
   const int parameter=int(integer(field(sample,"parameter")));
   if(parameter==0||parameter==1)expectedOrder.emplace_back("motion");
   expectedOrder.emplace_back("chatter");
   if(parameter==2)expectedOrder.emplace_back("herodown");
   else{
    expectedOrder.emplace_back("clock");expectedOrder.emplace_back("active-index");
    if(integer(field(sample,"active_object_index"))!=0)expectedOrder.emplace_back("active-slot-1c");
   }
   const bool delegated=result==PlayerPassoutResult::delegatedToHeroDown;
   const uint32_t expectedMotion=uint32_t(integer(field(sample,"motion_subobject_offset")));
   const bool hasMotion=json_object_get_boolean(field(sample,"motion_called"));
   const bool bad=player.failedAt!=uint32_t(integer(field(sample,"failed_at_after")))||
       player.state!=int32_t(integer(field(sample,"state_after")))||
       (ops.motionOffsets.size()==1?ops.motionOffsets[0]:0)!=expectedMotion||
       (ops.motionOffsets.size()==1)!=hasMotion||
       uint64_t(ops.chatterCalls)!=integer(field(sample,"chatter_calls"))||
       integer(field(sample,"chatter_flag"))!=1||
       uint64_t(ops.heroDownCalls)!=integer(field(sample,"herodown_calls"))||
       uint64_t(ops.activeCalls)!=integer(field(sample,"active_callback_calls"))||
       uint64_t(ops.activeCalls)!=integer(field(sample,"list_helper_calls"))||
       integer(field(sample,"string_constructor_calls"))!=1||
       ops.calls!=expectedOrder||
       delegated!=json_object_get_boolean(field(sample,"delegated_to_herodown"))||
       (ops.heroDownCalls&&ops.heroDownArgument!=integer(field(sample,"herodown_argument")))||
       (ops.activeCalls&&ops.activeArgument!=integer(field(sample,"active_callback_index")));
   if(bad){failures++;std::cerr<<"Passout P-code mismatch sample="<<json_object_get_string(field(sample,"name"))<<" state="<<player.state<<" failed_at="<<player.failedAt<<"\n";}
  }
  json_object_put(fixture);
  std::cout<<"Player passout routine: "<<sampleCount<<" original P-code cases; mismatches="<<failures<<"\n";
  return failures?1:0;
 }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
