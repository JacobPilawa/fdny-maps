#pragma once

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <string>

// Shared host state for the source-confirmed C_Type=5 MiscEmitter command
// path. This records the immediate actor state only; its runtime descriptor,
// child animation, draw cadence, and visual resources remain unresolved.
struct MiscEmitterRuntimeState {
    int entity = -1;
    float uniformScale = 1.f; // InitializeMiscEmitterEntity writes root +0x7C = 1.0.
    std::optional<bool> active; // Set only by the verified turnon/turnoff callbacks.
};

inline bool applyMiscEmitterCommand(MiscEmitterRuntimeState& emitter,
                                    const std::string& verb,
                                    const std::string& arguments) {
    if (verb == "turnon") {
        emitter.active = true; // C_Type=5 internal vtable slot +0x18.
        return true;
    }
    if (verb == "turnoff") {
        emitter.active = false; // C_Type=5 internal vtable slot +0x1C.
        return true;
    }
    if (verb != "setsize") return false;

    // C_Type=5 consumes one scalar, stores it at owner +0x7C, and refreshes
    // the emitter root immediately. FireRescue's timed XYZ form is separate.
    const char* begin = arguments.c_str();
    char* end = nullptr;
    const float scale = std::strtof(begin, &end);
    if (end == begin || !std::isfinite(scale)) return false;
    while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') ++end;
    if (*end != '\0') return false;
    emitter.uniformScale = scale;
    return true;
}
