#pragma once
#include <cstdint>

// Value/update portion of 004868C0 and 00486980, after record lookup.
// Comparisons are signed int32; addition retains the original 32-bit wrap.
struct SourceStatisticRecord {
 int32_t mode=0; // source record +0x14; configuration is supplied by loader
 uint32_t value=0; // source record +0x24
 static int64_t signedWord(uint32_t word){
  return word&0x80000000u?int64_t(word)-0x100000000LL:int64_t(word);
 }
};
inline bool updateSourceStatistic(SourceStatisticRecord* record,uint32_t value){
 if(!record)return false;
 if(record->mode==0||
    (record->mode==1&&SourceStatisticRecord::signedWord(value)<SourceStatisticRecord::signedWord(record->value))||
    (record->mode==2&&SourceStatisticRecord::signedWord(record->value)<SourceStatisticRecord::signedWord(value))){
  record->value=value;return true;
 }
 return false;
}
inline bool addSourceStatistic(SourceStatisticRecord* record,uint32_t amount){
 if(!record)return false;
 record->value+=amount;return true;
}

// Ordered statistics portion of Player::escape (00451160). The caller owns
// Player+0x352C. The statistic store's 004868C0 helper applies each record's
// configured mode. Ops provides addStatistic, updateStatistic, readPickupClock
// (Player+0x5C4), and readClock (the global clock).
template<class Ops>
void recordTrainingRescueStatistics(uint32_t& playerRescueCounter,Ops&& ops){
 ++playerRescueCounter; // INC Player+0x352C
 ops.addStatistic("Rescues",1); // 00486980

 // Each callsite loads Player+0x5C4, then the global clock, then subtracts.
 // Keep both live reads (callbacks may change either value) and unsigned
 // 32-bit subtraction so elapsed time wraps as the source.
 const uint32_t longestPickupClock=ops.readPickupClock();
 const uint32_t longestElapsed=uint32_t(ops.readClock()-longestPickupClock);
 ops.updateStatistic("Longest Rescue Time",longestElapsed); // 004868C0
 const uint32_t shortestPickupClock=ops.readPickupClock();
 const uint32_t shortestElapsed=uint32_t(ops.readClock()-shortestPickupClock);
 ops.updateStatistic("Shortest Rescue Time",shortestElapsed); // 004868C0
}
