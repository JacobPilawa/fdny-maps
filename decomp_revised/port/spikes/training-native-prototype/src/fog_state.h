#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <string>

// Source-mapped state used by the retail fogenable/fogdisable helpers:
// current and target are (start, end, red, green, blue). Each target change
// interpolates for 2,000 ms. During that interval the retail driver remains
// enabled; the requested enabled state takes effect when interpolation ends.
struct RetailFogState {
    struct Values {
        float start = 0.f;
        float end = 0.f;
        float red = 0.f;
        float green = 0.f;
        float blue = 0.f;
    };

    static constexpr uint32_t TransitionDurationMs = 2000;
    static constexpr float CameraFarClipPadding = 20.f;

    Values current{};
    Values source{};
    Values target{};
    uint32_t transitionStartMs = 0;
    bool configured = false;
    bool targetEnabled = false;
    bool transitioning = false;

    void update(uint32_t now) {
        if (!transitioning) return;
        const uint32_t elapsed = now - transitionStartMs;
        if (elapsed >= TransitionDurationMs) {
            current = target;
            transitioning = false;
            return;
        }
        const float fraction = float(elapsed) / float(TransitionDurationMs);
        auto mix = [fraction](float from, float to) {
            return from + (to - from) * fraction;
        };
        current = {
            mix(source.start, target.start),
            mix(source.end, target.end),
            mix(source.red, target.red),
            mix(source.green, target.green),
            mix(source.blue, target.blue),
        };
    }

    void enable(int32_t ignoredStart, int32_t end, int32_t red, int32_t green,
                int32_t blue, uint32_t now) {
        (void)ignoredStart; // 0049C960 fixes the target start value to 1.0.
        update(now);
        source = current;
        target = {1.f, float(end), float(red), float(green), float(blue)};
        transitionStartMs = now;
        targetEnabled = true;
        configured = true;
        transitioning = true;
    }

    void disable(uint32_t now) {
        update(now);
        source = current;
        target = {1990.f, 2000.f, current.red, current.green, current.blue};
        transitionStartMs = now;
        targetEnabled = false;
        configured = true;
        transitioning = true;
    }

    bool renderEnabled() const { return transitioning || targetEnabled; }

    float cameraFarClip() const {
        if (!configured) return 30000.f;
        return std::max(1.01f, current.end + CameraFarClipPadding);
    }

    static float retailColorChannel(float value) {
        // 0049CF10 converts each interpolated channel to an integer toward
        // zero, then masks with 0xF0 before sending it to the driver.
        const double truncated = std::trunc(double(value));
        if (truncated < double(std::numeric_limits<int32_t>::min()) ||
            truncated > double(std::numeric_limits<int32_t>::max()))
            return 0.f;
        const int32_t integer = int32_t(truncated);
        return float(uint32_t(integer) & 0xF0u);
    }

    std::array<float, 4> normalizedColor() const {
        return {{retailColorChannel(current.red) / 255.f,
                 retailColorChannel(current.green) / 255.f,
                 retailColorChannel(current.blue) / 255.f,
                 1.f}};
    }
};

inline bool parseRetailFogEnableArguments(std::string arguments,
                                          std::array<int32_t, 5>& values) {
    for (char& ch : arguments) {
        if (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == ',' || ch == ';')
            ch = ' ';
    }
    std::istringstream stream(arguments);
    for (int32_t& value : values) {
        std::string token;
        if (!(stream >> token)) return false;
        char* end = nullptr;
        const long parsed = std::strtol(token.c_str(), &end, 10);
        if (end == token.c_str() || *end != '\0' ||
            parsed < std::numeric_limits<int32_t>::min() ||
            parsed > std::numeric_limits<int32_t>::max())
            return false;
        value = int32_t(parsed);
    }
    std::string extra;
    return !(stream >> extra);
}
