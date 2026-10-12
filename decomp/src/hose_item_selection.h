#pragma once
#include <array>
#include <stdexcept>
#include <string>
#include <vector>

// Player inventory selection used by 0044C840/0044C2A0/0044C360.
// Item bodies, UI internals, and hose frame construction remain service boundaries.
namespace HoseItemSelection {

struct Item {
 bool present=false;
 bool canDeactivate=true; // Result of the item's vtable +0x38 service.
};

struct State {
 std::array<Item,5> slots{};
 int count=5;
 int selected=0; // Player +0x357C, zero-based.
 int hoseMode=0; // Hose Spray +0xC68.
 int attachedValve=-1; // Host identity only; original pointer store is not in this slice.
};

struct Result {
 bool returned=false;
 bool reachedFrameTransform=false;
 bool hosePresent=false;
 std::vector<std::string> events;
};

inline std::string indexedEvent(const char* name,int index,int argument){
 return std::string(name)+":"+std::to_string(index)+":"+std::to_string(argument);
}

inline std::string serviceEvent(const char* name,int receiverOffset,int code,int parameter){
 return std::string(name)+":"+std::to_string(receiverOffset)+":"+std::to_string(code)+":"+std::to_string(parameter);
}

inline std::string hideEvent(int receiverOffset,int parameter){
 return std::string("hide:")+std::to_string(receiverOffset)+":"+std::to_string(parameter);
}

inline void setMode(State& state,int mode,Result& result){
 const int old=state.hoseMode;
 if(old==mode)return;
 state.hoseMode=mode;
 // 00463370: transitions out of 0/1 into 2/3 refresh mode 6 with item 4;
 // all other actual mode changes refresh mode 5 with a null parameter.
 if(((old!=0&&old!=1)||(mode!=2&&mode!=3)))
  result.events.push_back(serviceEvent("hud",0x5f4,5,0));
 else
  result.events.push_back(serviceEvent("hud",0x5f4,6,4));
}

inline bool selectSlot(State& state,int index,bool force,Result& result){
 if(index<0||index>=5||index>=state.count||!state.slots[size_t(index)].present)return false;
 if(index==state.selected)return true;
 if(state.selected==0){
  state.selected=index;
  result.events.push_back(indexedEvent("active",index,1));
 }else{
  const int old=state.selected;
  if(!force){
   const bool can=old>=0&&old<5&&state.slots[size_t(old)].present&&state.slots[size_t(old)].canDeactivate;
   result.events.push_back(indexedEvent("can",old,can?1:0));
   if(!can)return false;
  }
  result.events.push_back(indexedEvent("active",old,0));
  state.selected=index;
  result.events.push_back(indexedEvent("active",index,1));
 }
 result.events.push_back(serviceEvent("hud",0x5f4,5,0));
 return true;
}

inline Result attach(State& state){
 Result result;result.hosePresent=state.count>4&&state.slots[4].present;
 if(!result.hosePresent){
  result.events.push_back(hideEvent(0x508,0));result.returned=true;return result;
 }
 if(state.selected!=4){
  if(state.selected==0){
   state.selected=4;result.events.push_back(indexedEvent("active",4,1));
  }else{
   const int old=state.selected;
   const bool can=old>=0&&old<5&&state.slots[size_t(old)].present&&state.slots[size_t(old)].canDeactivate;
   result.events.push_back(indexedEvent("can",old,can?1:0));
   if(!can){result.events.push_back(hideEvent(0x508,0));result.returned=true;return result;}
   result.events.push_back(indexedEvent("active",old,0));state.selected=4;result.events.push_back(indexedEvent("active",4,1));
  }
  result.events.push_back(serviceEvent("hud",0x5f4,5,0));
 }
 result.reachedFrameTransform=true;
 return result;
}

inline void reselectFirst(State& state,Result& result){
 if(state.count<0)throw std::invalid_argument("negative Player item count");
 // 0044C360 scans count entries; 0044C2A0 accepts slots 0..4.
 for(int i=0;i<state.count&&i<5;i++)
  if(state.slots[size_t(i)].present&&selectSlot(state,i,true,result))return;
 state.selected=0;
}

inline Result detach(State& state){
 Result result;result.hosePresent=state.count>4&&state.slots[4].present;
 if(result.hosePresent)setMode(state,2,result);
 reselectFirst(state,result);
 result.events.push_back(hideEvent(0x4c0,0));result.returned=true;return result;
}

} // namespace HoseItemSelection
