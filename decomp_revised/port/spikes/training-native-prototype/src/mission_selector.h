#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <SDL.h>

#include "hint_overlay.h"

struct RescueMissionChoice {
    int catalogOrdinal = -1;
    std::string key;
    std::string title;
    std::filesystem::path scenePackage;
};

inline std::vector<RescueMissionChoice> loadRescueMissionChoices(
    const std::filesystem::path& resourcesRoot) {
    const auto manifestPath = resourcesRoot / "rescue_missions.json";
    J raw = json_object_from_file(manifestPath.string().c_str());
    if (!raw)
        throw std::runtime_error("Cannot load rescue mission catalog: " + manifestPath.string());
    SceneJson manifest(raw);
    if (str(field(manifest.get(), "format")) != "fdny-rescue-mission-catalog-v1")
        throw std::runtime_error("Unsupported rescue mission catalog format: " + manifestPath.string());
    J entries = field(manifest.get(), "entries");
    if (json_object_get_type(entries) != json_type_array || count(entries) != 8)
        throw std::runtime_error("The bundled rescue catalog must contain all eight retail rescue entries");

    std::vector<RescueMissionChoice> result;
    std::set<std::string> seen;
    result.reserve(size_t(count(entries)));
    int priorOrdinal = -1;
    for (int i = 0; i < count(entries); ++i) {
        J entry = at(entries, i);
        RescueMissionChoice choice;
        choice.catalogOrdinal = integer(field(entry, "catalog_ordinal"));
        choice.key = str(field(entry, "key"));
        choice.title = str(field(entry, "title"));
        const std::string rrf = str(field(entry, "rrf"));
        std::string foldedRrf = rrf;
        std::transform(foldedRrf.begin(), foldedRrf.end(), foldedRrf.begin(), [](unsigned char ch) {
            return char(std::tolower(ch));
        });
        if (choice.catalogOrdinal <= priorOrdinal || choice.key.empty() || choice.title.empty() ||
            foldedRrf != "rescue.rrf" || !seen.insert(choice.key).second)
            throw std::runtime_error("Invalid or unordered entry in bundled rescue catalog");
        if (!std::all_of(choice.key.begin(), choice.key.end(), [](unsigned char ch) {
                return std::isalnum(ch) || ch == '_';
            }))
            throw std::runtime_error("Unsafe level key in bundled rescue catalog: " + choice.key);
        choice.scenePackage = resourcesRoot / "scenes" / choice.key;
        if (!std::filesystem::is_directory(choice.scenePackage))
            throw std::runtime_error("Rescue scene package is missing: " + choice.scenePackage.string());
        priorOrdinal = choice.catalogOrdinal;
        result.push_back(std::move(choice));
    }
    return result;
}

inline void drawRescueMenuText(float x, float y, const std::string& text,
                               float scale, float red, float green, float blue) {
    glColor4f(red, green, blue, 1.f);
    glBegin(GL_QUADS);
    float cursor = x;
    for (unsigned char source : text) {
        const char value = char(std::toupper(source));
        const auto rows = HintOverlay::glyph(value);
        for (int row = 0; row < 7; ++row) {
            for (int column = 0; column < 5; ++column) {
                if ((rows[size_t(row)] & (1u << (4 - column))) == 0) continue;
                const float x0 = cursor + float(column) * scale;
                const float y0 = y + float(6 - row) * scale;
                glVertex2f(x0, y0);
                glVertex2f(x0 + scale * .86f, y0);
                glVertex2f(x0 + scale * .86f, y0 + scale * .86f);
                glVertex2f(x0, y0 + scale * .86f);
            }
        }
        cursor += 6.f * scale;
    }
    glEnd();
}

inline GLuint loadRetailMenuBitmap(const std::filesystem::path& bitmapPath) {
    if (!std::filesystem::is_regular_file(bitmapPath))
        throw std::runtime_error("Retail menu bitmap is missing: " + bitmapPath.string());
    SDL_Surface* loaded = SDL_LoadBMP(bitmapPath.string().c_str());
    if (!loaded)
        throw std::runtime_error("Cannot load retail menu bitmap " + bitmapPath.string() + ": " + SDL_GetError());
    SDL_Surface* rgba = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(loaded);
    if (!rgba)
        throw std::runtime_error(std::string("Cannot convert retail menu bitmap: ") + SDL_GetError());

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rgba->w, rgba->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, rgba->pixels);
    SDL_FreeSurface(rgba);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

