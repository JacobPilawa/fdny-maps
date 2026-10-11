#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <stdexcept>
// Shared original RNG: 00404B50 seed, 00404960 reload, 00404AC0 float,
// 00498B30 range, 004B9400 truncating integer conversion.
struct Random {
 std::array<uint32_t,624> words{};size_t index=624;uint64_t drawCount=0;
 // One observer for the shared stream. Rendering/audio must use this same
 // object, so their draws remain visible in a replay comparison.
 std::function<void(uint64_t,uint32_t)> observer;
 struct State{std::array<uint32_t,624> words;size_t nextIndex;uint64_t ordinal;};
 explicit Random(uint32_t value){seed(value);}
 State checkpoint()const{return {words,index,drawCount};}
 // Restore the stream itself, rather than guessing how many menu/startup
 // draws a seed has consumed. Original remaining=0 means reload next.
 void restore(const State& state){if(state.nextIndex>624)throw std::runtime_error("Invalid original RNG cursor");words=state.words;index=state.nextIndex;drawCount=state.ordinal;}

 void seed(uint32_t value){uint32_t word=(value&0x8ffffffdu)|1u;words[0]=word;for(size_t i=1;i<624;i++){word*=69069u;words[i]=word;}index=624;drawCount=0;}
 uint32_t nextWord(){if(index>=624){for(size_t i=0;i<624;i++){uint32_t joined=(words[i]&0x80000000u)|(words[(i+1)%624]&0x7fffffffu);words[i]=words[(i+397)%624]^(joined>>1)^((joined&1)?0x9908b0dfu:0);}index=0;}uint32_t value=words[index++];value^=value>>11;value^=(value<<7)&0x9d2c5680u;value^=(value<<15)&0xefc60000u;value^=value>>18;drawCount++;if(observer)observer(drawCount,value);return value;}
 // 00404B30: signed validity check, unsigned division. Zero/negative bounds
 // return zero without consuming the stream.
 uint32_t bounded(uint32_t bound){return int32_t(bound)<1?0:nextWord()%bound;}
 float unit(){uint32_t bits=(nextWord()&0x7fffffu)|0x3f800000u;float value;std::memcpy(&value,&bits,4);return value-1.f;}
 double range(float lo,float hi){return (double(hi)-lo)*unit()+lo;}
 int spotIndex(int count){if(count<1)throw std::runtime_error("No victim spawn candidates");float upper=float(double(count)-.01);return int(range(0,upper));}
 // 00488D40 sound collection: draw once; repeat selects a random neighbor,
 // including one-item collections. The wrap occurs after that second draw.
 int soundIndex(int count,int previous){int chosen=int(range(0,float(double(count)-.001)));if(chosen==previous)chosen+=range(0,1)<=.5?-1:1;if(chosen>=count)chosen=0;else if(chosen<0)chosen=count-1;return chosen;}
};
