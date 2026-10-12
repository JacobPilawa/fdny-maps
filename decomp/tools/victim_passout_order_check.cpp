#include "../src/victim_passout.h"

#include <cassert>
#include <string>
#include <vector>

struct RecordingOps {
    std::vector<std::string> calls;
    uint32_t selectedMotion = 99;
    float selectedSpeed = 0;
    VictimPassoutPlayerState* player = nullptr;
    VictimPassoutActorState* victim = nullptr;
    int correctionReads = 0;
    float frameTranslationX = 100;
    bool inspectVerticalBoundary = false;

    void setMotion(VictimPassoutActorState&, uint32_t draw, float speed) {
        selectedMotion = draw; selectedSpeed = speed; calls.push_back("motion");
    }
    void copyFirstPlayerFrameAxis(VictimPassoutActorState&, VictimPassoutPlayerState&) { calls.push_back("axis-a"); }
    void copySecondPlayerFrameAxis(VictimPassoutActorState&, VictimPassoutPlayerState&) { calls.push_back("axis-b"); }
    void rebuildVictimFrameBasis(VictimPassoutActorState&) { calls.push_back("basis"); }
    void copyPlayerFramePosition(VictimPassoutActorState&, VictimPassoutPlayerState&) { calls.push_back("position"); }
    void placeOnFloor(VictimPassoutActorState&) { calls.push_back("floor"); }
    void setWorldActorFlags(VictimPassoutActorState& actor, bool enabled) {
        assert(enabled && actor.worldActorFlags == 0x400); calls.push_back("flags-true");
    }
    void dispatchPassout(VictimPassoutActorState&) { calls.push_back("passout-event"); }
    void writePlayerPosition(const std::array<float,3>& value) {
        assert(player && player->position==value); calls.push_back("player-position-copy");
    }
    void resetPlayerRescue() { calls.push_back("player-reset-0044fce0-0"); }
    void writePlayerPositionX(float value) {
        assert(player && value==player->position[0]); calls.push_back("player-x-write");
    }
    void correctDroppedFrameX(float value){frameTranslationX-=value;calls.push_back("frame-correction");}
    float readPassoutCorrection() {
        if (correctionReads++ == 0) {
            if (inspectVerticalBoundary)
                assert(player->position[0] == 20 && frameTranslationX == 100);
            calls.push_back("player-correction-read");
        } else {
            if (inspectVerticalBoundary)
                assert(player->position[0] == 50 && frameTranslationX == 100);
            calls.push_back("victim-correction-read");
        }
        return 30.0f;
    }
};

int main() {
    const std::vector<std::string> expected = {
        "player-position-copy", "motion", "axis-a", "axis-b", "basis", "position", "floor",
        "flags-true", "passout-event", "player-reset-0044fce0-0",
        "player-correction-read", "player-x-write", "victim-correction-read", "frame-correction"
    };

    VictimPassoutPlayerState player;
    player.carriedVictimEntity = 17;
    player.position = {1, 2, 3};
    player.passoutPosition = {20, 5, 6};
    VictimPassoutActorState victim;
    victim.entity = 17;
    victim.motionCount = 6;
    Random random(1234), expectedRandom(1234);
    const uint32_t expectedMotion = expectedRandom.bounded(3);
    RecordingOps ops; ops.player = &player; ops.victim = &victim; ops.inspectVerticalBoundary = true;
    assert(applyVictimPassout(player, victim, random, ops) == VictimPassoutResult::dropped);
    assert(ops.calls == expected);
    assert(ops.selectedMotion == expectedMotion && ops.selectedSpeed == 1.0f);
    assert(random.drawCount == 1 && victim.state == 4 && victim.worldActorFlags == 0x400);
    assert(player.position[0] == 50 && player.position[1] == 5 && player.position[2] == 6);
    assert(ops.frameTranslationX == 70 && !player.carriedVictimEntity);

    // Non-six motion counts use that count as the bound and animation speed .2.
    VictimPassoutPlayerState otherPlayer; otherPlayer.carriedVictimEntity = 22;
    VictimPassoutActorState otherVictim; otherVictim.entity = 22; otherVictim.motionCount = 5;
    Random otherRandom(55), expectedOther(55); const uint32_t expectedOtherMotion = expectedOther.bounded(5);
    RecordingOps otherOps; otherOps.player = &otherPlayer; otherOps.victim = &otherVictim;
    assert(applyVictimPassout(otherPlayer, otherVictim, otherRandom, otherOps) == VictimPassoutResult::dropped);
    assert(otherOps.selectedMotion == expectedOtherMotion);
    assert(otherOps.selectedSpeed == 0.2f && otherRandom.drawCount == 1);
    assert(otherVictim.state == 4);

    // The helper's preamble still runs without a carried victim; no drop RNG or callbacks run.
    VictimPassoutPlayerState empty; empty.position = {0, 0, 0}; empty.passoutPosition = {7, 8, 9};
    VictimPassoutActorState untouched;
    Random emptyRandom(1); RecordingOps emptyOps; emptyOps.player = &empty;
    assert(applyVictimPassout(empty, untouched, emptyRandom, emptyOps) == VictimPassoutResult::noCarriedVictim);
    assert(empty.passoutCount == 1 && empty.position[0] == 7);
    assert(emptyRandom.drawCount == 0 && emptyOps.calls==std::vector<std::string>{"player-position-copy"});
}
