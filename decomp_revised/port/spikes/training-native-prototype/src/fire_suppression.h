#pragma once
// 00448970: recovered strength transition and optional hose finalization.
// Native frame delta is DAT_00502B04; the host must pass its matching value.
// Native contact xyz are ABI inputs but do not affect this arithmetic.
struct FireSuppression {
 float strength=1,sprayTime=1;int state=0;bool sprayed=false,flameEnabled=true,hasSound=false;
 void spray(float dt,float influence,bool terminal,
 const std::function<void()>& countDoused,const std::function<void()>& stopSound){
  if(state==5)return;
  double rest=1.0-double(influence);
  double value=double(strength)-(double(dt)/double(sprayTime))*(1.0-rest*rest);
  strength=float(value);
  // x87 invalid arithmetic stores its negative quiet NaN, including 0/0.
  if(std::isnan(value)){uint32_t bits=0xffc00000u;std::memcpy(&strength,&bits,4);}
  if(value<0)strength=0;
  if(terminal&&double(strength)<.1){state=5;countDoused();if(hasSound)stopSound();flameEnabled=false;}
  sprayed=true;
 }
};
