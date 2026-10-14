#pragma once
#include <vector>
// Fire visual component: original 0044B270 initialization, 0044B060 size
// update/placement, and visibility-gated sprite frame updates from 0047F960.
// World drawing and smoke services are supplied by the entity host.
struct FireVisual {
 FireEffectChoices effects;FireSize sizes;FirePlacement placement;
 std::array<SpriteAnimation,4> animations;
 V position{};float matrix[9]={1,0,0,0,1,0,0,0,1};
 int frameCount=19;bool enabled=false;
 std::vector<float> frameDurations;
 static constexpr float frameSeconds=.05f;
 static constexpr int animationFlags=0x35;
 void initialize(Random& shared,bool smoke,uint32_t now,int frames=19){
  if(frames<1)throw std::runtime_error("Fire descriptor has no frames");
  frameCount=frames;frameDurations.assign(size_t(frames),frameSeconds);
  sizes.first=sizes.next=-1;for(auto& key:sizes.keys)key.active=false;sizes.scale={1,1,1};enabled=true;
  bool existed=effects.spritesCreated;effects.initialize(shared,smoke,frames,true);
  if(existed){for(auto& a:animations)a.active=true;return;}
  for(int i=0;i<4;i++){auto& a=animations[i];a.frame=effects.sprites[i].frame;a.nextFrame=effects.sprites[i].nextFrame;a.rate=effects.sprites[i].rate;a.started=now;a.flags=animationFlags;a.active=true;}
 }
 void sizeUpdate(uint32_t now){
  if(!enabled)return;sizes.update(now);
  if(sizes.changed){V lo={float(double(position.x)-double(sizes.size.x)*.5),position.y,float(double(position.z)-double(sizes.size.z)*.5)};
   V hi={float(double(position.x)+double(sizes.size.x)*.5),float(double(position.y)+double(sizes.size.y)),float(double(position.z)+double(sizes.size.z)*.5)};
   placement.place(sizes.size,matrix,position,lo,hi);sizes.changed=false;
  }
 }
 // The original performs frame advancement only when this visual is drawn.
 void renderStep(uint32_t now,bool visible){if(!enabled||!visible)return;for(auto& a:animations)a.update(now,frameDurations);}
 // 0047D6D0, actual flame/glow descriptors (alpha 1, period .05, flags 0x35).
 static std::array<uint8_t,2> opacityBytes(uint32_t elapsed,float rate,float multiplier){
  auto one=Extended::integer(1),byte=Extended::integer(255);
  auto ratio=Extended::integer(elapsed)/(Extended::from(frameSeconds)*Extended::from(rate)*Extended::integer(1000));
  uint8_t current=uint8_t(((one-ratio)*byte+ratio*byte).truncate());
  return {uint8_t((Extended::integer(current)*Extended::from(multiplier)).truncate()),uint8_t((byte*Extended::from(multiplier)).truncate())};
 }
 int textureId(int sprite)const{if(sprite<0||sprite>=4)throw std::runtime_error("Invalid fire sprite");return (sprite==3?120:20)+animations[sprite].frame;}
};
