#include <array>
#include <cassert>
#include "../src/hose_spray_pose_list.h"

int main() {
    using namespace HoseSprayPoseList;
    State state;
    construct(state);
    assert(state.count == 12);
    assert(state.head == 0 && state.tail == 11 && state.current == 0);
    assert((state.posePosition == std::array<float, 3>{0.0f, 0.0f, 0.0f}));
    assert((state.auxiliaryVector24 == std::array<float, 3>{0.0f, -5000.0f, 0.0f}));
    assert((state.nodes.front().position == std::array<float, 3>{0.0f, 0.0f, 0.0f}));
    assert((state.nodes.front().vector14 == std::array<float, 3>{0.0f, 0.0f, 0.0f}));
    assert((state.nodes.front().vector20 == std::array<float, 3>{0.0f, 0.0f, 0.0f}));
    for (int i = 0; i < state.count; ++i) {
        assert(state.nodes[static_cast<std::size_t>(i)].previous == (i == 0 ? -1 : i - 1));
        assert(state.nodes[static_cast<std::size_t>(i)].next == (i + 1 == state.count ? -1 : i + 1));
    }

    state.current = 7;
    state.nodes[7].position = {7.0f, 8.0f, 9.0f};
    assert(!resize(state, 12)); // Same count is a no-op, not a rebuild.
    assert(state.current == 7);
    assert((state.nodes[7].position == std::array<float, 3>{7.0f, 8.0f, 9.0f}));

    assert(trainingInitializerCount(25) == 30);
    assert(initializeTraining(state, 25));
    assert(state.count == 30 && state.head == 0 && state.tail == 29 && state.current == 0);
    assert((state.nodes.front().position == std::array<float, 3>{0.0f, 0.0f, 0.0f}));
    assert((state.nodes[7].position == std::array<float, 3>{0.0f, 0.0f, 0.0f}));

    const std::array<float, 3> valvePosition{1368.0f, -176.0f, -2080.0f};
    attach(state, valvePosition);
    assert((state.posePosition == valvePosition));
    assert((state.nodes.front().position == valvePosition));
    assert(state.current == state.head);
    assert((state.nodes[1].position == std::array<float, 3>{0.0f, 0.0f, 0.0f}));
    assert((state.auxiliaryVector24 == std::array<float, 3>{0.0f, -5000.0f, 0.0f}));
    assert(!resize(state, 30)); // The original preserves the attached list at equal size.
    assert((state.nodes.front().position == valvePosition));

    assert(resize(state, 0));
    assert(state.count == 0 && state.head == -1 && state.tail == -1 && state.current == -1);
}
