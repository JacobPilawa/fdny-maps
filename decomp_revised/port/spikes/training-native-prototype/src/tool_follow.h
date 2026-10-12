#pragma once
#include "extended.h"
// Original 0043B120 comparison, tolerance expressed in mantissa bits.
struct ToolFollow {
 static uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
 static bool near(float a,float b,int tolerance=2){
  if(a==0)return b>-1e-28&&b<1e-28;
  if(b==0)return a>-1e-28&&a<1e-28;
  auto x=bits(a),y=bits(b);int exp=std::max((x>>23)&255,(y>>23)&255);
  if(exp==255)return a==b;
  if(!exp)return std::abs(int(x&0x7fffff)-int(y&0x7fffff))<(1<<tolerance);
  float difference=float(double(a)-double(b));
  return int((bits(difference)>>23)&255)<exp+tolerance-23;
 }
 // 00483030. Power uses host libm; full original CRT-power equivalence remains open.
 static float core(float current,float target,float smoothing,float previous,float step,float globalDt){
  if(smoothing<=1e-5)return target;
  if(smoothing>=1e20)return current;
  using E=Extended;auto c=E::from(current),t=E::from(target);
  if(near(target,previous)){
   auto factor=E::from(std::pow(.5,double(globalDt)/smoothing));
   return (factor*c+(E::from(1.f)-factor)*t).asFloat();
  }
  auto rate=(E::from(-0.6931471805599453)/E::from(smoothing))*E::from(step);
  auto delta=c-E::from(previous),half=E::from(.5);
  auto halfTarget=E::from((t-E::from(previous)).asDouble())*half;
  auto first=E::from((delta*rate).asDouble());
  auto second=((first*half+delta)-halfTarget)*rate;
  auto third=((second*half+delta)-halfTarget)*rate;
  auto fourth=((c+third)-t)*rate;
  return ((fourth+first)*E::from(1.0/6)+(third+E::from(second.asDouble()))*E::from(1.0/3)+c).asFloat();
 }
 // 00483140: interpolate targets and advance in .01-second pieces.
 static float step(float current,float target,float smoothing,float previous,float globalDt){
  using E=Extended;float remaining=globalDt;
  while(!near(remaining,.01f)&&remaining>.01f){
   auto fraction=E::from(.01f)/E::from(remaining);
   float middle=((E::from(1.f)-fraction)*E::from(previous)+fraction*E::from(target)).asFloat();
   remaining=(E::from(remaining)-E::from(.01f)).asFloat();
   current=core(current,middle,smoothing,previous,.01f,globalDt);
   previous=middle;
  }
  return core(current,target,smoothing,previous,remaining,globalDt);
 }
};
