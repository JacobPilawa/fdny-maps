#pragma once

#include <cstdint>
#include <functional>
#include <vector>

// Cursor constructed by 00481C00 and initialized by 00481BA0. The manager
// owns a category list at +0x54; its head is at +0x10. Each linked node stores
// {object pointer, next pointer} in its first two words. The registration and
// old-cursor destruction services stay explicit boundaries.
namespace BModelCandidateIterator {
struct Cursor {
    uint32_t list = 0;
    uint32_t node = 0;
};

struct Memory {
    virtual ~Memory() = default;
    virtual uint32_t read32(uint32_t address) const = 0;
};

using Register = std::function<void(uint32_t list, Cursor& cursor)>;

inline void initialize(Cursor& cursor, uint32_t managerGlobal,
                       const Memory& memory, const Register& registerCursor) {
    // 00481C00 writes both cursor words to zero before calling 00481BA0.
    cursor = {};

    // 00481BA0 adds 0x50 then tests for zero; only a nonzero result receives
    // the final +4. This preserves its 32-bit address arithmetic and sentinel.
    const uint32_t managerBase = managerGlobal + 0x50;
    cursor.list = managerBase == 0 ? 0 : managerBase + 4;

    // 00481BA0 registers the cursor before loading the current list head.
    registerCursor(cursor.list, cursor);
    cursor.node = cursor.list == 0 ? 0 : memory.read32(cursor.list + 0x10);
}

inline uint32_t current(const Cursor& cursor, const Memory& memory) {
    return cursor.node == 0 ? 0 : memory.read32(cursor.node);
}

inline void advance(Cursor& cursor, const Memory& memory) {
    if (cursor.list != 0 && cursor.node != 0)
        cursor.node = memory.read32(cursor.node + 4);
}

template <class Visitor>
void each(Cursor cursor, const Memory& memory, Visitor&& visitor) {
    for (;;) {
        const uint32_t object = current(cursor, memory);
        if (cursor.list != 0 && cursor.node != 0) advance(cursor, memory);
        if (object == 0) return;
        visitor(object);
    }
}
} // namespace BModelCandidateIterator
