#pragma once

#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "core.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

// PlayerHose component 0046D6B0 resolves resource 3 and creates these strip
// polygons with white LVertex colors. Its 0046C5A0 consumer scrolls a 0.15 U
// span at six units per second and maps V from 0 to 0.9. Texture sampling,
// Genesis material flag 0x20, and the final blend/depth behavior are not fully
// recovered; this class keeps those OpenGL state choices explicitly provisional.
struct PlayerHoseStreamRenderer {
    static constexpr float uSpan = 0.15f;
    static constexpr float vSpan = 0.9f;
    static constexpr float uScrollPerSecond = 6.f;

    GLuint texture = 0;

    static GLuint loadAlphaPair(const std::filesystem::path& spriteRoot) {
        const auto colorPath = spriteRoot / "hdstream.bmp";
        const auto alphaPath = spriteRoot / "hdstream_a.bmp";
        SDL_Surface* colorBitmap = SDL_LoadBMP(colorPath.string().c_str());
        if (!colorBitmap)
            throw std::runtime_error("Cannot load PlayerHose stream color texture " +
                                     colorPath.string() + ": " + SDL_GetError());
        SDL_Surface* alphaBitmap = SDL_LoadBMP(alphaPath.string().c_str());
        if (!alphaBitmap) {
            SDL_FreeSurface(colorBitmap);
            throw std::runtime_error("Cannot load PlayerHose stream alpha texture " +
                                     alphaPath.string() + ": " + SDL_GetError());
        }

        SDL_Surface* color = SDL_ConvertSurfaceFormat(
            colorBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_Surface* alpha = SDL_ConvertSurfaceFormat(
            alphaBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(colorBitmap);
        SDL_FreeSurface(alphaBitmap);
        if (!color || !alpha) {
            if (color) SDL_FreeSurface(color);
            if (alpha) SDL_FreeSurface(alpha);
            throw std::runtime_error(std::string("Cannot convert PlayerHose stream textures: ") +
                                     SDL_GetError());
        }

        const bool lockColor = SDL_MUSTLOCK(color);
        const bool lockAlpha = SDL_MUSTLOCK(alpha);
        if (lockColor && SDL_LockSurface(color) != 0) {
            SDL_FreeSurface(color);
            SDL_FreeSurface(alpha);
            throw std::runtime_error(std::string("Cannot lock PlayerHose stream color texture: ") +
                                     SDL_GetError());
        }
        if (lockAlpha && SDL_LockSurface(alpha) != 0) {
            if (lockColor) SDL_UnlockSurface(color);
            SDL_FreeSurface(color);
            SDL_FreeSurface(alpha);
            throw std::runtime_error(std::string("Cannot lock PlayerHose stream alpha texture: ") +
                                     SDL_GetError());
        }

        const int width = color->w;
        const int height = color->h;
        std::vector<uint8_t> rgba(size_t(width) * size_t(height) * 4);
        auto sampleAlpha = [alpha](float x, float y) {
            x = std::clamp(x, 0.f, float(alpha->w - 1));
            y = std::clamp(y, 0.f, float(alpha->h - 1));
            const int x0 = int(std::floor(x));
            const int y0 = int(std::floor(y));
            const int x1 = std::min(x0 + 1, alpha->w - 1);
            const int y1 = std::min(y0 + 1, alpha->h - 1);
            const float tx = x - float(x0);
            const float ty = y - float(y0);
            auto value = [alpha](int px, int py) {
                const auto* pixel = static_cast<const uint8_t*>(alpha->pixels) +
                                    py * alpha->pitch + px * 4;
                return float(pixel[0]);
            };
            const float top = value(x0, y0) + (value(x1, y0) - value(x0, y0)) * tx;
            const float bottom = value(x0, y1) + (value(x1, y1) - value(x0, y1)) * tx;
            return uint8_t(std::clamp(top + (bottom - top) * ty, 0.f, 255.f));
        };

        for (int y = 0; y < height; ++y) {
            const float alphaY = (float(y) + .5f) * float(alpha->h) /
                                 float(height) - .5f;
            for (int x = 0; x < width; ++x) {
                const auto* source = static_cast<const uint8_t*>(color->pixels) +
                                     y * color->pitch + x * 4;
                const float alphaX = (float(x) + .5f) * float(alpha->w) /
                                     float(width) - .5f;
                const size_t output = (size_t(y) * size_t(width) + size_t(x)) * 4;
                rgba[output + 0] = source[0];
                rgba[output + 1] = source[1];
                rgba[output + 2] = source[2];
                rgba[output + 3] = sampleAlpha(alphaX, alphaY);
            }
        }
        if (lockAlpha) SDL_UnlockSurface(alpha);
        if (lockColor) SDL_UnlockSurface(color);
        SDL_FreeSurface(alpha);
        SDL_FreeSurface(color);

        GLint previousUnpackAlignment = 4;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousUnpackAlignment);
        GLuint result = 0;
        glGenTextures(1, &result);
        glBindTexture(GL_TEXTURE_2D, result);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
        glPixelStorei(GL_UNPACK_ALIGNMENT, previousUnpackAlignment);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Repeating U lets the source's monotonically decreasing offset wrap;
        // clamp and filter choices remain a host approximation.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return result;
    }

    void load(const std::filesystem::path& spriteRoot) {
        release();
        texture = loadAlphaPair(spriteRoot);
    }

    int draw(const std::vector<V>& vertices, float timeSeconds) const {
        if (!texture || vertices.size() < 4) return 0;
        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT |
                     GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor4ub(255, 255, 255, 255);

        const float u = -timeSeconds * uScrollPerSecond;
        constexpr std::array<std::array<float, 2>, 4> localUv = {{
            {{0.f, 0.f}}, {{uSpan, 0.f}}, {{0.f, vSpan}}, {{uSpan, vSpan}},
        }};
        const size_t quadCount = vertices.size() / 4;
        glBegin(GL_QUADS);
        for (size_t quad = 0; quad < quadCount; ++quad) {
            for (size_t corner = 0; corner < 4; ++corner) {
                glTexCoord2f(u + localUv[corner][0], localUv[corner][1]);
                const V& point = vertices[quad * 4 + corner];
                glVertex3f(point.x, point.y, point.z);
            }
        }
        glEnd();
        glPopAttrib();
        return int(quadCount);
    }

    void release() {
        if (texture) glDeleteTextures(1, &texture);
        texture = 0;
    }
};
