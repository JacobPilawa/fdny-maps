#include <json-c/json.h>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../src/axe_action_callbacks.h"

using J=json_object*;
static J field(J o,const char*k){J v=nullptr;if(!json_object_object_get_ex(o,k,&v))throw std::runtime_error(std::string("Missing field: ")+k);return v;}
static std::string str(J v){return json_object_get_string(v);}
static int integer(J v){return json_object_get_int(v);}
static bool boolean(J v){return integer(v)!=0;}
struct Context{bool primaryFlag{},secondaryFlag{};std::vector<std::string> calls;};
static void update(bool primary,AxeActionCallbacks::State&s,void*p){auto&c=*static_cast<Context*>(p);c.calls.emplace_back(primary?"child:primary:00460920":"child:secondary:00460920");if(primary?c.primaryFlag:c.secondaryFlag)s.dat00502e04=true;}
static void reparent(uint32_t follower,uint32_t parent,uint32_t mode,void*p){auto&c=*static_cast<Context*>(p);c.calls.emplace_back("reparent:"+std::to_string(follower)+":"+std::to_string(parent)+":"+std::to_string(mode));}
static bool sameStrings(J expected,const std::vector<std::string>&actual){if(json_object_array_length(expected)!=actual.size())return false;for(size_t i=0;i<actual.size();i++)if(str(json_object_array_get_idx(expected,int(i)))!=actual[i])return false;return true;}
int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("Usage: hose_axe_action_root_pcode_check fixture.json");
 J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original Axe ACTION-root fixture");
 if(str(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Axe ACTION-root fixture executable hash mismatch");
 J samples=field(fixture,"samples");int mismatch=0,serviceCalls=0;
 for(int i=0;i<json_object_array_length(samples);i++){
  J row=json_object_array_get_idx(samples,i);std::string name=str(field(row,"name"));std::string kind=str(field(row,"kind"));
  auto report=[&](const char*what,bool ok){if(!ok){++mismatch;if(mismatch<20)std::cerr<<name<<" mismatch: "<<what<<"\n";}};
  AxeActionCallbacks::State state;AxeActionCallbacks::Result result;
  if(kind=="press"){
   uint32_t clock=static_cast<uint32_t>(integer(field(row,"clock")));AxeActionCallbacks::press(state,clock,result);
   report("deadline",state.deadline==static_cast<uint32_t>(integer(field(row,"deadline"))));
  }else if(kind=="held"){
   Context context{boolean(field(row,"primary_flag")),boolean(field(row,"secondary_flag")),{}};
   state.primaryEnabled=boolean(field(row,"primary_enabled"));state.deadline=static_cast<uint32_t>(integer(field(row,"deadline")));
   AxeActionCallbacks::held(state,static_cast<uint32_t>(integer(field(row,"clock"))),update,reparent,&context,result);
   report("primary state",state.primaryState==static_cast<uint32_t>(integer(field(row,"primary_state"))));
   report("primary timer",state.primaryTimerBits==static_cast<uint32_t>(integer(field(row,"primary_timer_bits"))));
   report("secondary state",state.secondaryState==static_cast<uint32_t>(integer(field(row,"secondary_state"))));
   report("secondary timer",state.secondaryTimerBits==static_cast<uint32_t>(integer(field(row,"secondary_timer_bits"))));
   report("global DAT_00502E04",state.dat00502e04==boolean(field(row,"dat_flag")));
   report("ordered child/reparent boundary calls",sameStrings(field(row,"events"),context.calls));serviceCalls+=int(context.calls.size());
  }else throw std::runtime_error("Unknown fixture sample kind: "+kind);
 }
 std::cout<<"Training Axe ACTION root callbacks: cases="<<json_object_array_length(samples)<<", child/reparent boundary calls="<<serviceCalls<<", mismatches="<<mismatch<<" (child 00460920 and follower service 00483A20 are explicit boundaries; no strike modeled)\n";
 json_object_put(fixture);return mismatch?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
