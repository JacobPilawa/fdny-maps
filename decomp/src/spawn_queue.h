#pragma once
#include <deque>
#include <functional>
#include <vector>

// Original 0049C670: drain constructors, first pass, then second pass.
// Constructor additions enter the current batch; initialization additions
// stay queued for the next drain. A null request ends the construction pass.
struct SpawnQueue {
 using Factory=std::function<int()>;
 std::deque<Factory> pending;
 std::function<void(int)> registerEntity, firstPass, secondPass;
 void enqueue(Factory factory){pending.push_back(std::move(factory));}
 void drain(){
  std::vector<int> batch;
  while(!pending.empty()){
   auto factory=std::move(pending.front());pending.pop_front();
   if(!factory)break;
   int entity=factory();registerEntity(entity);batch.push_back(entity);
  }
  for(int entity:batch)firstPass(entity);
  for(int entity:batch)secondPass(entity);
 }
};
