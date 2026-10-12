#pragma once

#include <cstdint>
#include <limits>
#include <string>

struct RescueExitState;

// Local action 1/2 of 004571A0. Counters stay in the shared RescueExitState;
// these are only the two timestamp words owned by the status object.
struct TrainingVictimStatusState {
    RescueExitState& rescue;
    uint32_t victimPickTimestamp = 0; // status this+0xA88
    uint32_t victimDropTimestamp = 0; // status this+0x2B38
};

enum class TrainingVictimStatusResult { ignoredAction, handled, noPositiveRemainder };

// Action 1 and 2 callbacks intentionally expose native helper boundaries.
// Action 2 count callback returns the source type-2 list count; statistic and
// membership callbacks represent the subsequent 00486920 and 00484260 calls.
template<class Ops>
TrainingVictimStatusResult trainingVictimStatusAction(
    TrainingVictimStatusState& state, int action, Ops&& ops) {
    auto& rescue = state.rescue;
    if (action == 1) {
        ops.setVictPickStatusLayer(true);                 // 0047DC90(+0x964, true)
        state.victimPickTimestamp = ops.readClock();      // status +0xA88
        const auto sample = ops.sampleVictPickList(0.0f); // 00484940(+0xA8C, 0)
        ops.updateVictPickStatus(sample);                 // 0047EDF0(+0x964, sample, 0)
        ops.setStatusKey("RC_VictPick", true);            // 004897C0
        return TrainingVictimStatusResult::handled;
    }
    if (action != 2) return TrainingVictimStatusResult::ignoredAction;

    ops.hideVictDropDetail();                             // 00455F10(+0x24C)
    ops.setVictDropStatusLayer(false);                    // 0047DC90(+0x964, false)
    state.victimDropTimestamp = ops.readClock();          // status +0x2B38
    ops.prepareVictDropMessage();                         // 004054A0, boundary opaque

    if (rescue.total == std::numeric_limits<uint32_t>::max()) {
        rescue.total = 0; // 0044FBA0 stores zero before enumerating.
        rescue.total = ops.countType2Entities();           // 0044FBA0 count loop
        ops.setRescueStatisticTotal(rescue.total);         // 00486920
        ops.refreshMembership();                           // 00484260
    }
    // Source subtracts unsigned words and branches on signed-positive result.
    const uint32_t remaining = rescue.total - rescue.field3530 - rescue.rescues;
    if (remaining == 0 || (remaining & 0x80000000u) != 0)
        return TrainingVictimStatusResult::noPositiveRemainder;

    if (remaining == 1) {
        ops.showVictDropMessageOne("There's still one survivor remaining!", 5.0f);
    } else {
        ops.showVictDropMessageMany(
            "Good job, but there are still %i more survivors!", 5.0f, remaining);
    }
    ops.setStatusKey("RC_VictDrop", true);                // 004897C0
    return TrainingVictimStatusResult::handled;
}
