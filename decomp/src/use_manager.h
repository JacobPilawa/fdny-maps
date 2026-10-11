#pragma once
#include <vector>
#include "interaction_sphere.h"

// Training RCS_PropModel target selection: 00459350, 00489CE0, 004834A0.
// The center and radius come from the entity's ZZ_location and F_TouchRadius
// fields (00444A40 copies the point; 00443860 copies the radius).
struct UseManager {
 struct Candidate { int entity=0; V center{}; float radius=0; };
 struct Selection { int index=-1; int action=0; V point{}; float score=0; };

 static int valveAction(int entity,int hoseMode,int attachedValve){
  // 0044C780: mode 2 offers attach 1001; modes 1/3 offer detach 1002 only
  // when this is the valve saved at Hose Spray +0xC6C.
  if(hoseMode!=3&&hoseMode!=1)return hoseMode==2?1001:0;
  return entity==attachedValve?1002:0;
 }

 static Selection select(V start,V direction,const std::vector<Candidate>& candidates,
                         int hoseMode,int attachedValve){
  Selection selected; float bestScore=0;
  const auto reachSquared=Extended::from(200.0f)*Extended::from(200.0f);
  for(size_t i=0;i<candidates.size();i++){
   const auto& candidate=candidates[i];
   int action=valveAction(candidate.entity,hoseMode,attachedValve);
   if(!action)continue;
   V point{}; float score=0;
   if(!InteractionSphere::query(start,direction,candidate.center,candidate.radius,&point,&score))continue;
   V delta; for(int axis=0;axis<3;axis++)delta[axis]=(Extended::from(point[axis])-Extended::from(start[axis])).asFloat();
   auto distanceSquared=Extended::from(delta.z)*Extended::from(delta.z)
                       +Extended::from(delta.y)*Extended::from(delta.y)
                       +Extended::from(delta.x)*Extended::from(delta.x);
   if(InteractionSphere::compare(distanceSquared,reachSquared)>0)continue;
   // 00459350 replaces an equal-scoring selection, so the later candidate wins.
   if(selected.index<0||bestScore<=score){selected={int(i),action,point,score};bestScore=score;}
  }
  return selected;
 }

 static bool sphereTouchesBox(V center,float radius,V minimum,V maximum){
  V nearest;
  for(int axis=0;axis<3;axis++)nearest[axis]=std::clamp(center[axis],minimum[axis],maximum[axis]);
  V delta;for(int axis=0;axis<3;axis++)delta[axis]=(Extended::from(center[axis])-Extended::from(nearest[axis])).asFloat();
  auto distanceSquared=Extended::from(delta.z)*Extended::from(delta.z)
                      +Extended::from(delta.y)*Extended::from(delta.y)
                      +Extended::from(delta.x)*Extended::from(delta.x);
  return InteractionSphere::compare(distanceSquared,Extended::from(radius)*Extended::from(radius))<=0;
 }
};
