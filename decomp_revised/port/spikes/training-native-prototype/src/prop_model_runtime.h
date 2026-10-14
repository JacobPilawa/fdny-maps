#pragma once

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "core.h"
#include "scene_data.h"
#include "actor_pose.h"

struct PropModelInstance {
    int entity = -1;
    int resourceModelId = -1;
    std::string name;
    V position{};
    float authoredHeading = 0.f;
    float authoredPitch = 0.f;
    float touchRadius = 0.f;
    V scale{1.f, 1.f, 1.f};
    int worldLeafIndex = -1; // static authored point; cached after scene load
    // Retail model creation initializes its +0x45 touch/render gate to true.
    // Asset availability is tracked by the renderer map, not by this state.
    bool modelEnabled = true;
    bool visible = true;
    bool makeValve = false;
};

inline ActorTransform propModelActorRoot(const PropModelInstance& prop) {
    constexpr double pi = 3.14159265358979323846;
    // PropModel setup 00443860 converts heading with
    // -heading * (1/12) * 360 - 90 and pitch with pitch * -90, then calls
    // 004968B0 followed by 00496B50. On the identity child frame these are
    // equivalent to local-Z heading and local-X pitch rotations.
    const float heading = (-prop.authoredHeading * 30.f - 90.f) * float(pi / 180.0);
    const float pitch = (-prop.authoredPitch * 90.f) * float(pi / 180.0);
    const float ch = std::cos(heading), sh = std::sin(heading);
    const float cp = std::cos(pitch), sp = std::sin(pitch);
    ActorTransform localHeading;
    localHeading.r[0] = ch;  localHeading.r[1] = -sh;
    localHeading.r[3] = sh;  localHeading.r[4] = ch;
    ActorTransform localPitch;
    localPitch.r[4] = cp;   localPitch.r[5] = -sp;
    localPitch.r[7] = sp;   localPitch.r[8] = cp;

    ActorTransform parent;
    parent.t = prop.position;
    ActorTransform result = actorMultiply(actorMultiply(parent, localHeading), localPitch);
    // 0042D280 stores independent X/Y/Z scale values in the child frame.
    for (int row = 0; row < 3; ++row) {
        result.r[row * 3] *= prop.scale.x;
        result.r[row * 3 + 1] *= prop.scale.y;
        result.r[row * 3 + 2] *= prop.scale.z;
    }
    return result;
}

inline bool propModelTouchCandidate(const PropModelInstance& prop, V candidatePoint) {
    if (!prop.modelEnabled || prop.touchRadius < 0.f) return false;
    const V offset = candidatePoint - prop.position;
    return dot(offset, offset) <= prop.touchRadius * prop.touchRadius;
}

// Genesis geWorld_LeafMightSeeLeaf first checks the source cluster's VISDATA
// bit for the target cluster. This ports that lower-level, data-backed gate.
// The higher-level geWorld_MightSeeLeaf also consults the runtime area matrix;
// that dynamic portal state is not exported here, so a positive PVS result is
// only a conservative candidate (and malformed/missing data remains visible).
class PropModelPvs {
public:
    PropModelPvs(json_object* collision, std::vector<uint8_t> clusterOffsets,
                 std::vector<uint8_t> visdata)
        : collision_(collision), clusterOffsets_(std::move(clusterOffsets)),
          visdata_(std::move(visdata)) {
        valid_ = collision_ && clusterOffsets_.size() >= 4 &&
                 clusterOffsets_.size() % 4 == 0 && !visdata_.empty() &&
                 json_object_get_type(SceneData::field(collision_, "models")) == json_type_array &&
                 json_object_get_type(SceneData::field(collision_, "nodes")) == json_type_array &&
                 json_object_get_type(SceneData::field(collision_, "leafs")) == json_type_array &&
                 json_object_get_type(SceneData::field(collision_, "planes")) == json_type_array;
    }

    // False is source-backed only when both points resolve to clustered world
    // leaves and the source cluster's authored visibility bit excludes the
    // target. True may still be rejected by retail's runtime area matrix.
    bool maySee(V sourcePoint, V targetPoint) const {
        return maySeeLeaves(worldLeaf(sourcePoint), worldLeaf(targetPoint));
    }

