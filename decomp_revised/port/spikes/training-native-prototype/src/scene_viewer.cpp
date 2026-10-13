#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#define GL_GLEXT_PROTOTYPES 1
#include <OpenGL/OpenGL.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#endif
#include <json-c/json.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "scene_data.h"

using J = json_object*;
J field(J object, const char* key) { return SceneData::field(object, key); }
J at(J array, int index) { return json_object_array_get_idx(array, size_t(index)); }
int count(J value) { return value && json_object_get_type(value) == json_type_array ? int(json_object_array_length(value)) : 0; }
float num(J value) { return value ? float(json_object_get_double(value)) : 0.f; }
int integer(J value) { return value ? json_object_get_int(value) : 0; }
std::string str(J value) { return value && json_object_get_type(value) == json_type_string ? json_object_get_string(value) : ""; }

#include "core.h"

namespace {
#if defined(__APPLE__)
struct HeadlessOpenGL {
    CGLPixelFormatObj pixelFormat = nullptr;
    CGLContextObj context = nullptr;
    GLuint framebuffer = 0;
    GLuint colorTexture = 0;
    GLuint depthBuffer = 0;

    void create(int width, int height) {
        CGLPixelFormatAttribute attributes[] = {
            kCGLPFAAllowOfflineRenderers, static_cast<CGLPixelFormatAttribute>(1),
            kCGLPFAColorSize, static_cast<CGLPixelFormatAttribute>(24),
            kCGLPFAAlphaSize, static_cast<CGLPixelFormatAttribute>(8),
            kCGLPFADepthSize, static_cast<CGLPixelFormatAttribute>(24),
            static_cast<CGLPixelFormatAttribute>(0),
        };
        GLint formats = 0;
        CGLError error = CGLChoosePixelFormat(attributes, &pixelFormat, &formats);
        if (error != kCGLNoError || formats == 0)
            throw std::runtime_error("CGL could not choose an offscreen pixel format: " + std::to_string(error));
        error = CGLCreateContext(pixelFormat, nullptr, &context);
        if (error != kCGLNoError)
            throw std::runtime_error("CGL could not create an offscreen context: " + std::to_string(error));
        error = CGLSetCurrentContext(context);
        if (error != kCGLNoError)
            throw std::runtime_error("CGL could not activate its offscreen context: " + std::to_string(error));

        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glGenFramebuffersEXT(1, &framebuffer);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colorTexture, 0);
        glGenRenderbuffersEXT(1, &depthBuffer);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
        GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
            throw std::runtime_error("CGL offscreen framebuffer is incomplete: " + std::to_string(status));
        GLenum glError = glGetError();
        if (glError != GL_NO_ERROR)
            throw std::runtime_error("CGL offscreen framebuffer setup failed with OpenGL error " + std::to_string(glError));
    }

