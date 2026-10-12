#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>
#include "victim_passout.h"
#include "victim_exit.h"

struct PlayerHeroDownEntity {
    int type = 0;
    std::string scriptName;
    int state = 0;
};

// Raw values written to the source's 0x34-byte staged spawn descriptor. Their
// semantics are intentionally not assigned here.
struct PlayerHeroDownSpawn {
    std::array<float,3> position{}; // copied from Player +0x4c..+0x54
    uint32_t word0 = 1000;
    const char* argumentFormat = "%i"; // DAT_004DD8E4 bytes are 25 69 00 00
    uint32_t countMinusOneBits = 0; // vararg bits pushed to 00402770
    int32_t countMinusOne = 0; // same bits interpreted by the %i format
    const char* fighterName = "POFighter";
    std::string formattedSuffix;
    std::string generatedName; // FUN_004040E0 appends suffix to fighterName
    const char* scriptName = "herodown";
    uint32_t word6 = 0;
    float word7 = 20.0f; // source bits 0x41A00000
    const char* entityClass = "RCS_VictimRescue";
    uint32_t classLookupArgument = 100;
};

struct PlayerHeroDownTailResult {
    bool countBranchEntered = false;
    bool spawned = false;
    int firefightersDown = -1; // -1 means the mode/argument gate skipped it.
};

inline bool playerHeroDownNameEqual(const std::string& value);

// Continuation of 0044FF10 after applyVictimPassout has returned. It does not
// repeat the passout preamble/transaction and does not schedule or draw RNG.
// Ops preserve boundaries around engine spawn/selection/status helpers:
//   spawnHeroDown(spec), setRescueStatisticTotal(value), logFirefightersDown(n),
//   reportCleanupArrayBounds(), cleanupSubobjectMode(),
//   transitionCleanupSubobjectTo2(), refreshSelection(), resetPassoutPlayerView(),
//   finishPlayerRescueStatus(). The last two correspond to 0044CD10 and
//   00456F70; status/clock internals remain opaque.
template<class Ops>
PlayerHeroDownTailResult continuePlayerHeroDownTail(
    VictimPassoutPlayerState& player, RescueExitState& rescue,
    int gameMode, char helperArgument,
    const std::vector<PlayerHeroDownEntity>& entities,
    int cleanupState, int cleanupCollectionCount, Ops&& ops) {
    PlayerHeroDownTailResult result;
    if (gameMode == 2 && helperArgument == 0) {
        result.countBranchEntered = true;
        int count = 0;
        for (const auto& entity : entities) {
            if (entity.type == 2 &&
                playerHeroDownNameEqual(entity.scriptName) &&
                (entity.state == 4 || entity.state == 7))
                ++count;
        }
        if (count < 3) {
            PlayerHeroDownSpawn spawn;
            spawn.position = player.position;
            spawn.countMinusOneBits = player.passoutCount - uint32_t(1);
            const uint32_t bits = spawn.countMinusOneBits;
            spawn.countMinusOne = bits <= uint32_t(std::numeric_limits<int32_t>::max())
                ? static_cast<int32_t>(bits)
                : -1 - static_cast<int32_t>(~bits);
            spawn.formattedSuffix = std::to_string(spawn.countMinusOne);
            spawn.generatedName = std::string(spawn.fighterName) + spawn.formattedSuffix;
            ops.spawnHeroDown(spawn); // descriptor construction + staged add boundary
            ++rescue.total;           // Player +0x3528, after 0049D4C0
            ops.setRescueStatisticTotal(rescue.total);
            ++count;
            result.spawned = true;
        }
        result.firefightersDown = count;
        ops.logFirefightersDown(count); // "Firefighters down = " source log
    }

    if (cleanupState != 0) {
        if (cleanupState == 4) {
            if (cleanupCollectionCount < 5)
                ops.reportCleanupArrayBounds(); // 00422060 diagnostic boundary; continuation retained
            if (ops.cleanupSubobjectMode() == 3)
                ops.transitionCleanupSubobjectTo2(); // 00463370(entity, 2)
        }
        ops.refreshSelection(); // 0044C360
    }
    ops.resetPassoutPlayerView(); // 0044CD10
    ops.finishPlayerRescueStatus(); // 00456F70; samples source global time internally
    return result;
}

inline bool playerHeroDownNameEqual(const std::string& value) {
    static constexpr char expected[] = "herodown";
    if (value.size() != sizeof(expected)-1) return false;
    for (size_t i=0;i<value.size();++i) {
        unsigned char c=static_cast<unsigned char>(value[i]);
        if (c>='A'&&c<='Z') c=static_cast<unsigned char>(c-'A'+'a');
        if (c!=static_cast<unsigned char>(expected[i])) return false;
    }
    return true;
}
