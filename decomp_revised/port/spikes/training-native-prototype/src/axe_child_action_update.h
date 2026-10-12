#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

// Axe child vtable +0x18 held-ACTION update, original 00460920. This is a
// single explicit call model; root/caller cadence remains outside this slice.
namespace AxeChildActionUpdate {

struct State {
 uint32_t childState=0;       // child +0x368
 uint32_t strikeStart=0;      // child +0x36C
 uint32_t strikeSpan=0;       // child +0x370
 uint32_t clock=0;            // DAT_00502AFC
};

// These are process-global stat fields, shared by both Axe children.
struct Globals {
 bool statOnceInitialized=false; // DAT_00501918
 std::array<uint32_t,4> statRecord{{0,0,0,0}}; // DWORD words at 00501908..14; final low byte is written alone
 uint8_t sharedHitFlag=0; // DAT_00502E04; this body has no stores to it
};

struct StringRef {
 bool aliasesStatRecord=false;
 int32_t referenceCount=1;
};
struct Result { std::vector<std::string> events; };
struct Services {
 std::function<StringRef(const char*)> stringConstruct00401d80;
 std::function<void(const char*)> statIncrement00486950;
 std::function<void()> stringRelease004011c0;
 std::function<void(uint32_t,uint8_t)> hide004a11a0;
 std::function<void(uint32_t,float)> recovery0045fa60;
};

inline Result update(State& state,Globals& globals,const Services& services){
 Result result;
 if(state.childState==0){
  if(!globals.statOnceInitialized){
   globals.statRecord[0]=2;
   globals.statRecord[1]=0;
   globals.statRecord[2]=1;
   // 00460975 is a byte store to DAT_00501914. Preserve its upper bytes.
   globals.statRecord[3]=(globals.statRecord[3]&0xffffff00u);
   globals.statOnceInitialized=true;
  }
  StringRef stringRef;
  if(services.stringConstruct00401d80)stringRef=services.stringConstruct00401d80("Axe Hits");
  if(services.statIncrement00486950)services.statIncrement00486950("Axe Hits");
  // Mirror the caller's pointer/refcount branch after 00401D80. The usual
  // source constructor returns a temporary string at refcount 1; shared or
  // multiply-referenced values follow the distinct source branches.
  if(!stringRef.aliasesStatRecord){
   --stringRef.referenceCount;
   if(stringRef.referenceCount<1){
    if(services.stringRelease004011c0)services.stringRelease004011c0();
   }
  }
  state.childState=1;
  if(services.hide004a11a0)services.hide004a11a0(0x3b8,0);
  return result;
 }
 if(state.childState==2&&static_cast<uint32_t>(state.strikeStart+state.strikeSpan)<state.clock){
  if(services.recovery0045fa60)services.recovery0045fa60(0,1.0f);
 }
 return result;
}

} // namespace AxeChildActionUpdate
