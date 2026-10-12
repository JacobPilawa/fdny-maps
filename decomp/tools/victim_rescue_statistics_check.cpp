#include "../src/victim_rescue_statistics.h"
#include <cassert>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

struct RecordingOps{
 std::vector<std::string> calls;
 std::vector<uint32_t> clockValues;
 size_t clockIndex=0;
 std::vector<uint32_t> pickupClockValues;
 size_t pickupClockIndex=0;
 std::vector<std::pair<std::string,uint32_t>> stats;
 uint32_t* counter=nullptr;
 bool mutatePickupClockOnLongest=true;
 void addStatistic(const char* name,int amount){
  if(counter)assert(*counter==8);assert(std::string(name)=="Rescues"&&amount==1);
  calls.emplace_back("add:Rescues:1");stats.emplace_back(name,uint32_t(amount));
 }
 uint32_t readPickupClock(){calls.emplace_back("pickup-clock");assert(pickupClockIndex<pickupClockValues.size());return pickupClockValues[pickupClockIndex++];}
 uint32_t readClock(){
  calls.emplace_back("clock");assert(clockIndex<clockValues.size());
  return clockValues[clockIndex++];
 }
 void updateStatistic(const char* name,uint32_t value){
  calls.emplace_back(std::string("update:")+name);
  stats.emplace_back(name,value);
  if(mutatePickupClockOnLongest&&std::string(name)=="Longest Rescue Time")pickupClockValues[1]=0x20u;
 }
};

int main(){
 assert(!updateSourceStatistic(nullptr,1)&&!addSourceStatistic(nullptr,1));
 struct Sample{int32_t mode;uint32_t oldValue,newValue;bool changed;};
 for(auto s:{Sample{0,9,9,true},Sample{0,9,2,true},Sample{1,9,2,true},
             Sample{1,9,12,false},Sample{1,9,9,false},Sample{2,9,12,true},
             Sample{2,9,2,false},Sample{2,9,9,false},Sample{3,9,2,false},
             Sample{1,0,0xffffffffu,true},Sample{2,0xffffffffu,0,true}}){
  SourceStatisticRecord record{s.mode,s.oldValue};
  assert(updateSourceStatistic(&record,s.newValue)==s.changed);
  assert(record.value==(s.changed?s.newValue:s.oldValue));
 }
 SourceStatisticRecord overflow{2,0xffffffffu};
 assert(addSourceStatistic(&overflow,2)&&overflow.value==1);
 uint32_t counter=7;
 RecordingOps ops{{},{0x10u,0x40u},0,{0xfffffff0u,0xfffffff0u},0,{},&counter,true};
 recordTrainingRescueStatistics(counter,ops);
 assert(counter==8);
 assert(ops.calls==std::vector<std::string>({
  "add:Rescues:1","pickup-clock","clock","update:Longest Rescue Time",
  "pickup-clock","clock","update:Shortest Rescue Time"}));
 assert(ops.clockIndex==2);
 assert(ops.pickupClockIndex==2);
 assert(ops.stats.size()==3);
 assert(ops.stats[0]==std::make_pair(std::string("Rescues"),1u));
 assert(ops.stats[1]==std::make_pair(std::string("Longest Rescue Time"),32u));
 assert(ops.stats[2]==std::make_pair(std::string("Shortest Rescue Time"),32u));

 // INC and elapsed subtraction operate on the source's 32-bit fields.
 uint32_t wrappedCounter=0xffffffffu;
 RecordingOps wrapOps{{},{0xffffffffu,0u},0,{0xffffffffu,0xffffffffu},0,{},nullptr,false};
 recordTrainingRescueStatistics(wrappedCounter,wrapOps);
 assert(wrappedCounter==0u);
 assert(wrapOps.stats[1].second==0u&&wrapOps.stats[2].second==1u);
}
