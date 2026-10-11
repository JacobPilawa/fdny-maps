#pragma once
// 00448630: a point trace, followed by the original floor-frame construction.
struct FireFloor {
 static bool alignInWorld(FireVisual& visual,Collision& world){return align(visual,[&](V start,V end,int contents,int flags,Hit& hit){return world.tracePoint(start,end,contents,flags,hit);});}
 static V cross(V a,V b){return {float(double(a.y)*b.z-double(a.z)*b.y),float(double(a.z)*b.x-double(a.x)*b.z),float(double(a.x)*b.y-double(a.y)*b.x)};}
 static bool align(FireVisual& visual,const std::function<bool(V,V,int,int,Hit&)>& trace){
  V start=visual.position,end=visual.position;start.y=float(double(start.y)+50);end.y=float(double(end.y)-400);Hit hit;
  if(!trace(start,end,1,2,hit))return false;
  visual.position.y=float(double(hit.point.y)+3);
  V n=hit.normal;double inverse=1.0/std::sqrt((double(n.z)*n.z+double(n.y)*n.y)+double(n.x)*n.x);
  V up={float(double(n.x)*inverse),float(double(n.y)*inverse),float(double(n.z)*inverse)};
  V oldX={visual.matrix[0],visual.matrix[3],visual.matrix[6]};V forward=cross(up,oldX),right=cross(forward,up);
  for(int a=0;a<3;a++){visual.matrix[a*3]=right[a];visual.matrix[a*3+1]=up[a];visual.matrix[a*3+2]=forward[a];}
  return true;
 }
};
