#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Small cross-platform bitmap HUD for authored ScriptActor hint messages.
// Message content and lifetime come from the script route; this panel's type,
// placement, font, and color are a readable preview approximation.
struct HintOverlay {
    static constexpr int pixelSize = 2;
    static constexpr int cellWidth = 12;
    static constexpr int lineHeight = 22;

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

    static std::vector<std::string> wrap(const std::string& text, size_t columns) {
        std::vector<std::string> lines;
        std::istringstream words(text);
        std::string word, line;
        while (words >> word) {
            while (word.size() > columns) {
                if (!line.empty()) { lines.push_back(line); line.clear(); }
                lines.push_back(word.substr(0, columns));
                word.erase(0, columns);
            }
            if (!line.empty() && line.size() + 1 + word.size() > columns) {
                lines.push_back(line);
                line.clear();
            }
            if (!line.empty()) line += ' ';
            line += word;
        }
        if (!line.empty()) lines.push_back(line);
        if (lines.size() > 5) {
            lines.resize(5);
            if (lines.back().size() > 3) lines.back().replace(lines.back().size() - 3, 3, "...");
        }
        return lines;
    }

    void draw(const std::string& text, int viewportWidth, int viewportHeight) const {
        if (text.empty() || viewportWidth < 64 || viewportHeight < 64) return;
        const float available = float(viewportWidth - 32);
        const size_t columns = std::max<size_t>(1, size_t(std::max(0.f, available - 32.f) / cellWidth));
        const auto lines = wrap(text, columns);
        if (lines.empty()) return;

        const float panelWidth = std::min(available, float(std::max<size_t>(1, std::max_element(
            lines.begin(), lines.end(), [](const auto& a, const auto& b) { return a.size() < b.size(); })->size()) * cellWidth + 32));
        const float panelHeight = float(lines.size() * lineHeight + 24);
        const float left = (viewportWidth - panelWidth) * .5f;
        const float sourcePanelCenterY = 240.f; // retail Hint widget rectangle center in its 640x480 UI space.
        const float bottom = std::clamp(sourcePanelCenterY * float(viewportHeight) / 480.f -
                                        panelHeight * .5f, 8.f,
                                        std::max(8.f, float(viewportHeight) - panelHeight - 8.f));

        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_LINE_BIT |
                     GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
        glOrtho(0, viewportWidth, 0, viewportHeight, -1, 1);
        glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

        glColor4f(.015f, .025f, .035f, .88f);
        glBegin(GL_QUADS);
        glVertex2f(left, bottom); glVertex2f(left + panelWidth, bottom);
        glVertex2f(left + panelWidth, bottom + panelHeight); glVertex2f(left, bottom + panelHeight);
        glEnd();
        glColor4f(.72f, .82f, .82f, .82f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(left, bottom); glVertex2f(left + panelWidth, bottom);
        glVertex2f(left + panelWidth, bottom + panelHeight); glVertex2f(left, bottom + panelHeight);
        glEnd();

        glColor4f(.95f, .98f, 1.f, 1.f);
        glBegin(GL_QUADS);
        for (size_t line = 0; line < lines.size(); ++line) {
            const float yTop = bottom + panelHeight - 12.f - float(line) * lineHeight;
            for (size_t character = 0; character < lines[line].size(); ++character) {
                const auto rows = glyph(lines[line][character]);
                const float x = left + 16.f + float(character) * cellWidth;
                for (int row = 0; row < 7; ++row) for (int column = 0; column < 5; ++column) {
                    if ((rows[size_t(row)] & (1u << (4 - column))) == 0) continue;
                    const float x0 = x + column * pixelSize;
                    const float y0 = yTop - (row + 1) * pixelSize;
                    glVertex2f(x0, y0); glVertex2f(x0 + pixelSize - .2f, y0);
                    glVertex2f(x0 + pixelSize - .2f, y0 + pixelSize - .2f);
                    glVertex2f(x0, y0 + pixelSize - .2f);
                }
            }
        }
        glEnd();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION); glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopAttrib();
    }
};
