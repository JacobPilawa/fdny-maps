#pragma once

#include "oxygen_hud_renderer.h"

// Resource 55 is the Player status object's rescuing/rescuing_a pair. The
// status action starts this screen-space rectangle track on pickup and hides
// it explicitly on rescue; the carried body transform remains unresolved.
struct VictimPickupHudRenderer {
    struct Rect {
        int left;
        int right;
        int top;
        int bottom;
    };

    struct Keyframe {
        float seconds;
        Rect rect;
    };

    static constexpr std::array<Keyframe, 4> keyframes{{
        {0.0f, {320, 320, 240, 240}},
        {0.1f, {256, 384, 176, 304}},
        {1.0f, {256, 384, 176, 304}},
        {1.1f, {502, 566, 5, 69}},
    }};

    GLuint texture = 0;
    uint32_t startTick = 0;
    bool visible = false;

    static Rect sample(float seconds) {
        if (seconds <= keyframes.front().seconds) return keyframes.front().rect;
        for (size_t i = 1; i < keyframes.size(); ++i) {
            const auto& previous = keyframes[i - 1];
            const auto& next = keyframes[i];
            if (seconds > next.seconds) continue;
            const float amount = (seconds - previous.seconds) /
                                 (next.seconds - previous.seconds);
            auto interpolate = [amount](int from, int to) {
                return int(std::trunc(float(from) + (float(to - from) * amount)));
            };
            return {
                interpolate(previous.rect.left, next.rect.left),
                interpolate(previous.rect.right, next.rect.right),
                interpolate(previous.rect.top, next.rect.top),
                interpolate(previous.rect.bottom, next.rect.bottom),
            };
        }
        return keyframes.back().rect;
    }

    void load(const std::filesystem::path& spriteRoot) {
        release();
        texture = OxygenHudRenderer::loadAlphaPair(
            spriteRoot, "rescuing", "victim pickup HUD");
    }

    void begin(uint32_t now) {
        startTick = now;
        visible = true;
    }

    void hide() { visible = false; }

    int draw(uint32_t now, int viewportWidth, int viewportHeight) const {
        return drawAtSeconds(float(uint32_t(now - startTick)) * 0.001f,
                             viewportWidth, viewportHeight);
    }

    int drawAtSeconds(float seconds, int viewportWidth, int viewportHeight) const {
        if (!visible || !texture || viewportWidth <= 0 || viewportHeight <= 0) return 0;
        const Rect rect = sample(seconds);
        if (rect.left == rect.right || rect.top == rect.bottom) return 0;

        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT |
                     GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor4f(1.f, 1.f, 1.f, 1.f);

        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
        glOrtho(0, viewportWidth, 0, viewportHeight, -1, 1);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        const float scaleX = float(viewportWidth) / 640.f;
        const float scaleY = float(viewportHeight) / 480.f;
        const std::array<float, 4> x = {
            float(rect.left), float(rect.right), float(rect.right), float(rect.left)
        };
        const std::array<float, 4> y = {
            float(rect.top), float(rect.top), float(rect.bottom), float(rect.bottom)
        };
        constexpr std::array<float, 4> u = {0.f, 1.f, 1.f, 0.f};
        constexpr std::array<float, 4> v = {0.f, 0.f, 1.f, 1.f};
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        for (size_t i = 0; i < x.size(); ++i) {
            glTexCoord2f(u[i], v[i]);
            glVertex2f(x[i] * scaleX, (480.f - y[i]) * scaleY);
        }
        glEnd();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        return 1;
    }

    void release() {
        if (texture) glDeleteTextures(1, &texture);
        texture = 0;
        visible = false;
        startTick = 0;
    }
};
