#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
#include "extended.h"
#include "player_ash_pool.h"

// A source-layout transform node used by Genesis routines 004977B0/00497300.
// Parent indices are an explicit host representation of the +0x40 pointer.
struct PlayerAshTransformNode {
    PlayerAshTransform value{};
    int parent = -1;
};

inline float playerAshFloatProduct(float left, float right) {
    return (Extended::from(left) * Extended::from(right)).asFloat();
}

inline float playerAshTransformDot3(float a0, float b0,
                                    float a1, float b1,
                                    float a2, float b2) {
    // 00497300 evaluates the first two products and adds them, then adds the
    // third product; the x87 value is rounded when stored to the output field.
    return ((Extended::from(a0) * Extended::from(b0)) +
            (Extended::from(a1) * Extended::from(b1)) +
            (Extended::from(a2) * Extended::from(b2))).asFloat();
}

inline PlayerAshTransform composePlayerAshTransform(
        const PlayerAshTransform& parent, const PlayerAshTransform& child) {
    PlayerAshTransform out;
    for (size_t childAxis = 0; childAxis < 3; ++childAxis) {
        for (size_t outputAxis = 0; outputAxis < 3; ++outputAxis) {
            out.axis[childAxis][outputAxis] = playerAshTransformDot3(
                parent.axis[0][outputAxis], child.axis[childAxis][0],
                parent.axis[1][outputAxis], child.axis[childAxis][1],
                parent.axis[2][outputAxis], child.axis[childAxis][2]);
        }
    }

    std::array<float, 3> scaledChildPosition{};
    for (size_t axis = 0; axis < 3; ++axis) {
        scaledChildPosition[axis] = playerAshFloatProduct(
            child.position[axis], parent.scale[axis]);
        out.scale[axis] = playerAshFloatProduct(child.scale[axis], parent.scale[axis]);
    }

    for (size_t outputAxis = 0; outputAxis < 3; ++outputAxis) {
        const Extended secondThird =
            Extended::from(parent.axis[1][outputAxis]) * Extended::from(scaledChildPosition[1]) +
            Extended::from(parent.axis[2][outputAxis]) * Extended::from(scaledChildPosition[2]);
        const float linear = (
            Extended::from(parent.axis[0][outputAxis]) * Extended::from(scaledChildPosition[0]) +
            secondThird).asFloat();
        out.position[outputAxis] = (Extended::from(linear) +
                                    Extended::from(parent.position[outputAxis])).asFloat();
    }
    return out;
}

inline PlayerAshTransform resolvePlayerAshTransform(
        const std::vector<PlayerAshTransformNode>& nodes, size_t index) {
    if (index >= nodes.size())
        throw std::out_of_range("ash transform node index");
    std::vector<bool> visiting(nodes.size(), false);
    visiting[index] = true;
    PlayerAshTransform result = nodes[index].value;
    int parent = nodes[index].parent;
    while (parent >= 0) {
        const size_t parentIndex = static_cast<size_t>(parent);
        if (parentIndex >= nodes.size())
            throw std::out_of_range("ash transform parent index");
        if (visiting[parentIndex])
            throw std::invalid_argument("cyclic ash transform parent chain");
        visiting[parentIndex] = true;
        // 004977B0 composes the immediate parent with the child first, then
        // recursively applies the grandparent to that rounded result.
        result = composePlayerAshTransform(nodes[parentIndex].value, result);
        parent = nodes[parentIndex].parent;
    }
    return result;
}
