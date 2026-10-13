#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
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
              << ", mesh groups: " << meshes.size()
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

void printSpawnCollision(const SceneData& scene, V spawn) {
    Collision collision(scene.collision());
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
}

int run(const std::filesystem::path& package, bool inspectOnly) {
    SceneData scene = SceneData::load(package);
    if (!inspectOnly && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        throw std::runtime_error(SDL_GetError());
    if (!inspectOnly) {
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    }

    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    std::vector<SceneMesh> meshes;
    try {
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
            if (entityClass == "RCS_BoundsTrigger") collision.disabled.insert(integer(field(entity, "model")));
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
            printSpawnCollision(scene, spawn);
            return 0;
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
        std::filesystem::path package;
        for (int i = 1; i < argc; ++i) {
            std::string argument(argv[i]);
            if (argument == "--help") {
                std::cout << "Usage: fdny-scene-viewer [scene-export-directory] [--inspect]\n"
                          << "Without a directory, loads the bundled MBase scene (or .local/exports/mbase in the project).\n";
                return 0;
            }
            if (argument == "--inspect") {
                inspectOnly = true;
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
        return run(package, inspectOnly);
    } catch (const std::exception& error) {
        std::cerr << "Scene viewer: " << error.what() << "\n";
        return 1;
    }
}
