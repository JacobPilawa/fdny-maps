#pragma once

#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "player_oxygen.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

// Retail Player status resources 12 and 13 are oxygauge/oxygauge_a and
// oxyneedle/oxyneedle_a. 004576B0 places both over source rectangle
// (15,304)..(111,400); 00482430 rotates the needle around its center from the
// smoothed oxygen value. Texture filtering and Genesis/D3D blend state still
// need a retail capture.
struct OxygenHudRenderer {
    GLuint gaugeTexture = 0;
    GLuint needleTexture = 0;
    GLuint visorEffectTexture = 0;

    static GLuint loadAlphaPair(const std::filesystem::path& spriteRoot,
                                const char* stem,
                                const char* purpose = "HUD") {
        const auto colorPath = spriteRoot / (std::string(stem) + ".bmp");
        const auto alphaPath = spriteRoot / (std::string(stem) + "_a.bmp");
        SDL_Surface* colorBitmap = SDL_LoadBMP(colorPath.string().c_str());
        if (!colorBitmap)
            throw std::runtime_error("Cannot load " + std::string(purpose) + " texture " + colorPath.string() +
                                     ": " + SDL_GetError());
        SDL_Surface* alphaBitmap = SDL_LoadBMP(alphaPath.string().c_str());
        if (!alphaBitmap) {
            SDL_FreeSurface(colorBitmap);
            throw std::runtime_error("Cannot load " + std::string(purpose) + " alpha texture " + alphaPath.string() +
                                     ": " + SDL_GetError());
        }

        SDL_Surface* colorSource = SDL_ConvertSurfaceFormat(
            colorBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_Surface* alphaSource = SDL_ConvertSurfaceFormat(
            alphaBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(alphaBitmap);
        SDL_FreeSurface(colorBitmap);
        if (!colorSource || !alphaSource) {
            if (alphaSource) SDL_FreeSurface(alphaSource);
            if (colorSource) SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot convert ") + purpose + " textures: " +
                                     SDL_GetError());
        }
        if (colorSource->w != alphaSource->w || colorSource->h != alphaSource->h) {
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string(purpose) + " color and alpha textures have different dimensions");
        }

        if (SDL_MUSTLOCK(colorSource) && SDL_LockSurface(colorSource) != 0) {
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot lock ") + purpose + " texture: " +
                                     SDL_GetError());
        }
        if (SDL_MUSTLOCK(alphaSource) && SDL_LockSurface(alphaSource) != 0) {
            if (SDL_MUSTLOCK(colorSource)) SDL_UnlockSurface(colorSource);
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot lock ") + purpose + " alpha texture: " +
                                     SDL_GetError());
        }

        const int width = colorSource->w;
        const int height = colorSource->h;
        std::vector<uint8_t> rgba(size_t(width) * size_t(height) * 4);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const auto* colorPixel = static_cast<const uint8_t*>(colorSource->pixels) +
                                         y * colorSource->pitch + x * 4;
                const auto* alphaPixel = static_cast<const uint8_t*>(alphaSource->pixels) +
                                         y * alphaSource->pitch + x * 4;
                const size_t offset = (size_t(y) * size_t(width) + size_t(x)) * 4;
                rgba[offset + 0] = colorPixel[0];
                rgba[offset + 1] = colorPixel[1];
                rgba[offset + 2] = colorPixel[2];
                rgba[offset + 3] = alphaPixel[0];
            }
        }
        if (SDL_MUSTLOCK(alphaSource)) SDL_UnlockSurface(alphaSource);
        if (SDL_MUSTLOCK(colorSource)) SDL_UnlockSurface(colorSource);
        SDL_FreeSurface(alphaSource);
        SDL_FreeSurface(colorSource);

        GLuint texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return texture;
    }

    void load(const std::filesystem::path& spriteRoot) {
        release();
        gaugeTexture = loadAlphaPair(spriteRoot, "oxygauge", "oxygen HUD");
        try {
            needleTexture = loadAlphaPair(spriteRoot, "oxyneedle", "oxygen HUD");
            visorEffectTexture = loadAlphaPair(spriteRoot, "shield", "oxygen visor effect");
        } catch (...) {
            release();
            throw;
        }
    }

    int draw(float displayedOxygen, float warningLayerAmount,
             int viewportWidth, int viewportHeight) const {
        if (!gaugeTexture || !needleTexture || viewportWidth <= 0 || viewportHeight <= 0 ||
            displayedOxygen > 0.99f) return 0;

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

        // Retail GUI coordinates use x-right/y-down in a 640x480 frame.
        constexpr std::array<float, 4> sourceX = {15.f, 111.f, 111.f, 15.f};
        constexpr std::array<float, 4> sourceY = {304.f, 304.f, 400.f, 400.f};
        constexpr std::array<float, 4> sourceU = {0.f, 1.f, 1.f, 0.f};
        constexpr std::array<float, 4> sourceV = {0.f, 0.f, 1.f, 1.f};
        constexpr float centerX = 63.f;
        constexpr float centerY = 352.f;
        constexpr float pi = 3.14159265358979323846f;
        const std::array<GLuint, 3> layers = {
            gaugeTexture, gaugeTexture, needleTexture,
        };
        const std::array<float, 3> angles = {
            0.f,
            0.f,
            -270.f * (0.5f - displayedOxygen),
        };
        constexpr std::array<std::array<float, 3>, 3> colors = {{
            {{1.f, 1.f, 1.f}},
            {{1.f, .25f, 0.f}},
            {{1.f, 1.f, 1.f}},
        }};
        const std::array<float, 3> alphas = {
            1.f,
            std::max(0.f, std::min(.1f, warningLayerAmount)),
            1.f,
        };
        const size_t layerCount = alphas[1] > 0.f ? 3u : 2u;

        for (size_t layer = 0; layer < layerCount; ++layer) {
            const float radians = angles[layer] * (pi / 180.f);
            const float cosine = std::cos(radians);
            const float sine = std::sin(radians);
            glBindTexture(GL_TEXTURE_2D, layers[layer]);
            glColor4f(colors[layer][0], colors[layer][1], colors[layer][2], alphas[layer]);
            glBegin(GL_QUADS);
            for (size_t corner = 0; corner < sourceX.size(); ++corner) {
                const float dx = sourceX[corner] - centerX;
                const float dy = sourceY[corner] - centerY;
                const float guiX = centerX + cosine * dx - sine * dy;
                const float guiY = centerY + sine * dx + cosine * dy;
                const float x = guiX * float(viewportWidth) / 640.f;
                const float y = (480.f - guiY) * float(viewportHeight) / 480.f;
                glTexCoord2f(sourceU[corner], sourceV[corner]);
                glVertex2f(x, y);
            }
            glEnd();
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        return int(layerCount);
    }

    int drawVisorEffect(const PlayerOxygenVisorEffect& effect, uint32_t now,
                        int viewportWidth, int viewportHeight) const {
        int32_t topY = 0;
        if (!visorEffectTexture || viewportWidth <= 0 || viewportHeight <= 0 ||
            !samplePlayerOxygenVisorEffect(effect, now, topY)) return 0;

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

        // 00455F60 uses UVs 0.1..1 and rectangle [0, topY, 640, topY+256].
        constexpr std::array<float, 4> sourceX = {0.f, 640.f, 640.f, 0.f};
        constexpr std::array<float, 4> sourceU = {.1f, 1.f, 1.f, .1f};
        constexpr std::array<float, 4> sourceV = {.1f, .1f, 1.f, 1.f};
        const std::array<float, 4> sourceY = {
            float(topY), float(topY), float(topY + 256), float(topY + 256),
        };
        glBindTexture(GL_TEXTURE_2D, visorEffectTexture);
        glBegin(GL_QUADS);
        for (size_t corner = 0; corner < sourceX.size(); ++corner) {
            const float x = sourceX[corner] * float(viewportWidth) / 640.f;
            const float y = (480.f - sourceY[corner]) * float(viewportHeight) / 480.f;
            glTexCoord2f(sourceU[corner], sourceV[corner]);
            glVertex2f(x, y);
        }
        glEnd();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        return 1;
    }

    void release() {
        if (gaugeTexture) glDeleteTextures(1, &gaugeTexture);
        if (needleTexture) glDeleteTextures(1, &needleTexture);
        if (visorEffectTexture) glDeleteTextures(1, &visorEffectTexture);
        gaugeTexture = needleTexture = visorEffectTexture = 0;
    }
};
