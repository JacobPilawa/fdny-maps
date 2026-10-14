#pragma once

#include <array>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

// Retail Genesis MOTN/PATH sampler for the key modes actually present in the
// copied BSP maps. The input JSON is produced by decode_gbsp_motions.py from
// raw chunk 24 and the hash-pinned Genesis.dll readers. This is deliberately
// separate from motion.h, which samples ACT skeletal animation tracks.
namespace RetailGbspMotion {

struct Key3 {
    float time = 0.f;
    V value{};
    V incoming{};
    V outgoing{};
};

struct KeyQ {
    float time = 0.f;
    Q value{}; // Genesis geQuaternion is stored as W, X, Y, Z.
};

struct Bracket {
    size_t first = 0;
    size_t second = 0;
    float fraction = 0.f;
};

struct PreparedPath {
    std::vector<Key3> translation;
    std::vector<KeyQ> rotation;

    Pose sample(float time) const;
};

inline V readVec3(J value, const char* channel) {
    if (count(value) != 3)
        throw std::runtime_error(std::string("Retail GBSP motion: malformed ") + channel + " XYZ value");
    return {num(at(value, 0)), num(at(value, 1)), num(at(value, 2))};
}

inline Q readQuaternion(J value) {
    if (count(value) != 4)
        throw std::runtime_error("Retail GBSP motion: malformed WXYZ quaternion value");
    return {num(at(value, 0)), num(at(value, 1)), num(at(value, 2)), num(at(value, 3))};
}

template <typename Key>
inline Bracket bracket(const std::vector<Key>& keys, float time) {
    if (keys.empty()) return {};
    if (keys.size() == 1 || time < keys.front().time) return {0, 0, 0.f};
    if (time >= keys.back().time) {
        const size_t last = keys.size() - 1;
        return {last, last, 0.f};
    }

    // Retail FUN_10014520 is a binary search for an exact key or the last
    // key before the sample time.
    size_t low = 0, high = keys.size() - 1;
    while (low <= high) {
        const size_t middle = low + (high - low) / 2;
        if (time == keys[middle].time) return {middle, middle, 0.f};
        if (time < keys[middle].time) {
            if (middle == 0) return {0, 0, 0.f};
            high = middle - 1;
        } else {
            low = middle + 1;
        }
    }
    const size_t first = high;
    const size_t second = first + 1;
    const float span = keys[second].time - keys[first].time;
    if (!(span > 0.f))
        throw std::runtime_error("Retail GBSP motion: key times are not strictly increasing");
    return {first, second, (time - keys[first].time) / span};
}

inline std::vector<Key3> readTranslation(J track, bool looped) {
    std::vector<Key3> keys;
    if (!json_object_get_boolean(field(track, "present"))) return keys;
    if (integer(field(track, "key_mode")) != 1 ||
        integer(field(track, "sampler_index")) != 1 ||
        str(field(track, "encoding")) != "xyz")
        throw std::runtime_error("Retail GBSP motion: translation mode is outside the ported mode-1 path");
    if (looped)
        throw std::runtime_error("Retail GBSP motion: looped translation time/tangents are not ported yet");

    J sourceKeys = field(track, "keys");
    keys.reserve(size_t(count(sourceKeys)));
    for (int i = 0; i < count(sourceKeys); ++i) {
        J source = at(sourceKeys, i);
        keys.push_back({num(field(source, "time")),
                        readVec3(field(source, "value"), "translation"), {}, {}});
    }

    // Genesis 100158D0: nonloop endpoint tangents are the adjacent value
    // differences. Interior tangents split the previous-to-next difference
    // according to adjacent key-time spans. The key record stores incoming
    // and outgoing XYZ tangent triples separately.
    for (size_t i = 0; i < keys.size(); ++i) {
        if (keys.size() < 2) break;
        if (i == 0) {
            keys[i].incoming = keys[i + 1].value - keys[i].value;
            keys[i].outgoing = keys[i].incoming;
        } else if (i + 1 == keys.size()) {
            keys[i].incoming = keys[i].value - keys[i - 1].value;
            keys[i].outgoing = keys[i].incoming;
        } else {
            const float before = keys[i].time - keys[i - 1].time;
            const float after = keys[i + 1].time - keys[i].time;
            const float span = before + after;
            if (!(before > 0.f) || !(after > 0.f) || !(span > 0.f))
                throw std::runtime_error("Retail GBSP motion: invalid times for translation tangent precompute");
            const V difference = keys[i + 1].value - keys[i - 1].value;
            keys[i].incoming = difference * (before / span);
            keys[i].outgoing = difference * (after / span);
        }
    }
    return keys;
}

inline std::vector<KeyQ> readRotation(J track, bool looped) {
    std::vector<KeyQ> keys;
    if (!json_object_get_boolean(field(track, "present"))) return keys;
    if (integer(field(track, "key_mode")) != 1 ||
        integer(field(track, "sampler_index")) != 4 ||
        str(field(track, "encoding")) != "quaternion_wxyz")
        throw std::runtime_error("Retail GBSP motion: rotation mode is outside the ported mode-1 path");
    if (looped)
        throw std::runtime_error("Retail GBSP motion: looped rotation time is not ported yet");

    J sourceKeys = field(track, "keys");
    keys.reserve(size_t(count(sourceKeys)));
    for (int i = 0; i < count(sourceKeys); ++i) {
        J source = at(sourceKeys, i);
        keys.push_back({num(field(source, "time")), readQuaternion(field(source, "value"))});
    }

    // Genesis 10005150/10004F90 choose the equivalent quaternion sign across
    // each adjacent pair when their orientation dot product is negative.
    for (size_t i = 1; i < keys.size(); ++i) {
        const Q& previous = keys[i - 1].value;
        Q& current = keys[i].value;
        const float orientationDot = previous.w * current.w + previous.x * current.x +
                                     previous.y * current.y + previous.z * current.z;
        if (orientationDot < 0.f)
            current = {-current.w, -current.x, -current.y, -current.z};
    }
    return keys;
}

inline Q slerpRetail(Q first, Q second, float t) {
    // Genesis geQuaternion_Slerp at 1002BB60. Its 1e-5 threshold and pi/2
    // constant come from the retail binary; mode-1 keys are preprocessed for
    // hemisphere continuity before this routine is reached.
    constexpr float epsilon = 1.0e-5f;
    constexpr double halfPiRetail = 1.57079637050628662109375;
    const float dotValue = first.w * second.w + first.x * second.x +
                           first.y * second.y + first.z * second.z;
    float scaleFirst = 1.f - t;
    float scaleSecond = t;

    if (double(dotValue) + 1.0 <= double(epsilon)) {
        const Q perpendicular{first.z, -first.y, first.x, -first.w};
        scaleFirst = float(std::sin(double(1.f - t) * halfPiRetail));
        scaleSecond = float(std::sin(double(t) * halfPiRetail));
        return {scaleFirst * first.w + scaleSecond * perpendicular.w,
                scaleFirst * first.x + scaleSecond * perpendicular.x,
                scaleFirst * first.y + scaleSecond * perpendicular.y,
                scaleFirst * first.z + scaleSecond * perpendicular.z};
    }

    if (1.0 - double(dotValue) > double(epsilon)) {
        const float angle = std::acos(dotValue);
        const float sine = std::sin(angle);
        if (sine >= epsilon) {
            scaleFirst = std::sin((1.f - t) * angle) / sine;
            scaleSecond = std::sin(t * angle) / sine;
        }
    }

    return {scaleFirst * first.w + scaleSecond * second.w,
            scaleFirst * first.x + scaleSecond * second.x,
            scaleFirst * first.y + scaleSecond * second.y,
            scaleFirst * first.z + scaleSecond * second.z};
}

inline V sampleTranslation(const std::vector<Key3>& keys, float time) {
    if (keys.empty()) return {};
    const Bracket segment = bracket(keys, time);
    if (segment.first == segment.second) return keys[segment.first].value;

    const float t = segment.fraction;
    const float t2 = t * t;
    const float t3 = t2 * t;
    const float h01 = 3.f * t2 - 2.f * t3;
    const float h00 = 1.f - h01;
    const float h10 = t - 2.f * t2 + t3;
    const float h11 = t3 - t2;
    return keys[segment.first].value * h00 + keys[segment.second].value * h01 +
           keys[segment.first].outgoing * h10 + keys[segment.second].incoming * h11;
}

inline Q sampleRotation(const std::vector<KeyQ>& keys, float time) {
    if (keys.empty()) return {};
    const Bracket segment = bracket(keys, time);
    if (segment.first == segment.second) return keys[segment.first].value;
    return slerpRetail(keys[segment.first].value, keys[segment.second].value, segment.fraction);
}

inline PreparedPath preparePath(J path) {
    if (json_object_get_boolean(field(path, "looped")))
        throw std::runtime_error("Retail GBSP motion: looped PATH time normalization is not ported yet");
    const bool looped = false;
    PreparedPath result;
    result.translation = readTranslation(field(path, "translation"), looped);
    result.rotation = readRotation(field(path, "rotation"), looped);
    return result;
}

inline Pose PreparedPath::sample(float time) const {
    Pose result;
    result.translation = sampleTranslation(this->translation, time);
    result.rotation = sampleRotation(this->rotation, time);
    return result;
}

} // namespace RetailGbspMotion
