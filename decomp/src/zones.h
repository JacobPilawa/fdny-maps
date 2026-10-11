#pragma once
#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

struct TrainingZoneBox {
 int entity=0;
 V mins{},maxs{};
};

struct TrainingZone {
 std::string label;
 std::vector<TrainingZoneBox> boxes;
 float summedVolume=0;
};

// makezone builds the map-zone tree; its query returns the smallest summed
// member-box volume containing a point. The label comparison mode is supplied
// by the manager's source-backed constructor setting.
struct TrainingZones {
 enum class LabelMode { CaseSensitive, AsciiInsensitive };
 LabelMode labelMode;
 std::vector<TrainingZone> groups;

 explicit TrainingZones(LabelMode mode):labelMode(mode){}

 static unsigned char folded(unsigned char c){return c>='A'&&c<='Z'?static_cast<unsigned char>(c+('a'-'A')):c;}
 bool labelsEqual(const std::string& a,const std::string& b)const{
  if(labelMode==LabelMode::CaseSensitive)return a==b;
  if(a.size()!=b.size())return false;
  for(size_t i=0;i<a.size();i++)if(folded((unsigned char)a[i])!=folded((unsigned char)b[i]))return false;
  return true;
 }
 bool labelLess(const std::string& a,const std::string& b)const{
  size_t n=std::min(a.size(),b.size());
  for(size_t i=0;i<n;i++){
   unsigned char x=(unsigned char)a[i],y=(unsigned char)b[i];
   if(labelMode==LabelMode::AsciiInsensitive){x=folded(x);y=folded(y);}
   if(x!=y)return x<y;
  }
  return a.size()<b.size();
 }
 static Extended boxVolume(const TrainingZoneBox& box){
  return (Extended::from(box.maxs.x)-Extended::from(box.mins.x))
       *(Extended::from(box.maxs.y)-Extended::from(box.mins.y))
       *(Extended::from(box.maxs.z)-Extended::from(box.mins.z));
 }
 TrainingZone& addTrigger(std::string label,int entity,V mins,V maxs){
  auto zone=groups.end();
  for(auto it=groups.begin();it!=groups.end();++it)if(labelsEqual(it->label,label)){zone=it;break;}
  if(zone==groups.end()){groups.push_back({std::move(label),{},0.f});zone=groups.end()-1;}
  TrainingZoneBox box{entity,mins,maxs};
  zone->boxes.push_back(box);
  // 00484740 adds the x87 float10 box volume to a float accumulator, then
  // rounds that assignment back to binary32 after each member trigger.
  zone->summedVolume=(Extended::from(zone->summedVolume)+boxVolume(box)).asFloat();
  return *zone;
 }
 static bool contains(const TrainingZoneBox& box,V p){
  for(int axis=0;axis<3;axis++)if(!(p[axis]>=box.mins[axis]&&p[axis]<=box.maxs[axis]))return false;
  return true;
 }
 bool contains(const TrainingZone& zone,V p)const{
  for(const auto& box:zone.boxes)if(contains(box,p))return true;
  return false;
 }
 const TrainingZone* lookup(V p)const{
  // The original zone records are enumerated from an ordered tree. In-order
  // ordering plus <= reproduces its later-enumerated equal-volume tie rule.
  std::vector<const TrainingZone*> ordered;ordered.reserve(groups.size());
  for(const auto& zone:groups)ordered.push_back(&zone);
  std::stable_sort(ordered.begin(),ordered.end(),[&](const auto* a,const auto* b){return labelLess(a->label,b->label);});
  const TrainingZone* result=nullptr;
  for(const auto* zone:ordered)if(contains(*zone,p)&&(result==nullptr||zone->summedVolume<=result->summedVolume))result=zone;
  return result;
 }
 const TrainingZone* find(const std::string& label)const{
  for(const auto& zone:groups)if(labelsEqual(zone.label,label))return &zone;
  return nullptr;
 }
};
