#include <json-c/json.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../src/axe_child_action_update.h"

using J=json_object*;
static J field(J o,const char*k){J v=nullptr;if(!json_object_object_get_ex(o,k,&v))throw std::runtime_error(std::string("Missing field: ")+k);return v;}
static uint32_t u32(J v){return static_cast<uint32_t>(json_object_get_uint64(v));}
static int integer(J v){return json_object_get_int(v);}
static std::string str(J v){return json_object_get_string(v);}
static bool sameStrings(J expected,const std::vector<std::string>&actual){if(json_object_array_length(expected)!=actual.size())return false;for(size_t i=0;i<actual.size();i++)if(str(json_object_array_get_idx(expected,int(i)))!=actual[i])return false;return true;}
static std::array<uint32_t,4> record(J a){std::array<uint32_t,4>x{};for(int i=0;i<4;i++)x[size_t(i)]=u32(json_object_array_get_idx(a,i));return x;}
static uint32_t bits(float f){uint32_t x;std::memcpy(&x,&f,sizeof x);return x;}
int main(int argc,char**argv){try{
 if(argc!=2)throw std::runtime_error("Usage: axe_child_action_update_pcode_check fixture.json");
 J fixture=json_object_from_file(argv[1]);if(!fixture)throw std::runtime_error("Cannot read original Axe child +0x18 P-code fixture");
 if(str(field(fixture,"exe_sha256"))!="4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")throw std::runtime_error("Axe child +0x18 fixture executable hash mismatch");
 J samples=field(fixture,"samples");int mismatches=0,serviceCalls=0;
 for(int i=0;i<json_object_array_length(samples);i++){
  J row=json_object_array_get_idx(samples,i);std::string name=str(field(row,"name"));AxeChildActionUpdate::State s;AxeChildActionUpdate::Globals g;
  s.childState=u32(field(row,"input_state"));s.strikeStart=u32(field(row,"input_start"));s.strikeSpan=u32(field(row,"input_span"));s.clock=u32(field(row,"input_clock"));g.statOnceInitialized=integer(field(row,"input_stat_once"))!=0;g.sharedHitFlag=static_cast<uint8_t>(integer(field(row,"input_shared_flag")));g.statRecord=record(field(row,"input_stat_record"));
  std::vector<std::string>events;AxeChildActionUpdate::Services services;
  services.stringConstruct00401d80=[&](const char*t){events.emplace_back(std::string("string_construct:00401D80:")+t);return AxeChildActionUpdate::StringRef{integer(field(row,"input_ctor_alias_stat_record"))!=0,static_cast<int32_t>(u32(field(row,"input_ctor_refcount")))};};
  services.statIncrement00486950=[&](const char*t){events.emplace_back(std::string("stat:00486950:0:")+t);};
  services.stringRelease004011c0=[&](){events.emplace_back("string_release:004011C0:Axe Hits");};
  services.hide004a11a0=[&](uint32_t receiver,uint8_t arg){events.emplace_back("hide:004A11A0:"+std::to_string(receiver)+":"+std::to_string(arg));};
  services.recovery0045fa60=[&](uint32_t receiver,float value){events.emplace_back("recovery:0045FA60:"+std::to_string(receiver)+":"+std::to_string(bits(value)));};
  AxeChildActionUpdate::update(s,g,services);
  auto report=[&](const char*what,bool ok){if(!ok){++mismatches;if(mismatches<30)std::cerr<<name<<" mismatch: "<<what<<"\n";}};
  report("child +0x368",s.childState==u32(field(row,"child_state")));
  report("DAT_00501918 one-time flag",g.statOnceInitialized==(integer(field(row,"stat_once"))!=0));
  report("DAT_00501908..+0x14 stat record",g.statRecord==record(field(row,"stat_record")));
  report("shared DAT_00502E04",g.sharedHitFlag==integer(field(row,"shared_flag")));
  report("ordered string/stat/hide/recovery service boundaries",sameStrings(field(row,"events"),events));serviceCalls+=int(events.size());
 }
 std::cout<<"Training Axe child +0x18 held-ACTION 00460920: cases="<<json_object_array_length(samples)<<", ordered service boundaries="<<serviceCalls<<", mismatches="<<mismatches<<" (string/stat/hide/recovery services are explicit; no strike or sound modeled)\n";
 json_object_put(fixture);return mismatches?1:0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 2;}}