    const char* version() const {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

    void destroy() {
        if (context && CGLGetCurrentContext() == context) {
            if (depthBuffer) glDeleteRenderbuffersEXT(1, &depthBuffer);
            if (framebuffer) glDeleteFramebuffersEXT(1, &framebuffer);
            if (colorTexture) glDeleteTextures(1, &colorTexture);
            CGLSetCurrentContext(nullptr);
        }
        if (context) CGLDestroyContext(context);
        if (pixelFormat) CGLDestroyPixelFormat(pixelFormat);
        context = nullptr;
        pixelFormat = nullptr;
        framebuffer = colorTexture = depthBuffer = 0;
    }

    ~HeadlessOpenGL() { destroy(); }
};
#endif

struct SceneMesh {
    int model = 0;
    std::vector<float> positions, uvs;
    GLuint texture = 0;
};

struct TextureLoadSummary {
    bool uploadAttempted = false;
    int uniqueFiles = 0;
    int decodedFiles = 0;
    int uploadedFiles = 0;
    int failedFiles = 0;
    std::vector<std::string> errors;
};

std::string property(J entity, const char* name) {
    return str(field(field(entity, "properties"), name));
}

V parseVector(const std::string& text, V fallback) {
    V value;
    if (std::sscanf(text.c_str(), "%f %f %f", &value.x, &value.y, &value.z) != 3) return fallback;
    return value;
}

float parseFloat(const std::string& text, float fallback) {
    if (text.empty() || text == "<null>") return fallback;
    char* end = nullptr;
    float value = std::strtof(text.c_str(), &end);
    return end == text.c_str() ? fallback : value;
}

std::vector<SceneMesh> loadMeshes(const SceneData& scene, TextureLoadSummary& textureSummary, bool uploadTextures) {
    textureSummary.uploadAttempted = uploadTextures;
    std::vector<SceneMesh> meshes;
    std::map<std::string, GLuint> textureCache;
    std::set<std::string> seenFiles;
    J groups = field(scene.worldMesh(), "groups");
    meshes.reserve(size_t(count(groups)));
    for (int i = 0; i < count(groups); ++i) {
        J group = at(groups, i);
        // These named BSP materials are collision/helper surfaces. Training's
        // existing renderer omits them from the visible world; collision.json
        // remains the source for their physical behavior.
        if (str(field(group, "texture")).rfind("col_", 0) == 0) continue;
        SceneMesh mesh;
        mesh.model = integer(field(group, "model"));
        J positions = field(group, "positions"), uvs = field(group, "uvs");
        if (count(positions) % 3 != 0 || count(uvs) % 2 != 0)
            throw std::runtime_error("Invalid vertex or UV array in scene mesh group " + std::to_string(i));
        mesh.positions.reserve(size_t(count(positions)));
        mesh.uvs.reserve(size_t(count(uvs)));
        for (int k = 0; k < count(positions); ++k) mesh.positions.push_back(num(at(positions, k)));
        for (int k = 0; k < count(uvs); ++k) mesh.uvs.push_back(num(at(uvs, k)));

        std::string textureFile = str(field(group, "texture_file"));
        if (textureFile.empty()) textureFile = "textures/" + str(field(group, "texture")) + ".bmp";
        try {
            auto path = scene.resolveAsset(textureFile);
            const std::string cacheKey = path.string();
            if (!seenFiles.insert(cacheKey).second) {
                auto cached = textureCache.find(cacheKey);
                if (cached != textureCache.end()) mesh.texture = cached->second;
                meshes.push_back(std::move(mesh));
                continue;
            }
            ++textureSummary.uniqueFiles;
            SDL_Surface* surface = SDL_LoadBMP(path.string().c_str());
            if (!surface) throw std::runtime_error(SDL_GetError());
            ++textureSummary.decodedFiles;
            if (!uploadTextures) {
                SDL_FreeSurface(surface);
                meshes.push_back(std::move(mesh));
                continue;
            }
            SDL_Surface* rgba = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
            SDL_FreeSurface(surface);
            if (!rgba) throw std::runtime_error(SDL_GetError());
            glGenTextures(1, &mesh.texture);
            if (!mesh.texture) {
                SDL_FreeSurface(rgba);
                throw std::runtime_error("OpenGL did not allocate a texture name");
            }
            glBindTexture(GL_TEXTURE_2D, mesh.texture);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgba->w, rgba->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba->pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            SDL_FreeSurface(rgba);
            GLenum glError = glGetError();
            if (glError != GL_NO_ERROR) {
                glDeleteTextures(1, &mesh.texture);
                mesh.texture = 0;
                throw std::runtime_error("OpenGL texture upload failed with error " + std::to_string(glError));
            }
            ++textureSummary.uploadedFiles;
            textureCache.emplace(cacheKey, mesh.texture);
        } catch (const std::exception& error) {
            ++textureSummary.failedFiles;
            if (textureSummary.errors.size() < 8)
                textureSummary.errors.push_back(textureFile + ": " + error.what());
        }
        meshes.push_back(std::move(mesh));
    }
    return meshes;
}

void clearTextures(std::vector<SceneMesh>& meshes) {
    std::set<GLuint> textures;
    for (const auto& mesh : meshes) if (mesh.texture) textures.insert(mesh.texture);
    for (GLuint texture : textures) glDeleteTextures(1, &texture);
}

std::filesystem::path saveRenderCheckFrame(const std::filesystem::path& package,
                                           const std::vector<uint8_t>& bottomUpPixels,
                                           int width, int height) {
    std::vector<uint8_t> topDownPixels(bottomUpPixels.size());
    const size_t rowBytes = size_t(width) * 4;
    for (int y = 0; y < height; ++y) {
        std::copy_n(bottomUpPixels.data() + size_t(height - 1 - y) * rowBytes, rowBytes,
                    topDownPixels.data() + size_t(y) * rowBytes);
    }
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(topDownPixels.data(), width, height, 32,
                                                              width * 4, SDL_PIXELFORMAT_RGBA32);
    if (!surface) throw std::runtime_error(std::string("Cannot allocate render-check image: ") + SDL_GetError());
    std::filesystem::path output = package;
    output += ".render-check.bmp";
    int saveResult = SDL_SaveBMP(surface, output.string().c_str());
    std::string error = SDL_GetError();
    SDL_FreeSurface(surface);
    if (saveResult != 0)
        throw std::runtime_error("Cannot save render-check image " + output.string() + ": " + error);
    return output;
}

void renderScene(const std::vector<SceneMesh>& meshes, const SceneData& scene,
                 const Collision& collision, V camera, float yaw, float pitch,
                 int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double nearPlane = 1.0, top = nearPlane * std::tan(60.0 * 3.141592653589793 / 360.0);
    const double right = top * (double(width) / double(std::max(height, 1)));
    glFrustum(-right, right, -top, top, nearPlane, 30000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-pitch * 57.29577951308232f, 1.f, 0.f, 0.f);
    glRotatef(-yaw * 57.29577951308232f, 0.f, 1.f, 0.f);
    glTranslatef(-camera.x, -camera.y, -camera.z);

    glClearColor(.08f, .10f, .12f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    for (const auto& mesh : meshes) {
        if (collision.disabled.count(mesh.model)) continue;
        glPushMatrix();
        float angle = 0.f;
        auto angleIt = collision.angles.find(mesh.model);
        if (angleIt != collision.angles.end()) angle = angleIt->second;
        auto poseIt = collision.poses.find(mesh.model);
        if (angle != 0.f || poseIt != collision.poses.end()) {
            J models = field(scene.collision(), "models");
            V pivot{};
            for (int k = 0; k < count(models); ++k) {
                J model = at(models, k);
                if (integer(field(model, "index")) == mesh.model) { pivot = vec(field(model, "origin")); break; }
            }
            Pose pose;
            if (poseIt != collision.poses.end()) pose = poseIt->second;
            else pose.rotation = {std::cos(angle * .5f), 0.f, std::sin(angle * .5f), 0.f};
            V x = rotateQ({1, 0, 0}, pose.rotation), y = rotateQ({0, 1, 0}, pose.rotation), z = rotateQ({0, 0, 1}, pose.rotation);
            GLfloat matrix[16] = {x.x,x.y,x.z,0, y.x,y.y,y.z,0, z.x,z.y,z.z,0, 0,0,0,1};
            glTranslatef(pivot.x + pose.translation.x, pivot.y + pose.translation.y, pivot.z + pose.translation.z);
            glMultMatrixf(matrix);
            glTranslatef(-pivot.x, -pivot.y, -pivot.z);
        }
        if (mesh.texture) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh.texture);
            glColor3f(1.f, 1.f, 1.f);
        } else {
            glDisable(GL_TEXTURE_2D);
            glColor3f(.65f, .68f, .70f);
        }
        glBegin(GL_TRIANGLES);
        for (size_t k = 0; k < mesh.positions.size() / 3; ++k) {
            if (k * 2 + 1 < mesh.uvs.size()) glTexCoord2f(mesh.uvs[k * 2], mesh.uvs[k * 2 + 1]);
            glVertex3f(mesh.positions[k * 3], mesh.positions[k * 3 + 1], mesh.positions[k * 3 + 2]);
        }
        glEnd();
        glPopMatrix();
    }
}

