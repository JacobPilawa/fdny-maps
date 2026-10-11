#pragma once
#include <array>
#include <cstdint>
#include <deque>
#include <stdexcept>

// ID service used by 0049D450/0049B810 and resolved by 0049B7E0.
// Constructor 0049B9E0 seeds a FIFO of 512 route IDs, 100 through 611.
struct EntityIdRegistry {
 static constexpr uint32_t firstRoute=100;
 static constexpr uint32_t capacity=512;
 static constexpr uint32_t lastRoute=firstRoute+capacity-1;
 struct Handle {uint32_t route=0,generation=0;};
 struct Entry {uint32_t generation=0;void* object=nullptr;};

 uint32_t nextGeneration=1;
 std::array<Entry,lastRoute+1> entries{};
 std::deque<uint32_t> freeRoutes;

 EntityIdRegistry(){for(uint32_t id=firstRoute;id<=lastRoute;id++)freeRoutes.push_back(id);}

 bool registerObject(void* object,Handle& result){
  // 0049B810 assumes its free-list head is non-null; it has no exhaustion
  // branch. Surface that unsupported state instead of inventing a route.
  if(freeRoutes.empty())throw std::runtime_error("Original entity ID pool exhausted");
  uint32_t route=freeRoutes.front();freeRoutes.pop_front();
  uint32_t generation=nextGeneration++;
  entries[route]={generation,object};result={route,generation};return true;
 }

 void* resolve(uint32_t route,uint32_t generation)const{
  if(route>=entries.size())return nullptr;
  const Entry& entry=entries[route];
  return entry.object&&entry.generation==generation?entry.object:nullptr;
 }

 // 0049B780 releases by a nonzero identity generation and route, then returns
 // the ID to the FIFO free pool and zeroes the caller's generation word.
 bool release(Handle& handle){
  if(!handle.generation)return false;
  if(handle.route<firstRoute||handle.route>lastRoute)
   throw std::runtime_error("Entity ID release outside the original route pool");
  entries[handle.route]={};freeRoutes.push_back(handle.route);handle.generation=0;return true;
 }
};
