#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include "extended.h"
#include "player_ash_pool.h"

// Descriptor fields used by the three presentation helpers at 0047D420,
// 0047D6D0, and 0047D8A0. These names describe the consumed offsets rather
// than claiming a complete engine descriptor type.
struct AshPresentationFrame {
    float duration;                    // record +0x04
    float byteChannel;                  // record +0x08
    float alpha;                        // record +0x0c
    std::array<float,3> color;           // record +0x10..+0x18
};

struct AshPresentationInputs {
    uint32_t flags;                     // particle +0x18
    uint32_t currentIndex;              // particle +0x04
    uint32_t nextIndex;                 // particle +0x08
    uint32_t timestamp;                 // particle +0x14
    float animationRate;                // particle +0x1c
    float byteChannelScale;              // particle +0x20, explicit caller input
    uint32_t now;                       // global 00502AFC
};

inline AshPresentationInputs playerAshPresentationInputs(
    const PlayerAshParticle& p,uint32_t now) {
    return {p.animationFlags,p.animationIndex,p.nextAnimationIndex,
            p.animationTimestamp,p.activationScale,p.byteChannelScale,now};
}

inline float ashPresentationFloat(uint32_t word) {
    float value;std::memcpy(&value,&word,sizeof(value));return value;
}

inline std::array<AshPresentationFrame,PlayerAshPool::descriptorWords.size()>
playerAshTrainingPresentationFrames(size_t container) {
    std::array<AshPresentationFrame,PlayerAshPool::descriptorWords.size()> frames{};
    for(size_t i=0;i<frames.size();i++) {
        const auto d=PlayerAshPool::descriptorFor(container,i);
        frames[i]={ashPresentationFloat(d.scalarBits[0]),
                   ashPresentationFloat(d.scalarBits[1]),
                   ashPresentationFloat(d.scalarBits[2]),
                   {ashPresentationFloat(d.trailingBits[0]),
                    ashPresentationFloat(d.trailingBits[1]),
                    ashPresentationFloat(d.trailingBits[2])}};
    }
    return frames;
}

namespace player_ash_presentation_detail {
using E=Extended;

inline E timelineFraction(const AshPresentationInputs& p,
                          const AshPresentationFrame& current) {
    uint32_t wrapped=p.now-p.timestamp;
    int32_t signedElapsed;
    std::memcpy(&signedElapsed,&wrapped,sizeof(signedElapsed));
    E elapsed=E::from(double(signedElapsed));
    if (signedElapsed<0) elapsed=elapsed+E::from(4294967296.0f); // 004DFA30
    E denominator=E::from(current.duration)*E::from(p.animationRate)*
                  E::from(1000.0f); // 004E0ED4
    return elapsed/denominator;
}

inline E colorFraction(const AshPresentationInputs& p,
                       const AshPresentationFrame& current) {
    // Only 0047D420 applies the two binary64 duration guards at 004E0B68 and
    // 004E0B60 (+1e-28 and -1e-28); D6D0/D8A0 divide without this check.
    constexpr double epsilon=1.0e-28;
    if (double(current.duration)<epsilon && double(current.duration)>-epsilon)
        return E::from(0.0f);
    return timelineFraction(p,current);
}

inline uint8_t byteFromExtended(E value) {
    // 004B9400 adjusts the x87 rounded integer by the signed fractional
    // residue, implementing truncation toward zero; callers then keep AL.
    return static_cast<uint8_t>(value.truncate());
}
} // namespace player_ash_presentation_detail

// 0047D420: when bit 0x20 is clear it returns the current record's RGB
// triplet. Otherwise both weighted vectors are stored as binary32 by
// 00446390 before 0042D050 adds them and the result is cached at +0x24.
template<size_t N>
std::array<float,3> playerAshPresentationColor(
    const AshPresentationInputs& p,const std::array<AshPresentationFrame,N>& frames) {
    const auto& current=frames.at(p.currentIndex);
    const auto& next=frames.at(p.nextIndex);
    if ((p.flags&0x20)==0) return current.color;

    using E=Extended;
    float fraction=player_ash_presentation_detail::colorFraction(p,current).asFloat();
    float remaining=(E::from(1.0f)-E::from(fraction)).asFloat();
    std::array<float,3> weightedCurrent{},weightedNext{},result{};
    for(size_t i=0;i<3;i++) {
        weightedCurrent[i]=(E::from(current.color[i])*E::from(remaining)).asFloat();
        weightedNext[i]=(E::from(next.color[i])*E::from(fraction)).asFloat();
        result[i]=(E::from(weightedNext[i])+E::from(weightedCurrent[i])).asFloat();
    }
    return result;
}

// 0047D6D0: source +8 values are first scaled to byte space and truncated. Bits
// 1/2 select either independent fade weights or a current-byte crossfade;
// both channels then receive the explicit +0x20 multiplier.
template<size_t N>
std::array<uint8_t,2> playerAshPresentationByteChannels(
    const AshPresentationInputs& p,const std::array<AshPresentationFrame,N>& frames) {
    using E=Extended;
    const auto& current=frames.at(p.currentIndex);
    const auto& next=frames.at(p.nextIndex);
    auto toByte=[](float v) {
        return player_ash_presentation_detail::byteFromExtended(
            E::from(v)*E::from(255.0f));
    };
    uint8_t a=toByte(current.byteChannel),b=toByte(next.byteChannel);
    if ((p.flags&0x06)!=0) {
        E fraction=player_ash_presentation_detail::timelineFraction(p,current);
        if (p.flags&0x02) {
            a=player_ash_presentation_detail::byteFromExtended(
                (E::from(1.0f)-fraction)*E::integer(uint32_t(a)));
            b=player_ash_presentation_detail::byteFromExtended(
                fraction*E::integer(uint32_t(b)));
        } else if (p.flags&0x04) {
            E left=(E::from(1.0f)-fraction)*E::integer(uint32_t(a));
            E right=fraction*E::integer(uint32_t(b));
            a=player_ash_presentation_detail::byteFromExtended(left+right);
        }
    }
    a=player_ash_presentation_detail::byteFromExtended(
        E::integer(uint32_t(a))*E::from(p.byteChannelScale));
    b=player_ash_presentation_detail::byteFromExtended(
        E::integer(uint32_t(b))*E::from(p.byteChannelScale));
    return {a,b};
}

// 0047D8A0 returns this x87 value in ST0. Keep it extended through the caller's
// opacity multiplication instead of forcing an early binary32 result.
template<size_t N>
Extended playerAshPresentationAlpha(
    const AshPresentationInputs& p,const std::array<AshPresentationFrame,N>& frames) {
    using E=Extended;
    const auto& current=frames.at(p.currentIndex);
    const auto& next=frames.at(p.nextIndex);
    if ((p.flags&0x10)==0) return E::from(current.alpha);
    E fraction=player_ash_presentation_detail::timelineFraction(p,current);
    return (E::from(1.0f)-fraction)*E::from(current.alpha)+
           E::from(next.alpha)*fraction;
}

// Tail of 0047F960 after 0047D8A0: particle +0xA8 scales alpha and the caller
// spills the x87 product as binary32 for geWorld_AddPolyOnce.
template<size_t N>
float playerAshPresentationScaledAlpha(
    const AshPresentationInputs& p,const std::array<AshPresentationFrame,N>& frames,
    float opacityScale) {
    return (playerAshPresentationAlpha(p,frames)*Extended::from(opacityScale)).asFloat();
}
