#pragma once
#include <cstdint>
#include <functional>
#include "extended.h"
#include "random.h"

// 00451640 pain branch, after the exposure threshold check. The RC_Pain host
// service runs first and may consume the same shared RNG stream (004897C0
// resolves a TChatterSet and 00488D40 selects an entry with Random::soundIndex).
// Actual media playback remains a host service. The deadline's
// own 00498B30(1000, 2500) call then consumes exactly one additional draw.
// Raw x87 code adds the unsigned millisecond clock before 004B9400 truncates
// toward zero and stores the low dword at Player+0x390. It then adds 1 to the
// Serious Burns statistic through 00486980. Both host services stay injected.
inline bool updatePlayerPainDeadline(
    uint32_t& deadline, uint32_t now, Random& random,
    const std::function<void(Random&)>& triggerPainService,
    const std::function<void()>& addSeriousBurns) {
 if(now<=deadline)return false; // source uses unsigned JBE to skip
 triggerPainService(random);
 const Extended unit=Extended::from(random.unit());
 const Extended delay=(Extended::from(2500.0f)-Extended::from(1000.0f))*unit
                     +Extended::from(1000.0f);
 deadline=uint32_t((Extended::integer(now)+delay).truncate());
 addSeriousBurns();
 return true;
}
