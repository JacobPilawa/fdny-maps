#pragma once
#include <cstdint>

// Genesis normal-world actor submission at 0x1003D770 tests flags & 0x1400.
// This predicate covers that normal render path only; it does not represent
// reflection or other renderer passes.
namespace ActorRenderFlags {
constexpr uint32_t normalWorldRenderMask = 0x1400u;

inline bool eligibleForNormalWorldRender(uint32_t flags){
 return (flags & normalWorldRenderMask) != 0;
}
}
