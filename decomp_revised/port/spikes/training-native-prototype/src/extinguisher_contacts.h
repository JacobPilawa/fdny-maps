#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>

// 00481A50/00481A60: the original contact collector is a count followed by
// 32 four-word records. 00481A60 queues one message per accepted record; the
// recipient and two route values are supplied by the contacted fire object.
struct ExtinguisherContacts {
 struct Contact { float x=0,y=0,z=0,strength=0; };
 static constexpr int capacity=32;
 static constexpr uint32_t messageId=30000;
 struct Dispatch {
  uint32_t receiver=0,route0=0,route1=0,message=messageId;
  int slot=0;
  const Contact* payload=nullptr;
 };
 int count=0;
 std::array<Contact,capacity> records{};

 void reset(){count=0;}
 bool append(uint32_t receiver,uint32_t route0,uint32_t route1,V point,float strength,
             const std::function<void(const Dispatch&)>& queue){
  if(count>=capacity)return false;
  int slot=count;
  records[slot]={point.x,point.y,point.z,strength};
  Dispatch dispatch{receiver,route0,route1,messageId,slot,&records[slot]};
  queue(dispatch);
  ++count;
  return true;
 }
};

static_assert(offsetof(ExtinguisherContacts,records)==4);
static_assert(sizeof(ExtinguisherContacts::Contact)==16);
static_assert(sizeof(ExtinguisherContacts)==4+32*16);