// The authored retail title bitmap is used directly. Button placement and
// input semantics remain port UI until the retail menu widgets are traced.
inline void drawRescueTitleMenu(int width, int height, size_t selected, GLuint backgroundTexture) {
    glViewport(0, 0, width, height);
    glClearColor(.018f, .032f, .046f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT |
                 GL_LINE_BIT | GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    const float targetAspect = 4.f / 3.f;
    const float windowAspect = float(width) / float(std::max(height, 1));
    float backgroundLeft = 0.f, backgroundBottom = 0.f;
    float backgroundRight = float(width), backgroundTop = float(height);
    if (windowAspect > targetAspect) {
        const float contentWidth = float(height) * targetAspect;
        backgroundLeft = (float(width) - contentWidth) * .5f;
        backgroundRight = backgroundLeft + contentWidth;
    } else {
        const float contentHeight = float(width) / targetAspect;
        backgroundBottom = (float(height) - contentHeight) * .5f;
        backgroundTop = backgroundBottom + contentHeight;
    }
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 1.f); glVertex2f(backgroundLeft, backgroundBottom);
    glTexCoord2f(1.f, 1.f); glVertex2f(backgroundRight, backgroundBottom);
    glTexCoord2f(1.f, 0.f); glVertex2f(backgroundRight, backgroundTop);
    glTexCoord2f(0.f, 0.f); glVertex2f(backgroundLeft, backgroundTop);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    const std::array<std::string, 2> labels = {"START RESCUE", "QUIT"};
    const float rowHeight = std::min(52.f, float(height) * .075f);
    const float rowPitch = std::min(64.f, float(height) * .092f);
    const float rowTop = float(height) * .47f;
    const float left = float(width) * .32f;
    const float right = float(width) * .68f;
    for (size_t i = 0; i < labels.size(); ++i) {
        const float top = rowTop - float(i) * rowPitch;
        const float bottom = top - rowHeight;
        const bool isSelected = i == selected;
        glColor4f(isSelected ? .48f : .08f, isSelected ? .075f : .105f,
                  isSelected ? .075f : .13f, isSelected ? .96f : .78f);
        glBegin(GL_QUADS);
        glVertex2f(left, bottom); glVertex2f(right, bottom);
        glVertex2f(right, top); glVertex2f(left, top);
        glEnd();
        glColor4f(isSelected ? .95f : .28f, isSelected ? .42f : .34f,
                  isSelected ? .22f : .38f, .9f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(left, bottom); glVertex2f(right, bottom);
        glVertex2f(right, top); glVertex2f(left, top);
        glEnd();
        const float textScale = std::clamp(rowHeight / 10.f, 2.2f, 3.8f);
        const float textX = (float(width) - labels[i].size() * 6.f * textScale) * .5f;
        const float textY = bottom + (rowHeight - 7.f * textScale) * .5f;
        drawRescueMenuText(textX, textY, labels[i], textScale,
                           isSelected ? 1.f : .80f,
                           isSelected ? .93f : .86f,
                           isSelected ? .83f : .88f);
    }

    const std::string controls = "UP DOWN SELECT    ENTER CONFIRM";
    const float controlsScale = std::clamp(float(width) / 700.f, 1.6f, 2.2f);
    drawRescueMenuText((float(width) - controls.size() * 6.f * controlsScale) * .5f,
                       float(height) * .075f, controls, controlsScale,
                       .68f, .76f, .78f);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}

inline bool chooseRescueTitleMenu(const std::filesystem::path& backgroundPath) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        throw std::runtime_error(std::string("SDL title menu initialization failed: ") + SDL_GetError());
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_Window* window = SDL_CreateWindow("FDNY Firefighters: American Hero",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1120, 800,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        const std::string message = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error("Cannot create title menu window: " + message);
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        const std::string message = SDL_GetError();
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Cannot create title menu OpenGL context: " + message);
    }
    SDL_GL_SetSwapInterval(1);
    const GLuint backgroundTexture = loadRetailMenuBitmap(backgroundPath);

    size_t selected = 0;
    std::optional<bool> result;
    bool running = true;
    while (running && !result) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { running = false; break; }
            if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                if (event.key.keysym.sym == SDLK_ESCAPE) { running = false; break; }
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
                    selected = 1 - selected;
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
                    result = selected == 0;
            }
            if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN) {
                int logicalWidth = 1, logicalHeight = 1, drawableWidth = 1, drawableHeight = 1;
                SDL_GetWindowSize(window, &logicalWidth, &logicalHeight);
                SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
                const int mouseX = (event.type == SDL_MOUSEMOTION ? event.motion.x : event.button.x) *
                    drawableWidth / std::max(1, logicalWidth);
                const int mouseY = drawableHeight - (event.type == SDL_MOUSEMOTION ? event.motion.y : event.button.y) *
                    drawableHeight / std::max(1, logicalHeight);
                const float left = float(drawableWidth) * .32f;
                const float right = float(drawableWidth) * .68f;
                const float rowHeight = std::min(52.f, float(drawableHeight) * .075f);
                const float rowPitch = std::min(64.f, float(drawableHeight) * .092f);
                const float rowTop = float(drawableHeight) * .47f;
                if (mouseX >= left && mouseX <= right) {
                    for (size_t i = 0; i < 2; ++i) {
                        const float top = rowTop - float(i) * rowPitch;
                        if (mouseY <= top && mouseY >= top - rowHeight) {
                            selected = i;
                            if (event.type == SDL_MOUSEBUTTONDOWN &&
                                event.button.button == SDL_BUTTON_LEFT)
                                result = selected == 0;
                            break;
                        }
                    }
                }
            }
        }
        int width = 1, height = 1;
        SDL_GL_GetDrawableSize(window, &width, &height);
        drawRescueTitleMenu(width, height, selected, backgroundTexture);
        SDL_GL_SwapWindow(window);
        SDL_Delay(8);
    }

    glDeleteTextures(1, &backgroundTexture);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return result.value_or(false);
}

