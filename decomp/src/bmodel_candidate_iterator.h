#pragma once

#include <cstdint>
#include <vector>

// Source-equivalent layout for the list cursor initialized by 00481BA0.
// The manager owns a category list at +0x54; its head is at +0x10. Each
// linked node stores {object pointer, next pointer} in its first two words.
namespace BModelCandidateIterator {
struct Cursor {
    uint32_t list = 0;
    uint32_t node = 0;
};

struct Memory {
    virtual ~Memory() = default;
    virtual uint32_t read32(uint32_t address) const = 0;
};

inline void initialize(Cursor& cursor, uint32_t manager, const Memory& memory) {
    cursor.list = manager + 0x54;
    cursor.node = memory.read32(cursor.list + 0x10);
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
