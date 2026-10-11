#pragma once
// 0044A840: three intersecting flame planes and a ground glow quad.
struct FirePlacement {
 struct Sprite{std::array<V,4> vertices;float width=0,originY=0;};
 std::array<Sprite,4> sprites;
 static V rotate(const float* r,V v){V out;for(int a=0;a<3;a++)out[a]=float((double(r[a*3+2])*v.z+double(r[a*3+1])*v.y)+double(r[a*3])*v.x);return out;}
 static V add(V a,V b){return {float(double(a.x)+b.x),float(double(a.y)+b.y),float(double(a.z)+b.z)};}
 void place(V size,const float* matrix,V position,V mins,V maxs){
  const double angles[4]={1.0471975511965976,4.1887902047863905,2.0943951023931953,5.235987755982989};
  std::array<V,6> ends={V{1,0,0},V{-1,0,0},V{float(std::cos(angles[0])),0,float(std::sin(angles[0]))},V{float(std::cos(angles[1])),0,float(std::sin(angles[1]))},V{float(std::cos(angles[2])),0,float(std::sin(angles[2]))},V{float(std::cos(angles[3])),0,float(std::sin(angles[3]))}};
  for(auto& v:ends){v.x=float(double(size.x)*v.x*.4);v.z=float(double(v.z)*size.z*.4);// 00495D20 stores rotated X/Y first; Z stays in x87 through translation.
   V rotated=add(rotate(matrix,v),position);rotated.z=float((double(matrix[8])*v.z+double(matrix[7])*v.y)+double(matrix[6])*v.x+double(position.z));v=rotated;}
  for(int i=0;i<3;i++){V a=ends[i*2],b=ends[i*2+1];sprites[i].vertices={add(a,{0,size.y,0}),add(b,{0,size.y,0}),b,a};}
  std::array<V,4> glow={V{maxs.x,mins.y,maxs.z},V{maxs.x,mins.y,mins.z},V{mins.x,mins.y,mins.z},V{mins.x,mins.y,maxs.z}};
  float lowest=1e12f;
  for(int i=0;i<4;i++){V delta={float(double(glow[i].x)-position.x),float(double(glow[i].y)-position.y),float(double(glow[i].z)-position.z)};V local=rotate(matrix,delta);sprites[3].vertices[i]=add(local,position);lowest=std::min(lowest,local.y);}
  if(lowest<0)for(int i=0;i<3;i++){sprites[i].width=float(double(sprites[i].width)-double(lowest)*2);sprites[i].originY=float(double(sprites[i].originY)+double(lowest)*.5);}
 }
};
