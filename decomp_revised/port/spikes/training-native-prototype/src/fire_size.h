#pragma once
// 0044B620 / 0044AE80: pooled FireRescue size/bounds-vector keys and
// interpolation. Retail setsize and moveto share this track at root+0x12C;
// it feeds the component's +0x44..+0x4C extents, not world position.
struct FireSize {
 struct Key{bool active=false;uint32_t time=0;V size;};
 std::vector<Key> keys;int first=-1,next=-1;V size{},scale{1,1,1};bool changed=false;
 int available(){for(int i=0;i<int(keys.size());i++)if(!keys[i].active)return i;keys.push_back({});return int(keys.size())-1;}
 void setKey(int i,V value,uint32_t when){keys[i]={true,when,value};}
 void add(V value,uint32_t when,uint32_t now){
  if(first<0){first=available();setKey(first,value,when);return;}
  int target;
  if(now<=when){
   if(next<0){next=available();target=next;}
   else if(int32_t(when)<int32_t(keys[next].time)){setKey(first,size,now);setKey(next,value,when);return;}
   else target=available();
  }else target=available();
  setKey(target,value,when);
 }
 // 00447920 clears the FireRescue timed-vector track, anchors the current
 // dimensions at game-clock + 10, then adds the requested future dimensions.
 void setMotion(V target,uint32_t targetTime,uint32_t now){
  for(auto& key:keys)key.active=false;
  first=next=-1;
  add(size,now+10u,now);
  add(target,targetTime,now);
 }
 // 00449340 clears the same track used by setsize, anchors zero at the
 // current game clock minus 10, then inserts root[+0x108..+0x110] at the
 // route-derived future timestamp.
 void setRouteMotion(V target,uint32_t targetDelay,uint32_t now){
  for(auto& key:keys)key.active=false;
  first=next=-1;
  add({0,0,0},now-10u,now);
  add(target,now+targetDelay,now);
 }
 void update(uint32_t now){
  if(first<0)return;
  if(next>=0&&keys[next].time<now){keys[first].active=false;first=next;next=-1;
   for(int i=0;i<int(keys.size());i++)if(keys[i].active&&i!=first&&int32_t(keys[first].time)<int32_t(keys[i].time)&&(next<0||int32_t(keys[i].time)<int32_t(keys[next].time)))next=i;
  }
  V value=keys[first].size;
  if(next>=0){double ratio=double(uint32_t(now-keys[first].time))/double(int32_t(keys[next].time-keys[first].time));float part=float(ratio);if(ratio>1)part=1;else if(part<0)part=0;
   for(int a=0;a<3;a++){float delta=float(double(keys[next].size[a])-double(value[a]));float product=float(double(delta)*double(part));value[a]=float(double(value[a])+double(product));}
  }
  for(int a=0;a<3;a++)value[a]=float(double(value[a])*double(scale[a]));
  V delta;for(int a=0;a<3;a++)delta[a]=float(double(value[a])-double(size[a]));
  if(double(delta.x)*delta.x+double(delta.y)*delta.y+double(delta.z)*delta.z>=.0001){changed=true;size=value;}
 }
};
