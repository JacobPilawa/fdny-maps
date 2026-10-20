#pragma once

#include <algorithm>
#include <vector>

#include "bmodel_use.h"
#include "interaction_box.h"

// Shared host adapter for the Player BModel interaction selection object:
// 00459000/00459050/00459090 and 00459350. The caller supplies contact-driven
// registrations and source-shaped use actions; contact scheduling remains a
// separate host concern.
namespace BModelInteractionSelection {

struct Registry {
 std::vector<int> slots;

 bool contains(int entity) const {
  return std::find(slots.begin(), slots.end(), entity) != slots.end();
 }

 void add(int entity) {
  if (contains(entity)) return;
  auto hole = std::find(slots.begin(), slots.end(), -1);
  if (hole != slots.end()) *hole = entity;
  else slots.push_back(entity);
 }

 void remove(int entity) {
  auto candidate = std::find(slots.begin(), slots.end(), entity);
  if (candidate != slots.end()) *candidate = -1;
 }
};

struct Candidate {
 int entity = -1;
 int action = 0;
 bool hasQueryInterface = true;
 V minimum{};
 V maximum{};
};

struct Selection {
 int slot = -1;
 int entity = -1;
 int action = 0;
 V point{};
 float score = 0.f;
};

inline Selection select(V start, V direction, const Registry& registry,
                        const std::vector<Candidate>& candidates) {
 Selection selected;
 float bestScore = 0.f;
 const Extended reachSquared = Extended::from(200.f) * Extended::from(200.f);

 for (size_t slot = 0; slot < registry.slots.size(); ++slot) {
  const int entity = registry.slots[slot];
  if (entity < 0) continue;
  auto candidate = std::find_if(candidates.begin(), candidates.end(),
      [entity](const Candidate& value) { return value.entity == entity; });
  if (candidate == candidates.end() || candidate->action == 0) continue;

  if (!candidate->hasQueryInterface) {
   // 00459350 immediately accepts a nonzero action without a query object.
   selected = {int(slot), entity, candidate->action, {}, 0.f};
   return selected;
  }

  V point{};
  float score = 0.f;
  if (!InteractionBox::query(start, direction, candidate->minimum,
                             candidate->maximum, &point, &score))
   continue;

  V delta;
  for (int axis = 0; axis < 3; ++axis)
   delta[axis] = (Extended::from(point[axis]) - Extended::from(start[axis])).asFloat();
  const Extended distanceSquared = Extended::from(delta.z) * Extended::from(delta.z)
                                + Extended::from(delta.y) * Extended::from(delta.y)
                                + Extended::from(delta.x) * Extended::from(delta.x);
  if (InteractionSphere::compare(distanceSquared, reachSquared) > 0) continue;

  // The retail scan replaces equal scores, so later live slots win ties.
  if (selected.slot < 0 || bestScore <= score) {
   selected = {int(slot), entity, candidate->action, point, score};
   bestScore = score;
  }
 }
 return selected;
}

} // namespace BModelInteractionSelection
