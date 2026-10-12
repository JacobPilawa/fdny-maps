#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

// Source-comparable leaf for FUN_0043E2A0, called by Axe strike 00460B90.
// Script dispatch (004705B0) and final BModel destruction (0043D660) remain
// injected engine services; all local gates, hitpoint writes, and flag bits
// are modeled here.
namespace BModelAxeHit {
struct State {
    uint32_t hitpoints{};       // BModel data pointer +0x34
    uint32_t scriptActor{};     // BModel data pointer +0x38; only null accepts chop
    float strikeTime{};         // BModel +0x1EC
    uint32_t rootAddress{};     // PE32 receiver identity, used for service-call comparison
};

struct Result {
    uint32_t flags{};           // EAX low byte from 0043E2A0
    std::vector<std::string> events;
};

struct Services {
    // The constructor returns the underlying string-reference address stored
    // in the temporary string object passed to 004705B0.
    std::function<uint32_t(const char* command)> constructString00402790;
    std::function<bool(uint32_t receiver, uint32_t stringReference,
                       const char* command)> scriptEvent004705B0;
    std::function<void(uint32_t receiver)> destroy0043d660;
};

inline Result invoke(State& state, const Services& services) {
    Result result;

    // The field is FLD float; the original FCOMP operands are binary64 at
    // 004E0B60 (-1e-28) and 004E0B68 (+1e-28). Promote the stored float exactly
    // as C++ does for the mixed-width comparison. No float bit pattern can be
    // equal to either double constant, so boundary neighbors are the right tests.
    constexpr double low = -1.0e-28;
    constexpr double high = 1.0e-28;
    if (state.hitpoints == 0 || state.scriptActor != 0 ||
        !(state.strikeTime <= low || high <= state.strikeTime)) {
        return result;
    }

    result.flags = 2;
    result.events.emplace_back("string:00402790:chop");
    result.events.emplace_back("script:chop");
    uint32_t stringReference=services.constructString00402790("chop");
    if (services.scriptEvent004705B0(state.rootAddress+0x24,stringReference,"chop")) result.flags |= 4;

    --state.hitpoints;
    result.events.emplace_back("hitpoints:decrement");
    if (state.hitpoints == 0) {
        result.events.emplace_back("string:00402790:break");
        result.events.emplace_back("script:break");
        stringReference=services.constructString00402790("break");
        if (!services.scriptEvent004705B0(state.rootAddress+0x24,stringReference,"break")) result.flags &= ~uint32_t{4};
        result.flags |= 1;
        result.events.emplace_back("destroy:0043D660");
        services.destroy0043d660(state.rootAddress);
    }
    return result;
}
} // namespace BModelAxeHit
