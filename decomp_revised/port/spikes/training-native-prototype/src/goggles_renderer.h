#pragma once

#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "goggles_scan.h"

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

// Resource 703 is rescue.rrf's heatpulse.bmp / heatpulse_a.bmp pair. Retail
// 00462730 updates one effect child per accepted goggles target during the
// source-derived 8.5-second scan window. The native renderer preserves the
// texture and lifetime, then places a camera-facing quad at the host actor
// position plus an estimated vertical offset. Retail's actor-local transform
// and child attachment are not yet reproduced; world size, placement, blend,
// and child animation remain provisional.
struct GogglesRenderer {
    GLuint texture = 0;
    GLuint topVisorTexture = 0;
    GLuint bottomVisorTexture = 0;

    static GLuint loadAlphaMask(const std::filesystem::path& path) {
        SDL_Surface* source = SDL_LoadBMP(path.string().c_str());
        if (!source)
            throw std::runtime_error("Cannot load goggles alpha mask " + path.string() + ": " + SDL_GetError());
        SDL_Surface* rgbaSource = SDL_ConvertSurfaceFormat(source, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(source);
        if (!rgbaSource)
            throw std::runtime_error(std::string("Cannot convert goggles alpha mask: ") + SDL_GetError());

        if (SDL_MUSTLOCK(rgbaSource) && SDL_LockSurface(rgbaSource) != 0) {
            SDL_FreeSurface(rgbaSource);
            throw std::runtime_error(std::string("Cannot lock goggles alpha mask: ") + SDL_GetError());
        }
        std::vector<uint8_t> rgba(size_t(rgbaSource->w) * size_t(rgbaSource->h) * 4);
        for (int y = 0; y < rgbaSource->h; ++y) {
            for (int x = 0; x < rgbaSource->w; ++x) {
                const auto* pixel = static_cast<const uint8_t*>(rgbaSource->pixels) +
                                    y * rgbaSource->pitch + x * 4;
                const size_t offset = (size_t(y) * size_t(rgbaSource->w) + size_t(x)) * 4;
                rgba[offset + 0] = 255;
                rgba[offset + 1] = 255;
                rgba[offset + 2] = 255;
                rgba[offset + 3] = uint8_t((uint16_t(pixel[0]) + pixel[1] + pixel[2]) / 3);
            }
        }
        const int width = rgbaSource->w, height = rgbaSource->h;
        if (SDL_MUSTLOCK(rgbaSource)) SDL_UnlockSurface(rgbaSource);
        SDL_FreeSurface(rgbaSource);

        GLuint result = 0;
        glGenTextures(1, &result);
        glBindTexture(GL_TEXTURE_2D, result);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return result;
    }

    void load(const std::filesystem::path& spriteRoot) {
        const auto colorPath = spriteRoot / "heatpulse.bmp";
        const auto alphaPath = spriteRoot / "heatpulse_a.bmp";
        SDL_Surface* colorBitmap = SDL_LoadBMP(colorPath.string().c_str());
        if (!colorBitmap)
            throw std::runtime_error("Cannot load goggles texture " + colorPath.string() + ": " + SDL_GetError());
        SDL_Surface* alphaBitmap = SDL_LoadBMP(alphaPath.string().c_str());
        if (!alphaBitmap) {
            SDL_FreeSurface(colorBitmap);
            throw std::runtime_error("Cannot load goggles alpha texture " + alphaPath.string() + ": " + SDL_GetError());
        }
        SDL_Surface* colorSource = SDL_ConvertSurfaceFormat(colorBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_Surface* alphaSource = SDL_ConvertSurfaceFormat(alphaBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(alphaBitmap);
        SDL_FreeSurface(colorBitmap);
        if (!colorSource || !alphaSource) {
            if (alphaSource) SDL_FreeSurface(alphaSource);
            if (colorSource) SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot convert goggles textures: ") + SDL_GetError());
        }
        if (colorSource->w != alphaSource->w || colorSource->h != alphaSource->h) {
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error("Goggles color and alpha textures have different dimensions");
        }

        if (SDL_MUSTLOCK(colorSource) && SDL_LockSurface(colorSource) != 0) {
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot lock goggles texture: ") + SDL_GetError());
        }
        if (SDL_MUSTLOCK(alphaSource) && SDL_LockSurface(alphaSource) != 0) {
            if (SDL_MUSTLOCK(colorSource)) SDL_UnlockSurface(colorSource);
            SDL_FreeSurface(alphaSource);
            SDL_FreeSurface(colorSource);
            throw std::runtime_error(std::string("Cannot lock goggles alpha texture: ") + SDL_GetError());
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Goggles actor constructor 00462A60 binds alpha resources 300/301
        // (goggtop/goggbott) to 640x32 GUI rectangles at source Y 68..100
        // and 380..412. The GL tint/alpha is a readable host approximation.
        topVisorTexture = loadAlphaMask(spriteRoot / "goggtop.bmp");
        bottomVisorTexture = loadAlphaMask(spriteRoot / "goggbott.bmp");
    }

    int drawVisor(bool active, int viewportWidth, int viewportHeight) const {
        if (!active || viewportWidth <= 0 || viewportHeight <= 0 ||
            !topVisorTexture || !bottomVisorTexture) return 0;
        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT |
                     GL_TEXTURE_BIT | GL_TRANSFORM_BIT);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
        glOrtho(0, viewportWidth, 0, viewportHeight, -1, 1);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        auto drawBand = [&](GLuint image, int sourceTop, int sourceBottom) {
            const float y0 = float(480 - sourceBottom) * float(viewportHeight) / 480.f;
            const float y1 = float(480 - sourceTop) * float(viewportHeight) / 480.f;
            glBindTexture(GL_TEXTURE_2D, image);
            glColor4f(.68f, .86f, .9f, .48f); // source mask retained; tint awaits retail capture.
            glBegin(GL_QUADS);
            glTexCoord2f(0.f, 1.f); glVertex2f(0.f, y0);
            glTexCoord2f(1.f, 1.f); glVertex2f(float(viewportWidth), y0);
            glTexCoord2f(1.f, 0.f); glVertex2f(float(viewportWidth), y1);
            glTexCoord2f(0.f, 0.f); glVertex2f(0.f, y1);
            glEnd();
        };
        drawBand(topVisorTexture, 68, 100);
        drawBand(bottomVisorTexture, 380, 412);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        return 2;
    }

    int draw(const GogglesScan& scan, uint32_t now, float cameraYaw, float cameraPitch) const {
        if (!texture || !scan.effectWindowOpen(now)) return 0;
        const float half = 32.f; // Retained 64x64 source texture; world-space size is provisional.
        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                     GL_TEXTURE_BIT | GL_CURRENT_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        int drawn = 0;
        for (const auto& target : scan.targets) {
            glPushMatrix();
            glTranslatef(target.position.x, target.position.y + (target.kind == 2 ? 80.f : 48.f),
                         target.position.z); // Vertical attachment offset is a preview approximation.
            glRotatef(cameraYaw * (180.f / 3.14159265358979323846f), 0.f, 1.f, 0.f);
            glRotatef(cameraPitch * (180.f / 3.14159265358979323846f), 1.f, 0.f, 0.f);
            glColor4f(1.f, 1.f, 1.f, 0.9f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.f, 0.f); glVertex3f(-half, -half, 0.f);
            glTexCoord2f(1.f, 0.f); glVertex3f( half, -half, 0.f);
            glTexCoord2f(1.f, 1.f); glVertex3f( half,  half, 0.f);
            glTexCoord2f(0.f, 1.f); glVertex3f(-half,  half, 0.f);
            glEnd();
            glPopMatrix();
            ++drawn;
        }
        glPopAttrib();
        return drawn;
    }

    void release() {
        if (texture) glDeleteTextures(1, &texture);
        if (topVisorTexture) glDeleteTextures(1, &topVisorTexture);
        if (bottomVisorTexture) glDeleteTextures(1, &bottomVisorTexture);
        texture = topVisorTexture = bottomVisorTexture = 0;
    }
};
