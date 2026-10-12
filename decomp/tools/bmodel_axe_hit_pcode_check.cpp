#include <json-c/json.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../src/bmodel_axe_hit.h"

using J=json_object*;
static J field(J o,const char*k){J v=nullptr;if(!json_object_object_get_ex(o,k,&v))throw std::runtime_error(std::string("Missing field: ")+k);return v;}
static uint32_t u32(J v){return static_cast<uint32_t>(json_object_get_uint64(v));}
static std::string str(J v){return json_object_get_string(v);}
static bool sameStrings(J expected,const std::vector<std::string>&actual){if(json_object_array_length(expected)!=actual.size())return false;for(size_t i=0;i<actual.size();i++)if(str(json_object_array_get_idx(expected,int(i)))!=actual[i])return false;return true;}
static bool sameU32(J expected,const std::vector<uint32_t>&actual){if(json_object_array_length(expected)!=actual.size())return false;for(size_t i=0;i<actual.size();i++)if(u32(json_object_array_get_idx(expected,int(i)))!=actual[i])return false;return true;}
int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("Usage: bmodel_axe_hit_pcode_check fixture.json");
 J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original 0043E2A0 fixture");
 if(str(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("BModel Axe-hit fixture executable hash mismatch");
 J samples=field(fixture,"samples");int mismatches=0,serviceCalls=0;
 for(int i=0;i<json_object_array_length(samples);i++){
  J row=json_object_array_get_idx(samples,i),in=field(row,"input");std::string name=str(field(row,"name"));
  BModelAxeHit::State state;state.hitpoints=u32(field(in,"hitpoints"));state.scriptActor=u32(field(in,"script_actor"));state.rootAddress=u32(field(row,"model_address"));uint32_t bits=u32(field(in,"strike_time_bits"));std::memcpy(&state.strikeTime,&bits,sizeof bits);
  bool chop=u32(field(in,"chop_result"))!=0,breaking=u32(field(in,"break_result"))!=0;int scriptCalls=0,destroyCalls=0;std::vector<uint32_t>constructedRefs,scriptReceivers,scriptRefs,destroyReceivers;
  J expectedConstructed=field(row,"constructed_refs");uint32_t staticStringRef=json_object_array_length(expectedConstructed)?u32(json_object_array_get_idx(expectedConstructed,0)):0x00501908u;
  BModelAxeHit::Services services;
  services.constructString00402790=[&](const char*){constructedRefs.push_back(staticStringRef);return staticStringRef;};
  services.scriptEvent004705B0=[&](uint32_t receiver,uint32_t stringRef,const char*command){++scriptCalls;scriptReceivers.push_back(receiver);scriptRefs.push_back(stringRef);return std::string(command)=="chop"?chop:breaking;};
  services.destroy0043d660=[&](uint32_t receiver){++destroyCalls;destroyReceivers.push_back(receiver);};
  auto out=BModelAxeHit::invoke(state,services);
  auto report=[&](const char*what,bool ok){if(!ok){++mismatches;if(mismatches<30)std::cerr<<name<<" mismatch: "<<what<<"\n";}};
  report("hitpoints after",state.hitpoints==u32(field(row,"hitpoints_after")));
  report("return flag bits",out.flags==u32(field(row,"result_flags")));
  report("ordered string/script/decrement/destroy behavior",sameStrings(field(row,"events"),out.events));
  report("string constructor reference address",sameU32(field(row,"constructed_refs"),constructedRefs));
  report("004705B0 receiver is BModel+0x24",sameU32(field(row,"script_receivers"),scriptReceivers));
  report("004705B0 string reference argument",sameU32(field(row,"script_refs"),scriptRefs));
  report("0043D660 receiver is BModel root",sameU32(field(row,"destroy_receivers"),destroyReceivers));
  report("script callback count",scriptCalls==int(u32(field(row,"script_calls"))));
  report("destruction callback count",destroyCalls==int(u32(field(row,"destroy_calls"))));serviceCalls+=scriptCalls+destroyCalls;
 }
 std::cout<<"BModel Axe hit leaf 0043E2A0: cases="<<json_object_array_length(samples)<<", script/destroy calls="<<serviceCalls<<", mismatches="<<mismatches<<" (receiver addresses, string refs, and event order checked; string construction, script interpreter, and destroy internals are explicit boundaries)\n";
 json_object_put(fixture);return mismatches?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
