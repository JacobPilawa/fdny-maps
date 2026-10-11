#pragma once
#include <vector>
// 004A0B60 pauses playback and clears pending requests. 004A0BE0 appends
// requests while paused. 004A0D60 resumes them in insertion order.
// The unit draw happens after playback starts, only for looping requests.
// Duplicate pending requests remain separate, just as in the original.
struct SoundRequests{
 struct Request{int id;bool loop;};
 Random& random;bool paused=false;std::vector<Request> pending;
 std::function<void(int,bool)> beginPlayback;
 std::function<void(int,float)> setPhase;
 explicit SoundRequests(Random& shared):random(shared){}
 void beginLoading(){paused=true;pending.clear();}
 void request(int id,bool loop){
  if(paused){pending.push_back({id,loop});return;}
  if(!beginPlayback||!setPhase)throw std::runtime_error("Sound backend is not connected");
  beginPlayback(id,loop);
  if(loop)setPhase(id,random.unit());
 }
 void resume(){paused=false;for(size_t i=0;i<pending.size();i++){auto event=pending[i];request(event.id,event.loop);}pending.clear();}
};
