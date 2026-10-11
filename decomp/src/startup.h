#pragma once
// 004297E0 registers factories in this order; 0049F160 appends each one.
// 0049E4D0 visits factories, then each class's original BSP entity order.
// 0049C670 constructs the batch, calls +0x20 for the whole batch, then
// calls +0x24 for the whole batch. Victim alternate points register in the
// first pass; victim spawn selection occurs in the second pass.
inline std::vector<J> originalStartupOrder(J entities){
 static const char* classes[]={"World","RCS_BlinkLight","RCS_BModel","RCS_BoundsTrigger","RCS_PropModel","RCS_MiscEmitter","RCS_PlayerHose","RCS_CamHose","RCS_DouseHose","RCS_WindowFire","RCS_PlayerRescue","RCS_CamRescue","RCS_FireRescue","RCS_VictimRescue","RCS_VictimAltSpot","RCS_PathPoint","RCS_EscapeArrow"};
 std::vector<J> ordered;std::set<int> included;
 for(auto name:classes)for(int i=0;i<count(entities);i++){auto e=at(entities,i);if(str(field(e,"class"))==name){ordered.push_back(e);included.insert(integer(field(e,"index")));}}
 for(int i=0;i<count(entities);i++){auto e=at(entities,i);if(str(field(e,"class")).rfind("RCS_",0)==0&&!included.count(integer(field(e,"index"))))throw std::runtime_error("Unregistered Training class: "+str(field(e,"class")));}
 return ordered;
}
struct VictimCandidate{int entity;V position;float heading;};
struct VictimSpawn{
 int entity;std::string name,type;std::vector<VictimCandidate> candidates;
 int initialMotion=-1,selectedCandidate=-1,selectedEntity=-1,appearanceIndex=-1,modelId=12;
 float initialMotionSpeed=1.f,selectedHeading=0.f;
 V selectedCandidatePosition{},selectedPosition{};
 // Victim model bounds initialized by 0045A3D0: +24 is written to the
 // maximum vector and 0042D2B0 negates it into the minimum vector.
 V modelMins{-24.f,-24.f,-24.f},modelMaxs{24.f,24.f,24.f};
 bool motionInitialized=false,spawnSelected=false,floorPlacementAttempted=false,floorPlacementSucceeded=false;
 // VictimRescueEnt::Init (0045B690) reads ActorAnimator motion count at
 // entity +0x180 after 00480D00 stores it. Six motions select bounded(3),
 // speed 1.0; all other counts use bounded(count), speed .2, and state 4.
 void initializeMotion(Random& shared,int motionCount){
  if(motionCount==6){initialMotion=int(shared.bounded(3));initialMotionSpeed=1.f;}
  else{initialMotion=int(shared.bounded(uint32_t(motionCount)));initialMotionSpeed=.2f;}
  motionInitialized=true;
 }
 // VictimRescueEnt::Spawn (0045D5B0) uses one [0,count-.01) float range
 // and truncation, then copies the chosen authored position and heading.
 int choose(Random& shared){
  selectedCandidate=shared.spotIndex(int(candidates.size()));
  const auto& candidate=candidates.at(size_t(selectedCandidate));
  selectedEntity=candidate.entity;selectedCandidatePosition=candidate.position;selectedPosition=candidate.position;
  selectedHeading=candidate.heading;spawnSelected=true;return selectedCandidate;
 }
};
inline V location(J props,const char* key){V result;auto text=str(field(props,key));if(sscanf(text.c_str(),"%f %f %f",&result.x,&result.y,&result.z)!=3)throw std::runtime_error("Missing authored location");return result;}
// 0045DDD0 adds the editor origin first. 00459840 registers alternate points
// in BSP order. All candidates, including the origin, enter the same draw.
inline std::vector<VictimSpawn> originalVictimCandidates(const std::vector<J>& ordered){
 std::vector<VictimSpawn> victims;std::map<std::string,size_t> names;
 for(auto e:ordered){auto props=field(e,"properties");if(str(field(e,"class"))=="RCS_VictimRescue"){
 names[str(field(props,"%name%"))]=victims.size();int id=integer(field(e,"index"));victims.push_back({id,str(field(props,"A_Name")),str(field(props,"D_Type")),{{id,location(props,"location"),std::stof(str(field(props,"G_Heading")))}}});
 }}
 for(auto e:ordered){auto props=field(e,"properties");if(str(field(e,"class"))=="RCS_VictimAltSpot"){
 auto target=names.find(str(field(props,"A_Victim")));if(target==names.end())throw std::runtime_error("Victim alternate point has no target");victims[target->second].candidates.push_back({integer(field(e,"index")),location(props,"ZZ_location"),std::stof(str(field(props,"B_Heading")))});
 }}
 return victims;
}