void printInventory(const SceneData& scene, const std::vector<SceneMesh>& meshes, const TextureLoadSummary& textures) {
    J report = field(scene.report(), "counts");
    size_t triangles = 0;
    for (const auto& mesh : meshes) triangles += mesh.positions.size() / 9;
    std::cout << "Scene: " << scene.key() << " (" << scene.sourceFile() << ")\n"
              << "BSP SHA-256: " << scene.sourceSha256() << "\n"
              << "Entities: " << count(scene.entities())
              << ", render groups: " << meshes.size() << "/" << count(field(scene.worldMesh(), "groups"))
              << " (" << count(field(scene.worldMesh(), "groups")) - int(meshes.size()) << " collision/helper groups skipped)"
              << ", collision models: " << count(field(scene.collision(), "models"))
              << ", nodes: " << count(field(scene.collision(), "nodes"))
              << ", leaves: " << count(field(scene.collision(), "leafs")) << "\n"
              << "BSP faces: " << integer(field(report, "faces"))
              << ", render triangles: " << triangles
              << ", texture files: " << textures.decodedFiles << "/" << textures.uniqueFiles << " decoded"
              << (textures.uploadAttempted ? ", " + std::to_string(textures.uploadedFiles) + " uploaded" : ", GL upload skipped in inspect mode")
              << ", texture failures: " << textures.failedFiles << "\n";
    for (const auto& error : textures.errors) std::cout << "  texture error: " << error << "\n";
}

