#pragma once
// 0049E240 initializes remaining=4 and complete=false. 0049E2C0 waits
// for four entity updates, executes the level script in full mode, then
// marks complete and requests main-state 3. No random draws occur here.
struct WorldStartup {
 uint32_t remaining=4;bool complete=false;
 void update(const std::function<void()>& executeLevel,const std::function<void()>& requestReady){
  if(complete)return;
  --remaining;
  if(remaining==0){executeLevel();complete=true;requestReady();}
 }
};
