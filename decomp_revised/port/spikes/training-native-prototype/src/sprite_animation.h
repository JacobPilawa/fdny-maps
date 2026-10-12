#pragma once
#include <cstdint>
#include <stdexcept>
#include <vector>
// 0047D660 and 0047D2B0: a sprite advances at most one frame per call.
struct SpriteAnimation {
 int frame=0,nextFrame=1,flags=1;uint32_t started=0;float rate=1;bool active=true;
 bool normalize(int& index,int frames){if(index<frames)return false;if(flags&1)index=frames==1?0:index%frames;else index=frames-1;return true;}
 void update(uint32_t now,const std::vector<float>& durations){
  if(frame<0||frame>=int(durations.size()))throw std::runtime_error("Invalid sprite frame");
  double deadline=double(durations[frame])*double(rate)*1000.0+double(int32_t(started));
  uint32_t due=uint32_t(int64_t(deadline));if(due>now)return;
  started=now;frame++;if(normalize(frame,int(durations.size()))&&(flags&8))active=false;
  nextFrame=frame+1;normalize(nextFrame,int(durations.size()));
 }
};
