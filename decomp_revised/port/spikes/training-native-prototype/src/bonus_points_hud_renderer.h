#pragma once

#include "oxygen_hud_renderer.h"

// Player status widget resource 502 uses tools.bmp/rescbon_a.bmp. The source
// track and font cells are mapped from the retail EXE; screen corner mapping,
// child alignment, and OpenGL material behavior remain provisional.
struct BonusPointsHudRenderer {
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
        {0.0f, {320, 320, 180, 180}},
        {0.1f, {256, 384, 176, 304}},
        {1.0f, {256, 384, 176, 304}},
        {1.1f, {5, 69, 83, 111}},
    }};

    GLuint spriteTexture = 0;
    GLuint fontTexture = 0;
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
                return int(std::trunc(float(from) + float(to - from) * amount));
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
        spriteTexture = OxygenHudRenderer::loadAlphaPairFiles(
            spriteRoot, "tools.bmp", "rescbon_a.bmp", "Bonus Points HUD");
        try {
            fontTexture = OxygenHudRenderer::loadAlphaPairFiles(
                spriteRoot, "confont.bmp", "confont_a.bmp", "Bonus Points font");
        } catch (...) {
            release();
            throw;
        }
    }

    void begin(uint32_t now) {
        startTick = now;
        visible = true;
    }

    void hide() { visible = false; }

    int draw(uint32_t now, int32_t score,
             int viewportWidth, int viewportHeight) const {
        return drawAtSeconds(float(uint32_t(now - startTick)) * 0.001f,
                             score, viewportWidth, viewportHeight);
    }

    int drawAtSeconds(float seconds, int32_t score,
                      int viewportWidth, int viewportHeight) const {
        if (!visible || !spriteTexture || !fontTexture || score == 0 ||
            viewportWidth <= 0 || viewportHeight <= 0) return 0;
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
        constexpr float vTop = 28.f / 63.f;
        constexpr float vBottom = 56.f / 63.f;
        constexpr std::array<float, 4> v = {vTop, vTop, vBottom, vBottom};

        glBindTexture(GL_TEXTURE_2D, spriteTexture);
        glBegin(GL_QUADS);
        for (size_t i = 0; i < x.size(); ++i) {
            glTexCoord2f(u[i], v[i]);
            glVertex2f(x[i] * scaleX, (480.f - y[i]) * scaleY);
        }
        glEnd();

        // The text component uses 8x14 cells and starts from parent-relative
        // fields +0x24/+0x28. Retail's measured lower edge minus glyph height
        // informs this anchor; exact placement and alignment are unconfirmed.
        const std::string text = std::to_string(score);
        float glyphX = float(rect.left + 24);
        const float glyphTop = float(rect.bottom - 14);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
        for (char character : text) {
            int cell = -1;
            if (character >= '0' && character <= '9')
                cell = 52 + (character - '0');
            else if (character == '-')
                cell = 77;
            if (cell < 0) continue;

            const int column = cell % 16;
            const int row = cell / 16;
            const float left = glyphX;
            const float right = glyphX + 8.f;
            const float top = glyphTop;
            const float bottom = glyphTop + 14.f;
            const float u0 = float(column * 8) / 128.f;
            const float u1 = float((column + 1) * 8) / 128.f;
            const float v0 = float(row * 14) / 128.f;
            const float v1 = float((row + 1) * 14) / 128.f;
            glBegin(GL_QUADS);
            glTexCoord2f(u0, v0); glVertex2f(left * scaleX, (480.f - top) * scaleY);
            glTexCoord2f(u1, v0); glVertex2f(right * scaleX, (480.f - top) * scaleY);
            glTexCoord2f(u1, v1); glVertex2f(right * scaleX, (480.f - bottom) * scaleY);
            glTexCoord2f(u0, v1); glVertex2f(left * scaleX, (480.f - bottom) * scaleY);
            glEnd();
            glyphX += 8.f;
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        return int(text.size()) + 1;
    }

    void release() {
        if (spriteTexture) glDeleteTextures(1, &spriteTexture);
        if (fontTexture) glDeleteTextures(1, &fontTexture);
        spriteTexture = 0;
        fontTexture = 0;
        visible = false;
        startTick = 0;
    }
};
