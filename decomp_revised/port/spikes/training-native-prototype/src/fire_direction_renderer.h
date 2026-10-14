#pragma once

#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

// Retail Player HUD setup (004567D0/00456090) displays resource 50's
// fireprox.bmp/fireprox_a.bmp pair at four 128x128 GUI rectangles. Each
// rectangle starts at x=256..384,y=0..128 and rotates around (320,240):
// front 0, positive-angle side -90, rear -180, negative-angle side -270.
// The paired alpha BMP is attached by the retail resource loader; its exact
// Genesis/D3D sampling and blend state for these child widgets remain open.
struct FireDirectionRenderer {
    GLuint texture = 0;

    void load(const std::filesystem::path& spriteRoot) {
        const auto colorPath = spriteRoot / "fireprox.bmp";
        const auto alphaPath = spriteRoot / "fireprox_a.bmp";
        SDL_Surface* colorBitmap = SDL_LoadBMP(colorPath.string().c_str());
        if (!colorBitmap)
            throw std::runtime_error("Cannot load fire-direction texture " + colorPath.string() + ": " + SDL_GetError());
        SDL_Surface* alphaBitmap = SDL_LoadBMP(alphaPath.string().c_str());
        if (!alphaBitmap) {
            SDL_FreeSurface(colorBitmap);
            throw std::runtime_error("Cannot load fire-direction alpha " + alphaPath.string() + ": " + SDL_GetError());
        }
        SDL_Surface* colorSource = SDL_ConvertSurfaceFormat(colorBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_Surface* alphaSource = SDL_ConvertSurfaceFormat(alphaBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(alphaBitmap);
        SDL_FreeSurface(colorBitmap);
        if (!colorSource || !alphaSource) {
            if (alphaSource) SDL_FreeSurface(alphaSource);
            if (colorSource) SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot convert fire-direction textures: ") + SDL_GetError());
        }
        if (colorSource->w != alphaSource->w || colorSource->h != alphaSource->h) {
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error("Fire-direction color and alpha textures have different dimensions");
        }

        if (SDL_MUSTLOCK(colorSource) && SDL_LockSurface(colorSource) != 0) {
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot lock fire-direction texture: ") + SDL_GetError());
        }
        if (SDL_MUSTLOCK(alphaSource) && SDL_LockSurface(alphaSource) != 0) {
            if (SDL_MUSTLOCK(colorSource)) SDL_UnlockSurface(colorSource);
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot lock fire-direction alpha: ") + SDL_GetError());
        }

        const int width = colorSource->w, height = colorSource->h;
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
                // fireprox_a.bmp is a grayscale palette, so its red channel
                // preserves the source mask without inventing a tint curve.
                rgba[offset + 3] = alphaPixel[0];
            }
        }
        if (SDL_MUSTLOCK(alphaSource)) SDL_UnlockSurface(alphaSource);
        if (SDL_MUSTLOCK(colorSource)) SDL_UnlockSurface(colorSource);
        SDL_FreeSurface(alphaSource);
        SDL_FreeSurface(colorSource);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    int draw(const std::array<uint8_t, 4>& indicators, int viewportWidth,
             int viewportHeight) const {
        if (!texture || viewportWidth <= 0 || viewportHeight <= 0) return 0;
        int activeCount = 0;
        for (uint8_t indicator : indicators) activeCount += indicator != 0;
        if (activeCount == 0) return 0;

        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT |
                     GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glEnable(GL_BLEND);
        // Port-side blend is provisional; the retail resource's exact
        // GUI blend state has not yet been traced through Genesis/D3D.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor4f(1.f, 1.f, 1.f, 1.f);

        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
        glOrtho(0, viewportWidth, 0, viewportHeight, -1, 1);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        // Retail viewport is 640x480, x right/y down. The source rectangle is
        // rotated around its viewport center, then mapped to GL's y-up space.
        constexpr float angles[] = {0.f, -180.f, -90.f, -270.f};
        constexpr float pi = 3.14159265358979323846f;
        constexpr float sourceX[] = {256.f, 384.f, 384.f, 256.f};
        constexpr float sourceY[] = {0.f, 0.f, 128.f, 128.f};
        constexpr float sourceU[] = {0.f, 1.f, 1.f, 0.f};
        constexpr float sourceV[] = {0.f, 0.f, 1.f, 1.f};
        for (size_t sector = 0; sector < indicators.size(); ++sector) {
            if (!indicators[sector]) continue;
            const float radians = angles[sector] * (pi / 180.f);
            const float cosine = std::cos(radians), sine = std::sin(radians);
            glBegin(GL_QUADS);
            for (size_t corner = 0; corner < 4; ++corner) {
                const float dx = sourceX[corner] - 320.f;
                const float dy = sourceY[corner] - 240.f;
                const float uiX = 320.f + cosine * dx - sine * dy;
                const float uiY = 240.f + sine * dx + cosine * dy;
                const float x = uiX * float(viewportWidth) / 640.f;
                const float y = (480.f - uiY) * float(viewportHeight) / 480.f;
                glTexCoord2f(sourceU[corner], sourceV[corner]);
                glVertex2f(x, y);
            }
            glEnd();
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        return activeCount;
    }

    void release() {
        if (texture) glDeleteTextures(1, &texture);
        texture = 0;
    }
};
