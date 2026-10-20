#pragma once
#include "random.h"
#include <cstdint>

// The source Player stores the last accepted sound clock at +0x384. Its
// selector also rejects the integer stored at +0x558. The latter starts at
// zero; no later static writer was found, so this host keeps that value at
// zero until another writer is established.
struct PlayerFootsteps {
    uint32_t lastSoundClock = 0;
    int rejectedIndex = 0;

    int update(uint32_t now, int soundCount, bool eligible, Random& random) {
        if (!eligible || soundCount < 2 || now < lastSoundClock + 100u)
            return -1;

        int selected = 0;
        do {
            selected = static_cast<int>(random.range(
                0.f, static_cast<float>(soundCount) - .001f));
        } while (selected == rejectedIndex);

        lastSoundClock = now;
        return selected;
    }
};