void printSpawnCollision(const SceneData& scene, V spawn, Collision& collision) {
    Hit floorHit;
    Player player;
    player.p = spawn;
    const V minimum{-30.f, -30.f, -30.f};
    const V maximum{30.f, 85.f, 30.f};
    bool floorFound = collision.trace(spawn, {0.f, -3000.f, 0.f}, minimum, maximum, floorHit);
    bool occupied = collision.solid(spawn, minimum, maximum);
    std::cout << "Collision at spawn: " << (occupied ? "overlapping solid" : "clear") << "; floor sweep: ";
    if (floorFound)
        std::cout << "hit model " << floorHit.model << " at (" << floorHit.point.x << ", " << floorHit.point.y
                  << ", " << floorHit.point.z << ") normal (" << floorHit.normal.x << ", " << floorHit.normal.y
                  << ", " << floorHit.normal.z << ")\n";
    else std::cout << "no hit in 3000 units\n";
    for (int frame = 0; frame < 300; ++frame) player.step(1.f / 60.f, 0.f, 0.f, 0.f, false, false, false, collision);
    std::cout << "Idle fall trace (5s): position (" << player.p.x << ", " << player.p.y << ", " << player.p.z
              << "), grounded=" << (player.ground ? "yes" : "no") << "\n";
    std::cout << "Disabled collision/render models: ";
    if (collision.disabled.empty()) std::cout << "none";
    else {
        bool first = true;
        for (int model : collision.disabled) {
            if (!first) std::cout << ", ";
            std::cout << model;
            first = false;
        }
    }
    std::cout << "\n";
}

