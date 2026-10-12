#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <json-c/json.h>
#include "../src/player_fire_exposure.h"
#include "../src/player_pain_deadline.h"

static json_object* field(json_object* object,const char* name){
 json_object* value=nullptr;
 if(!json_object_object_get_ex(object,name,&value))throw std::runtime_error(std::string("Missing P-code field: ")+name);
 return value;
}
static float real(json_object* value){return float(json_object_get_double(value));}
static uint64_t integer(json_object* value){return json_object_get_uint64(value);}
static uint32_t bits(float value){uint32_t word;std::memcpy(&word,&value,sizeof(word));return word;}

int main(int argc,char** argv){
 try{
  if(argc!=2)throw std::runtime_error("Usage: player_fire_pcode-check original-player-pain-suffix-pcode.json");
  json_object* fixture=json_object_from_file(argv[1]);
  if(!fixture)throw std::runtime_error("Cannot read original P-code fixture");
  auto* samples=field(fixture,"samples");
  const int sampleCount=int(json_object_array_length(samples));
  const uint32_t seed=uint32_t(integer(field(fixture,"rng_seed")));
  int failures=0;
  for(int i=0;i<sampleCount;i++){
   auto* sample=json_object_array_get_idx(samples,i);
   Random random(seed);
   uint32_t deadline=uint32_t(integer(field(sample,"deadline_before")));
   int painCalls=0,statCalls=0,passoutCalls=0;
   bool due=updatePlayerPainDeadline(deadline,
       uint32_t(integer(field(sample,"now"))),random,
       [&](Random&){painCalls++;},[&]{statCalls++;});
   float exposure=real(field(sample,"exposure_before"));
   auto result=accumulatePlayerFireExposure(exposure,real(field(sample,"dt")),
       real(field(sample,"influence")),[&]{passoutCalls++;});
   bool mismatch=deadline!=uint32_t(integer(field(sample,"deadline_after")))||
       random.drawCount!=integer(field(sample,"original_rng_helper_calls"))||
       uint64_t(painCalls)!=integer(field(sample,"pain_stub_calls"))||
       uint64_t(statCalls)!=integer(field(sample,"stat_stub_calls"))||
       bits(exposure)!=uint32_t(integer(field(sample,"exposure_bits_after")))||
       uint64_t(passoutCalls)!=uint64_t(json_object_get_boolean(field(sample,"passout_pending")))||
       due!=(painCalls==1)||result.passoutTriggered!=(passoutCalls==1);
   if(mismatch){
    failures++;
    std::cerr<<"P-code mismatch sample="<<json_object_get_string(field(sample,"name"))
             <<" deadline="<<deadline<<" draws="<<random.drawCount
             <<" exposure_bits="<<bits(exposure)<<" passout="<<result.passoutTriggered<<"\n";
   }
  }
  json_object_put(fixture);
  std::cout<<"Player fire deadline/exposure: "<<sampleCount
           <<" original P-code suffix cases; zero-draw RC_Pain stub; mismatches="<<failures<<"\n";
  return failures?1:0;
 }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
