#include <json-c/json.h>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "../src/axe_child_think.h"

using J=json_object*;
static J field(J o,const char*k){J v=nullptr;if(!json_object_object_get_ex(o,k,&v))throw std::runtime_error(std::string("Missing field: ")+k);return v;}
static int integer(J v){return json_object_get_int(v);}
static uint32_t u32(J v){return static_cast<uint32_t>(json_object_get_uint64(v));}
static std::string str(J v){return json_object_get_string(v);}
static std::vector<float> floats(J a){std::vector<float> r;for(int i=0;i<json_object_array_length(a);++i)r.push_back(static_cast<float>(json_object_get_double(json_object_array_get_idx(a,i))));return r;}
static bool sameStrings(J expected,const std::vector<std::string>&actual){if(json_object_array_length(expected)!=actual.size())return false;for(size_t i=0;i<actual.size();i++)if(str(json_object_array_get_idx(expected,int(i)))!=actual[i])return false;return true;}
static float fromBits(uint32_t b){float f;std::memcpy(&f,&b,sizeof f);return f;}
static double doubleFromBits(uint64_t b){double d;std::memcpy(&d,&b,sizeof d);return d;}
static uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,sizeof b);return b;}
static std::string uevent(const char*name,const char*addr,uint32_t bits){return std::string(name)+":"+addr+":"+std::to_string(bits);}
int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("Usage: axe_child_think_pcode_check fixture.json");
 J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original Axe child Think P-code fixture");
 if(str(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Axe child Think fixture executable hash mismatch");
 J samples=field(fixture,"samples");int mismatch=0,serviceCalls=0,modelUpdates=0;
 for(int i=0;i<json_object_array_length(samples);i++){
  J row=json_object_array_get_idx(samples,i);std::string name=str(field(row,"name"));
  AxeChildThink::State s;s.active=integer(field(row,"active"))!=0;s.player345c=static_cast<uint8_t>(integer(field(row,"player_345c_before")));s.actionState=static_cast<uint32_t>(integer(field(row,"state")));
  auto cp=floats(field(row,"child_position")),pp=floats(field(row,"player_position"));s.childOrigin={cp[0],cp[1],cp[2]};s.playerOrigin={pp[0],pp[1],pp[2]};
  s.distanceThreshold=fromBits(u32(field(row,"distance_threshold_bits")));s.modelTimerThreshold=doubleFromBits(json_object_get_uint64(field(row,"model_threshold_bits")));s.modelUpdateArgument=u32(field(row,"model_update_argument"));
  J models=field(row,"models");for(int m=0;m<json_object_array_length(models);m++){J mr=json_object_array_get_idx(models,m);AxeChildThink::ModelTimer mt;mt.modelAddress=u32(field(mr,"model"));mt.hasFrame=integer(field(mr,"frame_present"))!=0;mt.frameTime=fromBits(u32(field(mr,"frame_time_bits")));mt.modelTime=fromBits(u32(field(mr,"model_time_bits")));mt.enabled=static_cast<uint8_t>(integer(field(mr,"enabled")));s.models.push_back(mt);}
  std::vector<std::string> events;std::vector<std::pair<uint32_t,uint32_t>> modelArguments;AxeChildThink::Services services;
  services.strike00460b90=[&](AxeChildThink::State&){events.emplace_back("strike:00460B90");};
  services.recovery0045fa60=[&](AxeChildThink::State&,float v){events.push_back(uevent("recovery:0045FA60","0",bits(v)));};
  services.visualScale0045fec0=[&](AxeChildThink::State&,float v){events.push_back(uevent("scale:0045FEC0","0",bits(v)));};
  services.playerGetterVtable18=[&](AxeChildThink::State&){events.emplace_back("player_getter:vtable+18");return s.playerOrigin;};
  services.modelUpdate0047f960=[&](size_t m,uint32_t model,uint32_t scene,AxeChildThink::ModelTimer&){char b[48];std::snprintf(b,sizeof b,"%u",model-u32(field(json_object_array_get_idx(models,0),"model")));events.push_back(uevent("model_update:0047F960",b,scene));modelArguments.emplace_back(model,scene);};
  auto result=AxeChildThink::think(s,services);
  auto report=[&](const char*what,bool ok){if(!ok){++mismatch;if(mismatch<30)std::cerr<<name<<" mismatch: "<<what<<"\n";}};
  report("Player+0x345C",s.player345c==integer(field(row,"player_345c")));
  report("actor +0x80",s.actorFlag80==integer(field(row,"actor_flag80")));
  report("model count",s.models.size()==json_object_array_length(models));
  for(size_t m=0;m<s.models.size()&&m<size_t(json_object_array_length(models));++m){J mr=json_object_array_get_idx(models,int(m));report(("model["+std::to_string(m)+"] +0x30 enabled byte").c_str(),s.models[m].enabled==integer(field(mr,"enabled")));report(("model["+std::to_string(m)+"] frame time bits").c_str(),bits(s.models[m].frameTime)==u32(field(mr,"frame_time_bits")));report(("model["+std::to_string(m)+"] model time bits").c_str(),bits(s.models[m].modelTime)==u32(field(mr,"model_time_bits")));report(("model["+std::to_string(m)+"] frame pointer presence").c_str(),s.models[m].hasFrame==(integer(field(mr,"frame_present"))!=0));}
  report("ordered service boundaries",sameStrings(field(row,"events"),events));serviceCalls+=int(events.size());
  report("model update argument count",modelArguments.size()==s.models.size());
  for(size_t m=0;m<modelArguments.size()&&m<s.models.size();++m){J mr=json_object_array_get_idx(models,int(m));report(("model update["+std::to_string(m)+"] receiver pointer").c_str(),modelArguments[m].first==u32(field(mr,"model")));report(("model update["+std::to_string(m)+"] scene argument").c_str(),modelArguments[m].second==u32(field(row,"model_update_argument")));}
  report("model update count",int(result.modelUpdates.size())==int(json_object_array_length(models)));modelUpdates+=int(result.modelUpdates.size());
 }
 std::cout<<"Training Axe child Think 004610E0: cases="<<json_object_array_length(samples)<<", ordered service boundaries="<<serviceCalls<<", model updates="<<modelUpdates<<", mismatches="<<mismatch<<" (strike/recovery/scale/player/model services intercepted; 004610E0 and model timer compare/store execute original P-code)\n";
 json_object_put(fixture);return mismatch?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
