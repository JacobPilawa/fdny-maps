#pragma once

#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

// Retail hint widgets use resource 1020: font1.bmp/font1_a.bmp, with 16x24
// cells and the glyph sequence recovered at EXE VA 004E39C8. The source-space
// bounds are known; horizontal/vertical alignment for mode (2,2) remains an
// approximation until that renderer branch is fully mapped.
struct HintOverlay {
    static constexpr int AtlasWidth = 256;
    static constexpr int AtlasHeight = 256;
    static constexpr int CellWidth = 16;
    static constexpr int CellHeight = 24;
    static constexpr int AtlasColumns = AtlasWidth / CellWidth;
    static constexpr float BoundsLeft = 120.f;
    static constexpr float BoundsBottom = 40.f;
    static constexpr float BoundsRight = 520.f;
    static constexpr float BoundsTop = 440.f;
    static constexpr std::string_view GlyphOrder =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+|`-=\\{}:\"<>?[];',./";

    struct Glyph {
        char value = ' ';
        std::array<uint8_t, 3> color{{255, 255, 255}};
    };

    GLuint fontTexture = 0;

    static_assert(GlyphOrder.size() == 94);

    // Kept for the separate mission chooser, whose preview typography is not
    // part of the retail Player hint widget.
    static std::array<uint8_t, 7> glyph(char value) {
        switch (char(std::toupper(static_cast<unsigned char>(value)))) {
            case ' ': return {0,0,0,0,0,0,0};
            case 'A': return {14,17,17,31,17,17,17}; case 'B': return {30,17,17,30,17,17,30};
            case 'C': return {14,17,16,16,16,17,14}; case 'D': return {30,17,17,17,17,17,30};
            case 'E': return {31,16,16,30,16,16,31}; case 'F': return {31,16,16,30,16,16,16};
            case 'G': return {14,17,16,23,17,17,15}; case 'H': return {17,17,17,31,17,17,17};
            case 'I': return {14,4,4,4,4,4,14}; case 'J': return {7,2,2,2,18,18,12};
            case 'K': return {17,18,20,24,20,18,17}; case 'L': return {16,16,16,16,16,16,31};
            case 'M': return {17,27,21,21,17,17,17}; case 'N': return {17,25,21,19,17,17,17};
            case 'O': return {14,17,17,17,17,17,14}; case 'P': return {30,17,17,30,16,16,16};
            case 'Q': return {14,17,17,17,21,18,13}; case 'R': return {30,17,17,30,20,18,17};
            case 'S': return {15,16,16,14,1,1,30}; case 'T': return {31,4,4,4,4,4,4};
            case 'U': return {17,17,17,17,17,17,14}; case 'V': return {17,17,17,17,17,10,4};
            case 'W': return {17,17,17,21,21,21,10}; case 'X': return {17,17,10,4,10,17,17};
            case 'Y': return {17,17,10,4,4,4,4}; case 'Z': return {31,1,2,4,8,16,31};
            case '0': return {14,17,19,21,25,17,14}; case '1': return {4,12,4,4,4,4,14};
            case '2': return {14,17,1,2,4,8,31}; case '3': return {30,1,1,14,1,1,30};
            case '4': return {2,6,10,18,31,2,2}; case '5': return {31,16,16,30,1,1,30};
            case '6': return {14,16,16,30,17,17,14}; case '7': return {31,1,2,4,8,8,8};
            case '8': return {14,17,17,14,17,17,14}; case '9': return {14,17,17,15,1,1,14};
            case '.': return {0,0,0,0,0,12,12}; case ',': return {0,0,0,0,12,12,8};
            case ':': return {0,12,12,0,12,12,0}; case ';': return {0,12,12,0,12,12,8};
            case '!': return {4,4,4,4,4,0,4}; case '?': return {14,17,1,2,4,0,4};
            case '-': return {0,0,0,31,0,0,0}; case '_': return {0,0,0,0,0,0,31};
            case '/': return {1,1,2,4,8,16,16}; case '\\': return {16,16,8,4,2,1,1};
            case '\'': return {4,4,2,0,0,0,0}; case '"': return {10,10,5,0,0,0,0};
            case '(': return {2,4,8,8,8,4,2}; case ')': return {8,4,2,2,2,4,8};
            case '+': return {0,4,4,31,4,4,0}; case '=': return {0,31,0,31,0,0,0};
            default: return {14,17,1,2,4,0,4};
        }
    }

    static bool lock(SDL_Surface* surface) {
        return !SDL_MUSTLOCK(surface) || SDL_LockSurface(surface) == 0;
    }

    static void unlock(SDL_Surface* surface) {
        if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    }

    void load(const std::filesystem::path& spriteRoot) {
        release();
        const auto colorPath = spriteRoot / "font1.bmp";
        const auto alphaPath = spriteRoot / "font1_a.bmp";
        SDL_Surface* colorBitmap = SDL_LoadBMP(colorPath.string().c_str());
        if (!colorBitmap)
            throw std::runtime_error("Cannot load retail hint font " + colorPath.string() +
                                     ": " + SDL_GetError());
        SDL_Surface* alphaBitmap = SDL_LoadBMP(alphaPath.string().c_str());
        if (!alphaBitmap) {
            SDL_FreeSurface(colorBitmap);
            throw std::runtime_error("Cannot load retail hint alpha font " + alphaPath.string() +
                                     ": " + SDL_GetError());
        }

        SDL_Surface* color = SDL_ConvertSurfaceFormat(colorBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_Surface* alpha = SDL_ConvertSurfaceFormat(alphaBitmap, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(colorBitmap);
        SDL_FreeSurface(alphaBitmap);
        if (!color || !alpha) {
            if (color) SDL_FreeSurface(color);
            if (alpha) SDL_FreeSurface(alpha);
            throw std::runtime_error(std::string("Cannot convert retail hint font: ") + SDL_GetError());
        }
        if (color->w != AtlasWidth || color->h != AtlasHeight ||
            alpha->w != color->w || alpha->h != color->h) {
            SDL_FreeSurface(alpha);
            SDL_FreeSurface(color);
            throw std::runtime_error("Retail hint font resources must be matching 256x256 atlases");
        }
        if (!lock(color)) {
            SDL_FreeSurface(alpha);
            SDL_FreeSurface(color);
            throw std::runtime_error(std::string("Cannot lock retail hint font: ") + SDL_GetError());
        }
        if (!lock(alpha)) {
            unlock(color);
            SDL_FreeSurface(alpha);
            SDL_FreeSurface(color);
            throw std::runtime_error(std::string("Cannot lock retail hint alpha font: ") + SDL_GetError());
        }

        std::vector<uint8_t> rgba(size_t(AtlasWidth) * AtlasHeight * 4);
        for (int y = 0; y < AtlasHeight; ++y) {
            for (int x = 0; x < AtlasWidth; ++x) {
                const auto* colorPixel = static_cast<const uint8_t*>(color->pixels) +
                                         y * color->pitch + x * 4;
                const auto* alphaPixel = static_cast<const uint8_t*>(alpha->pixels) +
                                         y * alpha->pitch + x * 4;
                const size_t offset = (size_t(y) * AtlasWidth + size_t(x)) * 4;
                rgba[offset + 0] = colorPixel[0];
                rgba[offset + 1] = colorPixel[1];
                rgba[offset + 2] = colorPixel[2];
                rgba[offset + 3] = alphaPixel[0];
            }
        }
        unlock(alpha);
        unlock(color);
        SDL_FreeSurface(alpha);
        SDL_FreeSurface(color);

        glGenTextures(1, &fontTexture);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, AtlasWidth, AtlasHeight, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void release() {
        if (fontTexture) glDeleteTextures(1, &fontTexture);
        fontTexture = 0;
    }

    static bool parseColor(std::string_view text, size_t& cursor,
                           std::array<uint8_t, 3>& color) {
        if (cursor + 2 > text.size() || text[cursor] != '^' || text[cursor + 1] != '(')
            return false;
        size_t end = text.find(')', cursor + 2);
        if (end == std::string_view::npos) return false;
        std::array<int, 3> values{};
        size_t valueIndex = 0;
        size_t position = cursor + 2;
        while (position < end && valueIndex < values.size()) {
            while (position < end && (text[position] == ' ' || text[position] == '\t')) ++position;
            const size_t begin = position;
            while (position < end && text[position] >= '0' && text[position] <= '9') ++position;
            if (begin == position) return false;
            int parsed = 0;
            for (size_t digit = begin; digit < position; ++digit) {
                const int next = text[digit] - '0';
                if (parsed > (255 - next) / 10) return false;
                parsed = parsed * 10 + next;
            }
            values[valueIndex++] = parsed;
            while (position < end && (text[position] == ' ' || text[position] == '\t')) ++position;
            if (valueIndex < values.size()) {
                if (position >= end || text[position] != ',') return false;
                ++position;
            }
        }
        while (position < end && (text[position] == ' ' || text[position] == '\t')) ++position;
        if (valueIndex != values.size() || position != end) return false;
        color = {{uint8_t(values[0]), uint8_t(values[1]), uint8_t(values[2])}};
        cursor = end + 1;
        return true;
    }

    static std::vector<Glyph> parse(std::string_view text) {
        std::vector<Glyph> result;
        std::array<uint8_t, 3> color{{255, 255, 255}};
        for (size_t cursor = 0; cursor < text.size();) {
            if (parseColor(text, cursor, color)) continue;
            char value = text[cursor++];
            if (value == '\r') continue;
            if (value == '\t') value = ' ';
            result.push_back({value, color});
        }
        return result;
    }

    static std::vector<std::vector<Glyph>> wrap(std::string_view text, size_t columns) {
        std::vector<std::vector<Glyph>> lines;
        std::vector<Glyph> line;
        std::vector<Glyph> word;
        bool pendingSpace = false;
        const Glyph space{' ', {{255, 255, 255}}};
        auto flushWord = [&]() {
            if (word.empty()) return;
            size_t consumed = 0;
            while (consumed < word.size()) {
                if (pendingSpace && !line.empty()) {
                    if (line.size() + 1 + (word.size() - consumed) > columns) {
                        lines.push_back(std::move(line));
                        line.clear();
                    } else {
                        line.push_back(space);
                    }
                }
                pendingSpace = false;
                if (line.size() == columns) {
                    lines.push_back(std::move(line));
                    line.clear();
                }
                const size_t available = columns - line.size();
                const size_t take = std::min(available, word.size() - consumed);
                line.insert(line.end(), word.begin() + std::ptrdiff_t(consumed),
                            word.begin() + std::ptrdiff_t(consumed + take));
                consumed += take;
                if (consumed < word.size()) {
                    lines.push_back(std::move(line));
                    line.clear();
                }
            }
            word.clear();
        };
        for (const Glyph& glyph : parse(text)) {
            if (glyph.value == '\n') {
                flushWord();
                lines.push_back(std::move(line));
                line.clear();
                pendingSpace = false;
            } else if (glyph.value == ' ') {
                flushWord();
                pendingSpace = true;
            } else {
                word.push_back(glyph);
            }
        }
        flushWord();
        if (!line.empty() || lines.empty() || (!text.empty() && text.back() == '\n'))
            lines.push_back(std::move(line));
        return lines;
    }

    int draw(const std::string& text, int viewportWidth, int viewportHeight) const {
        if (!fontTexture || text.empty() || viewportWidth <= 0 || viewportHeight <= 0) return 0;
        constexpr size_t maxColumns = size_t((BoundsRight - BoundsLeft) / CellWidth);
        const auto lines = wrap(text, maxColumns);
        if (lines.empty()) return 0;

        constexpr float uiWidth = 640.f;
        constexpr float uiHeight = 480.f;
        const float blockHeight = float(lines.size() * CellHeight);
        float firstLineTop = (BoundsBottom + BoundsTop + blockHeight) * .5f;
        if (firstLineTop > BoundsTop) firstLineTop = BoundsTop;
        const float scaleX = float(viewportWidth) / uiWidth;
        const float scaleY = float(viewportHeight) / uiHeight;

        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT |
                     GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT |
                     GL_SCISSOR_BIT);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glEnable(GL_SCISSOR_TEST);
        glScissor(int(std::lround(BoundsLeft * scaleX)),
                  int(std::lround(BoundsBottom * scaleY)),
                  int(std::lround((BoundsRight - BoundsLeft) * scaleX)),
                  int(std::lround((BoundsTop - BoundsBottom) * scaleY)));
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, uiWidth, 0, uiHeight, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        int drawnGlyphs = 0;
        glBegin(GL_QUADS);
        for (size_t lineIndex = 0; lineIndex < lines.size(); ++lineIndex) {
            const auto& line = lines[lineIndex];
            const float lineWidth = float(line.size() * CellWidth);
            const float left = (BoundsLeft + BoundsRight - lineWidth) * .5f;
            const float top = firstLineTop - float(lineIndex * CellHeight);
            const float bottom = top - CellHeight;
            for (size_t character = 0; character < line.size(); ++character) {
                const Glyph& glyph = line[character];
                const size_t index = GlyphOrder.find(glyph.value);
                if (glyph.value == ' ' || index == std::string_view::npos) continue;
                const int atlasColumn = int(index % AtlasColumns);
                const int atlasRow = int(index / AtlasColumns);
                const float u0 = (float(atlasColumn * CellWidth) + .5f) / AtlasWidth;
                const float u1 = (float((atlasColumn + 1) * CellWidth) - .5f) / AtlasWidth;
                const float tTop = (float(atlasRow * CellHeight) + .5f) / AtlasHeight;
                const float tBottom = (float((atlasRow + 1) * CellHeight) - .5f) / AtlasHeight;
                const float x0 = left + float(character * CellWidth);
                const float x1 = x0 + CellWidth;
                glColor4ub(glyph.color[0], glyph.color[1], glyph.color[2], 255);
                glTexCoord2f(u0, tBottom); glVertex2f(x0, bottom);
                glTexCoord2f(u1, tBottom); glVertex2f(x1, bottom);
                glTexCoord2f(u1, tTop); glVertex2f(x1, top);
                glTexCoord2f(u0, tTop); glVertex2f(x0, top);
                ++drawnGlyphs;
            }
        }
        glEnd();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
        return drawnGlyphs;
    }
};

struct RetailHintLifetime {
    uint32_t startTick = 0;
    float durationSeconds = 0.f;
    bool configured = false;

    void begin(uint32_t now, float seconds) {
        startTick = now;
        durationSeconds = seconds;
        configured = std::isfinite(seconds);
    }

    bool visible(uint32_t now) const {
        if (!configured || !(now > startTick)) return false;
        // Retail's ordinary no-wrap path uses strict start/end comparisons;
        // 004E0ED4 supplies the float value 1000.0 for seconds-to-ticks.
        const float durationMs = durationSeconds * 1000.0f;
        const double endTick = double(startTick) + double(durationMs);
        return double(now) < endTick;
    }
};
