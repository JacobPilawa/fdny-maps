#pragma once
#include <array>
#include <cstdint>
#include <deque>
#include <functional>

// 0049B4C0/0049B610: pending messages refer to one of 64 reusable mapper
// records. Delivery rereads that record, resolves its route generation, calls
// the receiver if it still exists, then releases the record.
struct FireMessageQueue {
 static constexpr int capacity=64;
 struct Record {
  uint32_t route0=0,route1=0,message=0;
  const void* payload=nullptr;
  bool active=false;
 };
 struct Pending {uint8_t slot=0,flag=0;};
 struct Delivery {
  uint8_t slot=0;
  const Record* record=nullptr;
  bool resolved=false;
  int pendingAfterPop=0;
 };
 using Receiver=std::function<void(uint32_t,const void*)>;
 int highWater=0;
 std::array<Record,capacity> records{};
 std::deque<Pending> pending;

 bool enqueue(uint32_t route0,uint32_t route1,uint32_t message,const void* payload,uint8_t flag=0){
  int slot=highWater;
  if(slot<capacity){
   while(slot<capacity&&records[slot].active)++slot;
  }
  if(slot>=capacity){
   slot=0;
   while(slot<highWater&&records[slot].active)++slot;
   if(slot>=highWater)return false;
  }
  highWater=slot;
  records[slot]={route0,route1,message,payload,true};
  pending.push_back({uint8_t(slot),flag});
  return true;
 }

 template<class Resolve,class AfterDelivery>
 void drain(Resolve&& resolve,AfterDelivery&& afterDelivery){
  while(!pending.empty()){
   Pending node=pending.front();pending.pop_front();
   Record& record=records[node.slot];
   Receiver receiver=resolve(record.route0,record.route1);
   if(receiver)receiver(record.message,record.payload);
   afterDelivery(Delivery{node.slot,&record,bool(receiver),int(pending.size())});
   record.active=false;
  }
 }
};