int run(const std::filesystem::path& package, bool inspectOnly, bool renderCheck) {
    SceneData scene = SceneData::load(package);
#if defined(__APPLE__)
    if (!inspectOnly && !renderCheck && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
#else
    if (!inspectOnly && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
#endif
        throw std::runtime_error(SDL_GetError());
    if (!inspectOnly && !renderCheck) {
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    }

    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
#if defined(__APPLE__)
    HeadlessOpenGL headlessContext;
#endif
    std::vector<SceneMesh> meshes;
    try {
#if defined(__APPLE__)
        if (renderCheck) {
            headlessContext.create(1280, 800);
        } else
#endif
        if (!inspectOnly) {
            window = SDL_CreateWindow(("FDNY scene preview — " + scene.key()).c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            if (!window) throw std::runtime_error(SDL_GetError());
            context = SDL_GL_CreateContext(window);
            if (!context) throw std::runtime_error(SDL_GetError());
            SDL_GL_SetSwapInterval(1);
        }

        TextureLoadSummary textureSummary;
        meshes = loadMeshes(scene, textureSummary, !inspectOnly);
        Collision collision(scene.collision());
        V spawn{};
        float yaw = 0.f;
        bool foundPlayer = false;
        J entities = scene.entities();
        for (int i = 0; i < count(entities); ++i) {
            J entity = at(entities, i);
            std::string entityClass = str(field(entity, "class"));
            if (entityClass == "RCS_BoundsTrigger") {
                J model = field(entity, "model");
                // Missing model data converts to 0 through json-c's integer accessor.
                // Model 0 is the complete world BSP, so a trigger without an explicit
                // brush-model reference must never disable world rendering/collision.
                if (model && json_object_get_type(model) == json_type_int) {
                    int modelIndex = integer(model);
                    if (modelIndex > 0) collision.disabled.insert(modelIndex);
                }
            }
            if (entityClass == "RCS_PlayerRescue" || entityClass == "RCS_PlayerHose") {
                spawn = parseVector(property(entity, "ZZ_location"), parseVector(property(entity, "location"), spawn));
                yaw = parseFloat(property(entity, "C_Heading"), yaw) * .01745329251994329577f;
                foundPlayer = true;
            }
        }
        if (!foundPlayer) {
            J models = field(scene.collision(), "models");
            if (count(models)) {
                J world = at(models, 0);
                V lo = vec(field(world, "mins")), hi = vec(field(world, "maxs"));
                spawn = {(lo.x + hi.x) * .5f, lo.y + 128.f, (lo.z + hi.z) * .5f};
            }
        }
        printInventory(scene, meshes, textureSummary);
        if (foundPlayer) std::cout << "Spawn: " << spawn.x << ", " << spawn.y << ", " << spawn.z << "\n";
        else std::cout << "Spawn: fallback near world-model bounds (no player entity found)\n";
        if (inspectOnly) {
            printSpawnCollision(scene, spawn, collision);
            return 0;
        }

        if (renderCheck) {
            int width = 1280, height = 800;
#if !defined(__APPLE__)
            SDL_GL_GetDrawableSize(window, &width, &height);
#else
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, headlessContext.framebuffer);
#endif
            if (width < 1 || height < 1) throw std::runtime_error("Hidden OpenGL window has no drawable size");
            renderScene(meshes, scene, collision,
                        {spawn.x, spawn.y + 70.f, spawn.z}, yaw, 0.f, width, height);
            glFinish();
#if defined(__APPLE__)
            glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
#else
            glReadBuffer(GL_BACK);
#endif
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            std::vector<uint8_t> frame(size_t(width) * size_t(height) * 4);
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, frame.data());
            GLenum glError = glGetError();
            std::set<uint32_t> colors;
            size_t nonBackgroundPixels = 0;
            for (size_t i = 0; i + 3 < frame.size(); i += 4) {
                uint32_t rgba = (uint32_t(frame[i]) << 24) | (uint32_t(frame[i + 1]) << 16) |
                                (uint32_t(frame[i + 2]) << 8) | uint32_t(frame[i + 3]);
                colors.insert(rgba);
                if (rgba != 0x141a1fff) ++nonBackgroundPixels;
            }
            auto imagePath = saveRenderCheckFrame(package, frame, width, height);
            std::cout << "Render check: " << textureSummary.uploadedFiles << "/"
                      << textureSummary.uniqueFiles << " textures uploaded; "
                      << colors.size() << " distinct framebuffer colors; " << nonBackgroundPixels
                      << " non-background pixels; frame saved to " << imagePath.string();
#if defined(__APPLE__)
            std::cout << "; OpenGL " << (headlessContext.version() ? headlessContext.version() : "unknown");
#endif
            if (glError != GL_NO_ERROR) std::cout << "; OpenGL error " << glError;
            std::cout << "\n";
            clearTextures(meshes);
#if defined(__APPLE__)
            headlessContext.destroy();
#endif
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return glError == GL_NO_ERROR && textureSummary.failedFiles == 0 &&
                   textureSummary.uploadedFiles == textureSummary.uniqueFiles ? 0 : 1;
        }

        Player player;
        player.p = spawn;
        bool running = true, captured = true, jumpEdge = false;
        float pitch = 0.f;
        float yawInput = yaw;
        uint32_t previous = SDL_GetTicks();
        uint32_t previousTitleUpdate = previous;
        SDL_SetRelativeMouseMode(SDL_TRUE);
        std::cout << "WASD move, mouse look, Space/right mouse jump, C crouch, Escape releases the mouse.\n"
                  << "Press F to snap to the nearest walkable collision floor below you.\n"
                  << "This preview loads BSP geometry/textures and the recovered collision/player movement; mission systems are not wired yet.\n";
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        captured = !captured;
                        SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
                    }
                    if (event.key.keysym.sym == SDLK_SPACE) jumpEdge = true;
                    if (event.key.keysym.sym == SDLK_f) {
                        Hit floorHit;
                        if (collision.trace(player.p, {0.f, -3000.f, 0.f}, {-30.f, -30.f, -30.f}, player.maxs(), floorHit) &&
                            floorHit.normal.y >= .5f) {
                            player.p = floorHit.point;
                            player.v = {};
                            player.delta = {};
                            player.ground = true;
                            std::cout << "Moved to floor at " << player.p.x << ", " << player.p.y << ", " << player.p.z << "\n";
                        } else {
                            std::cout << "No walkable floor found within 3000 units below the player.\n";
                        }
                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) jumpEdge = true;
                if (event.type == SDL_MOUSEMOTION && captured) {
                    // Keep the same input scaling as the existing native workbench.
                    yawInput -= event.motion.xrel * .0025f;
                    yaw = yawInput;
                    pitch -= event.motion.yrel * (.1f * .01745329251994329577f);
                    pitch = std::clamp(pitch, -1.04719755f, 1.04719755f);
                }
            }
            uint32_t now = SDL_GetTicks();
            float dt = std::min(.1f, float(now - previous) * .001f);
            previous = now;
            const Uint8* keys = SDL_GetKeyboardState(nullptr);
            float forward = captured ? float(keys[SDL_SCANCODE_W] - keys[SDL_SCANCODE_S]) : 0.f;
            float side = captured ? .75f * float(keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]) : 0.f;
            bool crouchEdge = false;
            static bool crouchWasDown = false;
            bool crouchDown = keys[SDL_SCANCODE_C] != 0;
            crouchEdge = crouchDown && !crouchWasDown;
            crouchWasDown = crouchDown;
            player.step(dt, forward, side, yaw, keys[SDL_SCANCODE_SPACE] != 0 ||
                (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)),
                jumpEdge, crouchEdge, collision, pitch);
            jumpEdge = false;
            if (now - previousTitleUpdate >= 500) {
                std::string title = "FDNY scene preview — " + scene.key() + " | " +
                    std::to_string(textureSummary.uploadedFiles) + "/" +
                    std::to_string(textureSummary.uniqueFiles) + " textures | " +
                    (player.ground ? "grounded" : "falling") + " | xyz " +
                    std::to_string(int(player.p.x)) + "," + std::to_string(int(player.p.y)) + "," +
                    std::to_string(int(player.p.z));
                SDL_SetWindowTitle(window, title.c_str());
                previousTitleUpdate = now;
            }
            int width = 1, height = 1;
            SDL_GL_GetDrawableSize(window, &width, &height);
            renderScene(meshes, scene, collision, {player.p.x, player.p.y + (player.crouch ? 30.f : 70.f), player.p.z},
                        yaw, pitch, width, height);
            SDL_GL_SwapWindow(window);
            SDL_Delay(1);
        }
        clearTextures(meshes);
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    } catch (...) {
        if (context) SDL_GL_DeleteContext(context);
        if (window) SDL_DestroyWindow(window);
        if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_Quit();
        throw;
    }
}
} // namespace

