#pragma once
// 00449FE0: update gating, recovery and visual strength. Services are supplied
// by the entity host in the original order; lifecycle/audio remain host work.
struct FireUpdate {
 uint8_t divider=0;int exposureCount=0;float presentationScale=1,alphaInput=1;
 bool think(FireSuppression& fire,float dt,const std::function<void()>& light,
  const std::function<void()>& bookkeeping,const std::function<void()>& spread,
  const std::function<void()>& sizeUpdate,const std::function<void(float)>& scale,
  const std::function<void(float)>& alpha,
  const std::function<void(float)>& color = {}){
  if(fire.state==5)return false;
  light();
  // 00449FE0 applies the current suppression-strength tint before its
  // ordinary early-return gate and before recovery updates the strength.
  if(color)color(fire.strength);
  if(!fire.sprayed&&fire.state!=4){divider=uint8_t(divider-1);if(divider!=0){sizeUpdate();return false;}}
  divider=3;bookkeeping();if(fire.state==4)spread();sizeUpdate();
  presentationScale=std::abs(double(fire.strength)-1.0)<=.001?1.f:float(int32_t(double(fire.strength)*10.f)*.1);
  scale(presentationScale);
  if(!fire.sprayed){double elapsed=double(dt)*(fire.state==4?1.0:4.0);double recovered=elapsed*.1+double(fire.strength);fire.strength=float(recovered);if(recovered>1)fire.strength=1;}
  alphaInput=double(fire.strength)>=.5?1.f:float(double(fire.strength)*2);alpha(alphaInput);
  fire.sprayed=false;exposureCount=0;return true;
 }
};
