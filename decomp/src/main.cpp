#include <SDL.h>
#include <OpenGL/gl.h>
#include <json-c/json.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <set>
#include <map>
#include <regex>
#include <string>
#include <vector>
#include <stdexcept>
#include "ui.h"
using J=json_object*;
J field(J j,const char* k){J v=nullptr;json_object_object_get_ex(j,k,&v);return v;}
J at(J j,int i){return json_object_array_get_idx(j,i);}
int count(J j){return j?json_object_array_length(j):0;}
float num(J j){return j?json_object_get_double(j):0;}
int integer(J j){return j?json_object_get_int(j):0;}
std::string str(J j){return j?json_object_get_string(j):"";}
#include "assets.h"
J load(const char* name){auto p=assetDirectory()+"/"+name;auto j=json_object_from_file(p.c_str());if(!j)throw std::runtime_error("Cannot load "+p);return j;}
#include "core.h"
#include "bmodel_contact.h"
#include "scripts.h"
#include "random.h"
#include "startup.h"
#include "spawn_queue.h"
#include "model_floor.h"
#include "world_startup.h"
#include "actor_animation.h"
#include "actor_pose.h"
#include "actor_renderer.h"
#include "fire_effects.h"
#include "fire_suppression.h"
#include "fire_update.h"
#include "fire_size.h"
#include "fire_placement.h"
#include "sprite_animation.h"
#include "extended.h"
#include "item_view_pose.h"
#include "head_motion.h"
#include "extinguisher_frame_record.h"
#include "fire_visual.h"
#include "fire_render_state.h"
#include "fire_entity.h"
#include "fire_floor.h"
#include "fire_renderer.h"
#include "fire_contact.h"
#include "extinguisher_samples.h"
#include "extinguisher_contacts.h"
#include "fire_message_queue.h"
#include "entity_id_registry.h"
#include "tool_follow.h"
#include "follower.h"
#include "view_update.h"
#include "extinguisher_pose.h"
#include "hose_spray_action.h"
#include "extinguisher_effects.h"
#include "victim_sound.h"
#include "victim_pickup.h"
#include "sound_queue.h"
#include "interaction_sphere.h"
#include "use_manager.h"
#include "clock.h"
struct Mesh{int model;std::vector<float> positions,uvs;GLuint texture=0;};
struct TrainingPropModel{int entity=0;std::string name;V center{};float radius=0;bool makeValve=false;};
struct TrainingEntityIdentity{
 int sourceId=0;J source=nullptr;std::string className;EntityIdRegistry::Handle route{};FireEntity* fire=nullptr;
};
int checkBModelContact(){
 struct Case{const char* name;V actorMin,actorMax;bool expected;bool nanModelMin=false;};
 constexpr V modelMin{10,20,30},modelMax{20,30,40};
 const float nan=std::numeric_limits<float>::quiet_NaN();
 const std::array<Case,13> cases{{
  {"interior",{12,22,32},{18,28,38},true},
  {"x-low-inclusive",{0,22,32},{5,28,38},true},
  {"x-high-inclusive",{25,22,32},{30,28,38},true},
  {"y-low-inclusive",{12,10,32},{18,15,38},true},
  {"y-high-inclusive",{12,35,32},{18,40,38},true},
  {"z-low-inclusive",{12,22,20},{18,28,25},true},
  {"z-high-inclusive",{12,22,45},{18,28,50},true},
  {"x-outside",{0,22,32},{4.99f,28,38},false},
  {"y-outside",{12,10,32},{18,14.99f,38},false},
  {"z-outside",{12,22,20},{18,28,24.99f},false},
  {"x-actor-max-nan",{12,22,32},{nan,28,38},false},
  {"y-actor-min-nan",{12,nan,32},{18,28,38},false},
  {"z-model-bound-nan",{12,22,32},{18,28,38},false,true}
 }};
 int mismatches=0;
 for(const auto& sample:cases){V lo=modelMin,hi=modelMax;if(sample.nanModelMin)lo.z=nan;bool actual=BModelContact::overlapsExpanded(lo,hi,sample.actorMin,sample.actorMax,5.f);if(actual!=sample.expected){mismatches++;std::cout<<"BModel contact mismatch: "<<sample.name<<"\n";}}
 struct ReachCase{const char* name;float authored;int hp,liftable;float motionEnd,expected;};
 const float eps=1.0e-28f;
 const std::array<ReachCase,10> reachCases{{
  {"no-hitpoints",0,0,0,.5f,0},
  {"liftable",20,2,1,1.f,20},
  {"zero-motion-end",20,2,0,0.f,20},
  {"inside-zero-tolerance",0,2,0,1.0e-29f,0},
  {"positive-motion-end",20,2,0,.5f,48},
  {"negative-motion-end",0,2,0,-.5f,48},
  {"positive-tolerance-edge",0,2,0,eps,48},
  {"negative-tolerance-edge",0,2,0,-eps,48},
  {"reach-equals-48",48,2,0,.5f,48},
  {"reach-above-48",50,2,0,.5f,50}
 }};
 for(const auto& sample:reachCases){float actual=BModelContact::effectiveReach(sample.authored,sample.hp,sample.liftable,sample.motionEnd);if(actual!=sample.expected){mismatches++;std::cout<<"BModel reach mismatch: "<<sample.name<<" actual="<<actual<<" expected="<<sample.expected<<"\n";}}
 float nanGate=BModelContact::effectiveReach(0,2,0,nan);if(nanGate!=48.f){mismatches++;std::cout<<"BModel reach mismatch: unordered motion end\n";}
 std::cout<<"Source-mapped BModel contact: "<<cases.size()<<" AABB boundary/NaN cases and "<<reachCases.size()+1<<" reach-gate cases; mismatches="<<mismatches<<"\n";
 return mismatches?48:0;
}
std::map<int,float> trainingBModelReaches(J entities,J motions){
 std::map<std::string,int> modelIdByName;
 for(int i=0;i<count(entities);i++){auto e=at(entities,i);if(str(field(e,"class"))!="%Model%")continue;auto p=field(e,"properties");std::string name=str(field(p,"%name%"));if(!name.empty())modelIdByName[name]=integer(field(p,"Model"));}
 std::map<int,float> knownMotionEnds;std::set<int> unknownMotionEnds;
 for(int i=0;i<count(motions);i++){auto motion=at(motions,i);int modelId=integer(field(motion,"model"));for(const char* channelName:{"translation","rotation"}){auto channel=field(motion,channelName);auto keys=field(channel,"keys");int keyCount=integer(field(channel,"keyCount"));if(keys&&json_object_get_type(keys)==json_type_array&&count(keys)>0){for(int k=0;k<count(keys);k++)knownMotionEnds[modelId]=std::max(knownMotionEnds[modelId],num(at(at(keys,k),0)));}else if(keyCount>0)unknownMotionEnds.insert(modelId);}}
 std::map<int,float> reaches;
 for(int i=0;i<count(entities);i++){auto e=at(entities,i);if(str(field(e,"class"))!="RCS_BModel")continue;auto p=field(e,"properties");auto parseFloat=[&](const char* name){const std::string value=str(field(p,name));char* end=nullptr;float parsed=std::strtof(value.c_str(),&end);return end==value.c_str()?0.f:parsed;};float authoredReach=parseFloat("E_Reach");int hitPoints=int(parseFloat("F_HitPoints")),liftable=int(parseFloat("G_Liftable"));float motionEnd=0;bool unknownEnd=false;
  if(hitPoints!=0&&liftable==0&&authoredReach<48.f){json_object_object_foreach(p,key,value){if(std::strncmp(key,"C_AnimModel",11)!=0)continue;std::string modelName=str(value);if(modelName.empty()||modelName=="<null>")continue;auto model=modelIdByName.find(modelName);if(model==modelIdByName.end())throw std::runtime_error("Cannot resolve Training BModel child model "+modelName);auto known=knownMotionEnds.find(model->second);if(known!=knownMotionEnds.end())motionEnd=std::max(motionEnd,known->second);if(unknownMotionEnds.count(model->second))unknownEnd=true;}
   if(motionEnd<1.0e-28f&&unknownEnd)throw std::runtime_error("Training BModel contact reach needs an unextracted child motion extent for entity "+std::to_string(integer(field(e,"index"))));
  }
  reaches[integer(field(e,"index"))]=BModelContact::effectiveReach(authoredReach,hitPoints,liftable,motionEnd);
 }
 return reaches;
}
int checkTrainingBModelContactAssets(){auto entities=load("entities.json");auto motions=load("motions.json");auto reaches=trainingBModelReaches(entities,motions);const std::map<int,float> expected{{112,50.f},{113,48.f},{212,48.f},{213,48.f}};int mismatches=0;for(const auto& item:expected){float actual=reaches.at(item.first);if(actual!=item.second){mismatches++;std::cout<<"Training BModel reach mismatch: entity="<<item.first<<" actual="<<actual<<" expected="<<item.second<<"\n";}else std::cout<<"Training BModel entity "<<item.first<<" effective reach="<<actual<<"\n";}return mismatches?49:0;}
int main(int argc,char** argv){try{
 if(argc>1&&std::string(argv[1])=="--asset-path"){std::cout<<assetDirectory()<<"\n";return 0;}
 if(argc>1&&std::string(argv[1])=="--bmodel-contact-check")return checkBModelContact();
 if(argc>1&&std::string(argv[1])=="--bmodel-contact-asset-check")return checkTrainingBModelContactAssets();
 auto entities=load("entities.json");auto world=load("world_mesh.json");Collision collision(load("collision.json"));Player player;int playerId=115;std::string hint;float hintRemaining=0;std::string selected="axe",previousSelected="axe",selectedBeforeHose="axe";std::set<std::string> equipment{"axe"};float extinguisherCharge=1;int hoseMode=0,attachedValve=-1;Scripts vm(load("scripts.json"));std::vector<Door> doors;std::vector<TrainingPropModel> propModels;
 auto bmodelReachByEntity=trainingBModelReaches(entities,load("motions.json"));
 for(int i=0;i<count(entities);i++){auto e=at(entities,i);auto props=field(e,"properties");if(str(field(e,"class"))=="RCS_BModel"&&str(field(props,"S_Script01"))=="*Door"){
 int mi=integer(field(e,"model"));V pivot{};auto models=field(collision.data,"models");for(int k=0;k<count(models);k++)if(integer(field(at(models,k),"index"))==mi)pivot=vec(field(at(models,k),"origin"));auto b=field(e,"bounds");doors.push_back({mi,vec(field(b,"mins")),vec(field(b,"maxs")),pivot,float(std::stof(str(field(props,"E_Reach"))))});}
 if(str(field(e,"class"))=="RCS_BoundsTrigger")collision.disabled.insert(integer(field(e,"model")));if(str(field(e,"class"))=="RCS_PlayerRescue"){playerId=integer(field(e,"index"));std::string pos=str(field(props,"ZZ_location"));sscanf(pos.c_str(),"%f %f %f",&player.p.x,&player.p.y,&player.p.z);}}
 vm.hint=[&](std::string text,float seconds){hint=text;hintRemaining=seconds;};
 vm.findEntity=[&](std::string name){if(name=="player"||name=="Player")return playerId;for(int i=0;i<count(entities);i++){auto e=at(entities,i);if(str(field(field(e,"properties"),"A_Name"))==name)return integer(field(e,"index"));}return -1;};
 vm.host=[&](int id,std::string command){auto [verb,args]=token(command);
 if(id==playerId){if(verb=="equip"){equipment.insert(args);return true;}if(verb=="select"){if(equipment.count(args))selected=args;return true;}if(verb=="touchhose"&&args=="25"){hoseMode=2;equipment.insert("hose");return true;}if(command=="toolcmd extinguisher refill"){extinguisherCharge=1;return true;}if(verb=="climb"){if(!player.climbing){player.climbing=true;player.v={};}return true;}if(verb=="unclimb"){player.climbing=false;return true;}if(verb=="setashdensity"){char* end=nullptr;float value=std::strtof(args.c_str(),&end);if(end!=args.c_str())player.setAshDensity(value);return true;}if(verb=="setrescuebot"){int value;if(std::sscanf(args.c_str(),"%i",&value)==1)player.rescueBotEnabled=value!=0;return true;}}
 if(verb=="makezone"||verb=="makevalve"||verb=="setscale"||verb=="settimescale"||verb=="sethitpoints"||verb=="openaway"||verb=="close"||verb=="flush")return true;
 return false;
 };
 auto startup=originalStartupOrder(entities);auto victimSpawns=originalVictimCandidates(startup);std::map<int,VictimSpawn*> victimByEntity;std::map<int,VictimSoundChoices> victimSounds;
 for(auto& victim:victimSpawns)victimByEntity.emplace(victim.entity,&victim);
 J trainingManAsset=load("actors/vic_man.json");int trainingManMotionCount=count(field(trainingManAsset,"motions"));
 if(trainingManMotionCount!=6)throw std::runtime_error("Training victim ACT motion count changed; review the original spawn branch");
 // PropModel instance fields are reconstructed from the saved Training BSP
 // entities in their original factory/class order. makevalve is the source
 // switch that enables the valve action query; Hose1 remains a touch prop.
 for(auto e:startup)if(str(field(e,"class"))=="RCS_PropModel"){
  auto p=field(e,"properties");TrainingPropModel prop;prop.entity=integer(field(e,"index"));prop.name=str(field(p,"A_Name"));prop.center=location(p,"ZZ_location");prop.radius=std::stof(str(field(p,"F_TouchRadius")));prop.makeValve=str(field(p,"S_Script01"))=="makevalve";propModels.push_back(prop);
 }
 std::vector<UseManager::Candidate> valveCandidates;for(const auto& prop:propModels)if(prop.makeValve)valveCandidates.push_back({prop.entity,prop.center,prop.radius});
 vm.loadLevel("training.evs",playerId,true);WorldStartup worldStartup;
 auto startupTick=[&]{worldStartup.update([&]{vm.loadLevel("training.evs",playerId,false);},[]{});};
 // The loaded BSP supplies this batch. Gameplay constructors and the later
 // camera batch will use the same dispatcher as those systems are recovered.
 SpawnQueue startupQueue;std::map<int,J> startupEntities;std::map<int,TrainingEntityIdentity> trainingEntities;std::vector<TrainingEntityIdentity*> fireContactCandidates;EntityIdRegistry trainingRoutes;
 // 0042AB00 queues the World factory item first (spawn id 0x2711); its slot-2
 // callback is 0049E300. The factory callbacks then follow the BSP batch order.
 J worldSource=nullptr;for(int i=0;i<count(entities);i++){auto e=at(entities,i);if(str(field(e,"class"))=="<untyped>"){worldSource=e;break;}}
 TrainingEntityIdentity worldIdentity{0x2711,worldSource,"World"};trainingRoutes.registerObject(&worldIdentity,worldIdentity.route);
 // Original application seeds from timeGetTime (0042C800). Use host monotonic
 // milliseconds; full startup draw ordering is still being integrated.
 uint32_t seed=uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());Random sharedRandom(seed);int fireDivider=0;std::map<int,FireEntity> fires;
 startupQueue.registerEntity=[&](int id){auto& identity=trainingEntities.at(id);trainingRoutes.registerObject(&identity,identity.route);};
 startupQueue.firstPass=[&](int id){auto e=startupEntities.at(id);auto entityClass=str(field(e,"class"));if(entityClass=="RCS_FireRescue"){
 auto p=field(e,"properties");V position;auto location=str(field(p,"ZZ_location"));sscanf(location.c_str(),"%f %f %f",&position.x,&position.y,&position.z);
 auto value=[&](const char* key){return std::stof(str(field(p,key)));};bool floor=value("L_FloorMe")!=0;auto& fire=fires[id];
 fire.initialize(sharedRandom,0,fireDivider,position,value("H_Radius"),value("H_Height"),value("K_SprayTime"),!str(field(p,"A_Name")).empty(),value("M_Smoking")!=0,[&](FireVisual& visual){if(floor)FireFloor::alignInWorld(visual,collision);},[](int){});auto& identity=trainingEntities.at(id);identity.fire=&fire;fireContactCandidates.push_back(&identity);
 }else if(entityClass=="RCS_VictimRescue"){
  auto victim=victimByEntity.find(id);if(victim==victimByEntity.end()||victim->second->type!="man")throw std::runtime_error("Training victim type is not covered by the recovered actor mapping");
  // The shipped victims.srf maps "man" to model 12. Its preserved ACT has
  // six motions, selecting one bounded(3) initial motion in this pass.
  VictimSoundChoices sound;sound.initialize(sharedRandom);victimSounds.emplace(id,sound);
  // 0045B690 constructs the victim sound controller before choosing its first ACT motion.
  victim->second->initializeMotion(sharedRandom,trainingManMotionCount);
 }vm.startupEntity(e);};
 std::map<int,int> victimAppearanceCounters;const int trainingManMaterialCount=count(field(trainingManAsset,"materials"));
 startupQueue.secondPass=[&](int id){auto e=startupEntities.at(id);if(str(field(e,"class"))!="RCS_VictimRescue")return;auto victim=victimByEntity.find(id);if(victim==victimByEntity.end())throw std::runtime_error("Training victim missing from recovered candidate list");auto& spawn=*victim->second;spawn.choose(sharedRandom);spawn.floorPlacementAttempted=true;spawn.floorPlacementSucceeded=placeModelOnFloor(spawn.selectedPosition,spawn.modelMins,spawn.modelMaxs,[&](V start,V delta,V mins,V maxs,V& point){Hit hit;if(!collision.trace(start,delta,mins,maxs,hit))return false;point=hit.point;return true;},[&]{std::cerr<<"Error placing \""<<spawn.name<<"\": cannot find floor!\n";});
 // 0045D5B0 assigns the next source-ordered material for this model ID. This does not draw from shared RNG.
 int& nextAppearance=victimAppearanceCounters[spawn.modelId];spawn.appearanceIndex=nextAppearance<trainingManMaterialCount?nextAppearance:0;nextAppearance=spawn.appearanceIndex+1;};
 for(auto e:startup){int id=integer(field(e,"index"));startupEntities[id]=e;trainingEntities.emplace(id,TrainingEntityIdentity{id,e,str(field(e,"class"))});startupQueue.enqueue([id]{return id;});}
 startupQueue.drain();
 if(argc>1&&std::string(argv[1])=="--training-fire-floor-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/genesis-point-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing point collision evidence");auto samples=field(fixture,"samples");Collision original(collision.data);int mismatches=0;
 for(int i=0;i<7;i++){auto s=at(samples,i),e=field(s,"expected");FireVisual visual;visual.position=vec(field(s,"start"));visual.position.y-=50;bool hit=FireFloor::alignInWorld(visual,original);if(!hit||visual.position.y!=num(at(field(e,"point"),1))+3||visual.position.x!=num(at(field(s,"start"),0))||visual.position.z!=num(at(field(s,"start"),2)))mismatches++;}
 std::cout<<"Training fire floor placements: 7, mismatches: "<<mismatches<<"\n";return mismatches?26:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-floor-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-floor-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing fire floor evidence");auto samples=field(fixture,"samples");int mismatches=0;auto bits=[](float f){uint32_t b;std::memcpy(&b,&f,4);return b;};
 for(int i=0;i<count(samples);i++){auto s=at(samples,i);FireVisual visual;visual.position=vec(field(s,"origin"));for(int k=0;k<9;k++)visual.matrix[k]=num(at(field(s,"matrix"),k));bool different=false;
 FireFloor::align(visual,[&](V start,V end,int contents,int flags,Hit& hit){auto req=field(s,"request");different|=contents!=integer(field(req,"contents"))||flags!=integer(field(req,"flags"));for(int a=0;a<3;a++){different|=bits(start[a])!=uint32_t(json_object_get_uint64(at(field(req,"start_bits"),a)));different|=bits(end[a])!=uint32_t(json_object_get_uint64(at(field(req,"end_bits"),a)));}hit.point=vec(field(s,"contact"));hit.normal=vec(field(s,"normal"));return true;});
 for(int k=0;k<9;k++)different|=bits(visual.matrix[k])!=uint32_t(json_object_get_uint64(at(field(s,"result_matrix_bits"),k)));for(int a=0;a<3;a++)different|=bits(visual.position[a])!=uint32_t(json_object_get_uint64(at(field(s,"result_origin_bits"),a)));if(different){mismatches++;if(mismatches<6)std::cout<<"Fire floor mismatch "<<i<<"\n";}}
 std::cout<<"Fire floor: "<<count(samples)<<" original successful alignments; mismatches="<<mismatches<<"\n";return mismatches?25:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-entity-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-entity-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing fire entity evidence");auto samples=field(fixture,"samples");int mismatches=0;
 auto bits=[](float f){uint32_t b;std::memcpy(&b,&f,4);return b;};
 for(int i=0;i<count(samples);i++){auto s=at(samples,i);Random shared(uint32_t(json_object_get_uint64(field(s,"seed"))));FireEntity fire;int cycle=0,played=-1;fire.initialize(shared,100,cycle,{824,-120,-2092},num(field(s,"radius")),num(field(s,"height")),num(field(s,"spray")),integer(field(s,"named")),true,[](FireVisual&){},[&](int sound){played=sound;});bool different=shared.drawCount!=uint64_t(integer(field(s,"draws")))||fire.soundChoice!=integer(field(s,"choice"))||played!=integer(field(s,"choice"))||fire.update.divider!=integer(field(s,"divider"))||bits(fire.suppression.sprayTime)!=uint32_t(json_object_get_uint64(field(s,"spray_bits")))||bits(fire.suppression.strength)!=uint32_t(json_object_get_uint64(field(s,"strength_bits")));
 for(int a=0;a<3;a++){different|=bits(fire.origin[a])!=uint32_t(json_object_get_uint64(at(field(s,"position_bits"),a)));different|=bits(fire.visual.sizes.keys[fire.visual.sizes.first].size[a])!=uint32_t(json_object_get_uint64(at(field(s,"key_bits"),a)));}if(different){mismatches++;if(mismatches<6)std::cout<<"Fire entity mismatch "<<i<<"\n";}}
 std::cout<<"Fire entity: "<<count(samples)<<" original placed-fire initializations; mismatches="<<mismatches<<" (world/audio services supplied by fixtures)\n";return mismatches?24:0;
 }
 if(argc>1&&std::string(argv[1])=="--follower-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/follower-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing follower evidence");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto s=at(samples,i);Follower f;f.position=vec(field(s,"position"));f.previous=vec(field(s,"previous"));f.flags=integer(field(s,"flags"));f.held=integer(field(s,"held"));f.allow=integer(field(s,"allow"));f.resume=integer(field(s,"resume"));f.update(vec(field(s,"target")),num(field(s,"dt")));bool bad=false;for(int a=0;a<3;a++)bad|=ToolFollow::bits(f.position[a])!=uint32_t(json_object_get_uint64(at(field(s,"result_bits"),a)));auto state=field(s,"state");bad|=f.held!=bool(integer(at(state,0)))||f.allow!=bool(integer(at(state,1)))||f.resume!=bool(integer(at(state,2)));if(bad){mismatches++;if(mismatches<6)std::cout<<"Follower mismatch "<<i<<"\n";}}
 std::cout<<"Follower: "<<count(samples)<<" original states; mismatches="<<mismatches<<"\n";return mismatches?33:0;
 }
 if(argc>1&&std::string(argv[1])=="--head-motion-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/head-motion-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing head-motion evidence");auto sequences=field(fixture,"sequences");int mismatches=0,frames=0;std::map<std::string,int> mismatchByField;double maxError=0;uint32_t maxUlp=0;
 auto fromBits=[](J value){uint32_t b=uint32_t(json_object_get_uint64(value));float f;std::memcpy(&f,&b,4);return f;};
 auto compare=[&](uint32_t actual,uint32_t want,const std::string& label,int seq,int frame){if(actual==want)return;float a,b;std::memcpy(&a,&actual,4);std::memcpy(&b,&want,4);mismatches++;mismatchByField[label]++;maxError=std::max(maxError,std::abs(double(a)-b));auto ordered=[](uint32_t v){return (v&0x80000000u)?~v:(v|0x80000000u);};uint32_t oa=ordered(actual),ow=ordered(want),delta=oa>ow?oa-ow:ow-oa;maxUlp=std::max(maxUlp,delta);if(mismatches<8)std::cout<<"Head motion mismatch "<<label<<" sequence="<<seq<<" frame="<<frame<<" actual="<<a<<" expected="<<b<<" ulp="<<delta<<"\n";};
 for(int si=0;si<count(sequences);si++){HeadMotion motion;auto samples=field(at(sequences,si),"frames");for(int i=0;i<count(samples);i++){auto s=at(samples,i),expected=field(s,"result");auto shake=field(s,"shake_bits");if(shake&&json_object_get_type(shake)!=json_type_null)motion.triggerShake(fromBits(shake));auto output=motion.update(integer(field(s,"mode")),fromBits(field(s,"dt_bits")));auto wantOutput=field(expected,"output_bits");compare(ToolFollow::bits(output.x),uint32_t(json_object_get_uint64(at(wantOutput,0))),"output-x",si,i);compare(ToolFollow::bits(output.y),uint32_t(json_object_get_uint64(at(wantOutput,1))),"output-y",si,i);compare(ToolFollow::bits(output.z),uint32_t(json_object_get_uint64(at(wantOutput,2))),"output-z",si,i);auto phases=field(expected,"phase_bits");compare(ToolFollow::bits(motion.phaseX),uint32_t(json_object_get_uint64(at(phases,0))),"phase-x",si,i);compare(ToolFollow::bits(motion.phaseY),uint32_t(json_object_get_uint64(at(phases,1))),"phase-y",si,i);compare(ToolFollow::bits(motion.phaseZ),uint32_t(json_object_get_uint64(at(phases,2))),"phase-z",si,i);if(motion.currentMode!=integer(field(expected,"mode"))||motion.shakeActive!=bool(integer(field(expected,"shake_active")))||motion.upwardTrend!=bool(integer(field(expected,"trend")))){mismatches++;if(mismatches<8)std::cout<<"Head motion state mismatch sequence="<<si<<" frame="<<i<<"\n";}compare(ToolFollow::bits(motion.shakePhase),uint32_t(json_object_get_uint64(field(expected,"shake_phase_bits"))),"shake-phase",si,i);auto history=field(expected,"previous_y_bits");for(int k=0;k<3;k++)compare(ToolFollow::bits(motion.previousY[k]),uint32_t(json_object_get_uint64(at(history,k))),"history-y",si,i);frames++;}}
 std::cout<<"00491140 head motion: "<<frames<<" original updates; mismatches="<<mismatches<<" max absolute error="<<maxError<<" max ULP="<<maxUlp<<" (history storage is an output-only fixture)\n";for(auto& item:mismatchByField)std::cout<<item.first<<" differences="<<item.second<<"\n";return mismatches?36:0;
 }
 if(argc>1&&std::string(argv[1])=="--extinguisher-pose-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-pose-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing extinguisher pose evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;double maxError=0;uint32_t maxUlp=0;
 auto compare=[&](uint32_t actual,uint32_t want,int sample,const char* stage,int index){words++;if(actual==want)return;float a,b;std::memcpy(&a,&actual,4);std::memcpy(&b,&want,4);mismatches++;maxError=std::max(maxError,std::abs(double(a)-b));auto ordered=[](uint32_t v){return (v&0x80000000u)?~v:(v|0x80000000u);};uint32_t oa=ordered(actual),ow=ordered(want),delta=oa>ow?oa-ow:ow-oa;maxUlp=std::max(maxUlp,delta);if(mismatches<8)std::cout<<"Extinguisher pose mismatch "<<stage<<" sample="<<sample<<" word="<<index<<" actual="<<a<<" expected="<<b<<" ulp="<<delta<<"\n";};
 for(int i=0;i<count(samples);i++){auto s=at(samples,i),input=field(s,"input");FrameTransform parent;auto axes=field(input,"parent_axes");for(int k=0;k<9;k++)parent.axis[k/3][k%3]=num(at(axes,k));parent.position=vec(field(input,"parent_position"));auto local=ExtinguisherPose::local().words();auto expectedLocal=field(s,"local_bits");for(int k=0;k<16;k++)compare(local[k],uint32_t(json_object_get_uint64(at(expectedLocal,k))),i,"local",k);auto world=ExtinguisherPose::world(parent).words();auto expectedWorld=field(s,"world_bits");for(int k=0;k<16;k++)compare(world[k],uint32_t(json_object_get_uint64(at(expectedWorld,k))),i,"world",k);}
 std::cout<<"Extinguisher frame 004612B0/004977B0: "<<count(samples)<<" original parent poses, "<<words<<" words; mismatches="<<mismatches<<" max absolute error="<<maxError<<" max ULP="<<maxUlp<<"\n";return mismatches?37:0;
 }
 if(argc>1&&std::string(argv[1])=="--extinguisher-frame-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-frame-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing extinguisher frame-builder evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;double maxError=0;uint32_t maxUlp=0;
 auto compare=[&](uint32_t actual,uint32_t want,int sample,int index){words++;if(actual==want)return;float a,b;std::memcpy(&a,&actual,4);std::memcpy(&b,&want,4);mismatches++;maxError=std::max(maxError,std::abs(double(a)-b));auto ordered=[](uint32_t v){return (v&0x80000000u)?~v:(v|0x80000000u);};uint32_t oa=ordered(actual),ow=ordered(want),delta=oa>ow?oa-ow:ow-oa;maxUlp=std::max(maxUlp,delta);if(mismatches<80)std::cout<<"Extinguisher frame mismatch sample="<<sample<<" word="<<index<<" actual="<<a<<" expected="<<b<<" bits="<<std::hex<<actual<<"/"<<want<<std::dec<<" ulp="<<delta<<"\n";};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i),input=field(sample,"input");V direction=vec(field(input,"direction"));auto scaled=ExtinguisherPose::scaledDirection(direction);auto intermediate=field(field(sample,"intermediates"),"scaled_direction_bits");for(int k=0;k<3;k++)compare(ToolFollow::bits(scaled[k]),uint32_t(json_object_get_uint64(at(intermediate,k))),i,16+k);auto actual=ExtinguisherPose::fromVectors(vec(field(input,"position")),direction).words();auto expected=field(sample,"local_bits");for(int k=0;k<16;k++)compare(actual[k],uint32_t(json_object_get_uint64(at(expected,k))),i,k);}
 std::cout<<"Extinguisher local frame 004612B0: "<<count(samples)<<" original controlled poses, compared words="<<words<<"; mismatches="<<mismatches<<" max abs error="<<maxError<<" max ULP="<<maxUlp<<" (directions and positions are fixtures)\n";return mismatches?39:0;
 }
 if(argc>1&&std::string(argv[1])=="--extinguisher-spray-pose-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-spray-pose-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing extinguisher spray-pose evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;double maxError=0;uint32_t maxUlp=0;
 auto compare=[&](uint32_t actual,uint32_t want,int sample,const char* label,int index){words++;if(actual==want)return;float a,b;std::memcpy(&a,&actual,4);std::memcpy(&b,&want,4);mismatches++;maxError=std::max(maxError,std::abs(double(a)-b));auto ordered=[](uint32_t v){return (v&0x80000000u)?~v:(v|0x80000000u);};uint32_t oa=ordered(actual),ow=ordered(want),delta=oa>ow?oa-ow:ow-oa;maxUlp=std::max(maxUlp,delta);if(mismatches<8)std::cout<<"Extinguisher pose-phase mismatch "<<label<<" sample="<<sample<<" component="<<index<<" actual="<<a<<" expected="<<b<<" ulp="<<delta<<"\n";};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i),input=field(sample,"input"),expected=field(sample,"result");Random random(uint32_t(json_object_get_uint64(field(input,"seed"))));auto result=ExtinguisherPose::activeMotion(random,num(field(input,"dt")),num(field(input,"initial_phase")),vec(field(input,"position_offset")));compare(ToolFollow::bits(result.phase),uint32_t(json_object_get_uint64(field(expected,"phase_bits"))),i,"phase",0);auto direction=field(expected,"direction_bits"),position=field(expected,"position_bits");for(int k=0;k<3;k++){compare(ToolFollow::bits(result.direction[k]),uint32_t(json_object_get_uint64(at(direction,k))),i,"direction",k);compare(ToolFollow::bits(result.position[k]),uint32_t(json_object_get_uint64(at(position,k))),i,"position",k);}if(random.drawCount!=uint64_t(integer(field(expected,"draws")))){mismatches++;if(mismatches<8)std::cout<<"Extinguisher pose-phase RNG draw count mismatch sample="<<i<<" actual="<<random.drawCount<<" expected="<<integer(field(expected,"draws"))<<"\n";}}
 std::cout<<"00461F80 active pose-phase prefix: "<<count(samples)<<" original captures, compared words="<<words<<"; mismatches="<<mismatches<<" max abs error="<<maxError<<" max ULP="<<maxUlp<<" (0045F600 and 00482430 are return-stubbed; capture stops before 004977B0)\n";return mismatches?40:0;
 }
 if(argc>1&&std::string(argv[1])=="--extinguisher-pose-state-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-pose-state-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing extinguisher pose-state evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;double maxError=0;uint32_t maxUlp=0;
 auto compare=[&](uint32_t actual,uint32_t want,int sample,const char* label,int index){words++;if(actual==want)return;float a,b;std::memcpy(&a,&actual,4);std::memcpy(&b,&want,4);mismatches++;maxError=std::max(maxError,std::abs(double(a)-b));auto ordered=[](uint32_t v){return (v&0x80000000u)?~v:(v|0x80000000u);};uint32_t oa=ordered(actual),ow=ordered(want),delta=oa>ow?oa-ow:ow-oa;maxUlp=std::max(maxUlp,delta);if(mismatches<8)std::cout<<"Extinguisher callback mismatch "<<label<<" sample="<<sample<<" component="<<index<<" actual="<<a<<" expected="<<b<<" ulp="<<delta<<"\n";};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i),input=field(sample,"input"),expected=field(sample,"result");FrameTransform frame;auto axes=field(input,"frame_axes");for(int k=0;k<9;k++)frame.axis[k/3][k%3]=num(at(axes,k));frame.position=vec(field(input,"frame_position"));ExtinguisherPose::CallbackState state;state.cooldown=uint32_t(json_object_get_uint64(field(input,"old_cooldown")));auto action=str(field(input,"action"));state=action=="begin"?ExtinguisherPose::beginWindow(state,uint32_t(json_object_get_uint64(field(input,"tick"))),frame):ExtinguisherPose::endWindow(state,uint32_t(json_object_get_uint64(field(input,"tick"))),frame);compare(state.active,uint32_t(json_object_get_uint64(field(expected,"active"))),i,"active",0);compare(state.cooldown,uint32_t(json_object_get_uint64(field(expected,"cooldown"))),i,"cooldown",0);auto window=field(expected,"window");compare(state.start,uint32_t(json_object_get_uint64(at(window,0))),i,"window-start",0);compare(state.end,uint32_t(json_object_get_uint64(at(window,1))),i,"window-end",0);auto position=field(expected,"position_bits"),direction=field(expected,"direction_bits");for(int k=0;k<3;k++){compare(ToolFollow::bits(state.position[k]),uint32_t(json_object_get_uint64(at(position,k))),i,"position",k);compare(ToolFollow::bits(state.direction[k]),uint32_t(json_object_get_uint64(at(direction,k))),i,"direction",k);}}
 std::cout<<"004614FE/004615BC pose-window callbacks: "<<count(samples)<<" original captures, compared words="<<words<<"; mismatches="<<mismatches<<" max abs error="<<maxError<<" max ULP="<<maxUlp<<" (animation event dispatch is not integrated)\n";return mismatches?41:0;
 }
 if(argc>1&&std::string(argv[1])=="--hose-spray-action-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/hose-spray/action-callback-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing Hose Spray action-callback evidence");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i),input=field(sample,"input"),expected=field(sample,"result");HoseSprayAction::State state;state.active=integer(field(input,"active"))!=0;state.effectDeadline=uint32_t(json_object_get_uint64(field(input,"deadline")));auto action=str(field(input,"action"));uint32_t now=uint32_t(json_object_get_uint64(field(input,"now")));if(action=="begin")HoseSprayAction::begin(state,now);else HoseSprayAction::end(state);if(state.active!=(integer(field(expected,"active"))!=0)||state.effectDeadline!=uint32_t(json_object_get_uint64(field(expected,"deadline")))){mismatches++;if(mismatches<6)std::cout<<"Hose Spray action mismatch sample="<<i<<"\n";}}
 std::cout<<"Hose Spray ACTION callbacks 004632D0/00463300: "<<count(samples)<<" original cases; state mismatches="<<mismatches<<" (audio service excluded)\n";return mismatches?42:0;
 }
 if(argc>1&&std::string(argv[1])=="--extinguisher-frame-record-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-frame-record-inline-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing inline extinguisher frame-record evidence");auto samples=field(fixture,"samples");
 const std::array<int,8> offsets={0x10,0x14,0x34,0x38,0x58,0x5c,0x7c,0x80};int mismatches=0,words=0;double maxError=0;uint32_t maxUlp=0;
 auto fromBits=[](J value){return ExtinguisherFrameRecord::fromBits(uint32_t(json_object_get_uint64(value)));};
 for(int i=0;i<count(samples);i++){
  auto sample=at(samples,i),input=field(sample,"input");auto center=field(input,"center"),keysJson=field(input,"keys");std::array<int,4> keys{};for(int k=0;k<4;k++)keys[k]=integer(at(keysJson,k));
  ExtinguisherFrameRecord record;record.words[0x108/4]=601;
  record.update(integer(field(input,"width")),integer(field(input,"height")),0,0,integer(at(center,0)),integer(at(center,1)),fromBits(field(input,"rate_bits")),keys,fromBits(field(input,"progress_bits")));
  auto expected=field(sample,"result_words");for(int offset:offsets){uint32_t actual=record.words[offset/4],want=uint32_t(json_object_get_uint64(at(expected,offset/4)));words++;if(actual==want)continue;float a=ExtinguisherFrameRecord::fromBits(actual),b=ExtinguisherFrameRecord::fromBits(want);mismatches++;maxError=std::max(maxError,std::abs(double(a)-b));auto ordered=[](uint32_t v){return (v&0x80000000u)?~v:(v|0x80000000u);};uint32_t oa=ordered(actual),ow=ordered(want),delta=oa>ow?oa-ow:ow-oa;maxUlp=std::max(maxUlp,delta);if(mismatches<121)std::cout<<"Frame-record mismatch sample="<<i<<" offset=0x"<<std::hex<<offset<<std::dec<<" actual="<<a<<" expected="<<b<<" ulp="<<delta<<"\n";}
  words++;uint32_t actualStatus=record.words[0x100/4]&0xff,wantStatus=uint32_t(json_object_get_uint64(at(expected,0x100/4)))&0xff;if(actualStatus!=wantStatus){mismatches++;if(mismatches<121)std::cout<<"Frame-record visibility mismatch sample="<<i<<" actual="<<actualStatus<<" expected="<<wantStatus<<"\n";}
 }
 std::cout<<"00482430 bitmap-frame numeric fixture: "<<count(samples)<<" original cases; compared corner/visibility values="<<words<<"; mismatches="<<mismatches<<" max abs error="<<maxError<<" max ULP="<<maxUlp<<" (source UV words are zero; constructor state and live presentation are outside this check)\n";return mismatches?43:0;
 }
 if(argc>1&&std::string(argv[1])=="--view-update-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/view-update-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing view update evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0,matrixDiff=0,positionDiff=0,basisDiff=0,stateDiff=0;double maxError=0;
 auto fromBits=[](J value){uint32_t b=uint32_t(json_object_get_uint64(value));float f;std::memcpy(&f,&b,4);return f;};
 for(int i=0;i<count(samples);i++){
  auto sample=at(samples,i),input=field(sample,"input"),expected=field(sample,"result");FrameTransform root;root.position=vec(field(input,"player_position"));
  for(int a=0;a<9;a++)root.axis[a/3][a%3]=num(at(field(input,"player_basis"),a));
  Follower follower;follower.position=root.position;follower.previous=root.position;follower.flags=field(input,"follower_flags")?integer(field(input,"follower_flags")):0x2a;
  follower.threshold=field(input,"follower_thresholds")?vec(field(input,"follower_thresholds")):V{20,1000,20};
  follower.settling=field(input,"follower_settling")?vec(field(input,"follower_settling")):V{.05f,0,.05f};
  follower.smoothing=field(input,"follower_smoothing")?vec(field(input,"follower_smoothing")):V{0,.05f,0};
  follower.held=field(input,"follower_held")?bool(integer(field(input,"follower_held"))):false;
  follower.allow=field(input,"follower_allow")?bool(integer(field(input,"follower_allow"))):true;
  follower.resume=field(input,"follower_resume")?bool(integer(field(input,"follower_resume"))):false;
  auto head=field(expected,"head_motion_bits");V headMotion{fromBits(at(head,0)),fromBits(at(head,1)),fromBits(at(head,2))};
  V positionEndpoint=field(input,"position_endpoint")?vec(field(input,"position_endpoint")):V{0,-20,-30};
  V targetEndpoint=field(input,"target_endpoint")?vec(field(input,"target_endpoint")):V{-60,1000,0};
  int playerState=integer(field(input,"mode"));float height=playerState==4?10.f:70.f;
  auto result=ViewUpdate::transition(root,headMotion,vec(field(input,"tool_offset")),vec(field(input,"tool_rotation")),positionEndpoint,targetEndpoint,height,num(field(input,"fraction")),num(field(input,"angle")),num(field(input,"frame_dt")),follower,playerState==4);
  auto expectedMatrix=field(expected,"matrix_bits");auto actualMatrix=result.local.words();
  auto compare=[&](uint32_t actual,uint32_t want,int& category,const char* label,int sample,int fieldIndex){words++;if(actual==want)return;category++;mismatches++;float a,b;std::memcpy(&a,&actual,4);std::memcpy(&b,&want,4);maxError=std::max(maxError,std::abs(double(a)-b));if(mismatches<8)std::cout<<label<<" mismatch sample="<<sample<<" field="<<fieldIndex<<" actual="<<a<<" expected="<<b<<"\n";};
  for(int k=1;k<16;k++)compare(actualMatrix[k],uint32_t(json_object_get_uint64(at(expectedMatrix,k))),matrixDiff,"View matrix",i,k);
  auto expectedPos=field(expected,"follower_position_bits");for(int a=0;a<3;a++){uint32_t actual;std::memcpy(&actual,&result.follower.position[a],4);compare(actual,uint32_t(json_object_get_uint64(at(expectedPos,a))),positionDiff,"View follower position",i,a);}
  auto expectedBasis=field(expected,"follower_basis_bits");for(int k=0;k<9;k++){uint32_t actual;std::memcpy(&actual,&result.follower.basis[k/3][k%3],4);compare(actual,uint32_t(json_object_get_uint64(at(expectedBasis,k))),basisDiff,"View follower basis",i,k);}
  auto state=field(expected,"follower_state");bool different=result.follower.held!=bool(integer(at(state,0)))||result.follower.allow!=bool(integer(at(state,1)))||result.follower.resume!=bool(integer(at(state,2)));if(different){stateDiff++;mismatches++;if(mismatches<8)std::cout<<"View follower state mismatch sample="<<i<<"\n";}
 }
 std::cout<<"0044F340 timed-transition fixture: "<<count(samples)<<" original captures; "<<words<<" float words; differing matrix/position/basis/state="<<matrixDiff<<"/"<<positionDiff<<"/"<<basisDiff<<"/"<<stateDiff<<"; max abs error="<<maxError<<"\n";return mismatches?35:0;
 }
 if(argc>1&&std::string(argv[1])=="--view-item-offset-chain-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/view-item-offset-chain-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing item-offset/view chain evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;double maxError=0;
 auto fromBits=[](J value){uint32_t b=uint32_t(json_object_get_uint64(value));float f;std::memcpy(&f,&b,4);return f;};
 auto compare=[&](uint32_t got,uint32_t want,int sample,const char* label,int index){words++;if(got==want)return;float a,b;std::memcpy(&a,&got,4);std::memcpy(&b,&want,4);mismatches++;maxError=std::max(maxError,std::abs(double(a)-b));if(mismatches<8)std::cout<<label<<" mismatch sample="<<sample<<" field="<<index<<" actual="<<a<<" expected="<<b<<"\n";};
 for(int i=0;i<count(samples);i++){
  auto sample=at(samples,i),input=field(sample,"input"),offsets=field(sample,"item_offset_update"),expected=field(sample,"result");
  auto pose=ItemViewPose::update(num(field(input,"frame_dt")),num(field(input,"item_advance")),num(field(input,"item_initial_time")),7);
  compare(ToolFollow::bits(pose.currentTime),uint32_t(json_object_get_uint64(field(offsets,"current_time_bits"))),i,"Item current time",0);
  compare(ToolFollow::bits(pose.previousTime),uint32_t(json_object_get_uint64(field(offsets,"previous_time_bits"))),i,"Item previous time",0);
  auto itemView=field(offsets,"view_offset_bits"),itemAim=field(offsets,"aim_offset_bits");for(int k=0;k<3;k++){compare(ToolFollow::bits(pose.viewOffset[k]),uint32_t(json_object_get_uint64(at(itemView,k))),i,"Item view offset",k);compare(ToolFollow::bits(pose.aimOffset[k]),uint32_t(json_object_get_uint64(at(itemAim,k))),i,"Item aim offset",k);}
  FrameTransform root;root.position=vec(field(input,"player_position"));for(int k=0;k<9;k++)root.axis[k/3][k%3]=num(at(field(input,"player_basis"),k));
  Follower follower;follower.position=root.position;follower.previous=root.position;follower.flags=integer(field(input,"follower_flags"));follower.threshold=vec(field(input,"follower_thresholds"));follower.settling=vec(field(input,"follower_settling"));follower.smoothing=vec(field(input,"follower_smoothing"));follower.held=bool(integer(field(input,"follower_held")));follower.allow=bool(integer(field(input,"follower_allow")));follower.resume=bool(integer(field(input,"follower_resume")));
  auto head=field(expected,"head_motion_bits");V headMotion{fromBits(at(head,0)),fromBits(at(head,1)),fromBits(at(head,2))};
  auto result=ViewUpdate::transition(root,headMotion,pose.viewOffset,pose.aimOffset,vec(field(input,"position_endpoint")),vec(field(input,"target_endpoint")),70.f,num(field(input,"fraction")),num(field(input,"angle")),num(field(input,"frame_dt")),follower,false);
  auto matrix=field(expected,"matrix_bits");auto actualMatrix=result.local.words();for(int k=1;k<16;k++)compare(actualMatrix[k],uint32_t(json_object_get_uint64(at(matrix,k))),i,"View matrix",k);
  auto position=field(expected,"follower_position_bits");for(int k=0;k<3;k++)compare(ToolFollow::bits(result.follower.position[k]),uint32_t(json_object_get_uint64(at(position,k))),i,"View follower position",k);
  auto basis=field(expected,"follower_basis_bits");for(int k=0;k<9;k++)compare(ToolFollow::bits(result.follower.basis[k/3][k%3]),uint32_t(json_object_get_uint64(at(basis,k))),i,"View follower basis",k);
  auto state=field(expected,"follower_state");if(result.follower.held!=bool(integer(at(state,0)))||result.follower.allow!=bool(integer(at(state,1)))||result.follower.resume!=bool(integer(at(state,2)))){mismatches++;if(mismatches<8)std::cout<<"View follower state mismatch sample="<<i<<"\n";}
 }
 std::cout<<"0045FEC0 → 0044F340 chain: "<<count(samples)<<" original sequential captures, compared words="<<words<<"; mismatches="<<mismatches<<" max abs error="<<maxError<<" (item bone refresh and 0044F340 callbacks are fixtures)\n";return mismatches?42:0;
 }
 if(argc>1&&std::string(argv[1])=="--view-movement-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/view-movement-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing ordinary view-update evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0,modeDiff=0,stateDiff=0;double maxError=0;
 auto fromBits=[](J value){uint32_t b=uint32_t(json_object_get_uint64(value));float f;std::memcpy(&f,&b,4);return f;};
 for(int i=0;i<count(samples);i++){
  auto sample=at(samples,i),input=field(sample,"input"),expected=field(sample,"result");FrameTransform root;root.position=vec(field(input,"player_position"));
  for(int a=0;a<9;a++)root.axis[a/3][a%3]=num(at(field(input,"player_basis"),a));
  Follower follower;follower.position=root.position;follower.previous=root.position;follower.flags=integer(field(input,"follower_flags"));
  follower.threshold=vec(field(input,"follower_thresholds"));follower.settling=vec(field(input,"follower_settling"));follower.smoothing=vec(field(input,"follower_smoothing"));
  follower.held=bool(integer(field(input,"follower_held")));follower.allow=bool(integer(field(input,"follower_allow")));follower.resume=bool(integer(field(input,"follower_resume")));
  auto head=field(expected,"head_motion_bits");V headMotion{fromBits(at(head,0)),fromBits(at(head,1)),fromBits(at(head,2))};
  int playerState=integer(field(input,"mode")),active=integer(field(input,"active")),moving=integer(field(input,"moving"));
  auto result=ViewUpdate::transition(root,headMotion,vec(field(input,"tool_offset")),vec(field(input,"tool_rotation")),vec(field(input,"position_endpoint")),vec(field(input,"target_endpoint")),playerState==4?10.f:70.f,num(field(input,"fraction")),num(field(input,"angle")),num(field(input,"frame_dt")),follower,playerState==4);
  auto matrix=field(expected,"matrix_bits");auto actual=result.local.words();
  for(int k=1;k<16;k++){words++;uint32_t want=uint32_t(json_object_get_uint64(at(matrix,k)));if(actual[k]!=want){mismatches++;float a,b;std::memcpy(&a,&actual[k],4);std::memcpy(&b,&want,4);maxError=std::max(maxError,std::abs(double(a)-b));if(mismatches<6)std::cout<<"View movement matrix mismatch sample="<<i<<" word="<<k<<" actual="<<a<<" expected="<<b<<"\n";}}
  auto positions=field(expected,"follower_position_bits");for(int a=0;a<3;a++){uint32_t got=ToolFollow::bits(result.follower.position[a]),want=uint32_t(json_object_get_uint64(at(positions,a)));words++;if(got!=want){mismatches++;float x,y;std::memcpy(&x,&got,4);std::memcpy(&y,&want,4);maxError=std::max(maxError,std::abs(double(x)-y));if(mismatches<6)std::cout<<"View movement follower mismatch sample="<<i<<" axis="<<a<<" actual="<<x<<" expected="<<y<<"\n";}}
  auto basis=field(expected,"follower_basis_bits");for(int k=0;k<9;k++){uint32_t got=ToolFollow::bits(result.follower.basis[k/3][k%3]),want=uint32_t(json_object_get_uint64(at(basis,k)));words++;if(got!=want){mismatches++;float x,y;std::memcpy(&x,&got,4);std::memcpy(&y,&want,4);maxError=std::max(maxError,std::abs(double(x)-y));if(mismatches<6)std::cout<<"View movement follower basis mismatch sample="<<i<<" axis="<<k<<" actual="<<x<<" expected="<<y<<"\n";}}
  int selected=HeadMotion::viewMode(playerState,active!=0,moving!=0);if(selected!=integer(field(expected,"head_motion_mode"))){modeDiff++;mismatches++;if(mismatches<6)std::cout<<"View movement head mode mismatch sample="<<i<<"\n";}
  auto state=field(expected,"follower_state");if(result.follower.held!=bool(integer(at(state,0)))||result.follower.allow!=bool(integer(at(state,1)))||result.follower.resume!=bool(integer(at(state,2)))){stateDiff++;mismatches++;if(mismatches<6)std::cout<<"View movement follower state mismatch sample="<<i<<"\n";}
 }
 std::cout<<"0044F340 ordinary movement path: "<<count(samples)<<" original captures; compared words="<<words<<"; head-mode/state differences="<<modeDiff<<"/"<<stateDiff<<"; mismatches="<<mismatches<<" max abs error="<<maxError<<" (tool callbacks and initial head amplitudes are fixtures)\n";return mismatches?37:0;
 }
 if(argc>1&&std::string(argv[1])=="--item-view-pose-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/item-view-pose-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing item view pose evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;double maxError=0;uint32_t maxUlp=0;
 auto fromBits=[](J value){uint32_t b=uint32_t(json_object_get_uint64(value));float f;std::memcpy(&f,&b,4);return f;};
 auto compare=[&](uint32_t got,uint32_t want,int sample,const char* label){words++;if(got==want)return;float a,b;std::memcpy(&a,&got,4);std::memcpy(&b,&want,4);mismatches++;maxError=std::max(maxError,std::abs(double(a)-b));auto ordered=[](uint32_t v){return (v&0x80000000u)?~v:(v|0x80000000u);};uint32_t oa=ordered(got),ow=ordered(want),delta=oa>ow?oa-ow:ow-oa;maxUlp=std::max(maxUlp,delta);if(mismatches<8)std::cout<<"Item view offset mismatch "<<label<<" sample="<<sample<<" actual="<<a<<" expected="<<b<<" ulp="<<delta<<"\n";};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i),input=field(sample,"input"),expected=field(sample,"result");auto result=ItemViewPose::update(fromBits(field(input,"frame_dt_bits")),fromBits(field(input,"advance_bits")),fromBits(field(input,"initial_time_bits")),7);compare(ToolFollow::bits(result.currentTime),uint32_t(json_object_get_uint64(field(expected,"current_time_bits"))),i,"current-time");compare(ToolFollow::bits(result.previousTime),uint32_t(json_object_get_uint64(field(expected,"previous_time_bits"))),i,"previous-time");if(result.animationFlag!=uint32_t(json_object_get_uint64(field(expected,"animation_flag")))){mismatches++;if(mismatches<8)std::cout<<"Item view flag mismatch sample="<<i<<"\n";}auto aim=field(expected,"aim_offset_bits"),view=field(expected,"view_offset_bits");for(int k=0;k<3;k++){compare(ToolFollow::bits(result.aimOffset[k]),uint32_t(json_object_get_uint64(at(aim,k))),i,"aim");compare(ToolFollow::bits(result.viewOffset[k]),uint32_t(json_object_get_uint64(at(view,k))),i,"view");}}
 std::cout<<"0045FEC0 item view/aim offsets: "<<count(samples)<<" original captures; compared words="<<words<<"; mismatches="<<mismatches<<" max abs error="<<maxError<<" max ULP="<<maxUlp<<" (bone refresh is a fixture)\n";return mismatches?38:0;
 }
 if(argc>1&&std::string(argv[1])=="--tool-follow-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/tool-follow-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing tool follow evidence");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i),a=field(sample,"args");float value=ToolFollow::core(num(at(a,0)),num(at(a,1)),num(at(a,2)),num(at(a,3)),num(at(a,4)),num(field(sample,"global_dt")));if(ToolFollow::bits(value)!=uint32_t(json_object_get_uint64(field(sample,"result_bits")))){mismatches++;if(mismatches<6)std::cout<<"Tool follow mismatch "<<i<<"\n";}}
 auto steps=field(fixture,"steps");for(int i=0;i<count(steps);i++){auto sample=at(steps,i),a=field(sample,"args");float value=ToolFollow::step(num(at(a,0)),num(at(a,1)),num(at(a,2)),num(at(a,3)),num(field(sample,"global_dt")));if(ToolFollow::bits(value)!=uint32_t(json_object_get_uint64(field(sample,"result_bits")))){mismatches++;if(mismatches<6)std::cout<<"Tool subdivision mismatch "<<i<<"\n";}}
 std::cout<<"Tool subdivision: "<<count(steps)<<" original calls\n";
 std::cout<<"Tool follow core: "<<count(samples)<<" original calls; mismatches="<<mismatches<<"\n";return mismatches?32:0;
 }
 if(argc>1&&std::string(argv[1])=="--extinguisher-samples-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-samples-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing extinguisher sample evidence");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto s=at(samples,i);float matrix[9];for(int k=0;k<9;k++)matrix[k]=num(at(field(s,"matrix"),k));auto actual=ExtinguisherSamples::points(matrix,vec(field(s,"origin")));for(int k=0;k<12;k++){float value=actual[k/3][k%3];uint32_t b;std::memcpy(&b,&value,4);if(b!=uint32_t(json_object_get_uint64(at(field(s,"points_bits"),k)))){mismatches++;if(mismatches<6)std::cout<<"Sample mismatch "<<i<<":"<<k<<"\n";}}}
 std::cout<<"Extinguisher sampling: "<<count(samples)<<" original poses; differing coordinates: "<<mismatches<<"\n";return mismatches?31:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-message-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-message-oracle.json").c_str());
 if(!fixture)throw std::runtime_error("Missing fire-message evidence");
 auto fromBits=[](J value){uint32_t word=uint32_t(json_object_get_uint64(value));float result;std::memcpy(&result,&word,4);return result;};
 auto mismatch=[&](int& total,const char* fieldName,int index){total++;if(total<8)std::cout<<"Fire-message mismatch "<<fieldName<<" index="<<index<<"\n";};
 auto compareWord=[&](int& total,uint32_t actual,J expected,const char* fieldName,int index){if(actual!=uint32_t(json_object_get_uint64(expected)))mismatch(total,fieldName,index);};
 int mismatches=0;auto input=field(fixture,"input"),messages=field(fixture,"messages");
 float dt=fromBits(field(input,"frame_dt_bits")),divisor=fromBits(field(input,"model_divisor_bits"));
 struct NativeFire{FireSuppression value;uint32_t targetOffset=0;};
 NativeFire fireA,fireB,fireStale;
 auto initialize=[&](NativeFire& fire,const char* name,uint32_t target){fire.value.strength=fromBits(field(field(input,"fire_strength_bits"),name));fire.value.sprayTime=divisor;fire.value.state=integer(field(input,"fire_state"));fire.targetOffset=target;};
 initialize(fireA,"A",uint32_t(json_object_get_uint64(field(at(messages,0),"fire_offset"))));
 initialize(fireB,"B",uint32_t(json_object_get_uint64(field(at(messages,1),"fire_offset"))));
 initialize(fireStale,"stale",uint32_t(json_object_get_uint64(field(at(messages,2),"fire_offset"))));
 auto routeA=at(messages,0),routeB=at(messages,1);
 EntityIdRegistry ids;EntityIdRegistry::Handle handleA,handleB;
 if(!ids.registerObject(&fireA,handleA))mismatch(mismatches,"register-fire-A",0);
 if(!ids.registerObject(&fireB,handleB))mismatch(mismatches,"register-fire-B",0);
 auto idAllocations=field(field(fixture,"id_registry"),"allocations");
 compareWord(mismatches,handleA.route,field(at(idAllocations,0),"route"),"route-A",0);
 compareWord(mismatches,handleA.generation,field(at(idAllocations,0),"generation"),"generation-A",0);
 compareWord(mismatches,handleB.route,field(at(idAllocations,1),"route"),"route-B",0);
 compareWord(mismatches,handleB.generation,field(at(idAllocations,1),"generation"),"generation-B",0);
 ExtinguisherContacts contactStorage;FireMessageQueue queue;
 struct NativeDelivery{uint32_t receiver=0,message=0;int payloadOffset=0;std::array<uint32_t,4> payload{};};
 struct NativeDrain{uint32_t slot=0,active=0,pending=0;std::array<uint32_t,3> strength{};std::array<uint32_t,3> sprayed{};};
 std::vector<NativeDelivery> deliveries;std::vector<NativeDrain> drain;
 for(int i=0;i<count(messages);i++){
  auto message=at(messages,i),record=field(message,"record_bits");
  V point{fromBits(at(record,0)),fromBits(at(record,1)),fromBits(at(record,2))};
  float strength=fromBits(at(record,3));
  bool accepted=contactStorage.append(
   uint32_t(json_object_get_uint64(field(message,"fire_offset"))),
   uint32_t(json_object_get_uint64(field(message,"route0"))),
   uint32_t(json_object_get_uint64(field(message,"route1"))),point,strength,
   [&](const ExtinguisherContacts::Dispatch& event){
    if(!queue.enqueue(event.route0,event.route1,event.message,event.payload))
     mismatch(mismatches,"enqueue",i);
   });
  if(!accepted)mismatch(mismatches,"contact-append",i);
 }
 if(contactStorage.count!=count(messages))mismatch(mismatches,"contact-count",0);
 if(int(queue.pending.size())!=integer(field(fixture,"pending_before_drain")))mismatch(mismatches,"pending-before",0);
 if(queue.highWater!=integer(field(fixture,"mapping_high_water_before_drain")))mismatch(mismatches,"mapping-high-water",0);
 auto activeBefore=field(fixture,"active_before_drain");
 for(int i=0;i<FireMessageQueue::capacity;i++){
  uint32_t expected=i<count(activeBefore)?uint32_t(integer(at(activeBefore,i))):0;
  if(uint32_t(queue.records[i].active)!=expected)mismatch(mismatches,"active-before",i);
 }
 auto resolver=[&](uint32_t route,uint32_t generation)->FireMessageQueue::Receiver{
  auto* target=static_cast<NativeFire*>(ids.resolve(route,generation));if(!target)return {};
  return [&,target](uint32_t messageId,const void* payload){
   auto* contact=static_cast<const ExtinguisherContacts::Contact*>(payload);
   NativeDelivery call;call.receiver=target->targetOffset;call.message=messageId;
   call.payloadOffset=int(reinterpret_cast<const unsigned char*>(payload)-reinterpret_cast<const unsigned char*>(&contactStorage));
   call.payload={ToolFollow::bits(contact->x),ToolFollow::bits(contact->y),ToolFollow::bits(contact->z),ToolFollow::bits(contact->strength)};
   deliveries.push_back(call);
   if(target->value.state!=5&&messageId==ExtinguisherContacts::messageId)
    target->value.spray(dt,contact->strength,false,[]{},[]{}); // 00449A40 passes terminal=0 to 00448970.
  };
 };
 queue.drain(resolver,[&](const FireMessageQueue::Delivery& event){
  drain.push_back({event.slot,uint32_t(event.record->active),uint32_t(event.pendingAfterPop),
   {ToolFollow::bits(fireA.value.strength),ToolFollow::bits(fireB.value.strength),ToolFollow::bits(fireStale.value.strength)},
   {uint32_t(fireA.value.sprayed),uint32_t(fireB.value.sprayed),uint32_t(fireStale.value.sprayed)}});
 });
 auto expectedCalls=field(fixture,"handler_calls");
 if(deliveries.size()!=size_t(count(expectedCalls)))mismatch(mismatches,"handler-count",int(deliveries.size()));
 for(size_t i=0;i<std::min(deliveries.size(),size_t(count(expectedCalls)));i++){
  auto actual=deliveries[i];auto expected=at(expectedCalls,int(i));
  compareWord(mismatches,actual.receiver,field(expected,"receiver_offset"),"receiver",int(i));
  compareWord(mismatches,actual.message,field(expected,"message"),"message-id",int(i));
  compareWord(mismatches,uint32_t(actual.payloadOffset),field(expected,"payload_offset"),"payload-offset",int(i));
  for(int k=0;k<4;k++)compareWord(mismatches,actual.payload[k],at(field(expected,"payload_bits"),k),"payload",int(i)*4+k);
 }
 auto expectedDrain=field(fixture,"drain_steps");
 if(drain.size()!=size_t(count(expectedDrain)))mismatch(mismatches,"drain-count",int(drain.size()));
 const char* labels[]={"A","B","stale"};
 for(size_t i=0;i<std::min(drain.size(),size_t(count(expectedDrain)));i++){
  auto actual=drain[i];auto expected=at(expectedDrain,int(i));
  compareWord(mismatches,actual.slot,field(expected,"record_slot"),"record-slot",int(i));
  compareWord(mismatches,actual.active,field(expected,"record_active_before_clear"),"record-active",int(i));
  compareWord(mismatches,actual.pending,field(expected,"queued_count_after_pop"),"queue-after-pop",int(i));
  for(int k=0;k<3;k++){
   compareWord(mismatches,actual.strength[k],field(field(expected,"fire_strength_bits"),labels[k]),"fire-strength",int(i)*3+k);
   compareWord(mismatches,actual.sprayed[k],field(field(expected,"sprayed_flags"),labels[k]),"sprayed",int(i)*3+k);
  }
 }
 auto activeAfter=field(fixture,"active_after_drain");
 for(int i=0;i<FireMessageQueue::capacity;i++){
  uint32_t expected=i<count(activeAfter)?uint32_t(integer(at(activeAfter,i))):0;
  if(uint32_t(queue.records[i].active)!=expected)mismatch(mismatches,"active-after",i);
 }
 if(int(queue.pending.size())!=integer(field(fixture,"pending_after_drain")))mismatch(mismatches,"pending-after",0);
 auto final=field(fixture,"fire_results");NativeFire* fires[]={&fireA,&fireB,&fireStale};
 for(int i=0;i<3;i++){auto expected=field(final,labels[i]);compareWord(mismatches,ToolFollow::bits(fires[i]->value.strength),field(expected,"strength_bits"),"fire-result",i);compareWord(mismatches,uint32_t(fires[i]->value.sprayed),field(expected,"sprayed"),"fire-result-sprayed",i);}
 if(int(idAllocations?count(idAllocations):0)!=int(EntityIdRegistry::capacity))mismatch(mismatches,"id-pool-size",0);
 std::array<uint32_t,EntityIdRegistry::capacity> registrationTokens{};
 for(int i=2;i<count(idAllocations);i++){
  EntityIdRegistry::Handle handle;
  if(!ids.registerObject(&registrationTokens[i],handle)){mismatch(mismatches,"id-pool-register",i);break;}
  auto expected=at(idAllocations,i);
  compareWord(mismatches,handle.route,field(expected,"route"),"allocated-route",i);
  compareWord(mismatches,handle.generation,field(expected,"generation"),"allocated-generation",i);
 }
 uint32_t oldRoute=handleA.route,oldGeneration=handleA.generation;
 if(!ids.release(handleA))mismatch(mismatches,"id-release",0);
 compareWord(mismatches,handleA.generation,field(field(fixture,"id_registry"),"released_A_generation"),"released-generation",0);
 if(ids.resolve(oldRoute,oldGeneration))mismatch(mismatches,"released-route-resolved",0);
 uint32_t reusedToken=0;EntityIdRegistry::Handle reused;
 if(!ids.registerObject(&reusedToken,reused))mismatch(mismatches,"id-reuse",0);
 auto expectedReused=field(field(fixture,"id_registry"),"reused_after_pool_exhaustion");
 compareWord(mismatches,reused.route,field(expectedReused,"route"),"reused-route",0);
 compareWord(mismatches,reused.generation,field(expectedReused,"generation"),"reused-generation",0);
 if(ids.resolve(oldRoute,oldGeneration))mismatch(mismatches,"stale-generation-resolved-after-reuse",0);
 if(ids.resolve(reused.route,reused.generation)!=&reusedToken)mismatch(mismatches,"reused-receiver",0);
 if(!ids.freeRoutes.empty())mismatch(mismatches,"id-free-pool-after-reuse",int(ids.freeRoutes.size()));
 if(integer(field(field(fixture,"provenance"),"rng_draws"))!=0)mismatch(mismatches,"unexpected-rng-draws",0);
 std::cout<<"0049B810 ID registry: "<<count(idAllocations)<<" original allocations, release and FIFO ID reuse compared; 0049B4C0/0049B610 messages="<<count(messages)<<", handler calls="<<deliveries.size()<<", FIFO records drained="<<drain.size()<<"; mismatches="<<mismatches<<" (live contact→queue path is connected; original owner order remains unresolved)\n";
 return mismatches?43:0;
 }
 if(argc>1&&std::string(argv[1])=="--extinguisher-contact-path-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-contact-path-full-busy-pool-oracle.json").c_str());
 if(!fixture)throw std::runtime_error("Missing extinguisher contact-path evidence");
 auto fromBits=[](J value){uint32_t word=uint32_t(json_object_get_uint64(value));float result;std::memcpy(&result,&word,4);return result;};
 int mismatches=0,words=0;
 auto compareNumber=[&](uint32_t actual,uint32_t want,const char* label,int index){words++;if(actual!=want){mismatches++;if(mismatches<8)std::cout<<"Contact-path mismatch "<<label<<" index="<<index<<" actual="<<actual<<" expected="<<want<<"\n";}};
 auto compare=[&](uint32_t actual,J expected,const char* label,int index){compareNumber(actual,uint32_t(json_object_get_uint64(expected)),label,index);};
 auto input=field(fixture,"input"),source=field(input,"fire"),provenance=field(fixture,"provenance");
 uint32_t gameTime=uint32_t(json_object_get_uint64(field(input,"game_time")));
 Random shared(uint32_t(json_object_get_uint64(field(provenance,"seed"))));
 shared.range(6.f,8.f); // first 00461F80 draw: active-pose phase
 float matrix[9];auto basis=field(input,"item_basis_bits");for(int i=0;i<9;i++)matrix[i]=fromBits(at(basis,i));
 auto points=ExtinguisherSamples::points(matrix,vec(field(input,"item_origin")));
 auto expectedPoints=field(fixture,"generated_points_bits");
 for(int i=0;i<4;i++)for(int axis=0;axis<3;axis++)compare(ToolFollow::bits(points[i][axis]),at(at(expectedPoints,i),axis),"sample-point",i*3+axis);

 FireEntity fire;fire.suppression.strength=fromBits(field(source,"strength_bits_before"));
 fire.suppression.sprayTime=fromBits(field(source,"model_divisor_bits"));fire.suppression.state=2;
 EntityIdRegistry ids;EntityIdRegistry::Handle handle;
 if(!ids.registerObject(&fire,handle))throw std::runtime_error("Original Training route allocation failed");
 compare(handle.route,field(source,"route"),"fire-route",0);
 compare(handle.generation,field(source,"generation"),"fire-generation",0);

 V center;auto centerBits=field(source,"visual_center_bits"),extentBits=field(source,"visual_extents_bits");
 V extents;for(int axis=0;axis<3;axis++){center[axis]=fromBits(at(centerBits,axis));extents[axis]=fromBits(at(extentBits,axis));}
 float alpha=fromBits(field(source,"sprite_alpha_bits"));ExtinguisherContacts contacts;FireMessageQueue queue;
 auto originalTypes=field(input,"candidate_types");bool fireTypePresent=false;
 for(int i=0;i<count(originalTypes);i++)if(integer(at(originalTypes,i))==6)fireTypePresent=true;
 if(!fireTypePresent)throw std::runtime_error("Fixture has no FireRescue candidate");
 uint32_t receiver=uint32_t(json_object_get_uint64(field(at(field(field(fixture,"contacts"),"queue_calls"),0),"receiver_offset")));
 auto originalContacts=field(field(fixture,"contacts"),"records_bits");
 uint32_t effectTimer=0;ExtinguisherEffects::SpawnRequest busyPoolSpawn;
 for(int i=0;i<4;i++){
  float influence=FireContact::influence(extents,center,points[i],60.f,alpha);
  if(influence<.001f)continue; // original 00461F80 threshold at 004DFA28
  float strength=float(double(influence)+double(influence));
  V point=points[i];
  contacts.append(receiver,handle.route,handle.generation,point,strength,[&](const ExtinguisherContacts::Dispatch& event){
   if(!queue.enqueue(event.route0,event.route1,event.message,event.payload))mismatches++;
  });
  ExtinguisherEffects::afterContact(shared,gameTime,fire.suppression.strength,effectTimer,false,busyPoolSpawn);
 }
 compare(effectTimer,field(field(fixture,"queue"),"effect_timer_after_contact"),"effect-deadline",0);
 compare(uint32_t(shared.drawCount),field(provenance,"rng_draws"),"shared-draw-count",0);
 compare(uint32_t(contacts.count),field(field(fixture,"contacts"),"count"),"contact-count",0);
 for(int i=0;i<contacts.count;i++){
  const auto& item=contacts.records[i];uint32_t actual[]={ToolFollow::bits(item.x),ToolFollow::bits(item.y),ToolFollow::bits(item.z),ToolFollow::bits(item.strength)};
  for(int k=0;k<4;k++)compare(actual[k],at(at(originalContacts,i),k),"contact-record",i*4+k);
 }
 auto expectedQueue=field(fixture,"queue");
 compare(uint32_t(queue.pending.size()),field(expectedQueue,"pending_before_drain"),"pending-before",0);
 compare(uint32_t(queue.highWater),field(expectedQueue,"mapping_high_water_before_drain"),"mapping-high-water",0);
 auto expectedActive=field(expectedQueue,"active_before_drain");
 for(int i=0;i<FireMessageQueue::capacity;i++){uint32_t active=i<count(expectedActive)?uint32_t(integer(at(expectedActive,i))):0;compareNumber(uint32_t(queue.records[i].active),active,"active-record",i);}

 struct DrainState{uint32_t slot,active,pending,strength,sprayed;};std::vector<DrainState> drain;
 float dt=fromBits(field(input,"frame_dt_bits"));
 queue.drain([&](uint32_t route,uint32_t generation)->FireMessageQueue::Receiver{
  auto* target=static_cast<FireEntity*>(ids.resolve(route,generation));if(!target)return {};
  return [&,target](uint32_t message,const void* payload){if(message!=ExtinguisherContacts::messageId)return;
   auto* contact=static_cast<const ExtinguisherContacts::Contact*>(payload);
   target->spray(dt,contact->strength,false,[]{},[]{}); // 00449A40 passes terminal=0 to 00448970.
  };
 },[&](const FireMessageQueue::Delivery& event){drain.push_back({event.slot,uint32_t(event.record->active),uint32_t(event.pendingAfterPop),ToolFollow::bits(fire.suppression.strength),uint32_t(fire.suppression.sprayed)});});
 auto expectedDrain=field(expectedQueue,"drain_steps");
 compareNumber(uint32_t(drain.size()),uint32_t(count(expectedDrain)),"drain-count",0);
 for(int i=0;i<std::min(int(drain.size()),count(expectedDrain));i++){auto actual=drain[i];auto expected=at(expectedDrain,i);
  compare(actual.slot,field(expected,"record_slot"),"drain-slot",i);compare(actual.active,field(expected,"record_active_before_clear"),"drain-active",i);compare(actual.pending,field(expected,"queued_count_after_pop"),"drain-pending",i);
  compare(actual.strength,field(expected,"fire_strength_bits"),"drain-fire-strength",i);compare(actual.sprayed,field(expected,"sprayed"),"drain-sprayed",i);
 }
 compare(uint32_t(queue.pending.size()),field(expectedQueue,"pending_after_drain"),"pending-after",0);
 compare(ToolFollow::bits(fire.suppression.strength),field(expectedQueue,"fire_strength_bits_after_drain"),"fire-strength-final",0);
 compare(uint32_t(fire.suppression.sprayed),field(expectedQueue,"fire_sprayed_after_drain"),"fire-sprayed-final",0);
 auto pool=field(input,"effect_pool");
 if(integer(field(pool,"capacity"))!=1||integer(field(pool,"entries"))!=1||integer(field(pool,"busy_entries"))!=1)mismatches++;
 auto sourceFixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-contact-path-source-empty-pool-oracle.json").c_str());
 if(!sourceFixture)throw std::runtime_error("Missing source-empty-pool contact-path evidence");
 auto sourceInput=field(sourceFixture,"input"),sourceProvenance=field(sourceFixture,"provenance"),sourceQueue=field(sourceFixture,"queue");
 Random sourceRandom(uint32_t(json_object_get_uint64(field(sourceProvenance,"seed"))));
 sourceRandom.range(6.f,8.f);
 uint32_t sourceTimer=0;ExtinguisherEffects::SpawnRequest sourceSpawn;
 auto sourceFire=field(sourceInput,"fire");V sourceCenter,sourceExtents;
 auto sourceCenterBits=field(sourceFire,"visual_center_bits"),sourceExtentBits=field(sourceFire,"visual_extents_bits");
 for(int axis=0;axis<3;axis++){sourceCenter[axis]=fromBits(at(sourceCenterBits,axis));sourceExtents[axis]=fromBits(at(sourceExtentBits,axis));}
 float sourceAlpha=fromBits(field(sourceFire,"sprite_alpha_bits"));int sourceRequests=0;
 for(const V point:points){
  float influence=FireContact::influence(sourceExtents,sourceCenter,point,60.f,sourceAlpha);
  if(influence<.001f)continue;
  if(ExtinguisherEffects::afterContact(sourceRandom,uint32_t(json_object_get_uint64(field(sourceInput,"game_time"))),fromBits(field(sourceFire,"strength_bits_before")),sourceTimer,true,sourceSpawn))sourceRequests++;
 }
 compare(sourceTimer,field(sourceQueue,"effect_timer_after_contact"),"source-pool-deadline",0);
 compare(uint32_t(sourceRandom.drawCount),field(sourceProvenance,"rng_draws"),"source-pool-draw-count",0);
 auto sourcePool=field(sourceInput,"effect_pool");
 compareNumber(uint32_t(json_object_get_int64(field(sourcePool,"entries_after_contact"))),1,"source-pool-entry-count",0);
 compareNumber(uint32_t(json_object_get_int64(field(sourcePool,"limit"))),UINT32_MAX,"source-pool-limit",0);
 auto sourceEmitters=field(sourcePool,"emitters_after_contact");
 if(count(sourceEmitters)!=1)mismatches++;
 if(count(sourceEmitters)>0){auto emitter=at(sourceEmitters,0),offsets=field(emitter,"offset_vector_bits"),origin=field(emitter,"origin_vector_bits");
  compareNumber(uint32_t(sourceRequests),1,"source-emitter-request-count",0);
  compareNumber(uint32_t(integer(field(emitter,"active"))),1,"source-emitter-active",0);
  compare(ToolFollow::bits(sourceSpawn.offset.x),at(offsets,0),"source-emitter-offset",0);
  compare(ToolFollow::bits(sourceSpawn.offset.y),at(offsets,1),"source-emitter-offset",1);
  compare(ToolFollow::bits(sourceSpawn.offset.z),at(offsets,2),"source-emitter-offset",2);
  for(int axis=0;axis<3;axis++)compare(0,at(origin,axis),"source-emitter-origin",axis);
 }
 auto returns=field(sourceProvenance,"pool_returns");
 if(count(returns)!=1||count(field(at(returns,0),"eax_values"))!=1||json_object_get_uint64(at(field(at(returns,0),"eax_values"),0))==0)mismatches++;
 std::cout<<"00461F80 controlled contact path: full/busy pool points=4, contacts="<<contacts.count<<", drained="<<drain.size()<<", draws="<<shared.drawCount<<"; source-empty pool draws="<<sourceRandom.drawCount<<", emitter offsets matched; compared values="<<words<<", mismatches="<<mismatches<<" (fixture order; not live Training scheduling)\n";
 return mismatches?44:0;
 }
 if(argc>1&&std::string(argv[1])=="--extinguisher-contacts-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/extinguisher-contacts-oracle.json").c_str());
 if(!fixture)throw std::runtime_error("Missing extinguisher contact evidence");
 auto fromBits=[](J value){uint32_t word=uint32_t(json_object_get_uint64(value));float result;std::memcpy(&result,&word,4);return result;};
 auto mismatch=[&](int& total,const char* fieldName,int index){total++;if(total<8)std::cout<<"Extinguisher contact mismatch "<<fieldName<<" index="<<index<<"\n";};
 int mismatches=0;ExtinguisherContacts collector;
 auto inputs=field(fixture,"input_words"),counts=field(fixture,"counts_after"),accepted=field(fixture,"accepted");
 auto expectedCalls=field(fixture,"queue_calls");
 if(count(inputs)!=count(counts)||count(inputs)!=count(accepted))throw std::runtime_error("Malformed extinguisher contact evidence");
 struct CapturedCall{uint32_t receiver,route0,route1,message;int slot,countAtCall,payloadOffset;std::array<uint32_t,4> payload;};
 std::vector<CapturedCall> calls;
 auto append=[&](uint32_t receiver,uint32_t route0,uint32_t route1,J words){
  V point{fromBits(at(words,0)),fromBits(at(words,1)),fromBits(at(words,2))};
  float strength=fromBits(at(words,3));
  return collector.append(receiver,route0,route1,point,strength,[&](const ExtinguisherContacts::Dispatch& dispatch){
   auto* base=reinterpret_cast<const unsigned char*>(&collector);
   auto* payload=reinterpret_cast<const unsigned char*>(dispatch.payload);
   calls.push_back({dispatch.receiver,dispatch.route0,dispatch.route1,dispatch.message,dispatch.slot,collector.count,int(payload-base),
    {ToolFollow::bits(dispatch.payload->x),ToolFollow::bits(dispatch.payload->y),ToolFollow::bits(dispatch.payload->z),ToolFollow::bits(dispatch.payload->strength)}});
  });
 };
 auto firstExpected=at(expectedCalls,0);
 uint32_t receiver=uint32_t(json_object_get_uint64(field(firstExpected,"receiver_offset")));
 uint32_t route0=uint32_t(json_object_get_uint64(field(firstExpected,"route0")));
 uint32_t route1=uint32_t(json_object_get_uint64(field(firstExpected,"route1")));
 for(int i=0;i<count(inputs);i++){
  bool result=append(receiver,route0,route1,at(inputs,i));
  if(result!=bool(integer(at(accepted,i))))mismatch(mismatches,"accepted",i);
  if(collector.count!=integer(at(counts,i)))mismatch(mismatches,"count",i);
 }
 auto compareRecords=[&](const char* name,J expected){
  if(count(expected)!=ExtinguisherContacts::capacity){mismatch(mismatches,name,-1);return;}
  for(int slot=0;slot<ExtinguisherContacts::capacity;slot++){
   const auto& record=collector.records[slot];uint32_t actual[]={ToolFollow::bits(record.x),ToolFollow::bits(record.y),ToolFollow::bits(record.z),ToolFollow::bits(record.strength)};
   for(int word=0;word<4;word++)if(actual[word]!=uint32_t(json_object_get_uint64(at(at(expected,slot),word))))mismatch(mismatches,name,slot*4+word);
  }
 };
 compareRecords("final-records",field(fixture,"final_records"));
 auto compareCall=[&](const CapturedCall& actual,J expected,int index){
  auto check=[&](uint32_t got,J want,const char* name){if(got!=uint32_t(json_object_get_uint64(want)))mismatch(mismatches,name,index);};
  check(actual.receiver,field(expected,"receiver_offset"),"receiver");check(actual.route0,field(expected,"route0"),"route0");check(actual.route1,field(expected,"route1"),"route1");
  check(actual.message,field(expected,"message"),"message-id");check(uint32_t(actual.payloadOffset),field(expected,"payload_offset"),"payload-offset");
  check(uint32_t(actual.countAtCall),field(expected,"count_at_call"),"count-at-call");
  if(uint32_t(actual.slot)!=(uint32_t(json_object_get_uint64(field(expected,"payload_offset")))-4)/16)mismatch(mismatches,"slot",index);
  auto payload=field(expected,"payload_bits");for(int word=0;word<4;word++)if(actual.payload[word]!=uint32_t(json_object_get_uint64(at(payload,word))))mismatch(mismatches,"queued-payload",index*4+word);
 };
 int initialCalls=int(json_object_get_uint64(field(fixture,"call_count_before_reset")));
 if(calls.size()!=size_t(initialCalls))mismatch(mismatches,"queue-count",int(calls.size()));
 if(initialCalls!=int(expectedCalls?count(expectedCalls)-1:0))mismatch(mismatches,"initial-call-count",initialCalls);
 for(size_t i=0;i<std::min(calls.size(),size_t(initialCalls));i++)compareCall(calls[i],at(expectedCalls,int(i)),int(i));

 collector.reset();
 if(collector.count!=integer(field(fixture,"reset_count")))mismatch(mismatches,"reset-count",0);
 compareRecords("records-after-reset",field(fixture,"records_after_reset"));
 J resetWords=field(fixture,"reset_append_words"),resetExpected=field(fixture,"reset_queue_call");
 uint32_t resetReceiver=uint32_t(json_object_get_uint64(field(resetExpected,"receiver_offset")));
 uint32_t resetRoute0=uint32_t(json_object_get_uint64(field(resetExpected,"route0")));
 uint32_t resetRoute1=uint32_t(json_object_get_uint64(field(resetExpected,"route1")));
 int priorCalls=int(calls.size());
 bool resetAccepted=append(resetReceiver,resetRoute0,resetRoute1,resetWords);
 if(!resetAccepted||collector.count!=integer(field(fixture,"reset_append_count")))mismatch(mismatches,"reset-append",0);
 compareRecords("records-after-reset-append",field(fixture,"records_after_reset_append"));
 if(calls.size()!=size_t(priorCalls+1))mismatch(mismatches,"reset-queue-count",int(calls.size()));
 if(calls.size()>size_t(priorCalls))compareCall(calls.back(),resetExpected,priorCalls);
 std::cout<<"00481A50/00481A60 collector: "<<count(inputs)<<" original appends, "<<initialCalls<<" queued message-call captures; 32-slot cap and reset compared; mismatches="<<mismatches<<" (deferred delivery has a separate component check)\n";
 return mismatches?42:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-contact-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-contact-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing fire contact evidence");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto s=at(samples,i);float value=FireContact::influence(vec(field(s,"size")),{},vec(field(s,"point")),num(field(s,"padding")),num(field(s,"alpha")));uint32_t b;std::memcpy(&b,&value,4);if(b!=uint32_t(json_object_get_uint64(field(s,"result_bits")))){mismatches++;if(mismatches<6)std::cout<<"Contact mismatch "<<i<<"\n";}}
 std::cout<<"Fire contact: "<<count(samples)<<" original calls, mismatches: "<<mismatches<<"\n";return mismatches?30:0;
 }
 if(argc>1&&std::string(argv[1])=="--interaction-sphere-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/prop-interaction-sphere-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing PropModel interaction-sphere evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;
 auto fromBits=[](J j){uint32_t b=uint32_t(json_object_get_uint64(j));float v;std::memcpy(&v,&b,4);return v;};
 auto floatFromWord=[](uint32_t b){float v;std::memcpy(&v,&b,4);return v;};
 auto bits=[](float v){uint32_t b;std::memcpy(&b,&v,4);return b;};
 auto expected=[](J j){return uint32_t(json_object_get_uint64(j));};
 for(int i=0;i<count(samples);i++){
  auto sample=at(samples,i);V start=vec(field(sample,"start")),direction=vec(field(sample,"direction")),center=vec(field(sample,"center"));float radius=fromBits(field(sample,"radius_bits"));
  float nearDistance=floatFromWord(0x7fc00001),farDistance=floatFromWord(0x7fc00002);
  bool ray=InteractionSphere::ray(start,direction,center,radius,nearDistance,farDistance);
  bool badHit=uint32_t(ray)!=expected(field(sample,"sphere_hit"));
  bool badNear=bits(nearDistance)!=expected(field(sample,"near_bits"));bool badFar=bits(farDistance)!=expected(field(sample,"far_bits"));words+=3;
  V point{fromBits(at(field(sample,"point_bits"),0)),fromBits(at(field(sample,"point_bits"),1)),fromBits(at(field(sample,"point_bits"),2))};float score=fromBits(field(sample,"score_bits"));
  bool query=InteractionSphere::query(start,direction,center,radius,&point,&score);
  bool badQuery=uint32_t(query)!=expected(field(sample,"query_hit")),badPoint=false;for(int axis=0;axis<3;axis++){badPoint|=bits(point[axis])!=expected(at(field(sample,"point_bits"),axis));words++;}bool badScore=bits(score)!=expected(field(sample,"score_bits"));words++;
  if(badHit||badNear||badFar||badQuery||badPoint||badScore){mismatches++;if(mismatches<6)std::cout<<"Interaction-sphere mismatch sample="<<i<<" flags="<<badHit<<badNear<<badFar<<badQuery<<badPoint<<badScore<<" near="<<bits(nearDistance)<<"/"<<expected(field(sample,"near_bits"))<<" far="<<bits(farDistance)<<"/"<<expected(field(sample,"far_bits"))<<" query="<<query<<"/"<<expected(field(sample,"query_hit"))<<" point="<<bits(point.x)<<","<<bits(point.y)<<","<<bits(point.z)<<" expected="<<expected(at(field(sample,"point_bits"),0))<<","<<expected(at(field(sample,"point_bits"),1))<<","<<expected(at(field(sample,"point_bits"),2))<<" score="<<bits(score)<<"/"<<expected(field(sample,"score_bits"))<<"\n";}
 }
 std::cout<<"PropModel interaction sphere: "<<count(samples)<<" original queries, compared outputs="<<words<<", mismatches="<<mismatches<<"\n";return mismatches?46:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-render-state-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/driver-blend-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing driver-state evidence");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto s=at(samples,i),states=field(s,"states");auto actual=FireRenderState::fromDriverFlags(uint32_t(integer(field(s,"flags"))));bool bad=actual.blending!=bool(integer(field(states,"27")))||actual.depthWrite!=bool(integer(field(states,"14")))||actual.alwaysDepth!=(integer(field(states,"23"))==8);if(actual.blending)bad|=integer(field(states,"19"))!=5||integer(field(states,"20"))!=(actual.additive?2:6);if(bad)mismatches++;}
 std::cout<<"Original driver-state cases: "<<count(samples)<<", mismatches: "<<mismatches<<"\n";return mismatches?28:0;
 }
 if(argc>1&&std::string(argv[1])=="--sprite-alpha-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/sprite-alpha-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing sprite alpha evidence");auto samples=field(fixture,"samples");int mismatches=0;
 auto f=[](J j){uint32_t b=uint32_t(json_object_get_uint64(j));float v;std::memcpy(&v,&b,4);return v;};
 for(int i=0;i<count(samples);i++){auto s=at(samples,i);auto actual=FireVisual::opacityBytes(uint32_t(integer(field(s,"elapsed"))),f(field(s,"rate_bits")),f(field(s,"alpha_bits")));if(actual[0]!=integer(field(s,"current"))||actual[1]!=integer(field(s,"next"))){mismatches++;if(mismatches<6)std::cout<<"Alpha mismatch "<<i<<"\n";}}
 std::cout<<"Fire alpha: "<<count(samples)<<" original calls; mismatches: "<<mismatches<<"\n";return mismatches?27:0;
 }
 if(argc>1&&std::string(argv[1])=="--sprite-animation-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/sprite-animation-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing sprite animation evidence");auto samples=field(fixture,"samples");int mismatches=0;auto fromBits=[](J j){uint32_t b=uint32_t(json_object_get_uint64(j));float f;std::memcpy(&f,&b,4);return f;};
 for(int i=0;i<count(samples);i++){auto s=at(samples,i);SpriteAnimation animation;int frames=integer(field(s,"frames"));animation.frame=frames-1;animation.nextFrame=0;animation.rate=fromBits(field(s,"rate_bits"));animation.flags=integer(field(s,"flags"));animation.started=uint32_t(json_object_get_uint64(field(s,"started")));animation.update(uint32_t(json_object_get_uint64(field(s,"now"))),std::vector<float>(frames,fromBits(field(s,"period_bits"))));if(animation.frame!=integer(field(s,"frame"))||animation.nextFrame!=integer(field(s,"next_frame"))||animation.started!=uint32_t(json_object_get_uint64(field(s,"result_started")))||animation.active!=bool(integer(field(s,"active")))){mismatches++;if(mismatches<6)std::cout<<"Sprite animation difference "<<i<<"\n";}}
 std::cout<<"Sprite animation: "<<count(samples)<<" original updates; differences="<<mismatches<<"\n";return mismatches?23:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-placement-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-placement-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing fire placement evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;float maximum=0;
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);float matrix[9];for(int k=0;k<9;k++)matrix[k]=num(at(field(sample,"matrix"),k));auto bounds=field(sample,"bounds");V lo={num(at(bounds,0)),num(at(bounds,1)),num(at(bounds,2))},hi={num(at(bounds,3)),num(at(bounds,4)),num(at(bounds,5))};FirePlacement placement;placement.place(vec(field(sample,"size")),matrix,vec(field(sample,"position")),lo,hi);auto expected=field(sample,"sprites");
 for(int k=0;k<4;k++){auto compare=[&](float value,J expected){uint32_t bits;std::memcpy(&bits,&value,4);uint32_t native=uint32_t(json_object_get_uint64(expected));float nf;std::memcpy(&nf,&native,4);maximum=std::max(maximum,std::abs(value-nf));if(bits!=native){mismatches++;if(mismatches<6)std::cout<<"Fire placement difference "<<i<<" sprite="<<k<<" actual="<<value<<" expected="<<nf<<"\n";}words++;};auto e=at(expected,k);for(int v=0;v<4;v++)for(int a=0;a<3;a++)compare(placement.sprites[k].vertices[v][a],at(at(field(e,"vertices_bits"),v),a));compare(placement.sprites[k].width,field(e,"width_bits"));compare(placement.sprites[k].originY,field(e,"origin_y_bits"));}}
 std::cout<<"Fire placement: "<<count(samples)<<" original calls, "<<words<<" result words; differences="<<mismatches<<"; max error="<<maximum<<"\n";return mismatches?22:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-size-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-size-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing fire size evidence");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);FireSize size;size.scale=vec(field(sample,"scale"));auto keys=field(sample,"keys");for(int k=0;k<count(keys);k++){auto key=at(keys,k);size.keys.push_back({true,uint32_t(json_object_get_uint64(field(key,"time"))),vec(field(key,"size"))});}size.first=0;size.next=count(keys)>1?1:-1;size.update(uint32_t(json_object_get_uint64(field(sample,"now"))));bool different=size.first!=integer(field(sample,"first"))||size.next!=integer(field(sample,"next"))||size.changed!=bool(integer(field(sample,"changed")));
 for(int k=0;k<3;k++){uint32_t bits;std::memcpy(&bits,&size.size[k],4);different|=bits!=uint32_t(json_object_get_uint64(at(field(sample,"result_bits"),k)));}for(int k=0;k<count(keys);k++)different|=size.keys[k].active!=bool(integer(at(field(sample,"active"),k)));
 if(different){mismatches++;if(mismatches<6)std::cout<<"Fire size mismatch "<<i<<"\n";}}
 auto transitions=field(fixture,"queue_samples");auto fromBits=[](J j){uint32_t bits=uint32_t(json_object_get_uint64(j));float f;std::memcpy(&f,&bits,4);return f;};
 auto readState=[&](J state){FireSize size;auto keys=field(state,"keys");for(int k=0;k<count(keys);k++){auto key=at(keys,k);V value;for(int a=0;a<3;a++)value[a]=fromBits(at(field(key,"size_bits"),a));size.keys.push_back({bool(integer(field(key,"active"))),uint32_t(json_object_get_uint64(field(key,"time"))),value});}size.first=integer(field(state,"first"));size.next=integer(field(state,"next"));size.changed=integer(field(state,"changed"));for(int a=0;a<3;a++)size.size[a]=fromBits(at(field(state,"size_bits"),a));return size;};
 auto equal=[](float a,float b){uint32_t x,y;std::memcpy(&x,&a,4);std::memcpy(&y,&b,4);return x==y;};
 for(int i=0;i<count(transitions);i++){auto t=at(transitions,i);auto size=readState(field(t,"before"));auto expected=readState(field(t,"after"));uint32_t now=uint32_t(json_object_get_uint64(field(t,"now")));if(str(field(t,"kind"))=="add")size.add(vec(field(t,"value")),uint32_t(json_object_get_uint64(field(t,"when"))),now);else size.update(now);bool different=size.first!=expected.first||size.next!=expected.next||size.changed!=expected.changed||size.keys.size()!=expected.keys.size();for(int a=0;a<3;a++)different|=!equal(size.size[a],expected.size[a]);for(int k=0;k<int(expected.keys.size())&&k<int(size.keys.size());k++){different|=size.keys[k].active!=expected.keys[k].active||size.keys[k].time!=expected.keys[k].time;for(int a=0;a<3;a++)different|=!equal(size.keys[k].size[a],expected.keys[k].size[a]);}if(different){mismatches++;if(mismatches<6)std::cout<<"Fire key transition mismatch "<<i<<"\n";}}
 std::cout<<"Fire sizes: "<<count(samples)<<" original updates, "<<count(transitions)<<" queue/update transitions; mismatches="<<mismatches<<"\n";return mismatches?21:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-update-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-update-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing fire update evidence");auto samples=field(fixture,"samples");int mismatches=0;
 auto fromBits=[](J value){uint32_t bits=uint32_t(json_object_get_uint64(value));float f;std::memcpy(&f,&bits,4);return f;};auto bits=[](float f){uint32_t b;std::memcpy(&b,&f,4);return b;};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);FireSuppression fire;fire.strength=fromBits(field(sample,"strength_bits"));fire.state=integer(field(sample,"state"));fire.sprayed=integer(field(sample,"sprayed"));FireUpdate update;update.divider=integer(field(sample,"divider"));update.exposureCount=4;int sizeCalls=0,spreadCalls=0,scaleCalls=0,alphaCalls=0;
 update.think(fire,fromBits(field(sample,"dt_bits")),[]{},[]{},[&]{spreadCalls++;},[&]{sizeCalls++;},[&](float){scaleCalls++;},[&](float){alphaCalls++;});
 bool different=bits(fire.strength)!=uint32_t(json_object_get_uint64(field(sample,"result_bits")))||fire.sprayed!=bool(integer(field(sample,"result_sprayed")))||update.divider!=integer(field(sample,"result_divider"))||update.exposureCount!=integer(field(sample,"exposure_count"));
 auto calls=field(sample,"calls");int expectedSize=0,expectedSpread=0;for(int k=0;k<count(calls);k++){auto call=str(at(calls,k));expectedSize+=call=="0x44b060";expectedSpread+=call=="0x449340";}
 different|=sizeCalls!=expectedSize||spreadCalls!=expectedSpread||scaleCalls!=int(field(sample,"presentation_bits")!=nullptr)||alphaCalls!=int(field(sample,"alpha_input_bits")!=nullptr);
 if(scaleCalls)different|=bits(update.presentationScale)!=uint32_t(json_object_get_uint64(field(sample,"presentation_bits")));if(alphaCalls)different|=bits(update.alphaInput)!=uint32_t(json_object_get_uint64(field(sample,"alpha_input_bits")));
 if(different){mismatches++;if(mismatches<6)std::cout<<"Fire update mismatch "<<i<<" strength="<<bits(fire.strength)<<" presentation="<<bits(update.presentationScale)<<"\n";}
 }
 std::cout<<"Fire update: "<<count(samples)<<" original Think calls; mismatches="<<mismatches<<" (visual/audio/path services supplied by fixtures)\n";return mismatches?20:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-suppression-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-suppression-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing fire suppression evidence");auto samples=field(fixture,"samples");int mismatches=0;
 auto fromBits=[](J value){uint32_t bits=uint32_t(json_object_get_uint64(value));float f;std::memcpy(&f,&bits,4);return f;};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);FireSuppression fire;fire.strength=fromBits(field(sample,"strength_bits"));fire.sprayTime=fromBits(field(sample,"spray_time_bits"));fire.state=integer(field(sample,"initial_state"));fire.hasSound=true;int doused=0,stopped=0;fire.spray(fromBits(field(sample,"dt_bits")),fromBits(field(sample,"influence_bits")),integer(field(sample,"terminal")),[&]{doused++;},[&]{stopped++;});uint32_t bits;std::memcpy(&bits,&fire.strength,4);
 if(bits!=uint32_t(json_object_get_uint64(field(sample,"result_bits")))||fire.state!=integer(field(sample,"state"))||fire.sprayed!=bool(integer(field(sample,"sprayed")))||fire.flameEnabled!=bool(integer(field(sample,"flame_enabled")))||doused!=integer(field(sample,"doused"))||stopped!=integer(field(sample,"stopped"))){mismatches++;if(mismatches<6)std::cout<<"Fire spray mismatch "<<i<<" result="<<bits<<" expected="<<json_object_get_uint64(field(sample,"result_bits"))<<"\n";}}
 std::cout<<"Fire suppression: "<<count(samples)<<" original spray/douse calls; mismatches="<<mismatches<<"\n";return mismatches?19:0;
 }
 if(argc>1&&std::string(argv[1])=="--genesis-transform-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/genesis-transform-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing Genesis transform evidence");auto samples=field(fixture,"samples");int mismatches=0,words=0;float maximum=0;std::map<std::string,int> types;
 auto matrix=[](J values){ActorTransform m;for(int i=0;i<9;i++)m.r[i]=num(at(values,i));m.t={num(at(values,9)),num(at(values,10)),num(at(values,11))};return m;};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);std::vector<float> actual;auto kind=str(field(sample,"kind"));if(kind=="point"){V p=matrix(field(sample,"matrix")).point(vec(field(sample,"point")));actual={p.x,p.y,p.z};}else{ActorTransform m;if(kind=="multiply")m=actorMultiply(matrix(field(sample,"a")),matrix(field(sample,"b")));else{auto q=field(sample,"quaternion");m=actorKey({{}, {num(at(q,0)),num(at(q,1)),num(at(q,2)),num(at(q,3))}});}actual.assign(m.r,m.r+9);if(kind=="multiply"){actual.push_back(m.t.x);actual.push_back(m.t.y);actual.push_back(m.t.z);}}
 auto expected=field(sample,"result_bits");for(int k=0;k<count(expected);k++){uint32_t bits;std::memcpy(&bits,&actual[k],4);uint32_t native=uint32_t(json_object_get_uint64(at(expected,k)));float value;std::memcpy(&value,&native,4);maximum=std::max(maximum,std::abs(value-actual[k]));if(bits!=native){mismatches++;types[kind]++;}words++;}
 }
 for(auto pair:types)std::cout<<pair.first<<" differing="<<pair.second<<"\n";std::cout<<"Genesis transforms: "<<count(samples)<<" native calls, "<<words<<" result words; differing words="<<mismatches<<"; maximum error="<<maximum<<"\n";return mismatches?18:0;
 }
 if(argc>1&&std::string(argv[1])=="--actor-pose-check"){
 auto asset=load("actors/vic_man.json");ActorPose actor(asset);auto source=field(asset,"vertices");auto bind=actor.vertices(-1,0);float maximum=0;int failures=0,frames=0;
 for(int i=0;i<count(source);i++){float error=length(bind[i]-vec(field(at(source,i),"bindPosition")));maximum=std::max(maximum,error);if(error>.001f)failures++;}
 auto motions=field(asset,"motions");for(int i=0;i<count(motions);i++){auto motion=at(motions,i);float begin=num(field(motion,"start")),end=num(field(motion,"end"));for(float part:{0.f,.5f,1.f}){auto vertices=actor.vertices(i,begin+(end-begin)*part);for(auto v:vertices)if(!std::isfinite(v.x)||!std::isfinite(v.y)||!std::isfinite(v.z))failures++;frames++;}}
 std::cout<<"Actor pose: "<<bind.size()<<" decoded bind vertices, "<<frames<<" sampled frames; maximum bind error="<<maximum<<"; failures="<<failures<<" (asset consistency, not native pose parity)\n";return failures?17:0;
 }
 if(argc>1&&std::string(argv[1])=="--actor-animation-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/actor-animation-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing actor animation evidence");auto samples=field(fixture,"samples");int mismatches=0,poses=0;
 auto u32=[](J value){return uint32_t(json_object_get_uint64(value));};auto bits=[](float value){uint32_t word;std::memcpy(&word,&value,4);return word;};auto fromBits=[&](J value){uint32_t word=u32(value);float f;std::memcpy(&f,&word,4);return f;};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);ActorAnimation animation;animation.select(0,fromBits(field(sample,"begin_bits")),fromBits(field(sample,"end_bits")),fromBits(field(sample,"speed_bits")),u32(field(sample,"started")));if(animation.period!=u32(field(sample,"period"))||bits(animation.duration)!=u32(field(sample,"duration_bits")))mismatches++;auto frames=field(sample,"poses");for(int k=0;k<count(frames);k++){auto frame=at(frames,k);if(bits(animation.poseTime(u32(field(frame,"now"))))!=u32(field(frame,"bits")))mismatches++;poses++;}}
 std::cout<<"Actor animation: "<<count(samples)<<" original selections, "<<poses<<" original pose updates; mismatches="<<mismatches<<"\n";return mismatches?16:0;
 }
 if(argc>1&&std::string(argv[1])=="--victim-sound-init-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/victim-sound-init-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing victim initial sound evidence");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);Random random(uint32_t(json_object_get_uint64(field(sample,"seed"))));for(int k=0;k<integer(field(sample,"advance"));k++)random.nextWord();auto before=random.drawCount;VictimSoundChoices choices;choices.initialize(random);if(choices.deadline!=uint32_t(json_object_get_uint64(field(sample,"deadline")))||random.drawCount-before!=uint64_t(integer(field(sample,"draws")))||624-random.index!=size_t(integer(field(sample,"remaining")))||choices.previous!=0)mismatches++;}
 std::cout<<"Victim initial sound: "<<count(samples)<<" original timer-store captures; mismatches="<<mismatches<<"\n";return mismatches?15:0;
 }
 if(argc>1&&std::string(argv[1])=="--victim-pickup-geometry-check"){
 Player player;VictimPickupBox standing=victimPickupBox(player,{});player.crouch=true;VictimPickupBox crouched=victimPickupBox(player,{});
 struct Case{const char* name;int state,type;bool available;VictimPickupBox box;V center;float radius;bool expected;};
 const Case cases[]={
  {"x-player-max-face-equality",0,1,true,standing,{90,42.5f,0},60,true},
  {"x-player-min-face-equality",0,1,true,standing,{-90,42.5f,0},60,true},
  {"x-outside",0,1,true,standing,{90.01f,42.5f,0},60,false},
  {"z-player-max-face-equality",0,1,true,standing,{0,42.5f,90},60,true},
  {"z-player-min-face-equality",0,1,true,standing,{0,42.5f,-90},60,true},
  {"y-player-min-face-equality",0,1,true,standing,{0,-90,0},60,true},
  {"standing-y-max-equality",0,1,true,standing,{0,145,0},60,true},
  {"crouch-y-max-equality",0,1,true,crouched,{0,85,0},60,true},
  {"crouch-y-outside",0,1,true,crouched,{0,85.01f,0},60,false},
  {"standing-y-over-crouch",0,1,true,standing,{0,85.01f,0},60,true},
  {"state-5-rejected",5,1,true,standing,{},60,false},
  {"state-6-rejected",6,1,true,standing,{},60,false},
  {"other-type-rejected",0,2,true,standing,{},60,false},
  {"missing-world-box-rejected",0,1,false,standing,{},60,false}
 };
 int mismatches=0;
 for(const auto& sample:cases){bool actual=sourceVictimPickupCandidate(sample.state,sample.type,sample.available,sample.box,sample.center,sample.radius);if(actual!=sample.expected){mismatches++;std::cout<<"Mismatch: "<<sample.name<<" expected="<<sample.expected<<" actual="<<actual<<"\n";}}
 std::cout<<"Victim pickup geometry: "<<std::size(cases)<<" source-derived boundary/gate cases; mismatches="<<mismatches<<" (isolated predicate, no scheduler claim)\n";return mismatches?49:0;
 }
 if(argc>1&&std::string(argv[1])=="--world-startup-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/world-startup-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing world startup evidence");auto samples=field(fixture,"samples");int updates=0,mismatches=0;
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);WorldStartup startup;startup.remaining=uint32_t(json_object_get_uint64(field(sample,"initial_remaining")));startup.complete=integer(field(sample,"initial_complete"));int state=2,calls=0;auto steps=field(sample,"steps");
 for(int k=0;k<count(steps);k++){startup.update([&]{calls++;},[&]{state=3;});auto expected=at(steps,k);if(startup.remaining!=uint32_t(json_object_get_uint64(field(expected,"remaining")))||startup.complete!=bool(integer(field(expected,"complete")))||state!=integer(field(expected,"main_state"))||calls!=integer(field(expected,"script_calls")))mismatches++;updates++;}
 auto callsRecorded=field(sample,"calls");for(int k=0;k<count(callsRecorded);k++)if(integer(field(at(callsRecorded,k),"mode")))mismatches++;
 }
 Scripts probe(load("scripts.json"));int hostCalls=0;probe.host=[&](int id,std::string command){hostCalls++;return id==playerId&&command=="select axe";};probe.hint=[](std::string,float){};probe.findEntity=[&](std::string name){return name=="player"?playerId:-1;};probe.loadLevel("training.evs",playerId,true);
 if(hostCalls||!probe.bindings.empty()||!probe.variables.count("Door")||!probe.reported.empty())mismatches++;
 WorldStartup phase;for(int i=0;i<4;i++){phase.update([&]{probe.loadLevel("training.evs",playerId,false);},[]{});if(hostCalls!=(i==3?1:0))mismatches++;}
 if(!phase.complete||probe.bindings.empty()||!probe.reported.empty())mismatches++;
 std::cout<<"World startup: "<<updates<<" original updates; Training early set/exec pass and delayed full pass; mismatches="<<mismatches<<"\n";return mismatches?14:0;
 }
 if(argc>1&&std::string(argv[1])=="--model-floor-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/model-floor-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing model floor evidence");int mismatches=0;auto samples=field(fixture,"samples");
 auto fromBits=[](J array){V v;for(int axis=0;axis<3;axis++){uint32_t bits=uint32_t(json_object_get_uint64(at(array,axis)));std::memcpy(&v[axis],&bits,4);}return v;};
 auto sameBits=[](V v,J expected){for(int axis=0;axis<3;axis++){uint32_t bits;std::memcpy(&bits,&v[axis],4);if(bits!=uint32_t(json_object_get_uint64(at(expected,axis))))return false;}return true;};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);auto request=field(sample,"request");V position=fromBits(field(sample,"position_bits")),mins=fromBits(field(sample,"mins_bits")),maxs=fromBits(field(sample,"maxs_bits"));bool called=false;
 bool found=placeModelOnFloor(position,mins,maxs,[&](V start,V delta,V lo,V hi,V& point){called=true;if(!sameBits(start,field(request,"start_bits"))||!sameBits(delta,field(request,"delta_bits"))||!sameBits(lo,field(request,"mins_bits"))||!sameBits(hi,field(request,"maxs_bits")))mismatches++;point=fromBits(field(sample,"contact_bits"));return true;},[]{throw std::runtime_error("Unexpected floor failure");});
 if(!called||!found||!sameBits(position,field(sample,"result_bits")))mismatches++;
 }
 std::cout<<"Model floor placement: "<<count(samples)<<" original calls; trace arguments and result bits; mismatches="<<mismatches<<"\n";return mismatches?13:0;
 }
 if(argc>1&&std::string(argv[1])=="--spawn-queue-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/spawn-queue-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing spawn queue evidence");int comparisons=0,mismatches=0;auto samples=field(fixture,"samples");
 for(int i=0;i<count(samples);i++){
 auto sample=at(samples,i);SpawnQueue queue;std::vector<std::pair<std::string,int>> events;std::map<std::pair<std::string,int>,std::vector<J>> additions;
 auto plans=field(sample,"additions");for(int k=0;k<count(plans);k++){auto plan=at(plans,k);additions[{str(at(plan,0)),integer(at(plan,1))}].push_back(at(plan,2));}
 std::function<void(J)> enqueue;std::function<void(const char*,int)> event;
 enqueue=[&](J value){if(!value){queue.enqueue({});return;}int id=integer(value);queue.enqueue([&,id]{event("construct",id);return id;});};
 event=[&](const char* phase,int id){events.push_back({phase,id});for(auto child:additions[{phase,id}])enqueue(child);};
 queue.registerEntity=[&](int id){event("register",id);};queue.firstPass=[&](int id){event("first",id);};queue.secondPass=[&](int id){event("second",id);};
 auto initial=field(sample,"initial");for(int k=0;k<count(initial);k++)enqueue(at(initial,k));auto batches=field(sample,"batches");
 for(int k=0;k<count(batches);k++){events.clear();queue.drain();auto expected=at(batches,k);auto native=field(expected,"events");if(events.size()!=size_t(count(native)))mismatches++;for(int j=0;j<count(native);j++){auto pair=at(native,j);if(size_t(j)>=events.size()||events[j]!=std::make_pair(str(at(pair,0)),integer(at(pair,1))))mismatches++;comparisons++;}if(queue.pending.size()!=size_t(integer(field(expected,"pending"))))mismatches++;comparisons++;}
 }
 std::cout<<"Spawn queue: "<<count(samples)<<" original scenarios, "<<comparisons<<" event/count comparisons; mismatches="<<mismatches<<"\n";return mismatches?12:0;
 }
 if(argc>1&&std::string(argv[1])=="--startup-check"){std::string previous;for(auto e:startup){auto cl=str(field(e,"class"));if(cl!=previous){std::cout<<"\n"<<cl<<":";previous=cl;}std::cout<<" "<<integer(field(e,"index"));}std::cout<<"\nWorld route="<<worldIdentity.route.route<<" generation="<<worldIdentity.route.generation<<"\n";for(auto e:startup)if(str(field(e,"class"))=="RCS_FireRescue"){int id=integer(field(e,"index"));auto& identity=trainingEntities.at(id);if(trainingRoutes.resolve(identity.route.route,identity.route.generation)!=&identity)throw std::runtime_error("Training fire route did not resolve to its startup entity");std::cout<<"Fire entity="<<id<<" route="<<identity.route.route<<" generation="<<identity.route.generation<<" state="<<(identity.fire?"initialized":"missing")<<"\n";}for(auto& victim:victimSpawns){std::cout<<victim.name<<" candidate entities:";for(auto& candidate:victim.candidates)std::cout<<" "<<candidate.entity;std::cout<<" motion="<<victim.initialMotion<<" speed="<<victim.initialMotionSpeed;if(victim.spawnSelected){std::cout<<" selected="<<victim.selectedEntity<<" authored="<<victim.selectedCandidatePosition.x<<","<<victim.selectedCandidatePosition.y<<","<<victim.selectedCandidatePosition.z<<" floor="<<(victim.floorPlacementSucceeded?"hit":"miss")<<" position="<<victim.selectedPosition.x<<","<<victim.selectedPosition.y<<","<<victim.selectedPosition.z<<" heading="<<victim.selectedHeading;}std::cout<<"\n";}return 0;}
 if(argc>1&&std::string(argv[1])=="--script-check"){for(int i=0;i<4;i++)startupTick();std::cout<<"Training startup: "<<vm.bindings.size()<<" scripted entities; "<<vm.variables.size()<<" variables; ash-density="<<std::to_string(player.ashDensity)<<"; rescuebot="<<(player.rescueBotEnabled?(*player.rescueBotEnabled?"on":"off"):"unset")<<"; unsupported="<<vm.reported.size()<<"\n";return vm.reported.empty()?0:5;}
 if(argc>1&&std::string(argv[1])=="--script-queue-check"){
  Scripts check(load("scripts.json"));std::vector<std::string> calls;int mismatches=0;
  check.host=[&](int,std::string command){calls.push_back(command);return true;};
  check.run("que [first, wait 3, second, third]",700,"queue-check");
  if(!calls.empty())mismatches++;
  check.advance(10);
  if(calls!=std::vector<std::string>{"first"})mismatches++;
  check.advance(20);
  if(calls!=std::vector<std::string>{"first"})mismatches++;
  check.advance(3010);
  if(calls!=std::vector<std::string>{"first"})mismatches++;
  check.advance(3021);
  if(calls!=std::vector<std::string>{"first","second"})mismatches++;
  check.advance(3030);
  if(calls!=std::vector<std::string>{"first","second","third"})mismatches++;
  check.run("que [wait 3, discarded]",701,"flush-check");check.advance(3040);
  check.run("flush",701,"flush-check");check.advance(3050);
  if(calls.size()!=3)mismatches++;
  check.run("que [wait 3, after-idle]",703,"idle-check");check.advance(3060);
  check.run("idle",703,"idle-check");check.advance(3070);
  if(calls.size()!=4||calls.back()!="after-idle")mismatches++;
  size_t before=calls.size();check.run("cmd [immediate-a, immediate-b]",702,"cmd-check");
  if(calls.size()!=before+2||calls[before]!="immediate-a"||calls[before+1]!="immediate-b")mismatches++;
  std::cout<<"Script queues: append order, one command per update, 3-second wait, flush, idle, and immediate cmd; mismatches="<<mismatches<<"\n";
  return mismatches?30:0;
 }
 if(argc>1&&std::string(argv[1])=="--clock-check"){auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/clock-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing clock oracle");auto samples=field(fixture,"samples");int mismatches=0;for(int i=0;i<count(samples);i++){auto sample=at(samples,i);auto before=field(sample,"before"),expected=field(sample,"after");auto value=[&](J array,int n){return uint32_t(json_object_get_uint64(at(array,n)));};auto asFloat=[&](uint32_t bits){float result;std::memcpy(&result,&bits,4);return result;};auto bits=[&](float n){uint32_t result;std::memcpy(&result,&n,4);return result;};GameClock clock;clock.previous=value(before,0);clock.current=value(before,1);clock.origin=value(before,2);clock.reserved=value(before,3);clock.dt=asFloat(value(before,4));clock.fps=asFloat(value(before,5));clock.filteredFps=asFloat(value(before,6));clock.paused=value(before,7)!=0;uint32_t now=uint32_t(json_object_get_uint64(field(sample,"now")));int sleeps=0;while(!clock.tick(now)&&!clock.paused){now++;sleeps++;if(sleeps>100)throw std::runtime_error("Clock wait runaway");}uint32_t actual[]={clock.previous,clock.current,clock.origin,clock.reserved,bits(clock.dt),bits(clock.fps),bits(clock.filteredFps),uint32_t(clock.paused)};for(int k=0;k<8;k++)if(actual[k]!=value(expected,k)){mismatches++;if(mismatches<8)std::cout<<"Clock mismatch sample="<<i<<" field="<<k<<" actual="<<actual[k]<<" expected="<<value(expected,k)<<"\n";}if(sleeps!=integer(field(sample,"sleep_calls"))||now!=uint32_t(json_object_get_uint64(field(sample,"end_now"))))mismatches++;}std::cout<<"Clock: "<<count(samples)<<" original updates, mismatches="<<mismatches<<"\n";return mismatches?7:0;}
 if(argc>1&&std::string(argv[1])=="--victim-sound-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/victim-sound-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing victim sound oracle");auto samples=field(fixture,"samples");int mismatches=0;Random random(0);VictimSoundChoices sounds;
 auto u32=[](J o,const char* name){return uint32_t(json_object_get_uint64(field(o,name)));};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);if(integer(field(sample,"step"))==0){random.seed(u32(sample,"seed"));sounds=VictimSoundChoices{500,-1};}auto before=random.drawCount;int played=sounds.update(random,u32(sample,"now"),integer(field(sample,"count")),num(field(sample,"low")),num(field(sample,"high")));bool different=sounds.deadline!=u32(sample,"deadline_after")||uint32_t(sounds.previous)!=u32(sample,"index")||random.drawCount-before!=uint64_t(integer(field(sample,"draws")))||int(played>=0)!=integer(field(sample,"plays"));if(different){mismatches++;if(mismatches<8)std::cout<<"Victim sound mismatch sample="<<i<<"\n";}}
 std::cout<<"Victim sounds: "<<count(samples)<<" original updates, mismatches="<<mismatches<<"\n";return mismatches?9:0;
 }
 if(argc>1&&std::string(argv[1])=="--fire-effects-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/fire-effects-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing fire-effect oracle");auto samples=field(fixture,"samples");int mismatches=0;Random random(0);FireVisual visual;
 auto bits=[](float v){uint32_t b;std::memcpy(&b,&v,4);return b;};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);int step=integer(field(sample,"step"));if(step==0){random.seed(uint32_t(json_object_get_uint64(field(sample,"seed"))));visual=FireVisual();}else visual.effects.spritesCreated=true;auto before=random.drawCount;visual.initialize(random,integer(field(sample,"smoke")),0,integer(field(sample,"frames")));auto& effects=visual.effects;auto expected=field(sample,"sprites");bool different=random.drawCount-before!=uint64_t(integer(field(sample,"draws")))||bits(effects.smokeInterval)!=uint32_t(json_object_get_uint64(field(sample,"smoke_interval_bits")));for(int k=0;k<4;k++){auto e=at(expected,k);auto& actual=effects.sprites[k];different|=bits(actual.rate)!=uint32_t(json_object_get_uint64(field(e,"rate_bits")))||actual.frame!=integer(field(e,"frame"))||actual.nextFrame!=integer(field(e,"next_frame"));}if(different){mismatches++;if(mismatches<8)std::cout<<"Fire effect mismatch sample="<<i<<" seed="<<json_object_get_uint64(field(sample,"seed"))<<" step="<<step<<"\n";}}
 std::cout<<"Fire effects: "<<count(samples)<<" original initializations, mismatches="<<mismatches<<"\n";return mismatches?8:0;
 }
 if(argc>1&&std::string(argv[1])=="--rng-state-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/rng-state-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing original RNG states");auto samples=field(fixture,"samples");int mismatches=0,comparisons=0;
 auto u32=[](J v){return uint32_t(json_object_get_uint64(v));};
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);Random generated(u32(field(sample,"seed")));int consumed=integer(field(sample,"drawn"));for(int k=0;k<consumed;k++)generated.nextWord();Random::State captured;captured.nextIndex=integer(field(sample,"next_index"));captured.ordinal=consumed;auto words=field(sample,"words");if(count(words)!=624)throw std::runtime_error("Incomplete original RNG capture");for(int k=0;k<624;k++){captured.words[k]=u32(at(words,k));comparisons++;if(captured.words[k]!=generated.words[k])mismatches++;}if(captured.nextIndex!=generated.index)mismatches++;Random replay(0);replay.restore(captured);auto following=field(sample,"following");for(int k=0;k<count(following);k++){comparisons++;if(replay.nextWord()!=u32(at(following,k)))mismatches++;}auto saved=replay.checkpoint();Random second(1);second.restore(saved);for(int k=0;k<128;k++)if(second.nextWord()!=replay.nextWord())mismatches++;}
 std::cout<<"RNG state: "<<count(samples)<<" original captures, "<<comparisons<<" word/output comparisons, mismatches="<<mismatches<<"\n";return mismatches?10:0;
 }
 if(argc>1&&std::string(argv[1])=="--sound-queue-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/sound-queue-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing original sound queues");auto samples=field(fixture,"samples");int mismatches=0;
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);Random random(uint32_t(json_object_get_uint64(field(sample,"seed"))));SoundRequests sounds(random);struct Event{std::string kind;int channel,position;};std::vector<Event> actual;int channel=0;std::map<int,int> channels;int sampleLength=integer(field(sample,"sample_length"));sounds.beginPlayback=[&](int id,bool loop){channels[id]=++channel;actual.push_back({"play",channel,0});};sounds.setPhase=[&](int id,float phase){actual.push_back({"phase",channels[id],int(double(phase)*sampleLength)});};sounds.beginLoading();auto requests=field(sample,"requests");for(int k=0;k<count(requests);k++){auto request=at(requests,k);sounds.request(integer(field(request,"id")),integer(field(request,"loop")));}if(int(sounds.pending.size())!=integer(field(sample,"queued"))||random.drawCount!=uint64_t(integer(field(sample,"loading_draws")))||!actual.empty())mismatches++;auto before=random.drawCount;sounds.resume();auto expected=field(sample,"events");if(random.drawCount-before!=uint64_t(integer(field(sample,"resume_draws")))||int(sounds.pending.size())!=integer(field(sample,"after_queue_size"))||actual.size()!=size_t(count(expected)))mismatches++;else for(size_t k=0;k<actual.size();k++){auto e=at(expected,int(k));if(actual[k].kind!=str(field(e,"kind"))||actual[k].channel!=integer(field(e,"channel"))||(actual[k].kind=="phase"&&actual[k].position!=integer(field(e,"position"))))mismatches++;}}
 std::cout<<"Sound queue: "<<count(samples)<<" original replays, mismatches="<<mismatches<<"\n";return mismatches?11:0;
 }
 if(argc>1&&std::string(argv[1])=="--rng-check"){
 auto fixture=json_object_from_file((assetDirectory()+"/../../evidence/rng-oracle.json").c_str());if(!fixture)throw std::runtime_error("Missing RNG oracle");auto sequences=field(fixture,"sequences");int draws=0,indices=0,mismatches=0;
 for(int i=0;i<count(sequences);i++){auto sequence=at(sequences,i);uint32_t seed=uint32_t(json_object_get_uint64(field(sequence,"seed")));Random random(seed);auto values=field(sequence,"float_bits");for(int k=0;k<count(values);k++){float value=random.unit();uint32_t bits;std::memcpy(&bits,&value,4);if(bits!=uint32_t(json_object_get_uint64(at(values,k))))mismatches++;draws++;}random.seed(seed);auto spots=field(sequence,"victim_indices");for(int k=0;k<count(spots);k++){auto sample=at(spots,k);if(random.spotIndex(integer(field(sample,"count")))!=integer(field(sample,"index")))mismatches++;indices++;}}
 int mixedCount=0;for(int i=0;i<count(sequences);i++){auto sequence=at(sequences,i);Random random(uint32_t(json_object_get_uint64(field(sequence,"seed"))));auto mixed=field(sequence,"mixed");for(int k=0;k<count(mixed);k++){auto sample=at(mixed,k);auto kind=str(field(sample,"kind"));uint32_t actual=0,expected=0;auto u32=[&](const char* name){return uint32_t(json_object_get_uint64(field(sample,name)));};auto floatBits=[&](float value){uint32_t bits;std::memcpy(&bits,&value,4);return bits;};if(kind=="word"){actual=random.nextWord();expected=u32("value");}else if(kind=="bounded"){actual=random.bounded(u32("bound"));expected=u32("value");}else if(kind=="unit"){actual=floatBits(random.unit());expected=u32("bits");}else if(kind=="range"){auto lo=u32("lo"),hi=u32("hi");float low,high;std::memcpy(&low,&lo,4);std::memcpy(&high,&hi,4);actual=floatBits(float(random.range(low,high)));expected=u32("bits");}else throw std::runtime_error("Unknown oracle operation");if(actual!=expected){mismatches++;if(mismatches<6)std::cout<<"RNG mismatch seed="<<json_object_get_uint64(field(sequence,"seed"))<<" operation="<<k<<" type="<<kind<<"\n";}mixedCount++;}}
 int soundCount=0;Random soundRandom(0);auto sounds=field(fixture,"sound_choices");for(int i=0;i<count(sounds);i++){auto sample=at(sounds,i);if(integer(field(sample,"step"))==0)soundRandom.seed(uint32_t(json_object_get_uint64(field(sample,"seed"))));auto before=soundRandom.drawCount;int chosen=soundRandom.soundIndex(integer(field(sample,"count")),integer(field(sample,"previous")));if(chosen!=integer(field(sample,"index"))||soundRandom.drawCount-before!=uint64_t(integer(field(sample,"draws"))))mismatches++;soundCount++;}
 std::cout<<"RNG: "<<draws<<" bit-exact float comparisons, "<<indices<<" victim-index comparisons, "<<mixedCount<<" mixed operations, "<<soundCount<<" original sound-choice comparisons; mismatches="<<mismatches<<"\n";return mismatches?6:0;
 }
 if(argc>1&&std::string(argv[1])=="--point-collision-check"){
 auto tests=json_object_from_file((assetDirectory()+"/../../evidence/genesis-point-oracle.json").c_str());if(!tests)throw std::runtime_error("Missing point collision evidence");auto samples=field(tests,"samples");Collision original(collision.data);int mismatches=0,contacts=0;float maximum=0;
 for(int i=0;i<count(samples);i++){auto s=at(samples,i),e=field(s,"expected");Hit hit;V start=vec(field(s,"start"));bool found=original.tracePoint(start,start+vec(field(s,"delta")),1,2,hit),expected=e&&json_object_get_type(e)!=json_type_null;bool bad=found!=expected;if(found&&expected){contacts++;float error=length(hit.point-vec(field(e,"point")));maximum=std::max(maximum,error);bad|=error>.001||length(hit.normal-vec(field(e,"normal")))>.001||hit.model!=integer(field(e,"model"));}if(bad){mismatches++;if(mismatches<6)std::cout<<"Point mismatch "<<i<<"\n";}}
 std::cout<<"Point traces: "<<count(samples)<<", contacts: "<<contacts<<", mismatches: "<<mismatches<<", maximum error: "<<maximum<<"\n";return mismatches?3:0;
 }
 if(argc>1&&std::string(argv[1])=="--collision-check"){
 auto tests=json_object_from_file((assetDirectory()+"/../../evidence/retail-genesis-collision-oracle.json").c_str());if(!tests)throw std::runtime_error("Missing collision evidence");auto samples=field(tests,"samples");Collision original(collision.data);int checked=0,mismatches=0;float maximum=0;
 for(int i=0;i<count(samples);i++){auto sample=at(samples,i);auto expected=field(sample,"expected");if(!expected||!integer(field(expected,"recordedContact")))continue;Hit h;bool hit=original.trace(vec(field(sample,"start")),vec(field(sample,"delta")),vec(field(sample,"mins")),vec(field(sample,"maxs")),h);V point=vec(field(expected,"point")),normal=vec(field(expected,"normal"));float error=hit?length(h.point-point):99999;maximum=std::max(maximum,error);checked++;if(!hit||error>.001f||length(h.normal-normal)>.001f){mismatches++;if(mismatches<6)std::cout<<"Mismatch "<<i<<" error="<<error<<" model="<<h.model<<"\n";}}
 std::cout<<"Collision contacts: "<<checked<<", mismatches: "<<mismatches<<", maximum point error: "<<maximum<<"\n";return mismatches?3:0;
 }
 if(argc>1&&std::string(argv[1])=="--smoke-test"){for(int i=0;i<120;i++)player.step(.017,0,0,0,false,false,false,collision);std::cout<<"Training loaded; settled spawn "<<player.p.x<<","<<player.p.y<<","<<player.p.z<<" grounded="<<player.ground<<"\n";return player.ground?0:2;}
 bool fireRenderCheck=argc>1&&std::string(argv[1])=="--fire-render-check";bool renderCheck=fireRenderCheck||(argc>1&&std::string(argv[1])=="--render-check");int rendered=0;
 if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER))throw std::runtime_error(SDL_GetError());SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);auto window=SDL_CreateWindow("FDNY Training — native reconstruction",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,800,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|(renderCheck?SDL_WINDOW_HIDDEN:0));if(!window)throw std::runtime_error(SDL_GetError());auto context=SDL_GL_CreateContext(window);if(!context)throw std::runtime_error(SDL_GetError());SDL_GL_SetSwapInterval(1);if(!renderCheck)SDL_SetRelativeMouseMode(SDL_TRUE);
 VictimRenderer victimRenderer(trainingManAsset);victimRenderer.load();
 std::vector<Mesh> meshes;auto groups=field(world,"groups");for(int i=0;i<count(groups);i++){auto g=at(groups,i);if(str(field(g,"texture")).rfind("col_",0)==0)continue;Mesh m;m.model=integer(field(g,"model"));for(int k=0;k<count(field(g,"positions"));k++)m.positions.push_back(num(at(field(g,"positions"),k)));for(int k=0;k<count(field(g,"uvs"));k++)m.uvs.push_back(num(at(field(g,"uvs"),k)));auto bmp=assetDirectory()+"/textures/"+str(field(g,"texture"))+".bmp";if(auto surface=SDL_LoadBMP(bmp.c_str())){auto rgb=SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ABGR8888,0);glGenTextures(1,&m.texture);glBindTexture(GL_TEXTURE_2D,m.texture);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,rgb->w,rgb->h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgb->pixels);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);SDL_FreeSurface(rgb);SDL_FreeSurface(surface);}meshes.push_back(std::move(m));}
 FireRenderer fireRenderer;fireRenderer.load();std::vector<FireEntity> renderFixture;int fireQuads=0;
 if(fireRenderCheck){Random shared(4357);int cycle=0;FireEntity fire;fire.initialize(shared,0,cycle,{824,-120,-2092},150,250,0,true,true,[&](FireVisual& v){FireFloor::alignInWorld(v,collision);},[](int){});fire.visual.sizeUpdate(0);renderFixture.push_back(fire);}
 TextOverlay overlay;std::map<int,bool> touching,propTouching;ExtinguisherContacts liveExtinguisherContacts;FireMessageQueue liveFireMessages;
 bool running=true,captured=true,actionHeld=false,actionPressed=false,actionReleased=false,usePressed=false;float yaw=3*3.14159265f/180,pitch=0;GameClock clock;clock.origin=SDL_GetTicks();std::map<int,ActorAnimation> victimAnimations;auto actorMotions=field(trainingManAsset,"motions");for(const auto& spawn:victimSpawns)if(spawn.motionInitialized&&spawn.initialMotion>=0){auto motion=at(actorMotions,spawn.initialMotion);auto& animation=victimAnimations[spawn.entity];animation.select(spawn.initialMotion,num(field(motion,"start")),num(field(motion,"end")),spawn.initialMotionSpeed,clock.current);}bool jumpEdge=false,crouchEdge=false;FrameTransform playerFrame;HeadMotion headMotion;ExtinguisherPose::CallbackState extinguisherAction;HoseSprayAction::State hoseSprayAction;float extinguisherPhase=0;
 // 0044D550 configures the player-view follower. The extinguisher constructor
 // attaches its tool follower to it; 00461F80 advances that follower only while active.
 Follower playerViewFollower;playerViewFollower.flags=0x2e;playerViewFollower.threshold={20,1000,20};playerViewFollower.settling={.05f,0,.05f};playerViewFollower.smoothing={0,.05f,0};playerViewFollower.held=true;
 struct TrainingToolState {
  Follower follower;FrameTransform worldFrame;
  // Extinguisher 00461830 initializes item +0x610 to (-20,-20,30).
  V activePoseOffsets{-20,-20,30};
  bool hasWorldFrame=false;
 };
 std::map<std::string,TrainingToolState> itemFollowers;std::set<std::string> attachedItems;
 // A fresh TPlayerInfo defaults to DifficultyLevel 1, selecting the source's
 // 40-degree pitch cap. SDL relative-Y units are not calibrated to the old backend.
 while(running){SDL_Event event;while(SDL_PollEvent(&event)){if(event.type==SDL_QUIT)running=false;if(event.type==SDL_KEYDOWN&&!event.key.repeat){auto key=event.key.keysym.sym;if(key==SDLK_ESCAPE){captured=!captured;SDL_SetRelativeMouseMode(captured?SDL_TRUE:SDL_FALSE);}if(key==SDLK_SPACE)jumpEdge=true;if(key==SDLK_SPACE||key==SDLK_LSHIFT||key==SDLK_RSHIFT)usePressed=true;if(key==SDLK_c||key==SDLK_LCTRL||key==SDLK_RCTRL)crouchEdge=true;if((key==SDLK_1||key==SDLK_KP_1)&&equipment.count("axe"))selected="axe";if((key==SDLK_2||key==SDLK_KP_2)&&equipment.count("extinguisher"))selected="extinguisher";}if(event.type==SDL_MOUSEBUTTONDOWN){if(event.button.button==SDL_BUTTON_RIGHT)jumpEdge=true;if(event.button.button==SDL_BUTTON_LEFT&&!actionHeld){actionHeld=true;actionPressed=true;}}if(event.type==SDL_MOUSEBUTTONUP&&event.button.button==SDL_BUTTON_LEFT&&actionHeld){actionHeld=false;actionReleased=true;}if(event.type==SDL_MOUSEMOTION&&captured){yaw-=event.motion.xrel*.0025f;pitch-=event.motion.yrel*(0.1f*0.01745329251994329577f);pitch=std::clamp(pitch,-40.f*0.01745329251994329577f,40.f*0.01745329251994329577f);}}
 if(!clock.tick(SDL_GetTicks())){SDL_Delay(1);continue;}float dt=clock.dt;startupTick();auto keys=SDL_GetKeyboardState(nullptr);float moveForward=captured?float(keys[SDL_SCANCODE_W]-keys[SDL_SCANCODE_S]):0.f;float moveSide=captured?.75f*float(keys[SDL_SCANCODE_D]-keys[SDL_SCANCODE_A]):0.f;for(auto& door:doors)door.update(player.p,dt,collision);if(captured)player.step(dt,moveForward,moveSide,yaw,keys[SDL_SCANCODE_SPACE]||(SDL_GetMouseState(nullptr,nullptr)&SDL_BUTTON(SDL_BUTTON_RIGHT)),jumpEdge,crouchEdge,collision,pitch);jumpEdge=crouchEdge=false;vm.advance(clock.current);
 // 00452570 tests the movement vector against 1e-7. 0044F340 then chooses
 // the 00491140 mode from that flag, player state, and active-view byte.
 bool moving=std::sqrt(moveForward*moveForward+moveSide*moveSide)>1e-7f;int playerState=player.crouch?4:3;int requestedHeadMode=HeadMotion::viewMode(playerState,true,moving);auto headOffset=headMotion.update(requestedHeadMode,dt);
 playerFrame.position=player.p;float headingCos=std::cos(yaw),headingSin=std::sin(yaw);playerFrame.axis={V{headingCos,0,-headingSin},V{0,1,0},V{headingSin,0,headingCos}};
 // Extinguisher ACT is static; its item view/aim offsets remain zero. ACTION
 // is MOUSE_LB (00437F30/004395C0), with callbacks before this view update.
 V itemViewOffset{},itemAimOffset{};float viewAngle=pitch*57.29577951308232f;float viewHeight=playerState==4?10.f:70.f;
 // 0044C2A0 switches the selected item through slot +0x3c. The extinguisher
 // setter 00461270 clears its ACTION state when the item is deselected.
 if(selected!=previousSelected){
  if(previousSelected=="extinguisher"&&extinguisherAction.active){auto old=itemFollowers.find("extinguisher");if(old!=itemFollowers.end()&&old->second.hasWorldFrame)extinguisherAction=ExtinguisherPose::endWindow(extinguisherAction,clock.current,old->second.worldFrame);}
  previousSelected=selected;
 }
 if(attachedItems.insert(selected).second){auto& initial=itemFollowers[selected];initial.follower.position=playerViewFollower.position;initial.follower.previous=playerViewFollower.position;initial.follower.basis=playerViewFollower.basis;}
 auto& tool=itemFollowers[selected];
 if(selected=="extinguisher"&&!tool.hasWorldFrame){tool.worldFrame=ExtinguisherPose::world(tool.follower);tool.hasWorldFrame=true;}
 if(selected!="extinguisher"){tool.follower.update(playerViewFollower.position,dt);tool.follower.basis=playerViewFollower.basis;}
 if(actionPressed&&selected=="extinguisher"&&tool.hasWorldFrame)extinguisherAction=ExtinguisherPose::beginWindow(extinguisherAction,clock.current,tool.worldFrame);
 if(actionReleased&&selected=="extinguisher"&&extinguisherAction.active)extinguisherAction=ExtinguisherPose::endWindow(extinguisherAction,clock.current,tool.worldFrame);
 // Hose Spray ACTION callbacks write +0xC58/+0x3B0 here; audio and its +0x24 update remain unported.
 if(actionPressed&&selected=="hose")HoseSprayAction::begin(hoseSprayAction,clock.current);
 if(actionReleased&&selected=="hose")HoseSprayAction::end(hoseSprayAction);
 actionPressed=actionReleased=false;
 V headPosition{headOffset.x,headOffset.y,headOffset.z};auto viewResult=ViewUpdate::transition(playerFrame,headPosition,itemViewOffset,itemAimOffset,{0,0,0},{0,0,0},viewHeight,1.f,viewAngle,dt,playerViewFollower,playerState==4);playerViewFollower=viewResult.follower;
 auto useTarget=UseManager::select(playerViewFollower.position,playerViewFollower.basis[2],valveCandidates,hoseMode,attachedValve);
 if(usePressed){usePressed=false;if(useTarget.index>=0){const auto& prop=valveCandidates[size_t(useTarget.index)];if(useTarget.action==1001){selectedBeforeHose=selected;hoseMode=3;attachedValve=prop.entity;selected="hose";}else if(useTarget.action==1002){hoseMode=2;attachedValve=-1;selected=equipment.count("axe")?"axe":(equipment.count("extinguisher")?"extinguisher":selectedBeforeHose);}vm.dispatch(prop.entity,"use");}}
 auto extinguisherIt=itemFollowers.find("extinguisher");bool extinguisherEquipped=selected=="extinguisher";
 if(extinguisherIt!=itemFollowers.end()&&extinguisherEquipped){
  auto& extTool=extinguisherIt->second;extTool.follower.update(playerViewFollower.position,dt);extTool.follower.basis=playerViewFollower.basis;
  if(!extinguisherAction.active){extTool.worldFrame=ExtinguisherPose::world(extTool.follower);extTool.hasWorldFrame=true;}
 }
 if(extinguisherIt!=itemFollowers.end()&&extinguisherEquipped&&extinguisherAction.active){
  auto& extTool=extinguisherIt->second;
  auto activePose=ExtinguisherPose::activeMotion(sharedRandom,dt,extinguisherPhase,extTool.activePoseOffsets);extinguisherPhase=activePose.phase;
  extTool.worldFrame=ExtinguisherPose::world(activePose,extTool.follower);extTool.hasWorldFrame=true;
  // 00461F80 samples its active world frame, then checks FireRescue candidates
  // and appends message 30000 payloads. The collector is the tool's reusable
  // 32-record buffer; this host drains below in the same local update pass.
  liveExtinguisherContacts.reset();float sampleMatrix[9];
  for(int row=0;row<3;row++)for(int column=0;column<3;column++)sampleMatrix[row*3+column]=extTool.worldFrame.axis[column][row];
  auto contactPoints=ExtinguisherSamples::points(sampleMatrix,extTool.worldFrame.position);
  ExtinguisherEffects::SpawnRequest lastSpraySpawn;
  for(auto* identity:fireContactCandidates){if(!identity||!identity->fire)continue;auto& fire=*identity->fire;fire.visual.sizeUpdate(clock.current);
   for(const V point:contactPoints){float influence=FireContact::influence(fire.visual.sizes.size,fire.visual.position,point,60.f,fire.alpha);
    if(!(influence>=.001f))continue;float strength=float(double(influence)+double(influence));auto route=identity->route;
    liveExtinguisherContacts.append(uint32_t(reinterpret_cast<uintptr_t>(&fire)),route.route,route.generation,point,strength,[&](const ExtinguisherContacts::Dispatch& event){liveFireMessages.enqueue(event.route0,event.route1,event.message,event.payload);});
    // 00461F80 checks the fire-strength and absolute timer gates after each
    // accepted contact. The Training pool has the source's unlimited limit;
    // consume its timer and emitter-offset draws now, while particle drawing
    // remains unwired.
    ExtinguisherEffects::afterContact(sharedRandom,clock.current,fire.suppression.strength,
      extinguisherAction.cooldown,true,lastSpraySpawn);
   }
  }
  // Drain +0x8C8 after contact/effect processing. Vtable slot 7 at
  // 004E28C0 maps the negative-charge stop callback to 00461590.
  if(ExtinguisherPose::consumeCharge(extinguisherCharge,dt)){
   extinguisherAction=ExtinguisherPose::endWindow(extinguisherAction,clock.current,extTool.worldFrame);
   extinguisherCharge=0.f;
  }
 }
 // The original owner drains mapped messages in 00429520. Same-frame placement
 // here follows the directly captured contact→drain sequence; player-vs-owner
 // frame order remains unresolved, so this is not whole-run scheduling parity.
 liveFireMessages.drain([&](uint32_t route,uint32_t generation)->FireMessageQueue::Receiver{
  auto* identity=static_cast<TrainingEntityIdentity*>(trainingRoutes.resolve(route,generation));if(!identity||!identity->fire)return {};
  auto* target=identity->fire;return [&,target](uint32_t message,const void* payload){if(message!=ExtinguisherContacts::messageId)return;auto* contact=static_cast<const ExtinguisherContacts::Contact*>(payload);target->spray(dt,contact->strength,false,[]{},[]{});}; // 00449A40 passes terminal=0 to 00448970.
 },[](const FireMessageQueue::Delivery&){});
 // The camera entity is parented to the player follower at +34a4. Use that
 // recovered world basis and position as the renderer's inverse view transform.
 V cameraPosition=playerViewFollower.position;
 hintRemaining=std::max(0.f,hintRemaining-dt);
 for(int i=0;i<count(entities);i++){auto e=at(entities,i);auto bounds=field(e,"bounds");if(!bounds)continue;V lo=vec(field(bounds,"mins")),hi=vec(field(bounds,"maxs"));int id=integer(field(e,"index"));float reach=0;if(str(field(e,"class"))=="RCS_BModel"){auto sourceReach=bmodelReachByEntity.find(id);if(sourceReach==bmodelReachByEntity.end())throw std::runtime_error("Missing source-mapped Training BModel reach for entity "+std::to_string(id));reach=sourceReach->second;}V mn=player.p+V{-30,-30,-30},mx=player.p+player.maxs();bool inside=BModelContact::overlapsExpanded(lo,hi,mn,mx,reach);
 if(inside&&!touching[i])vm.dispatch(id,"touch");if(inside)vm.dispatch(id,"touching");if(!inside&&touching[i])vm.dispatch(id,"untouch");
 touching[i]=inside;
 }
 // RCS_PropModel's authored touch sphere uses the same location/radius copied
 // to its model subobject for USE. Sphere-vs-player-box contacts drive the
 // Training touch scripts, including Hose1 -> touchhose 25.
 V propPlayerMin=player.p+V{-30,-30,-30},propPlayerMax=player.p+player.maxs();
 for(const auto& prop:propModels){bool inside=UseManager::sphereTouchesBox(prop.center,prop.radius,propPlayerMin,propPlayerMax);bool was=propTouching[prop.entity];if(inside&&!was)vm.dispatch(prop.entity,"touch");if(inside)vm.dispatch(prop.entity,"touching");if(!inside&&was)vm.dispatch(prop.entity,"untouch");propTouching[prop.entity]=inside;}
 int w,h;SDL_GL_GetDrawableSize(window,&w,&h);glViewport(0,0,w,h);glClearColor(.15,.2,.25,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);glEnable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);glMatrixMode(GL_PROJECTION);glLoadIdentity();double top=std::tan(70*3.14159265/360)*1;glFrustum(-top*w/h,top*w/h,-top,top,1,18000);glMatrixMode(GL_MODELVIEW);glLoadIdentity();float cameraView[16]={playerViewFollower.basis[0].x,playerViewFollower.basis[1].x,playerViewFollower.basis[2].x,0,playerViewFollower.basis[0].y,playerViewFollower.basis[1].y,playerViewFollower.basis[2].y,0,playerViewFollower.basis[0].z,playerViewFollower.basis[1].z,playerViewFollower.basis[2].z,0,0,0,0,1};glMultMatrixf(cameraView);glTranslatef(-cameraPosition.x,-cameraPosition.y,-cameraPosition.z);
 for(auto& m:meshes){if(collision.disabled.count(m.model))continue;glPushMatrix();float angle=collision.angles[m.model];if(angle!=0||collision.poses.count(m.model)){auto models=field(collision.data,"models");V pivot{};for(int k=0;k<count(models);k++)if(integer(field(at(models,k),"index"))==m.model)pivot=vec(field(at(models,k),"origin"));Pose pose;if(collision.poses.count(m.model))pose=collision.poses[m.model];else pose.rotation={std::cos(angle*.5f),0,std::sin(angle*.5f),0};V x=rotateQ({1,0,0},pose.rotation),y=rotateQ({0,1,0},pose.rotation),z=rotateQ({0,0,1},pose.rotation);float matrix[16]={x.x,x.y,x.z,0,y.x,y.y,y.z,0,z.x,z.y,z.z,0,0,0,0,1};glTranslatef(pivot.x+pose.translation.x,pivot.y+pose.translation.y,pivot.z+pose.translation.z);glMultMatrixf(matrix);glTranslatef(-pivot.x,-pivot.y,-pivot.z);}if(m.texture){glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,m.texture);glColor3f(1,1,1);}else{glDisable(GL_TEXTURE_2D);glColor3f(.7,.7,.7);}glBegin(GL_TRIANGLES);for(size_t k=0;k<m.positions.size()/3;k++){if(k*2+1<m.uvs.size())glTexCoord2f(m.uvs[k*2],m.uvs[k*2+1]);glVertex3f(m.positions[k*3],m.positions[k*3+1],m.positions[k*3+2]);}glEnd();glPopMatrix();}for(const auto& spawn:victimSpawns){auto animation=victimAnimations.find(spawn.entity);if(animation!=victimAnimations.end())victimRenderer.draw(spawn,animation->second,clock.current,victimActorRoot(spawn));}for(auto& entry:fires){entry.second.visual.sizeUpdate(clock.current);fireRenderer.draw(entry.second,clock.current,true);}for(auto& fire:renderFixture)fireQuads+=fireRenderer.draw(fire,clock.current,true);if(hintRemaining>0)overlay.draw(hint,w,h);SDL_GL_SwapWindow(window);if(renderCheck&&++rendered>=2){int textured=0;for(auto& m:meshes)if(m.texture)textured++;auto error=glGetError();std::cout<<"Rendered "<<meshes.size()<<" mesh groups, "<<textured<<" textured; OpenGL error="<<error<<"\n";if(error)return 4;if(fireRenderCheck){std::cout<<"Fire textures: "<<fireRenderer.textures.size()<<", drawn quads: "<<fireQuads<<"\n";if(fireRenderer.textures.size()!=38||fireQuads!=8)return 29;}running=false;}
 }
 victimRenderer.release();fireRenderer.release();SDL_GL_DeleteContext(context);SDL_DestroyWindow(window);SDL_Quit();return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
