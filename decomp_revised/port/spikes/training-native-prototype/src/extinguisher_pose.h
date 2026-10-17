#pragma once
#include "view_update.h"

// 004612B0 builds and orthogonalizes a local frame from position/direction.
// Extinguisher setup 00461830 supplies (-30,-18,40) and (.2,-1,.1) and calls
// 0045F660, which creates a +0x98 follower under Player+0x34A4 for that setup
// object. The exact relationship from this setup object to the visible retail
// ACT draw root is still open; this component models its source-shaped action
// pose, not the first-person mesh attachment.
struct ExtinguisherPose {
 struct MotionPose {float phase;V direction,position;};
 struct CallbackState {bool active=false;uint32_t cooldown=0,start=0,end=0;V position{},direction{};};
 static float blendProgress(uint32_t now,uint32_t start,uint32_t end){
  const int32_t elapsed=static_cast<int32_t>(now-start);
  const uint32_t duration=end-start;
  if(elapsed<=0)return 0.f;
  if(duration==0||uint32_t(elapsed)>=duration)return 1.f;
  return float(elapsed)/float(duration);
 }
 static MotionPose idleMotion(){return {0.f,{.2f,-1.f,.1f},{-30.f,-18.f,40.f}};}
 static MotionPose interpolate(MotionPose from,MotionPose to,float fraction){
  auto lerp=[&](float a,float b){return float(double(a)+(double(b)-double(a))*double(fraction));};
  return {to.phase,{lerp(from.direction.x,to.direction.x),lerp(from.direction.y,to.direction.y),lerp(from.direction.z,to.direction.z)},
                   {lerp(from.position.x,to.position.x),lerp(from.position.y,to.position.y),lerp(from.position.z,to.position.z)}};
 }
 static CallbackState beginWindow(CallbackState state,uint32_t tick,const FrameTransform& frame){
  state.active=true;state.cooldown=tick+100;state.start=tick;state.end=tick+300;
  state.position=frame.position;state.direction={-frame.axis[1].x,-frame.axis[1].y,-frame.axis[1].z};return state;
 }
 static CallbackState endWindow(CallbackState state,uint32_t tick,const FrameTransform& frame){
  state.active=false;state.start=tick;state.end=tick+1000;
  state.position=frame.position;state.direction={-frame.axis[1].x,-frame.axis[1].y,-frame.axis[1].z};return state;
 }
 // 00461F80 subtracts frame delta * the constructor rate at +0x8CC from
 // charge +0x8C8 in x87 precision. It stores a float but tests the unrounded
 // result, then invokes the stop callback and zeros charge if negative.
 static bool consumeCharge(float& charge,float dt){
  Extended remaining=Extended::from(charge)-Extended::from(dt)*Extended::from(.1f);
  charge=remaining.asFloat();
  return remaining.m!=0&&remaining.negative;
 }
 static Extended fromLongDouble(long double value){
  if(value==0)return {0,0,std::signbit(value)};
  bool negative=std::signbit(value);int exponent=0;
  long double fraction=std::frexp(std::abs(value),&exponent);
  auto significand=Extended::U(std::ldexp(fraction,127));
  return Extended::pack(significand,exponent-127,negative);
 }
 static long double toLongDouble(Extended value){
  long double result=std::ldexp((long double)value.m,value.e);
  return value.negative?-result:result;
 }
 static Extended squareRoot(Extended value){
  if(value.negative)throw std::runtime_error("Negative extinguisher vector length");
  if(!value.m)return {};
  const bool odd=(value.e&1)!=0;
  const int shift=odd?63:64;
  const int exponent=odd?(value.e-63)/2:(value.e-64)/2;
  Extended::U radicand=Extended::U(value.m)<<shift;
  uint64_t low=uint64_t(1)<<63,high=UINT64_MAX;
  while(low<high){uint64_t mid=low+((high-low+1)>>1);if(Extended::U(mid)<=radicand/mid)low=mid;else high=mid-1;}
  Extended::U root=low,remainder=radicand-root*root;
  if(remainder>root)root++;
  return Extended::pack(root,exponent);
 }
 static V cross(V a,V b){return ViewUpdate::cross(a,b);}
 static V normalize(V a){
  auto length=squareRoot(ViewUpdate::dotExact(a,a));
  if(!length.m)return a;
  auto reciprocal=Extended::from(1.f)/length;V result;
  for(int i=0;i<3;i++)result[i]=(Extended::from(a[i])*reciprocal).asFloat();
  return result;
 }
 static V scaledDirection(V direction){
  auto length=squareRoot(ViewUpdate::dotExact(direction,direction));
  // 004612B0 stores the x87 reciprocal to a float stack slot before 00446390.
  float reciprocal=(Extended::from(1.0)/length).asFloat();
  V negative{-direction.x,-direction.y,-direction.z},result;
  for(int i=0;i<3;i++)result[i]=(Extended::from(negative[i])*Extended::from(reciprocal)).asFloat();
  return result;
 }
 // Active +0x600 pose phase in 00461F80 through the first 004977B0 call.
 // The RNG range is evaluated at x87 precision; the stored phase and vectors
 // each retain the original float memory boundaries.
 static MotionPose activeMotion(Random& random,float dt,float initialPhase,V offsets){
  Extended rate=(Extended::from(8.f)-Extended::from(6.f))*Extended::from(random.unit())+Extended::from(6.f);
  Extended phase=rate*Extended::from(dt)+Extended::from(initialPhase);
  Extended cosine=fromLongDouble(std::cos(toLongDouble(phase)*.5L));
  Extended sineHalf=fromLongDouble(std::sin(toLongDouble(phase)))*Extended::from(.5f);
  MotionPose result;result.phase=phase.asFloat();
  result.direction={cosine.asFloat(),(sineHalf-Extended::from(2.f)).asFloat(),8.f};
  result.position={(cosine+Extended::from(offsets.x)).asFloat(),(sineHalf+Extended::from(offsets.y)).asFloat(),(cosine+Extended::from(offsets.z)).asFloat()};
  return result;
 }
 static FrameTransform fromVectors(V position,V direction){
  FrameTransform frame;frame.position=position;
  frame.axis[1]=scaledDirection(direction);
  frame.axis[0]=cross(frame.axis[1],V{0,1,0});
  frame.axis[2]=cross(frame.axis[0],frame.axis[1]);
  frame.axis[0]=normalize(cross(frame.axis[1],frame.axis[2]));
  frame.axis[1]=normalize(cross(frame.axis[2],frame.axis[0]));
  frame.axis[2]=normalize(cross(frame.axis[0],frame.axis[1]));
  return frame;
 }
 static FrameTransform local(){
  const auto pose=idleMotion();return fromVectors(pose.position,pose.direction);
 }
 static FrameTransform local(MotionPose pose){return fromVectors(pose.position,pose.direction);}
 static FrameTransform world(const FrameTransform& toolFollower){
  FrameTransform frame=local();frame.parent=&toolFollower;return frame.world();
 }
 static FrameTransform world(MotionPose pose,const Follower& follower){
  FrameTransform parent;parent.axis=follower.basis;parent.position=follower.position;
  FrameTransform frame=local(pose);frame.parent=&parent;return frame.world();
 }
 static FrameTransform followerFrame(const Follower& follower){
  FrameTransform frame;frame.axis=follower.basis;frame.position=follower.position;return frame;
 }
 static FrameTransform world(const Follower& follower){return world(followerFrame(follower));}
};
