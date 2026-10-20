#pragma once
#include "interaction_sphere.h"

// BModel axis-box use query: 00482B90 and 00489D80.
// The source routines use x87 extended intermediates and round their evolving
// near/far values to float stack slots. This mirrors those comparison and
// store points; the oracle fixture covers finite inputs only.
struct InteractionBox {
 using E=Extended;

 static int compare(E a,E b){return InteractionSphere::compare(a,b);}

 static bool ray(V start,V direction,V minimum,V maximum,float* nearOut,float* farOut){
  E nearValue=E::from(0.f);
  E farValue=E::from(3.4028234663852886e+38f);
  const E positiveThreshold=E::from(1.0e-28);
  const E negativeThreshold=E::from(-1.0e-28);
  for(int axis=0;axis<3;axis++){
   E origin=E::from(start[axis]),dir=E::from(direction[axis]);
   E lo=E::from(minimum[axis]),hi=E::from(maximum[axis]);
   bool parallel=compare(dir,positiveThreshold)<0&&compare(dir,negativeThreshold)>0;
   if(parallel){
    if(compare(origin,lo)<0||compare(origin,hi)>0)return false;
    continue;
   }
   bool positive=compare(dir,E::from(0.f))>=0;
   E reciprocal=E::from(1.f)/dir;
   E entry=((positive?lo:hi)-origin)*reciprocal;
   E exit=((positive?hi:lo)-origin)*reciprocal;
   // 00482B90 rejects an axis whose exit precedes the current ray interval,
   // narrows the far endpoint, then advances near only for a nonnegative entry.
   if(compare(exit,nearValue)<0)return false;
   if(compare(exit,farValue)<0)farValue=E::from(exit.asFloat());
   if(compare(entry,nearValue)>=0){
    nearValue=E::from(entry.asFloat());
    if(compare(entry,farValue)>0)return false;
   }
  }
  if(nearOut)*nearOut=nearValue.asFloat();
  if(farOut)*farOut=farValue.asFloat();
  return true;
 }

 static bool query(V start,V direction,V minimum,V maximum,V* point,float* score){
  // 00489D80 normalizes before handing the ray to 00482B90. Keep the unit
  // vector for both the slab query and the reconstructed hit point.
  const V unit=InteractionSphere::normalizedForPoint(direction);
  float nearDistance=0.f,farDistance=0.f;
  if(!ray(start,unit,minimum,maximum,&nearDistance,&farDistance))return false;
  if(point){
   V scaled;
   for(int axis=0;axis<3;axis++)scaled[axis]=(E::from(unit[axis])*E::from(nearDistance)).asFloat();
   for(int axis=0;axis<3;axis++)(*point)[axis]=(E::from(start[axis])+E::from(scaled[axis])).asFloat();
  }
  if(score)*score=1.f;
  return true;
 }
};
