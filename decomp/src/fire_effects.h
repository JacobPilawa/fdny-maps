#pragma once
// Original 0044B270, with sprite constructors 0047F850 / 0047D370.
// Every effect uses the simulation's shared Random; visuals never invent
// a second stream. A missing smoke emitter consumes its delay draw before
// the four pairs of sprite speed/frame draws.
struct FireSpriteChoice{float rate=1;int frame=0,nextFrame=1;};
struct FireEffectChoices{
 bool smokeCreated=false,spritesCreated=false;
 float smokeInterval=0;
 std::array<FireSpriteChoice,4> sprites;
 void initialize(Random& random,bool smoke,int frames,bool hasWorld=true){
  if(frames<1)throw std::runtime_error("Fire sprite has no frames");
  if(smoke&&!smokeCreated){smokeCreated=true;smokeInterval=float(random.range(.8f,1.2f)*300.0);}
  if(spritesCreated)return;
  spritesCreated=hasWorld;
  for(auto& sprite:sprites){sprite.rate=float(random.range(.5f,1.5f));sprite.frame=int(random.range(0,float(frames-1)));sprite.nextFrame=frames==1?0:(sprite.frame+1)%frames;}
 }
};
