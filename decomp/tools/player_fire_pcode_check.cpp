#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <json-c/json.h>
#include "../src/player_fire_message.h"

static json_object* field(json_object* object,const char* name){
 json_object* value=nullptr;
 if(!json_object_object_get_ex(object,name,&value))throw std::runtime_error(std::string("Missing P-code field: ")+name);
 return value;
}
static float real(json_object* value){return float(json_object_get_double(value));}
static uint64_t integer(json_object* value){return json_object_get_uint64(value);}
static uint32_t bits(float value){uint32_t word;std::memcpy(&word,&value,sizeof(word));return word;}

struct PassoutOps {
 uint32_t now=0;
 std::vector<std::string>* events=nullptr;
 void playPassoutMotion(uint32_t offset){if(offset!=0x430)throw std::runtime_error("Fire passout selected the wrong animator subobject");events->emplace_back("passout-motion");}
 void playChatterSet(const char* key,bool shuffled){if(std::string(key)!="RC_PassOut"||!shuffled)throw std::runtime_error("Fire passout selected the wrong chatter service");events->emplace_back("passout-chatter");}
 void runHeroDown(uint32_t){throw std::runtime_error("Fire passout unexpectedly delegated to HeroDown");}
 uint32_t readClock(){events->emplace_back("passout-clock");return now;}
 uint32_t readSelectedItemIndex(){events->emplace_back("selected-item-index");return 0;}
 void releaseSelectedItem(uint32_t){throw std::runtime_error("Fixture unexpectedly selected a tool");}
};

struct MessageOps {
 uint32_t now=0;
 float dt=0;
 int painCalls=0,statCalls=0,passoutCalls=0;
 bool statSawDeadline=false;
 Random* random=nullptr;
 uint32_t* painDeadline=nullptr;
 uint32_t expectedDeadline=0;
 std::vector<std::string> events;
 PassoutOps passoutOps;
 void turnTowardFire(){events.emplace_back("turn");}
 uint32_t readClock(){events.emplace_back("clock");return now;}
 void triggerPain(Random& shared){if(&shared!=random)throw std::runtime_error("Pain callback received a different RNG stream");painCalls++;events.emplace_back("pain");}
 void addSeriousBurns(){statCalls++;statSawDeadline=painDeadline&&*painDeadline==expectedDeadline;events.emplace_back("stat");}
 float readFrameDelta(){events.emplace_back("frame-delta");return dt;}
 void triggerPassout(PlayerPassoutState& player,int parameter){
  passoutCalls++;
  (void)applyPlayerPassout(player,parameter,passoutOps);
 }
};

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
   MessageOps ops;ops.now=uint32_t(integer(field(sample,"now")));
   ops.dt=real(field(sample,"dt"));ops.random=&random;ops.passoutOps.now=ops.now;ops.passoutOps.events=&ops.events;
   random.observer=[&](uint64_t,uint32_t){ops.events.emplace_back("deadline-rng");};
   PlayerPassoutState player{0,3};
   uint32_t deadline=uint32_t(integer(field(sample,"deadline_before")));
   ops.painDeadline=&deadline;ops.expectedDeadline=uint32_t(integer(field(sample,"deadline_after")));
   float exposure=real(field(sample,"exposure_before"));
   const auto result=handlePlayerFireMessage7531(player,deadline,exposure,
       real(field(sample,"influence")),random,ops);
   std::vector<std::string> expectedOrder{"turn","clock"};
   if(integer(field(sample,"pain_stub_calls"))){expectedOrder.emplace_back("pain");expectedOrder.emplace_back("deadline-rng");expectedOrder.emplace_back("stat");}
   expectedOrder.emplace_back("frame-delta");
   if(json_object_get_boolean(field(sample,"passout_pending")))
    expectedOrder.insert(expectedOrder.end(),{"passout-motion","passout-chatter","passout-clock","selected-item-index"});
   const bool passoutPending=json_object_get_boolean(field(sample,"passout_pending"));
   bool mismatch=deadline!=uint32_t(integer(field(sample,"deadline_after")))||
       random.drawCount!=integer(field(sample,"original_rng_helper_calls"))||
       uint64_t(ops.painCalls)!=integer(field(sample,"pain_stub_calls"))||
       uint64_t(ops.statCalls)!=integer(field(sample,"stat_stub_calls"))||
       bits(exposure)!=uint32_t(integer(field(sample,"exposure_bits_after")))||
       uint64_t(ops.passoutCalls)!=uint64_t(passoutPending)||
       result.ignoredFailedPlayer||result.painDeadlineUpdated!=(ops.painCalls==1)||
       result.exposure.passoutTriggered!=(ops.passoutCalls==1)||
       ops.events!=expectedOrder||
       (ops.statCalls&&(!ops.statSawDeadline||deadline!=uint32_t(integer(field(sample,"deadline_after")))))||
       (passoutPending&&(player.state!=6||player.failedAt!=ops.now));
   if(mismatch){
    failures++;
    std::cerr<<"P-code mismatch sample="<<json_object_get_string(field(sample,"name"))
             <<" deadline="<<deadline<<" draws="<<random.drawCount
             <<" exposure_bits="<<bits(exposure)<<" passout="<<result.exposure.passoutTriggered<<"\n";
   }
  }

  // 0045165B exits before facing, clock/RNG/stat reads, frame delta, or state
  // changes once Player is already in the failure state.
  Random failedRandom(seed);MessageOps failedOps;failedOps.random=&failedRandom;
  PlayerPassoutState failedPlayer{77,6};uint32_t failedDeadline=99;float failedExposure=.5f;
  const auto failedResult=handlePlayerFireMessage7531(failedPlayer,failedDeadline,
      failedExposure,.75f,failedRandom,failedOps);
  if(!failedResult.ignoredFailedPlayer||!failedOps.events.empty()||
     failedRandom.drawCount||failedPlayer.state!=6||failedDeadline!=99||failedExposure!=.5f){
   failures++;std::cerr<<"Failed-state 0x7531 guard mismatch\n";
  }

  json_object_put(fixture);
  std::cout<<"Player 0x7531 transaction: "<<sampleCount
           <<" original suffix cases plus failed-state guard; zero-draw RC_Pain stub; passout body composed from its separate oracle; mismatches="<<failures<<"\n";
  return failures?1:0;
 }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 2;}
}
