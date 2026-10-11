#pragma once
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include "core.h"
#include "extended.h"

// PropModel use-target geometry: 004834A0 and 00489CE0.
struct InteractionSphere {
 using E=Extended;

 static int compare(E a,E b){
  if(!a.m&&!b.m)return 0;
  if(a.negative!=b.negative)return a.negative?-1:1;
  int result=0;
  if(!a.m)result=-1;
  else if(!b.m)result=1;
  else if(a.e!=b.e)result=a.e<b.e?-1:1;
  else if(a.m!=b.m)result=a.m<b.m?-1:1;
  return a.negative?-result:result;
 }
 static E squareRoot(E value){
  if(value.negative)throw std::runtime_error("Negative interaction-sphere discriminant");
  if(!value.m)return {};
  bool odd=(value.e&1)!=0;int shift=odd?63:64;
  int exponent=odd?(value.e-63)/2:(value.e-64)/2;
  E::U radicand=E::U(value.m)<<shift;
  uint64_t low=uint64_t(1)<<63,high=UINT64_MAX;
  while(low<high){uint64_t mid=low+((high-low+1)>>1);if(E::U(mid)<=radicand/mid)low=mid;else high=mid-1;}
  E::U root=low,remainder=radicand-root*root;
  if(remainder>root)root++;
  return E::pack(root,exponent);
 }
 static E dot(V a,V b){
  return E::from(a.x)*E::from(b.x)+E::from(a.y)*E::from(b.y)+E::from(a.z)*E::from(b.z);
 }
 static E rayDot(V a,V b){
  return E::from(a.z)*E::from(b.z)+E::from(a.y)*E::from(b.y)+E::from(a.x)*E::from(b.x);
 }
 static V normalizedForPoint(V direction){
  E length=squareRoot(dot(direction,direction));
  float magnitude=length.asFloat();
  // 00459B80 leaves vectors shorter than 1e-28 untouched.
  if(magnitude<1e-28f)return direction;
  float reciprocal=(E::from(1.f)/E::from(magnitude)).asFloat();
  V result;
  for(int axis=0;axis<3;axis++)result[axis]=(E::from(reciprocal)*E::from(direction[axis])).asFloat();
  return result;
 }
 static bool ray(V start,V direction,V center,float radius,float& nearDistance,float& farDistance){
  V delta;
  for(int axis=0;axis<3;axis++)delta[axis]=(E::from(center[axis])-E::from(start[axis])).asFloat();
  E distanceSquared=rayDot(delta,delta);
  // 004834A0 stores the dot product in a float stack slot at 004834F2,
  // then reloads that rounded value for the facing test and discriminant.
  E projection=E::from(rayDot(delta,direction).asFloat());
  E radiusSquared=E::from(radius)*E::from(radius);
  if(compare(radiusSquared,distanceSquared)<=0&&compare(projection,{})<0)return false;
  E discriminant=radiusSquared-distanceSquared+projection*projection;
  if(compare(discriminant,{})<0)return false;
  E root=squareRoot(discriminant);
  E entry=projection-root;
  nearDistance=entry.m?entry.asFloat():0.f;
  farDistance=(root+projection).asFloat();
  return true;
 }
 static bool query(V start,V direction,V center,float radius,V* point,float* score){
  float nearDistance=0,farDistance=0;
  if(!ray(start,direction,center,radius,nearDistance,farDistance))return false;
  if(point){
   V unit=normalizedForPoint(direction),scaled;
   for(int axis=0;axis<3;axis++)scaled[axis]=(E::from(unit[axis])*E::from(nearDistance)).asFloat();
   for(int axis=0;axis<3;axis++)(*point)[axis]=(E::from(start[axis])+E::from(scaled[axis])).asFloat();
  }
  if(score)*score=1.f;
  return true;
 }
};