    int worldLeaf(V point) const {
        return valid_ ? worldLeafAt(point) : -1;
    }

    bool maySeeLeaves(int sourceLeaf, int targetLeaf) const {
        if (!valid_) return true;
        if (sourceLeaf < 0 || targetLeaf < 0) return true;

        auto* leaves = SceneData::field(collision_, "leafs");
        const int sourceCluster = integer(SceneData::field(
            json_object_array_get_idx(leaves, size_t(sourceLeaf)), "cluster"));
        const int targetCluster = integer(SceneData::field(
            json_object_array_get_idx(leaves, size_t(targetLeaf)), "cluster"));
        if (sourceCluster < 0 || targetCluster < 0) return true;

        const size_t clusterCount = clusterOffsets_.size() / 4;
        if (size_t(sourceCluster) >= clusterCount || size_t(targetCluster) >= clusterCount)
            return true;
        const int32_t visOffset = littleI32(clusterOffsets_, size_t(sourceCluster) * 4);
        if (visOffset < 0) return true;
        const size_t byteOffset = size_t(visOffset) + size_t(targetCluster >> 3);
        if (byteOffset >= visdata_.size()) return true;
        return (visdata_[byteOffset] & uint8_t(1u << (targetCluster & 7))) != 0;
    }

private:
    json_object* collision_ = nullptr; // owned by the enclosing SceneData
    std::vector<uint8_t> clusterOffsets_, visdata_;
    bool valid_ = false;

    static int integer(json_object* value) {
        return value ? json_object_get_int(value) : 0;
    }

    static int32_t littleI32(const std::vector<uint8_t>& bytes, size_t offset) {
        const uint32_t value = uint32_t(bytes[offset]) |
            (uint32_t(bytes[offset + 1]) << 8) |
            (uint32_t(bytes[offset + 2]) << 16) |
            (uint32_t(bytes[offset + 3]) << 24);
        const int64_t signedValue = (value & 0x80000000u)
            ? int64_t(value) - 0x100000000LL : int64_t(value);
        return int32_t(signedValue);
    }

    int worldLeafAt(V point) const {
        auto* models = SceneData::field(collision_, "models");
        auto* world = json_object_array_get_idx(models, 0);
        if (!world) return -1;
        int nodeIndex = integer(SceneData::field(world, "root"));
        auto* nodes = SceneData::field(collision_, "nodes");
        auto* planes = SceneData::field(collision_, "planes");
        const size_t maxSteps = json_object_array_length(nodes) + 1;
        for (size_t step = 0; nodeIndex >= 0 && step < maxSteps; ++step) {
            if (size_t(nodeIndex) >= json_object_array_length(nodes)) return -1;
            auto* node = json_object_array_get_idx(nodes, size_t(nodeIndex));
            const int planeIndex = integer(SceneData::field(node, "plane"));
            if (planeIndex < 0 || size_t(planeIndex) >= json_object_array_length(planes)) return -1;
            auto* plane = json_object_array_get_idx(planes, size_t(planeIndex));
            auto* normal = SceneData::field(plane, "normal");
            const int type = integer(SceneData::field(plane, "type"));
            const double distance = type >= 0 && type < 3
                ? double(point[type])
                : double(json_object_get_double(json_object_array_get_idx(normal, 0))) * point.x +
                  double(json_object_get_double(json_object_array_get_idx(normal, 1))) * point.y +
                  double(json_object_get_double(json_object_array_get_idx(normal, 2))) * point.z;
            const int side = distance >= json_object_get_double(SceneData::field(plane, "distance"))
                ? 0 : 1;
            auto* children = SceneData::field(node, "children");
            nodeIndex = integer(json_object_array_get_idx(children, size_t(side)));
        }
        if (nodeIndex >= 0) return -1;
        const int leaf = -nodeIndex - 1;
        return leaf >= 0 && size_t(leaf) < json_object_array_length(SceneData::field(collision_, "leafs"))
            ? leaf : -1;
    }
};
