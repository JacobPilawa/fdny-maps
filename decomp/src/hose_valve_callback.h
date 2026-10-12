#pragma once

// Source-derived callback branch in FDNYFirefighter.exe!00444120.
// The owner-side 0044C840 implementation and its frame/UI services remain a
// separate component; this models only the callback's call/return ordering.
namespace HoseValveCallback {

struct Outcome {
 bool callOwnerAction = false;
 bool forwardUseEvent = true;
};

inline Outcome invoke(int propModelState, int action) {
 const bool valveActive = propModelState == 1;
 const bool detach = action == 1002;
 return {valveActive, !(valveActive && detach)};
}

} // namespace HoseValveCallback
