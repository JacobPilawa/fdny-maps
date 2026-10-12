#pragma once

#include <cstdint>
#include <functional>

// Source-derived scalar/field effects from FDNYFirefighter.exe:
//   0043DB40: BModel use candidate/action gate
//   0044C520: Player's lift action eligibility
//   0043D2B0 -> 0044BC40: mode-1 held-object/state transition
//
// This deliberately models only fields and branches established by the
// Ghidra audit. Candidate construction, axis-box query, event delivery, model
// transforms and update scheduling are host responsibilities and unverified.
namespace BModelUse {

constexpr int kLiftAction = 0x3ec; // 1004, literal returned by 0044C520.
constexpr int kOtherNonzeroModeAction = 1000; // Raw source result; semantics unknown.

struct Candidate {
 bool hasQueryInterface = false;
 int action = 0;
};

// 0044C520 checks Player item-array slot 4 (the Hose Spray slot): an absent
// hose or hose mode other than 3 passes this gate. The separate Player byte
// +0x12C remains unnamed and is kept as an explicit input.
inline Candidate candidate(int useMode, bool hoseSlot4Present, int hoseMode,
                           std::uint8_t playerByte12C,
                           bool heldObjectIsNull, int bmodelObjectState) {
 if (useMode == 0) return {};
 if (useMode != 1) return {true, kOtherNonzeroModeAction};
 const bool hoseGate = !hoseSlot4Present || hoseMode != 3;
 const bool eligible = hoseGate && playerByte12C != 0 &&
                       heldObjectIsNull && bmodelObjectState == 0;
 return {true, eligible ? kLiftAction : 0};
}

struct LiftTransition {
 void* playerHeldObject = nullptr;
 int bmodelObjectState = 0;
 int playerState = 0;
};

using ActionSubobjectCall = std::function<void(void* receiver,
                                                std::uintptr_t firstArgument,
                                                const void* secondArgument)>;

// Models the observed order in 0044BC40. The 004571A0 callback is an explicit
// boundary because its service effects are not part of this component.
// Returns false for other modes. ScriptActor `use` forwarding follows this
// setter in 0043D2B0 and is intentionally outside this function.
inline bool enterLiftMode(int useMode, void* bmodelRoot,
                          void* playerActionSubobject,
                          LiftTransition& state,
                          const ActionSubobjectCall& actionCall) {
 if (useMode != 1) return false;
 state.playerHeldObject = bmodelRoot; // Player root +0x3440
 state.bmodelObjectState = 1;         // BModel root +0x25C
 actionCall(playerActionSubobject, 3, nullptr); // 004571A0(Player+0x5F4, 3, 0)
 state.playerState = 7;               // Player root +0x18C
 return true;
}

} // namespace BModelUse
