#include "../src/actor_render_flags.h"
#include <array>
#include <cstdint>
#include <iostream>

int main(){
 struct Sample{const char* name;uint32_t flags;bool expected;};
 constexpr std::array<Sample,10> samples{{
  {"no-flags",0x0000u,false},
  {"training-initial-0x400",0x0400u,true},
  {"second-mask-bit",0x1000u,true},
  {"both-mask-bits",0x1400u,true},
  {"outside-bit-0x2000",0x2000u,false},
  {"unrelated-low-bit",0x0001u,false},
  {"mask-plus-unrelated",0x2401u,true},
  {"adjacent-lower-bit",0x0800u,false},
  {"adjacent-higher-bit",0x2000u,false},
  {"training-picked-up-zero",0x0000u,false}
 }};
 int mismatches=0;
 for(const auto& sample:samples){
  const bool actual=ActorRenderFlags::eligibleForNormalWorldRender(sample.flags);
  if(actual!=sample.expected){
   ++mismatches;
   std::cerr<<sample.name<<": flags=0x"<<std::hex<<sample.flags<<std::dec
            <<" actual="<<actual<<" expected="<<sample.expected<<"\n";
  }
 }
 std::cout<<"Genesis normal-world render flag boundary check: "<<samples.size()
          <<" cases, mismatches="<<mismatches<<"\n";
 return mismatches?1:0;
}
