#pragma once
#include <cstdint>
#include <functional>
#include "extended.h"

struct PlayerFireExposureResult {
 Extended extendedSum{};
 float storedExposure=0;
 bool passoutTriggered=false;
};

// 00451640, instructions 00451820..00451850. Keep each x87 operation in the
// source order, store a binary32 copy to Player+0x3538, then compare the still
// live extended ST(0) against binary64 1.0. The callback is the explicit host
// boundary for 00450FB0(player, 1); actor/message scheduling stays disconnected.
inline PlayerFireExposureResult accumulatePlayerFireExposure(
    float& exposure, float frameDelta, float influence,
    const std::function<void()>& triggerPassout) {
 using E=Extended;
 E sum=E::from(1.2f)*E::from(frameDelta);
 sum=sum*E::from(influence);
 sum=sum+E::from(exposure);
 exposure=sum.asFloat(); // FST float at 00451839; ST(0) is not popped

 const E one=E::from(1.0f);
 const int sumOrder=sum.e+E::width(E::U(sum.m));
 const int oneOrder=one.e+64;
 const bool aboveOne=sum.m!=0 && !sum.negative &&
     (sumOrder>oneOrder || (sumOrder==oneOrder && sum.m>one.m));
 if(aboveOne)triggerPassout();
 return {sum,exposure,aboveOne};
}
