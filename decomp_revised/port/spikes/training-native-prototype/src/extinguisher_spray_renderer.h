#pragma once

#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "extinguisher_effects.h"
#include "view_update.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

// Retail extinguisher setup 00461830 selects resource 14 for a two-second
// particle using white color and a .75 alpha key. rescue.rrf maps resource 14
// to bsmoke.bmp + sm_alpha.bmp, though the pool manager's link to that RRF is
// not fully resolved. This host draws a camera-facing OpenGL quad in place of
// geWorld_AddPolyOnce; quad orientation/extent, key interpolation, blend flags,
// clipping, and exact resource-manager selection remain provisional.
struct ExtinguisherSprayRenderer {
    struct Particle {
        V origin{};
        V velocity{};
        uint32_t startTime = 0;
    };

    static constexpr uint32_t lifetimeMs = 2000;
    static constexpr float previewSize = 3.f;
    static constexpr uint8_t previewAlpha = 191; // source alpha key: trunc(.75 * 255)

    GLuint texture = 0;
    std::vector<Particle> particles;

    static GLuint loadAlphaPair(const std::filesystem::path& spriteRoot) {
        const auto colorPath = spriteRoot / "bsmoke.bmp";
        const auto alphaPath = spriteRoot / "sm_alpha.bmp";
        SDL_Surface* colorBitmap = SDL_LoadBMP(colorPath.string().c_str());
        if (!colorBitmap)
            throw std::runtime_error("Cannot load extinguisher particle texture " +
                                     colorPath.string() + ": " + SDL_GetError());
        SDL_Surface* alphaBitmap = SDL_LoadBMP(alphaPath.string().c_str());
        if (!alphaBitmap) {
            SDL_FreeSurface(colorBitmap);
            throw std::runtime_error("Cannot load extinguisher particle alpha " +
                                     alphaPath.string() + ": " + SDL_GetError());
        }
        SDL_Surface* color = SDL_ConvertSurfaceFormat(colorBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_Surface* alpha = SDL_ConvertSurfaceFormat(alphaBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(colorBitmap);
        SDL_FreeSurface(alphaBitmap);
        if (!color || !alpha) {
            if (color) SDL_FreeSurface(color);
            if (alpha) SDL_FreeSurface(alpha);
            throw std::runtime_error(std::string("Cannot convert extinguisher particle textures: ") +
                                     SDL_GetError());
        }
        if (color->w != alpha->w || color->h != alpha->h) {
            SDL_FreeSurface(color);
            SDL_FreeSurface(alpha);
            throw std::runtime_error("Extinguisher particle color and alpha dimensions disagree");
        }
        const bool lockColor = SDL_MUSTLOCK(color);
        const bool lockAlpha = SDL_MUSTLOCK(alpha);
        if (lockColor && SDL_LockSurface(color) != 0) {
            SDL_FreeSurface(color);
            SDL_FreeSurface(alpha);
            throw std::runtime_error(std::string("Cannot lock extinguisher particle texture: ") +
                                     SDL_GetError());
        }
        if (lockAlpha && SDL_LockSurface(alpha) != 0) {
            if (lockColor) SDL_UnlockSurface(color);
            SDL_FreeSurface(color);
            SDL_FreeSurface(alpha);
            throw std::runtime_error(std::string("Cannot lock extinguisher particle alpha: ") +
                                     SDL_GetError());
        }

        const int width = color->w, height = color->h;
        std::vector<uint8_t> rgba(size_t(width) * size_t(height) * 4);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const auto* source = static_cast<const uint8_t*>(color->pixels) +
                                     y * color->pitch + x * 4;
                const auto* mask = static_cast<const uint8_t*>(alpha->pixels) +
                                   y * alpha->pitch + x * 4;
                const size_t output = (size_t(y) * size_t(width) + size_t(x)) * 4;
                rgba[output + 0] = source[0];
                rgba[output + 1] = source[1];
                rgba[output + 2] = source[2];
                rgba[output + 3] = mask[0];
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return result;
    }

    void load(const std::filesystem::path& spriteRoot) {
        release();
        texture = loadAlphaPair(spriteRoot);
    }

    void spawn(const ExtinguisherEffects::SpawnRequest& request, V origin,
               uint32_t now) {
        if (!request.requested) return;
        particles.push_back({origin, request.offset, now});
    }

    void prune(uint32_t now) {
        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [now](const Particle& particle) {
                return uint32_t(now - particle.startTime) >= lifetimeMs;
            }), particles.end());
    }

    int draw(uint32_t now, const FrameTransform& camera) const {
        if (!texture || particles.empty()) return 0;
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
        glColor4ub(255, 255, 255, previewAlpha);

        constexpr float uv[4][2] = {{0.f, 0.f}, {.99f, 0.f}, {.99f, .99f}, {0.f, .99f}};
        const V right = camera.axis[0] * (previewSize * .5f);
        const V up = camera.axis[1] * (previewSize * .5f);
        int drawn = 0;
        glBegin(GL_QUADS);
        for (const Particle& particle : particles) {
            const uint32_t age = uint32_t(now - particle.startTime);
            if (age >= lifetimeMs) continue;
            const V center = particle.origin + particle.velocity * (float(age) * .001f);
            const std::array<V, 4> vertices = {
                center - right - up,
                center + right - up,
                center + right + up,
                center - right + up,
            };
            for (size_t corner = 0; corner < vertices.size(); ++corner) {
                glTexCoord2f(uv[corner][0], uv[corner][1]);
                glVertex3f(vertices[corner].x, vertices[corner].y, vertices[corner].z);
            }
            ++drawn;
        }
        glEnd();
        glPopAttrib();
        return drawn;
    }

    void release() {
        if (texture) glDeleteTextures(1, &texture);
        texture = 0;
        particles.clear();
    }
};