int main(int argc, char** argv) {
    try {
        bool inspectOnly = false;
        bool renderCheck = false;
        std::filesystem::path package;
        for (int i = 1; i < argc; ++i) {
            std::string argument(argv[i]);
            if (argument == "--help") {
                std::cout << "Usage: fdny-scene-viewer [scene-export-directory] [--inspect | --render-check]\n"
                          << "Without a directory, loads the bundled MBase scene (or .local/exports/mbase in the project).\n";
                return 0;
            }
            if (argument == "--inspect") {
                inspectOnly = true;
                continue;
            }
            if (argument == "--render-check") {
                renderCheck = true;
                continue;
            }
            if (!package.empty()) throw std::runtime_error("Only one scene export directory may be supplied.");
            package = argument;
        }
        if (package.empty()) {
            char* basePath = SDL_GetBasePath();
            if (basePath) {
                std::filesystem::path executableDirectory(basePath);
                SDL_free(basePath);
                auto bundledScene = executableDirectory.parent_path() / "Resources" / "scene";
                if (std::filesystem::is_directory(bundledScene)) package = bundledScene;
            }
            if (package.empty()) {
                auto projectScene = std::filesystem::current_path() / ".local" / "exports" / "mbase";
                if (std::filesystem::is_directory(projectScene)) package = projectScene;
            }
        }
        if (package.empty()) {
            throw std::runtime_error("No scene supplied. Pass a scene-export directory or launch the bundled app with its MBase data.");
        }
        if (inspectOnly && renderCheck)
            throw std::runtime_error("Choose either --inspect or --render-check.");
        return run(package, inspectOnly, renderCheck);
    } catch (const std::exception& error) {
        std::cerr << "Scene viewer: " << error.what() << "\n";
        return 1;
    }
}
