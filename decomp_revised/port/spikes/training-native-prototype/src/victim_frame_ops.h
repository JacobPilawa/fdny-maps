#pragma once

#include <functional>
#include <utility>
#include "view_update.h"
#include "model_floor.h"

// Concrete frame/floor portion shared by Training release and passout Ops.
// The referenced victim frame is the caller's live frame; Player frame reads
// remain separate at each source call site. Engine/event/status work belongs
// to the surrounding Ops object.
struct VictimFrameOps {
    using Trace = std::function<bool(V,V,V,V,V&)>;
    using ReportFailure = std::function<void()>;

    FrameTransform& victimFrame;
    std::function<const FrameTransform&()> playerFrame;
    Trace trace;
    ReportFailure reportFailure;
    V releaseOffset{};
    bool lastFloorTraceSucceeded = false;

    VictimFrameOps(FrameTransform& victim,
                   std::function<const FrameTransform&()> getPlayerFrame,
                   Trace traceFloor,
                   ReportFailure reportFloorFailure)
        : victimFrame(victim), playerFrame(std::move(getPlayerFrame)),
          trace(std::move(traceFloor)), reportFailure(std::move(reportFloorFailure)) {}

    template<class Actor, class Player>
    void copyFirstPlayerFrameAxis(Actor&, Player&) {
        const auto& frame = playerFrame();
        const V axis = frame.axis[0];
        victimFrame.axis[0] = {-axis.x, -axis.y, -axis.z};
    }

    template<class Actor, class Player>
    void copySecondPlayerFrameAxis(Actor&, Player&) {
        const auto& frame = playerFrame();
        const V axis = frame.axis[2];
        victimFrame.axis[2] = {-axis.x, -axis.y, -axis.z};
    }

    template<class Actor>
    void setVictimFrameUp(Actor&, float x, float y, float z) {
        ViewUpdate::setUp(victimFrame, {x,y,z});
    }

    template<class Actor>
    void releaseOffsetTimes5(Actor&) {
        releaseOffset = ViewUpdate::scale(victimFrame.axis[2], 5.0f);
    }

    template<class Actor>
    void releaseOffsetTimes24(Actor&) {
        releaseOffset = ViewUpdate::scale(releaseOffset, 24.0f);
    }

    template<class Actor, class Player>
    void setReleasePositionFromPlayerFrame(Actor&, Player&) {
        const auto& frame = playerFrame();
        victimFrame.position = ViewUpdate::subtract(frame.position, releaseOffset);
    }

    template<class Actor, class Player>
    void copyPlayerFramePosition(Actor&, Player&) {
        const auto& frame = playerFrame();
        victimFrame.position = frame.position;
    }

    template<class Actor>
    void rebuildVictimFrameBasis(Actor&) {
        // 0045C1A0 pushes (0,1,0) to the frame's +8 method; see evidence note.
        ViewUpdate::setUp(victimFrame, {0,1,0});
    }

    void correctDroppedFrameX(float correction){
        victimFrame.position.x=(Extended::from(victimFrame.position.x)-Extended::from(correction)).asFloat();
    }

    template<class Actor>
    void placeOnFloor(Actor&) {
        lastFloorTraceSucceeded = placeModelOnFloor(
            victimFrame.position, {-24,-24,-24}, {24,24,24}, trace, reportFailure);
    }
};