inline float drawRescueMissionChooser(const std::vector<RescueMissionChoice>& choices,
                                      size_t selected, int width, int height) {
    glViewport(0, 0, width, height);
    glClearColor(.018f, .032f, .046f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT |
                 GL_LINE_BIT | GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    glBegin(GL_QUADS);
    glColor4f(.028f, .046f, .062f, 1.f); glVertex2f(0.f, 0.f);
    glColor4f(.055f, .075f, .086f, 1.f); glVertex2f(float(width), 0.f);
    glColor4f(.018f, .031f, .046f, 1.f); glVertex2f(float(width), float(height));
    glColor4f(.026f, .040f, .055f, 1.f); glVertex2f(0.f, float(height));
    glEnd();

    const std::string heading = "FDNY FIRE RESCUE";
    const float headingScale = std::clamp(float(width) / 330.f, 2.5f, 4.0f);
    drawRescueMenuText((float(width) - heading.size() * 6.f * headingScale) * .5f,
                       float(height) * .88f, heading, headingScale, .95f, .97f, .98f);
    const std::string subheading = "SELECT A RESCUE MISSION";
    const float subtitleScale = std::clamp(float(width) / 500.f, 1.8f, 2.8f);
    drawRescueMenuText((float(width) - subheading.size() * 6.f * subtitleScale) * .5f,
                       float(height) * .82f, subheading, subtitleScale, .78f, .84f, .85f);

    const float rowHeight = std::min(42.f, float(height) * .054f);
    const float rowPitch = std::min(50.f, float(height) * .064f);
    const float rowTop = float(height) * .75f;
    const float left = float(width) * .18f;
    const float right = float(width) * .82f;
    for (size_t i = 0; i < choices.size(); ++i) {
        const float top = rowTop - float(i) * rowPitch;
        const float bottom = top - rowHeight;
        const bool isSelected = i == selected;
        glColor4f(isSelected ? .48f : .08f, isSelected ? .075f : .105f,
                  isSelected ? .075f : .13f, isSelected ? .96f : .78f);
        glBegin(GL_QUADS);
        glVertex2f(left, bottom); glVertex2f(right, bottom);
        glVertex2f(right, top); glVertex2f(left, top);
        glEnd();
        glColor4f(isSelected ? .95f : .28f, isSelected ? .42f : .34f,
                  isSelected ? .22f : .38f, .9f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(left, bottom); glVertex2f(right, bottom);
        glVertex2f(right, top); glVertex2f(left, top);
        glEnd();

        const std::string number = std::to_string(i + 1) + "  ";
        const std::string text = number + choices[i].title;
        const float textScale = std::clamp(rowHeight / 10.f, 2.0f, 3.4f);
        const float textY = bottom + (rowHeight - 7.f * textScale) * .5f;
        drawRescueMenuText(left + 18.f, textY, text, textScale,
                           isSelected ? 1.f : .80f,
                           isSelected ? .93f : .86f,
                           isSelected ? .83f : .88f);
    }

    const std::string controls = "UP DOWN SELECT    ENTER EXPLORE    ESC BACK";
    const float controlsScale = std::clamp(float(width) / 700.f, 1.6f, 2.2f);
    drawRescueMenuText((float(width) - controls.size() * 6.f * controlsScale) * .5f,
                       float(height) * .075f, controls, controlsScale,
                       .68f, .76f, .78f);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
    return rowPitch;
}

inline std::optional<size_t> chooseRescueMission(
    const std::vector<RescueMissionChoice>& choices) {
    if (choices.empty()) throw std::runtime_error("Rescue mission chooser has no entries");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        throw std::runtime_error(std::string("SDL mission chooser initialization failed: ") + SDL_GetError());
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_Window* window = SDL_CreateWindow("FDNY Fire Rescue — Mission Select",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1120, 800,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        const std::string message = SDL_GetError();
        SDL_Quit();
        throw std::runtime_error("Cannot create mission chooser window: " + message);
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        const std::string message = SDL_GetError();
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Cannot create mission chooser OpenGL context: " + message);
    }
    SDL_GL_SetSwapInterval(1);

    std::optional<size_t> result;
    size_t selected = 0;
    bool running = true;
    while (running && !result) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
                else if (event.key.keysym.sym == SDLK_UP && selected > 0) --selected;
                else if (event.key.keysym.sym == SDLK_DOWN && selected + 1 < choices.size()) ++selected;
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
                    result = selected;
            }
            if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN) {
                int logicalWidth = 1, logicalHeight = 1, drawableWidth = 1, drawableHeight = 1;
                SDL_GetWindowSize(window, &logicalWidth, &logicalHeight);
                SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
                const int mouseX = (event.type == SDL_MOUSEMOTION ? event.motion.x : event.button.x) *
                    drawableWidth / std::max(1, logicalWidth);
                const int mouseY = drawableHeight - (event.type == SDL_MOUSEMOTION ? event.motion.y : event.button.y) *
                    drawableHeight / std::max(1, logicalHeight);
                const float left = float(drawableWidth) * .18f;
                const float right = float(drawableWidth) * .82f;
                const float rowHeight = std::min(42.f, float(drawableHeight) * .054f);
                const float rowPitch = std::min(50.f, float(drawableHeight) * .064f);
                const float rowTop = float(drawableHeight) * .75f;
                if (mouseX >= left && mouseX <= right) {
                    for (size_t i = 0; i < choices.size(); ++i) {
                        const float top = rowTop - float(i) * rowPitch;
                        if (mouseY <= top && mouseY >= top - rowHeight) {
                            selected = i;
                            if (event.type == SDL_MOUSEBUTTONDOWN &&
                                event.button.button == SDL_BUTTON_LEFT) result = i;
                            break;
                        }
                    }
                }
            }
        }
        int width = 1, height = 1;
        SDL_GL_GetDrawableSize(window, &width, &height);
        drawRescueMissionChooser(choices, selected, width, height);
        SDL_GL_SwapWindow(window);
        SDL_Delay(8);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return result;
}
