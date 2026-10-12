#include "../src/victim_release.h"
#include "../src/actor_render_flags.h"

#include <cassert>
#include <string>
#include <vector>

struct RecordingOps {
    std::vector<std::string> calls;
    uint32_t now = 100;
    void recordRescueStatistics(TrainingVictimReleaseState&) { calls.push_back("stats"); }
    void copyFirstPlayerFrameAxis(TrainingVictimReleaseActor&, TrainingVictimReleaseState&) { calls.push_back("axis-a"); }
    void copySecondPlayerFrameAxis(TrainingVictimReleaseActor&, TrainingVictimReleaseState&) { calls.push_back("axis-b"); }
    void setVictimFrameUp(TrainingVictimReleaseActor&, float x, float y, float z) {
        assert(x == 0 && y == 1 && z == 0); calls.push_back("up");
    }
    void releaseOffsetTimes5(TrainingVictimReleaseActor&) { calls.push_back("scale-5"); }
    void releaseOffsetTimes24(TrainingVictimReleaseActor&) { calls.push_back("scale-24"); }
    void setReleasePositionFromPlayerFrame(TrainingVictimReleaseActor&, TrainingVictimReleaseState&) { calls.push_back("position"); }
    void placeOnFloor(TrainingVictimReleaseActor&) { calls.push_back("floor"); }
    void setWorldActorFlags(TrainingVictimReleaseActor& actor, uint32_t flags) {
        assert(flags == 0x400 && actor.worldActorFlags == flags); calls.push_back("flags");
    }
    void dispatchRescue(TrainingVictimReleaseActor&) {
        calls.push_back("rescue-event");
        now = 0xFFFFFF00u; // synchronous event advances the observed clock
    }
    uint32_t readClock() {
        calls.push_back("clock");
        return now;
    }
    void resetActorSubobject(TrainingVictimReleaseActor& actor) {
        assert(actor.state == 6 && actor.rescueTimer == 150 && actor.rescueActive == 1);
        calls.push_back("actor-reset");
    }
    void updateVictPickPath(TrainingVictimReleaseState&) { calls.push_back("path"); }
    void playerRescuePostEffect(TrainingVictimReleaseState&) { calls.push_back("player-post"); }
    void resetPlayerRescue() { calls.push_back("player-reset-0044fce0-0"); }
    void checkExits(TrainingVictimReleaseState&) { calls.push_back("exits"); }
};

int main() {
    const std::vector<std::string> expected = {
        "stats", "axis-a", "axis-b", "up", "scale-5", "scale-24",
        "position", "floor", "flags", "rescue-event", "clock", "actor-reset",
        "path", "player-post", "player-reset-0044fce0-0", "exits"
    };
    TrainingVictimReleaseState player{42};
    TrainingVictimReleaseActor victim;
    victim.entity = 42;
    RecordingOps ops;
    assert(ActorRenderFlags::eligibleForNormalWorldRender(victim.worldActorFlags));
    auto pickup=deliverVictimPickupMessage(0x7532,true,player=TrainingVictimReleaseState{},victim,
        [](int,uint32_t){},[](int){},[]{});
    assert(pickup==VictimPickupMessageResult::accepted);
    assert(victim.state==5 && !ActorRenderFlags::eligibleForNormalWorldRender(victim.worldActorFlags));
    assert(trainingEscapeRelease(player, victim, ops));
    assert(ops.calls == expected);
    assert(victim.state == 6 && victim.worldActorFlags == 0x400);
    assert(ActorRenderFlags::eligibleForNormalWorldRender(victim.worldActorFlags));
    assert(victim.rescueTimer == 150 && victim.rescueStart == 0x000006D0u);
    assert(victim.rescueEnd == 0x00000766u && victim.rescueActive == 1);
    assert(!player.carriedVictimEntity);

    TrainingVictimReleaseState empty{};
    TrainingVictimReleaseActor untouched;
    RecordingOps noCalls;
    assert(!trainingEscapeRelease(empty, untouched, noCalls));
    assert(noCalls.calls.empty() && untouched.state == 4 && untouched.worldActorFlags == 0x400);
}
