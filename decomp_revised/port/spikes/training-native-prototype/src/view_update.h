#pragma once
#include "follower.h"
#include <array>

// Genesis frame memory stores three basis columns, then position and scale.
struct FrameTransform {
 std::array<V,3> axis{{{1,0,0},{0,1,0},{0,0,1}}};
 V position{},scale{1,1,1};
 const FrameTransform* parent=nullptr;

 FrameTransform world() const {
  if(!parent)return *this;
  FrameTransform result;result.parent=nullptr;
  for(int k=0;k<3;k++)for(int c=0;c<3;c++){
   auto v=Extended::from(parent->axis[0][c])*Extended::from(axis[k].x)
         +Extended::from(parent->axis[1][c])*Extended::from(axis[k].y)
         +Extended::from(parent->axis[2][c])*Extended::from(axis[k].z);
   result.axis[k][c]=v.asFloat();
  }
  V scaled;
  for(int c=0;c<3;c++)scaled[c]=(Extended::from(position[c])*Extended::from(parent->scale[c])).asFloat();
  for(int c=0;c<3;c++){
   auto v=Extended::from(parent->axis[0][c])*Extended::from(scaled.x)
         +Extended::from(parent->axis[1][c])*Extended::from(scaled.y)
         +Extended::from(parent->axis[2][c])*Extended::from(scaled.z)
         +Extended::from(parent->position[c]);
   result.position[c]=v.asFloat();
   result.scale[c]=(Extended::from(parent->scale[c])*Extended::from(scale[c])).asFloat();
  }
  return result;
 }

 std::array<uint32_t,16> words() const {
  std::array<uint32_t,16> out{};out[0]=0x4dfc30;
  auto put=[&](int start,V v){for(int i=0;i<3;i++){uint32_t b;std::memcpy(&b,&v[i],4);out[start+i]=b;}};
  put(1,axis[0]);put(4,axis[1]);put(7,axis[2]);put(10,position);put(13,scale);
  return out;
 }
};

