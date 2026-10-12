#include <json-c/json.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../src/axe_strike_pose.h"

namespace {
using J = json_object*;

J field(J object, const char* key) {
    J value = nullptr;
    if (!json_object_object_get_ex(object, key, &value))
        throw std::runtime_error(std::string("Missing field: ") + key);
    return value;
}

uint32_t u32(J value) {
    return static_cast<uint32_t>(json_object_get_uint64(value));
}

std::string string(J value) {
    return json_object_get_string(value);
}

float fromBits(J value) {
    const uint32_t bits = u32(value);
    float result;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

AxeStrikePose::Vec3 inputVector(J frame, const char* key) {
    J bits = field(field(frame, key), "bits");
    if (json_object_array_length(bits) != 3)
        throw std::runtime_error(std::string("Expected 3 input bits: ") + key);
    return {fromBits(json_object_array_get_idx(bits, 0)),
            fromBits(json_object_array_get_idx(bits, 1)),
            fromBits(json_object_array_get_idx(bits, 2))};
}

bool equalsBits(const AxeStrikePose::Vec3& actual, J vector) {
    J bits = field(vector, "bits");
    if (json_object_array_length(bits) != 3) return false;
    for (int i = 0; i != 3; ++i) {
        uint32_t actualBits;
        std::memcpy(&actualBits, &actual[i], sizeof(actualBits));
        if (actualBits != u32(json_object_array_get_idx(bits, i))) return false;
    }
    return true;
}

J eventNamed(J events, const char* name) {
    for (size_t i = 0; i < json_object_array_length(events); ++i) {
        J event = json_object_array_get_idx(events, static_cast<int>(i));
        if (string(field(event, "name")) == name) return event;
    }
    return nullptr;
}
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 2)
            throw std::runtime_error("Usage: axe_strike_pose_pcode_check fixture.json");
        J root = json_object_from_file(argv[1]);
        if (!root) throw std::runtime_error("Cannot read Axe strike P-code fixture");
        if (string(field(root, "exe_sha256")) !=
            "4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e")
            throw std::runtime_error("Axe strike fixture executable hash mismatch");

        J samples = field(root, "samples");
        size_t compared = 0;
        int mismatches = 0;
        for (size_t i = 0; i < json_object_array_length(samples); ++i) {
            J sample = json_object_array_get_idx(samples, static_cast<int>(i));
            J events = field(sample, "events");
            J collision = eventNamed(events, "collision:00492CE0");
            if (!collision) continue; // The strict time-reject path never builds a pose.

            J frame = field(sample, "frame_input");
            AxeStrikePose::Inputs inputs;
            inputs.playerFrame04 = inputVector(frame, "plus4");
            inputs.playerFrame10 = inputVector(frame, "plus10");
            inputs.playerFrame1c = inputVector(frame, "plus1c");
            inputs.playerAt34cc = inputVector(frame, "player_plus34cc");
            const int signedSide = static_cast<int>(json_object_get_int(
                field(frame, "axe_signed_side_byte")));
            if (signedSide < -128 || signedSide > 127)
                throw std::runtime_error("Axe side byte is outside int8 range");
            inputs.axeFlag1fc = static_cast<int8_t>(signedSide);

            const AxeStrikePose::Query query = AxeStrikePose::build(inputs);
            const bool ok = equalsBits(query.start, field(collision, "start")) &&
                            equalsBits(query.mins, field(collision, "mins")) &&
                            equalsBits(query.maxs, field(collision, "maxs")) &&
                            equalsBits(query.displacement, field(collision, "displacement"));
            ++compared;
            if (!ok) {
                std::cerr << string(field(sample, "name"))
                          << " pose mismatch; expected original collision-call float bits\n";
                ++mismatches;
            }
        }
        if (compared == 0) throw std::runtime_error("Fixture has no collision pose samples");
        std::cout << "Axe strike pose P-code cases: " << compared
                  << ", mismatches: " << mismatches << "\n";
        json_object_put(root);
        return mismatches ? 1 : 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << "\n";
        return 2;
    }
}
