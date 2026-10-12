#pragma once
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include "extended.h"

// Numeric bitmap-frame portion of 00482430. The original constructor owns
// additional bitmap/viewport state; this model is intentionally limited to
// the controlled record fixture captured by extinguisher_frame_record_oracle.py.
struct ExtinguisherFrameRecord {
 std::array<uint32_t,0x120/4> words{};

 static float fromBits(uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
 static uint32_t bits(float value){uint32_t result;std::memcpy(&result,&value,4);return result;}
 static int viewportCoordinate(int coordinate,int extent,double scale,int offset){
  // 0048A910 multiplies integer reference coordinates by the viewport extent
  // and the 640x480 scale, passes the value through 004B9400, then adds offset.
  // Extended keeps the original x87 64-bit significand before its integer conversion.
  auto value=Extended::from(float(coordinate))*Extended::from(float(extent))*Extended::from(scale);
  return offset+int(value.truncate());
 }
 static float rotateX(float x,float y,float angle){
  const long double radians=(long double)angle*0.01745329238474369f;
  const float cosine=(float)std::cos(radians),sine=(float)std::sin(radians);
  return (Extended::from(cosine)*Extended::from(x)-Extended::from(sine)*Extended::from(y)).asFloat();
 }
 static float rotateY(float x,float y,float angle){
  const long double radians=(long double)angle*0.01745329238474369f;
  const float cosine=(float)std::cos(radians),sine=(float)std::sin(radians);
  return (Extended::from(sine)*Extended::from(x)+Extended::from(cosine)*Extended::from(y)).asFloat();
 }
 bool clip(float* source){
  const float left=float(viewportCoordinate(0,viewportWidth,0.0015625,viewportOffsetX));
  const float right=float(viewportOffsetX+viewportWidth);
  const float top=float(viewportCoordinate(0,viewportHeight,0.0020833333333333333,viewportOffsetY));
  const float bottom=float(viewportOffsetY+viewportHeight);
  float* uv=source;
  auto fpoint=[&](int offset){return fromBits(words[offset/4]);};
  float x0=fpoint(0x10),y0=fpoint(0x14);
  float x1=fromBits(words[0x34/4]),y1=fromBits(words[0x38/4]);
  float x2=fromBits(words[0x58/4]),y2=fromBits(words[0x5c/4]);
  float x3=fromBits(words[0x7c/4]),y3=fromBits(words[0x80/4]);
  float edgeX=x1-x0,edgeY=y2-y1;
  if(edgeX==0.f||edgeY==0.f)return false;
  if(x0<left){if(x1<left)return false;float t=(left-x0)/edgeX;edgeX=edgeX-(left-x0);uv[0]+=t;uv[9]+=t;x0=left;x3=left;}
  if(y0<top){if(y2<top)return false;float t=(top-y0)/edgeY;edgeY=edgeY-(top-y0);uv[1]+=t;uv[4]+=t;y0=top;y1=top;}
  if(x1>=right){if(x0>=right)return false;float t=(x1-right)/edgeX;uv[3]-=t;uv[6]-=t;x1=right-1.f;x2=right-1.f;}
  if(y2>=bottom){if(y0>=bottom)return false;float t=(y2-bottom)/edgeY;uv[7]-=t;uv[10]-=t;y2=bottom-1.f;y3=bottom-1.f;}
  for(auto* p : {&x0,&x1,&x2,&x3}){if(*p>right-1.f)*p=right-1.f;if(*p<left)*p=left;}
  for(auto* p : {&y0,&y1,&y2,&y3}){if(*p>bottom-1.f)*p=bottom-1.f;if(*p<top)*p=top;}
  const float values[8]={x0,y0,x1,y1,x2,y2,x3,y3};
  const int offsets[8]={0x10,0x14,0x34,0x38,0x58,0x5c,0x7c,0x80};
  for(int i=0;i<8;i++)words[offsets[i]/4]=bits(values[i]);
  return true;
 }
 void update(int width,int height,int offsetX,int offsetY,int centerX,int centerY,
             float rate,const std::array<int,4>& keys,float progress){
  viewportWidth=width;viewportHeight=height;viewportOffsetX=offsetX;viewportOffsetY=offsetY;
  const int cx=viewportCoordinate(centerX,width,0.0015625,offsetX);
  const int cy=viewportCoordinate(centerY,height,0.0020833333333333333,offsetY);
  const std::array<int,8> points={
   viewportCoordinate(keys[0],width,0.0015625,offsetX),
   viewportCoordinate(keys[2],height,0.0020833333333333333,offsetY),
   viewportCoordinate(keys[1],width,0.0015625,offsetX),
   viewportCoordinate(keys[2],height,0.0020833333333333333,offsetY),
   viewportCoordinate(keys[1],width,0.0015625,offsetX),
   viewportCoordinate(keys[3],height,0.0020833333333333333,offsetY),
   viewportCoordinate(keys[0],width,0.0015625,offsetX),
   viewportCoordinate(keys[3],height,0.0020833333333333333,offsetY)};
  for(int i=0;i<4;i++){
   words[(0x10+i*0x24)/4]=bits(float(points[i*2]));
   words[(0x14+i*0x24)/4]=bits(float(points[i*2+1]));
  }
  std::array<float,12> source{};
  for(int i=0;i<12;i++)source[i]=fromBits(words[(0xa0+4*i)/4]);
  const auto originalSource=source;
  bool visible=clip(source.data());
  for(int i=0;i<4;i++){
   const float angleX=fromBits(words[(0x10+i*0x24)/4])-float(cx),angleY=fromBits(words[(0x14+i*0x24)/4])-float(cy);
   const auto phase=(Extended::from(0.5)-Extended::from(progress))*-Extended::from(rate);
   const float angle=phase.asFloat();
   const float x=Extended::from(float(cx)).asFloat()+rotateX(angleX,angleY,angle);
   const float y=Extended::from(float(cy)).asFloat()+rotateY(angleX,angleY,angle);
   words[(0x10+i*0x24)/4]=bits(x);
   words[(0x14+i*0x24)/4]=bits(y);
  }
  source=originalSource;visible=clip(source.data());
  // 0047E6D0 copies the twelve source-frame words, and 0048A9C0/0048A650 clip
  // the rectangle against the viewport. The direct fixture's source UV words
  // are zero; populated constructor UV state is not covered by this check.
  for(int i=0;i<12;i++)words[(0xd0+i*4)/4]=bits(source[i]);
  words[0x112/4]&=0xffffff00u;
  words[0x100/4]=(words[0x100/4]&0xffffff00u)|(visible?1u:0u);
 }
 int viewportWidth=640,viewportHeight=480,viewportOffsetX=0,viewportOffsetY=0;
};