// Local-frame construction and follower update used by 0044F340. Captures
// cover ordinary 00452570 movement updates and the separate 00451080
// timed-transition caller; caller-supplied fractions/endpoints remain inputs.
struct ViewUpdate {
 using E=Extended;
 static constexpr double axisDotLimit=.999;
 static E fromLongDouble(long double value){
  if(value==0)return {};
  bool negative=std::signbit(value);int exponent=0;
  long double fraction=std::frexp(std::abs(value),&exponent);
  auto mantissa=(E::U)std::ldexp(fraction,64);
  return {uint64_t(mantissa),exponent-64,negative};
 }
 static E squareRoot(E value){
  if(value.negative)throw std::runtime_error("Negative frame vector length");
  if(!value.m)return {};
  // Round FSQRT at the source x87 64-bit significand without host long double.
  const bool odd=(value.e&1)!=0;
  const int shift=odd?63:64;
  const int exponent=odd?(value.e-63)/2:(value.e-64)/2;
  E::U radicand=E::U(value.m)<<shift;
  uint64_t low=uint64_t(1)<<63,high=UINT64_MAX;
  while(low<high){uint64_t mid=low+((high-low+1)>>1);if(E::U(mid)<=radicand/mid)low=mid;else high=mid-1;}
  E::U root=low,remainder=radicand-root*root;
  if(remainder>root)root++;
  return E::pack(root,exponent);
 }
 static V add(V a,V b){V r;for(int i=0;i<3;i++)r[i]=(E::from(a[i])+E::from(b[i])).asFloat();return r;}
 static V subtract(V a,V b){V r;for(int i=0;i<3;i++)r[i]=(E::from(a[i])-E::from(b[i])).asFloat();return r;}
 static V scale(V a,float f){V r;for(int i=0;i<3;i++)r[i]=(E::from(a[i])*E::from(f)).asFloat();return r;}
 static E dotExact(V a,V b){return E::from(a.x)*E::from(b.x)+E::from(a.y)*E::from(b.y)+E::from(a.z)*E::from(b.z);}
 static V cross(V a,V b){
  return {(E::from(a.y)*E::from(b.z)-E::from(a.z)*E::from(b.y)).asFloat(),
          (E::from(a.z)*E::from(b.x)-E::from(a.x)*E::from(b.z)).asFloat(),
          (E::from(a.x)*E::from(b.y)-E::from(a.y)*E::from(b.x)).asFloat()};
 }
 static V normalize(V a){
  // 00459B80 and the inline 0044F340 normalization accumulate Z², then Y²,
  // then X² on the x87 stack before FSQRT.
  E squared=E::from(a.z)*E::from(a.z)+E::from(a.y)*E::from(a.y)+E::from(a.x)*E::from(a.x);
  E length=squareRoot(squared);
  if(!length.m||std::abs(length.asDouble())<1e-28)return a;
  E reciprocal=E::from(1.f)/length;V result;
  for(int i=0;i<3;i++)result[i]=(E::from(a[i])*reciprocal).asFloat();
  return result;
 }
 static V normalizeDirectionDifference(V scaledBaseTarget,V scaledTargetEndpoint,V position){
  // 0044FA00 sums the two already-scaled target vectors in x87. X is rounded
  // only after subtracting position; Y stays extended through normalization;
  // Z is stored to binary32 before subtracting position. This mirrors the
  // inline 0044FA28..0044FA75 path before the virtual +0x14 normalizes again.
  float x=(E::from(scaledBaseTarget.x)+E::from(scaledTargetEndpoint.x)-E::from(position.x)).asFloat();
  E y=E::from(scaledBaseTarget.y)+E::from(scaledTargetEndpoint.y)-E::from(position.y);
  float targetZ=(E::from(scaledBaseTarget.z)+E::from(scaledTargetEndpoint.z)).asFloat();
  E z=E::from(targetZ)-E::from(position.z);
  E squared=z*z+y*y+E::from(x)*E::from(x);
  E reciprocal=E::from(1.0)/squareRoot(squared);
  return {(E::from(x)*reciprocal).asFloat(),(y*reciprocal).asFloat(),(z*reciprocal).asFloat()};
 }
 static bool nearlyOrthogonal(V a,V b){return std::abs(dotExact(a,b).asDouble())<=axisDotLimit;}
 static void permuteAxes(FrameTransform& f){auto x=f.axis[0];f.axis[0]=f.axis[1];f.axis[1]=f.axis[2];f.axis[2]=x;}
 static void setUp(FrameTransform& f,V up){
  f.axis[1]=normalize(up);
  if(nearlyOrthogonal(f.axis[2],f.axis[1])){
   f.axis[0]=normalize(cross(f.axis[1],f.axis[2]));
   f.axis[2]=normalize(cross(f.axis[0],f.axis[1]));return;
  }
  if(!nearlyOrthogonal(f.axis[0],f.axis[1]))permuteAxes(f);
  f.axis[2]=normalize(cross(f.axis[0],f.axis[1]));
  f.axis[0]=normalize(cross(f.axis[1],f.axis[2]));
 }
 static void setForward(FrameTransform& f,V forward){
  f.axis[2]=normalize(forward);
  if(nearlyOrthogonal(f.axis[2],f.axis[1])){
   f.axis[0]=normalize(cross(f.axis[1],f.axis[2]));
   f.axis[1]=normalize(cross(f.axis[2],f.axis[0]));return;
  }
  if(!nearlyOrthogonal(f.axis[2],f.axis[0]))permuteAxes(f);
  f.axis[1]=normalize(cross(f.axis[2],f.axis[0]));
  f.axis[0]=normalize(cross(f.axis[1],f.axis[2]));
 }
 static void rollDegrees(FrameTransform& f,float degrees){
  E radians=E::from(degrees)*E::from(0.01745329238474369f);
  long double angle=std::ldexp((long double)radians.m,radians.e);
  if(radians.negative)angle=-angle;
  E cosine=fromLongDouble(std::cos(angle)),sine=fromLongDouble(std::sin(angle));
  V y,z;
  for(int i=0;i<3;i++){
   y[i]=(cosine*E::from(f.axis[1][i])-sine*E::from(f.axis[2][i])).asFloat();
  }
  f.axis[1]=normalize(y);
  f.axis[2]=normalize(cross(f.axis[0],f.axis[1]));
  f.axis[0]=normalize(cross(f.axis[1],f.axis[2]));
 }
 static void orthogonalize(FrameTransform& f){
  if(nearlyOrthogonal(f.axis[1],f.axis[2])){
   f.axis[0]=normalize(cross(f.axis[1],f.axis[2]));
   f.axis[1]=normalize(cross(f.axis[2],f.axis[0]));
   f.axis[2]=normalize(cross(f.axis[0],f.axis[1]));return;
  }
  if(nearlyOrthogonal(f.axis[2],f.axis[0])){
   f.axis[1]=normalize(cross(f.axis[2],f.axis[0]));
   f.axis[2]=normalize(cross(f.axis[0],f.axis[1]));
   f.axis[0]=normalize(cross(f.axis[1],f.axis[2]));return;
  }
  if(!nearlyOrthogonal(f.axis[0],f.axis[1]))permuteAxes(f);
  f.axis[2]=normalize(cross(f.axis[0],f.axis[1]));
  f.axis[0]=normalize(cross(f.axis[1],f.axis[2]));
  f.axis[1]=normalize(cross(f.axis[2],f.axis[0]));
 }

 struct Result {FrameTransform local,world;Follower follower;};
 static Result transition(const FrameTransform& root,V headMotion,V toolPosition,V toolRotation,
                         V positionEndpoint,V targetEndpoint,float height,float fraction,
                         float roll,float dt,Follower follower,bool enterSettling=false){
  FrameTransform local;local.parent=&root;
  V offset=add(add(headMotion,{0,height,0}),scale(toolPosition,fraction));
  local.position=add(offset,scale(positionEndpoint,1.f-fraction));
  V scaledBaseTarget=scale(add(V{0,height,1000},toolRotation),fraction);
  V scaledTargetEndpoint=scale(targetEndpoint,1.f-fraction);
  setUp(local,{0,1,0});setForward(local,normalizeDirectionDifference(scaledBaseTarget,scaledTargetEndpoint,local.position));
  rollDegrees(local,roll);orthogonalize(local);
  FrameTransform world=local.world();
  // 0044F340 calls 00483680(1) before 00483720 while crouching or in the
  // other two recovered settling states.
  if(enterSettling){follower.held=false;follower.allow=true;}
  follower.update(world.position,dt);
  follower.basis=world.axis;
  return {local,world,follower};
 }
};
