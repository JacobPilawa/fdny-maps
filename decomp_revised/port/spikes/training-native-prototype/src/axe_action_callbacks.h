#pragma once

#include <cstdint>
#include <string>
#include <vector>

// A deliberately narrow model of Axe root ACTION callbacks. Child +0x18
// behavior (00460920) is an injected boundary; this does not implement a hit.
namespace AxeActionCallbacks {
struct State {
    uint8_t primaryEnabled{};      // Axe +0x1FC
    uint32_t deadline{};           // Axe +0xA90
    uint32_t primaryState{};       // Axe +0x568
    uint32_t secondaryState{};     // Axe +0x9B0
    uint32_t primaryTimerBits{};   // Axe +0x53C
    uint32_t secondaryTimerBits{}; // Axe +0x984
    bool dat00502e04{};
};

struct Result {
    std::vector<std::string> events;
};

inline void press(State& s, uint32_t globalClock, Result& out) {
    s.deadline = globalClock + 100u;
    out.events.emplace_back("deadline:" + std::to_string(s.deadline));
}

// Child +0x18 is a void callback. Its effect is communicated through the
// shared DAT_00502E04 byte in State, not through a return value.
using ChildUpdate = void (*)(bool primary, State& state, void* context);
// Offsets are from Axe and Player roots, preserving the original arguments to
// 00483A20: follower address, new parent address, then mode.
using Reparent = void (*)(uint32_t followerOffset, uint32_t parentOffset,
                          uint32_t mode, void* context);

inline void held(State& s, uint32_t globalClock, ChildUpdate childUpdate,
                 Reparent reparent, void* context, Result& out) {
    s.dat00502e04 = false;
    out.events.emplace_back("clear:00502E04");
    if (s.primaryEnabled && globalClock > s.deadline) {
        out.events.emplace_back("child:primary:00460920");
        childUpdate(true, s, context);
    }
    out.events.emplace_back("child:secondary:00460920");
    childUpdate(false, s, context);
    if (!s.dat00502e04) return;

    if (s.primaryEnabled) {
        s.primaryState = 3;
        s.primaryTimerBits = 0x3f400000u;
        out.events.emplace_back("primary-state:3");
        out.events.emplace_back("primary-timer:3f400000");
        reparent(0x35c, 0x34a4, 0, context);
        out.events.emplace_back("reparent:primary");
    }
    s.secondaryState = 3;
    s.secondaryTimerBits = 0x3f400000u;
    out.events.emplace_back("secondary-state:3");
    out.events.emplace_back("secondary-timer:3f400000");
    reparent(0x7a4, 0x34a4, 0, context);
    out.events.emplace_back("reparent:secondary");
}
} // namespace AxeActionCallbacks
