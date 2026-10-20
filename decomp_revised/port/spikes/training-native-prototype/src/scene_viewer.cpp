#include <SDL.h>
#if defined(__APPLE__)
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <SDL_opengl.h>
#if defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#endif
#include <json-c/json.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <sstream>
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
std::vector<uint8_t> readSceneChunkBytes(const SceneData& scene, const std::string& relativePath) {
    try {
        std::ifstream input(scene.resolveAsset(relativePath), std::ios::binary);
        if (!input) return {};
        return std::vector<uint8_t>(std::istreambuf_iterator<char>(input),
                                    std::istreambuf_iterator<char>());
    } catch (const std::exception&) {
        return {};
    }
}
bool jsonNumber(J value) {
    if (!value) return false;
    auto type = json_object_get_type(value);
    return type == json_type_int || type == json_type_double;
}

#include "core.h"
#include "view_update.h"
#include "victim_roam_motion.h"
#include "bmodel_contact.h"
#include "extended.h"
#include "zones.h"
#include "gbsp_motion_path.h"
#include "random.h"
#include "startup.h"
#include "model_floor.h"
#include "actor_animation.h"
#include "actor_pose.h"
#include "actor_renderer.h"
#include "equipment_actor_renderer.h"
#include "prop_model_runtime.h"
#include "hint_overlay.h"
#include "mission_selector.h"
#include "victim_rescue.h"
#include "victim_transactions.h"
#include "rescue_bot.h"
#include "rescue_quake.h"
#include "victim_proximity.h"
#include "victim_sound.h"
#include "fire_effects.h"
#include "fire_suppression.h"
#include "fire_update.h"
#include "fire_size.h"
#include "fire_placement.h"
#include "sprite_animation.h"
#include "fire_visual.h"
#include "fire_render_state.h"
#include "fire_entity.h"
#include "bonus_points_hud_renderer.h"
#include "victim_pickup_hud_renderer.h"
#include "fire_floor.h"
#include "fire_renderer.h"
#include "fog_state.h"
#include "fire_direction_renderer.h"
#include "fire_contact.h"
#include "fire_spread.h"
#include "fire_message_queue.h"
#include "player_fire_contact.h"
#include "player_fire_message.h"
#include "player_burn_cooling.h"
#include "player_passout.h"
#include "player_oxygen.h"
#include "axe_action_callbacks.h"
#include "axe_candidate_bounds.h"
#include "axe_strike_pose.h"
#include "axe_world_collision.h"
#include "bmodel_axe_hit.h"
#include "item_view_pose.h"
#include "extinguisher_pose.h"
#include "extinguisher_samples.h"
#include "extinguisher_contacts.h"
#include "extinguisher_effects.h"
#include "extinguisher_spray_renderer.h"
#include "goggles_scan.h"
#include "goggles_renderer.h"
#include "oxygen_hud_renderer.h"
#include "scripts.h"
#include "world_startup.h"
#include "scene_audio.h"
#include "hose_item_selection.h"
#include "hose_spray_action.h"
#include "hose_spray_pose_list.h"
#include "player_hose_stage.h"
#include "player_hose_stream_renderer.h"
#include "rescue_hose_spray_effects.h"
#include "rescue_hose_spray_stream.h"

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

struct SceneFaceSpan {
    int faceId = -1;
    int firstTriangle = 0;
    int triangleCount = 0;
    int texinfoId = -1;
    int texinfoFlags = 0;
    float texinfoAlpha = 0.f;
    float faceLight = 0.f;
    float reflectiveScale = 0.f;
    float mipMapBias = 0.f;
    int lightOffset = -1;
    int lightWidth = 0;
    int lightHeight = 0;
    int lightStyleCount = 0;
    std::array<int, 4> lightTypes{};
    std::vector<uint8_t> lightSamples;
    int lightmapAtlasX = 0;
    int lightmapAtlasY = 0;
    bool lightmapAtlasAssigned = false;
    std::array<int, 4> uploadedLightStyleScales{};
    bool hasUploadedLightStyleScales = false;
};

struct LightStyleState {
    std::array<std::string, 24> sequences{};
    std::array<size_t, 24> cursors{};
    std::array<int, 24> scales{};

    void set(int style, std::string sequence) {
        if (style < 1 || style >= int(sequences.size())) return;
        if (sequence.size() > 60) sequence.resize(60);
        sequences[size_t(style)] = std::move(sequence);
    }

    void advance() {
        for (size_t style = 0; style < sequences.size(); ++style) {
            const std::string& sequence = sequences[style];
            if (sequence.empty()) {
                cursors[style] = 0;
                continue;
            }
            // FUN_10032E30 advances first, wrapping when the next byte is NUL.
            cursors[style] = (cursors[style] + 1) % sequence.size();
            const int character = static_cast<unsigned char>(sequence[cursors[style]]);
            scales[style] = int(double(character - 96) * (256.0 / 27.0));
        }
    }
};

struct SceneMesh {
    int model = 0;
    std::vector<float> positions, uvs, lightmapUvs;
    std::vector<SceneFaceSpan> faceSpans;
    GLuint texture = 0;
    GLuint lightmapAtlasTexture = 0;
    bool hasLightmappedFaces = false;
    V boundsCenter{}, boundsMin{}, boundsMax{};
    float boundsRadius = 0.f;
};

struct TextureLoadSummary {
    bool uploadAttempted = false;
    int uniqueFiles = 0;
    int decodedFiles = 0;
    int uploadedFiles = 0;
    int uploadedFaceLightmaps = 0;
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

std::string lower(std::string value);

PlayerHoseStage loadPlayerHoseStage(const SceneData& scene) {
    PlayerHoseStage stage;
    std::map<std::string, int> modelIndices;
    std::map<int, J> collisionModels;
    J entities = scene.entities();
    for (int i = 0; i < count(entities); ++i) {
        J entity = at(entities, i);
        const std::string entityClass = str(field(entity, "class"));
        if (entityClass == "RCS_PlayerHose") {
            stage.present = true;
            stage.playerEntity = integer(field(entity, "index"));
            stage.spawn = parseVector(property(entity, "ZZ_location"),
                parseVector(property(entity, "location"), {}));
            stage.spawnYaw = parseFloat(property(entity, "C_Heading"), 0.f) *
                0.01745329251994329577f;
            stage.firstPathPoint = property(entity, "FirstPathPoint");
        } else if (entityClass == "%Model%") {
            std::string name = property(entity, "%name%");
            if (name.empty()) name = str(field(entity, "name"));
            const int model = int(parseFloat(property(entity, "Model"), -1.f));
            if (!name.empty() && model >= 0) {
                auto [found, inserted] = modelIndices.emplace(lower(name), model);
                if (!inserted && found->second != model)
                    throw std::runtime_error("Ambiguous hose %Model% name in scene export: " + name);
            }
        }
    }
    if (!stage.present) return stage;

    J exportedCollisionModels = field(scene.collision(), "models");
    for (int i = 0; i < count(exportedCollisionModels); ++i) {
        J model = at(exportedCollisionModels, i);
        collisionModels[integer(field(model, "index"))] = model;
    }

    for (int i = 0; i < count(entities); ++i) {
        J entity = at(entities, i);
        if (str(field(entity, "class")) != "RCS_HosePath") continue;
        PlayerHoseHotspot path;
        path.entity = integer(field(entity, "index"));
        path.name = property(entity, "%name%");
        if (path.name.empty()) path.name = str(field(entity, "name"));
        if (path.name.empty())
            throw std::runtime_error("RCS_HosePath has no authored name in " + scene.key());
        path.nextPath = property(entity, "A_NextPoint");
        path.lookAtPath = property(entity, "B_LookAtPoint");
        if (path.nextPath == "<null>") path.nextPath.clear();
        if (path.lookAtPath == "<null>") path.lookAtPath.clear();
        path.nextHotspot = property(entity, "D_NextHotSpot");
        if (path.nextHotspot == "<null>") path.nextHotspot.clear();
        path.position = parseVector(property(entity, "ZZ_location"),
            parseVector(property(entity, "location"), {}));
        path.isHotspot = parseFloat(property(entity, "C_IsAHotSpot"), 0.f) != 0.f;
        path.isFirstHotspot = parseFloat(property(entity, "M_IsFirstHotSpot"), 0.f) != 0.f;
        path.maxPressure = parseFloat(property(entity, "M_MaxPressure"), 0.f);
        path.pitchSpread = parseFloat(property(entity, "N_PitchSpread"), 45.f);
        path.spotTime = parseFloat(property(entity, "M_SpotTime"), 0.f);
        path.numCycles = int(parseFloat(property(entity, "Q_NumCycles"), 0.f));
        for (int slot = 0; slot < 9; ++slot) {
            const std::string suffix = std::to_string(slot);
            const std::string name = property(entity, ("ZF" + suffix + "_FireModel").c_str());
            const float sprayTime = parseFloat(
                property(entity, ("ZF" + suffix + "_SprayTime").c_str()), 0.f);
            path.fireModelSlots[size_t(slot)] = name;
            path.sprayTimeSlots[size_t(slot)] = sprayTime;
            if (name.empty() || name == "<null>") continue;
            const auto model = modelIndices.find(lower(name));
            if (model == modelIndices.end())
                throw std::runtime_error("Unresolved PlayerHose target " + name + " in " + scene.key());
            if (!collisionModels.count(model->second))
                throw std::runtime_error("PlayerHose target has no BSP collision model: " + name);
            path.targets.push_back({slot, model->second, name, sprayTime, 0.f, false, false});
        }
        const size_t nodeIndex = stage.pathNodes.size();
        if (!stage.byName.emplace(path.name, nodeIndex).second)
            throw std::runtime_error("Duplicate RCS_HosePath name in " + scene.key() + ": " + path.name);
        stage.pathNodes.push_back(std::move(path));
    }
    if (stage.firstPathPoint.empty() || !stage.byName.count(stage.firstPathPoint))
        throw std::runtime_error("PlayerHose FirstPathPoint is unresolved in " + scene.key());
    return stage;
}

std::vector<PropModelInstance> loadPropModelInstances(J entities) {
    std::vector<PropModelInstance> result;
    for (int i = 0; i < count(entities); ++i) {
        J entity = at(entities, i);
        if (str(field(entity, "class")) != "RCS_PropModel") continue;
        const int modelId = int(parseFloat(property(entity, "D_Model"), -1.f));
        if (modelId < 0) continue;
        PropModelInstance prop;
        prop.entity = integer(field(entity, "index"));
        prop.resourceModelId = modelId;
        prop.name = property(entity, "A_Name");
        prop.position = parseVector(property(entity, "ZZ_location"), {});
        prop.authoredHeading = parseFloat(property(entity, "E_Heading"), 0.f);
        prop.authoredPitch = parseFloat(property(entity, "E_Pitch"), 0.f);
        prop.touchRadius = parseFloat(property(entity, "F_TouchRadius"), 0.f);
        prop.makeValve = lower(property(entity, "S_Script01")) == "makevalve";
        result.push_back(std::move(prop));
    }
    return result;
}

bool parsePropScale(const std::string& arguments, V& scale) {
    std::istringstream input(arguments);
    V parsed;
    std::string extra;
    if (!(input >> parsed.x >> parsed.y >> parsed.z) || (input >> extra) ||
        !std::isfinite(parsed.x) || !std::isfinite(parsed.y) || !std::isfinite(parsed.z))
        return false;
    scale = parsed;
    return true;
}

ActorTransform actorRootFromFrame(const FrameTransform& frame) {
    ActorTransform root;
    for (int row = 0; row < 3; ++row)
        for (int column = 0; column < 3; ++column)
            root.r[row * 3 + column] = frame.axis[column][row];
    root.t = frame.position;
    return root;
}

ActorTransform halfTurnedToolRootFromCameraFrame(const FrameTransform& camera) {
    ActorTransform root = actorRootFromFrame(camera);
    // Current Axe preview candidate: the exported ACT shows its broad side
    // with this half-turn. Retail's composed attached-actor pose is still
    // unknown, so this per-model correction remains provisional.
    for (int row = 0; row < 3; ++row) {
        root.r[row * 3] = -root.r[row * 3];
        root.r[row * 3 + 2] = -root.r[row * 3 + 2];
    }
    return root;
}

ActorTransform rawToolRootFromCameraFrame(const FrameTransform& camera) {
    return actorRootFromFrame(camera);
}

V cameraRelativePoint(const FrameTransform& camera, float right, float up, float forward) {
    // The OpenGL view looks down local -Z. Keep the preview-only handheld
    // assets in front of that camera even though the retail actor attachment
    // frame's final draw transform is not yet recovered.
    return camera.position + camera.axis[0] * right + camera.axis[1] * up - camera.axis[2] * forward;
}

void scaleActorRoot(ActorTransform& root, float scale) {
    for (float& axis : root.r) axis *= scale;
}

bool parseFireRescueSizeCommand(const std::string& arguments, V& size, float& duration) {
    std::istringstream input(arguments);
    long long x = 0, y = 0, z = 0;
    if (!(input >> x >> y >> z >> duration)) return false;
    std::string extra;
    if (input >> extra) return false;
    const auto inIntRange = [](long long value) {
        return value >= std::numeric_limits<int32_t>::min() &&
               value <= std::numeric_limits<int32_t>::max();
    };
    if (!inIntRange(x) || !inIntRange(y) || !inIntRange(z) || !std::isfinite(duration))
        return false;
    size = {float(x), float(y), float(z)};
    return true;
}

bool parseFireRescueMoveCommand(const std::string& arguments, std::string& target,
                               float& divisor, float& firstKeyOffset,
                               float& secondKeyOffset) {
    auto [targetToken, values] = token(arguments);
    target = unwrap(targetToken);
    if (target.empty()) return false;
    std::istringstream input(values);
    if (!(input >> divisor >> firstKeyOffset >> secondKeyOffset)) return false;
    std::string extra;
    if (input >> extra) return false;
    return std::isfinite(divisor) && std::isfinite(firstKeyOffset) &&
           std::isfinite(secondKeyOffset);
}

struct VictimActorAsset {
    SceneJson data;
    std::unique_ptr<VictimRenderer> renderer;
    std::string stem;
    VictimActorAsset(const std::filesystem::path& root, std::string assetStem, bool prepareRenderer)
        : stem(std::move(assetStem)) {
        json_object* parsed = json_object_from_file((root / (stem + ".json")).string().c_str());
        if (!parsed) throw std::runtime_error("Cannot load victim ACT export " + (root / (stem + ".json")).string());
        data.reset(parsed);
        if (prepareRenderer) {
            renderer = std::make_unique<VictimRenderer>(data.get(), root, stem);
            renderer->load();
        }
    }
    int motionCount() const { return count(field(data.get(), "motions")); }
    int materialCount() const { return count(field(data.get(), "materials")); }
};

struct EquipmentActorAsset {
    SceneJson data;
    std::unique_ptr<EquipmentActorRenderer> renderer;
    std::string stem;
    int swingMotion = -1;
    float swingEnd = 0.f;
    EquipmentActorAsset(const std::filesystem::path& root, std::string assetStem, bool prepareRenderer)
        : stem(std::move(assetStem)) {
        json_object* parsed = json_object_from_file((root / (stem + ".json")).string().c_str());
        if (!parsed) throw std::runtime_error("Cannot load equipment ACT export " + (root / (stem + ".json")).string());
        data.reset(parsed);
        const J motions = field(data.get(), "motions");
        for (int index = 0; index < count(motions); ++index) {
            const J motion = at(motions, index);
            if (str(field(motion, "name")) != "swing") continue;
            swingMotion = index;
            swingEnd = num(field(motion, "end"));
        }
        if (prepareRenderer) {
            renderer = std::make_unique<EquipmentActorRenderer>(data.get(), root, stem);
            renderer->load();
        }
    }
};

struct PreviewPlayerFireOps {
    uint32_t now = 0;
    float dt = 0.f;
    V playerPosition{}, fireSource{};
    float yaw = 0.f, pitch = 0.f;
    bool cameraAudit = false;
    std::array<uint8_t, 4>* directionalIndicators = nullptr;
    uint32_t* seriousBurns = nullptr;
    int* lastPainChoice = nullptr;
    int* lastPassoutChoice = nullptr;
    uint32_t selectedItemIndex = 0;
    bool* incapacitated = nullptr;
    std::string* activeHint = nullptr;
    float* activeHintRemaining = nullptr;
    SceneAudio* audio = nullptr;
    Random* random = nullptr;
    std::function<void()> releaseSelectedItem;

    void markFireDirection() const {
        // Retail 0044BB00 writes a transient burn-direction byte; it does not
        // steer the camera or movement heading. The viewer uses yaw 0 for -Z,
        // matching the camera basis assembled below.
        if (!directionalIndicators) return;
        const V offset = fireSource - playerPosition;
        if (offset.x * offset.x + offset.z * offset.z < .001f) return;
        constexpr float pi = 3.14159265358979323846f;
        constexpr float quarterPi = pi * .25f;
        const float target = std::atan2(-offset.x, -offset.z);
        const float angle = std::remainder(target - yaw, 2.f * pi);
        if (angle >= -quarterPi && angle <= quarterPi) {
            (*directionalIndicators)[0] = 1; // Player +0x353C, center/front sector
        } else if (angle <= -3.f * quarterPi || angle >= 3.f * quarterPi) {
            (*directionalIndicators)[1] = 1; // Player +0x353D, rear sector
        } else if (angle > quarterPi && angle < 3.f * quarterPi) {
            (*directionalIndicators)[2] = 1; // Player +0x353E
        } else if (angle > -3.f * quarterPi && angle < -quarterPi) {
            (*directionalIndicators)[3] = 1; // Player +0x353F
        }
    }
    uint32_t readClock() const { return now; }
    void triggerPain(Random& shared) {
        if (cameraAudit)
            std::cout << "fire-camera-audit feedback=pain t=" << now
                      << " yaw=" << yaw << " pitch=" << pitch
                      << " player=" << playerPosition.x << "," << playerPosition.y
                      << "," << playerPosition.z << "\n";
        if (!audio || !lastPainChoice) return;
        *lastPainChoice = shared.soundIndex(4, *lastPainChoice);
        audio->play("RC_Pain" + std::to_string(*lastPainChoice), playerPosition, playerPosition,
                    yaw, 1);
    }
    void addSeriousBurns() {
        if (seriousBurns) ++*seriousBurns;
        if (activeHint) *activeHint = "Serious burns: " + std::to_string(seriousBurns ? *seriousBurns : 0);
        if (activeHintRemaining) *activeHintRemaining = 2.5f;
    }
    float readFrameDelta() const { return dt; }
    void triggerPassout(PlayerPassoutState& state, int parameter) {
        if ((parameter != 0 && parameter != 1) || state.state == 6) return;
        if (cameraAudit)
            std::cout << "fire-camera-audit feedback=passout t=" << now
                      << " parameter=" << parameter
                      << " yaw=" << yaw << " pitch=" << pitch
                      << " player=" << playerPosition.x << "," << playerPosition.y
                      << "," << playerPosition.z << "\n";
        struct PassoutOps {
            PreviewPlayerFireOps& host;
            void requestPassoutSoundInstance(uint32_t playerSoundOffset) {
                // Preserve the source-mapped +0x454 / +0x430 configured
                // sound-instance choice; its definitions are not in sound.rrf.
                if (host.cameraAudit)
                    std::cout << "passout sound adapter open offset=0x" << std::hex
                              << playerSoundOffset << std::dec << "\n";
            }
            void playChatterSet(const char* setName, bool enabled) {
                if (!enabled || std::string(setName) != "RC_PassOut" ||
                    !host.audio || !host.random || !host.lastPassoutChoice) return;
                *host.lastPassoutChoice = host.random->soundIndex(
                    3, *host.lastPassoutChoice);
                host.audio->play("RC_PassOut" + std::to_string(*host.lastPassoutChoice),
                                 host.playerPosition, host.playerPosition, host.yaw, 1);
            }
            uint32_t readClock() const { return host.now; }
            uint32_t readSelectedItemIndex() const { return host.selectedItemIndex; }
            void releaseSelectedItem(uint32_t) {
                if (host.releaseSelectedItem) host.releaseSelectedItem();
            }
            void runHeroDown(int) {}
        } ops{*this};
        applyPlayerPassout(state, parameter, ops);
        if (incapacitated) *incapacitated = true;
        if (activeHint) *activeHint = parameter == 0
            ? "You have run out of oxygen."
            : "You have been incapacitated by fire.";
        if (activeHintRemaining) *activeHintRemaining = 8.f;
    }
};

struct PreviewVictim {
    VictimSpawn spawn;
    VictimPickupActorState pickup;
    VictimSoundChoices sounds;
    ActorAnimation animation;
    std::string actorStem, label;
    int runtimeModelId = 12;
    bool hasRoamCommand = false, roaming = false, randomRoamDirection = true;
    bool noRescueBot = false;
    uint32_t botRescueField1F0 = 0;
    int roamState = 4;
    float roamRadius = 0.f, roamSpeed = 36.f, pickupRadius = 60.f;
    float facingDegrees = 0.f;
    V roamCenter{}, roamTarget{}, previousRoamTarget{}, roamDirection{};
    struct Obstacle { V center{}; float radiusSquared = 0.f; };
    std::vector<Obstacle> roamObstacles;
};

struct PreviewFire {
    struct PathMotion {
        int targetEntity = -1;
        bool active = false;
        V start{}, target{}, direction{};
        float distance = 0.f, progress = 0.f, rate = 0.f;
        float lastVisualProgress = 0.f, updateThreshold = 0.f;
        bool updateThresholdMapped = false;
        // Retail stores these as inputs to generated visual-key timestamps.
        // The route trail/key factory is not ported yet.
        float firstKeyOffset = 0.f, secondKeyOffset = 0.f;
    } pathMotion;
    int entity = -1;
    std::string label;
    std::string authoredName;
    bool floorMe = false;
    float routeVisualUpdateThreshold = 0.f;
    bool routeVisualUpdateThresholdMapped = false;
    // 0046F930 initializes the root +0x48 name string empty. FireRescue init
    // fills it from A_Name, so an absent or empty name keeps this gate closed.
    bool spreadNameGateEnabled = false;
    // FireRescue root +0x120 starts at float 99999. Retail updates the shared
    // proximity population from this prior value on each eligible callback.
    int lastSpreadProximity = 99999;
    uint32_t nextSpreadAttempt = 0;
    FireEntity runtime;
    bool loopSoundStarted = false;
    V lastLoopSoundSource{};
};

const char* fireLoopSoundName(int choice) {
    static constexpr const char* names[] = {"FireBurn1", "FireBurn2", "FireBurn3"};
    return choice >= 0 && choice < 3 ? names[choice] : nullptr;
}

struct PreviewPlayerLoadout {
    HoseItemSelection::State items;
    HoseSprayPoseList::State rescueHoseAttachment;

    enum class TouchHoseResult { Connected, Refreshed, AlreadyHaveHose };

    static int slotForName(std::string name) {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        if (name == "axe") return 1;
        if (name == "extinguisher") return 2;
        if (name == "goggles") return 3;
        return 0;
    }

    static const char* nameForSlot(int slot) {
        switch (slot) {
            case 1: return "axe";
            case 2: return "extinguisher";
            case 3: return "goggles";
            case 4: return "hose";
            default: return "none";
        }
    }

    void syncHoseMode(int previous, HoseItemSelection::Result& result) {
        if (previous == 4 && items.selected != 4 && items.hoseMode == 3)
            HoseItemSelection::setMode(items, 1, result);
        if (items.selected == 4 && items.hoseMode == 2)
            HoseItemSelection::setMode(items, 3, result);
    }

    bool equip(const std::string& name) {
        const int slot = slotForName(name);
        if (slot == 0) return false;
        auto& item = items.slots[size_t(slot)];
        if (item.present) {
            if (slot != 1) return true;
            // 0044D080's existing-Axe branch sets the root gate directly and
            // returns; it does not create or select another Axe. The first
            // equip must leave this gate clear so retail updates model 1.
            items.axeGate.primaryChildEnabled = true;
            return true;
        }
        item.present = true;
        const int previous = items.selected;
        HoseItemSelection::Result result;
        HoseItemSelection::selectSlot(items, slot, false, result);
        syncHoseMode(previous, result);
        return true;
    }

    bool select(const std::string& name) {
        const int slot = lower(name) == "hose" ? 4 : slotForName(name);
        if (slot == 0) return false;
        const int previous = items.selected;
        HoseItemSelection::Result result;
        if (!HoseItemSelection::selectSlot(items, slot, false, result)) return false;
        syncHoseMode(previous, result);
        return true;
    }

    TouchHoseResult touchhose(int argument) {
        auto& hose = items.slots[4];
        const bool alreadyPresent = hose.present;
        if (alreadyPresent && (items.hoseMode == 2 || items.hoseMode == 3))
            return TouchHoseResult::AlreadyHaveHose;

        if (!alreadyPresent) {
            hose.present = true;
            HoseSprayPoseList::initializeFromCommand(rescueHoseAttachment, argument);
        } else if (HoseSprayPoseList::currentArgumentEquivalent(rescueHoseAttachment) <
                   static_cast<float>(argument)) {
            HoseSprayPoseList::resize(
                rescueHoseAttachment, HoseSprayPoseList::commandRecordCount(argument));
        }

        HoseItemSelection::Result result;
        HoseItemSelection::setMode(items, 2, result);
        return alreadyPresent ? TouchHoseResult::Refreshed : TouchHoseResult::Connected;
    }

    std::string selectedName() const {
        return nameForSlot(items.selected);
    }
};

struct PreviewExtinguisher {
    Follower follower;
    FrameTransform worldFrame;
    ExtinguisherPose::CallbackState action;
    ExtinguisherPose::MotionPose localPose = ExtinguisherPose::idleMotion();
    ExtinguisherPose::MotionPose activePose = ExtinguisherPose::idleMotion();
    float phase = 0.f;
    float charge = 1.f;
    bool followerInitialized = false;
    bool hasWorldFrame = false;
};

struct PreviewRescueHoseSpray {
    HoseSprayAction::State action;
    RescueHoseSprayStream stream;
    uint32_t dousedFireCount = 0;
};

struct PreviewRoamObstacle {
    V center{};
    float radius = 24.f;
};

struct PreviewBModelChild {
    int model = -1;
    V rotationalCenter{};
    RetailGbspMotion::PreparedPath path;
};

struct PreviewBModelTimedCommand {
    bool active = true;
    float keyFrame = 0.f;
    std::string command;
};

struct PreviewBModelMotion {
    int entity = -1;
    std::string authoredName;
    std::vector<PreviewBModelChild> children;
    V boundsMin{1.0e30f, 1.0e30f, 1.0e30f};
    V boundsMax{-1.0e30f, -1.0e30f, -1.0e30f};
    V openAwayBoundsMin{1.0e30f, 1.0e30f, 1.0e30f};
    V openAwayBoundsMax{-1.0e30f, -1.0e30f, -1.0e30f};
    V sourcePosition{};
    bool hasBounds = false;
    bool hasOpenAwayBounds = false;
    float contactReach = 0.f;
    float startTime = 0.f;
    float endTime = 0.f;
    // InitializeBModelEntity writes 1.0 to the per-owner speed field.
    float timeScale = 1.f;
    float currentTime = 0.f;
    int hitPoints = 0;
    int liftable = 0;
    int motionState = 0; // retail states 3=open and 4=close
    std::optional<std::string> pendingCommand;
    std::vector<PreviewBModelTimedCommand> timedCommands;
};

struct PreviewBoundsTrigger {
    int entity = -1;
    int modelIndex = -1;
    std::string authoredName;
    std::string modelName;
    // 00441040 sets +0x164 when the authored `touch` event contains
    // `tell player escape`; 0044FCE0 toggles its +0x165 carry-mode byte.
    bool carryModeGate = false;
    bool carryMode = false;
    V boundsMin{1.0e30f, 1.0e30f, 1.0e30f};
    V boundsMax{-1.0e30f, -1.0e30f, -1.0e30f};
    float contactReach = 0.f;
};

struct PreviewWorldModelFlags {
    std::map<int, uint32_t> flags;

    bool rendersNormally(int model) const {
        auto found = flags.find(model);
        // Models with no ported ModelSetFlags call retain the existing default
        // draw path. Bit 0 as the normal-render gate is inferred from the
        // upstream Genesis3D implementation; FDNY's setter itself is confirmed.
        return found == flags.end() || (found->second & 1u) != 0;
    }
};

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

struct VictimActorResource { int modelId; std::string stem; };

VictimActorResource resolveVictimActor(const std::string& type, Random& random) {
    const std::string key = lower(type);
    if (key == "fatman") {
        static const VictimActorResource variants[] = {{11, "vic_boy"}, {12, "vic_man"}, {13, "vic_woman"}};
        return variants[random.bounded(3)];
    }
    if (key == "dog") return {16, "vic_dog"};
    if (key == "girl") return {15, "vic_girl"};
    if (key == "woman") return {13, "vic_woman"};
    if (key == "man") return {12, "vic_man"};
    if (key == "boy" || key == "boy1" || key.empty()) return {11, "vic_boy"};
    // victims.srf's first-definition fallback is the shipped Boy entry.
    return {11, "vic_boy"};
}

std::filesystem::path locateVictimAssets(const SceneData& scene) {
    if (const char* overridePath = std::getenv("FDNY_ACTOR_ASSETS_DIR")) {
        std::filesystem::path overrideRoot = overridePath;
        if (std::filesystem::is_regular_file(overrideRoot / "vic_man.json")) return overrideRoot;
        throw std::runtime_error("FDNY_ACTOR_ASSETS_DIR has no vic_man.json: " + overrideRoot.string());
    }
    const auto sceneParent = scene.root().parent_path();
    const std::vector<std::filesystem::path> candidates = {
        sceneParent / "actor_exports",
        sceneParent.parent_path() / "actor_exports",
    };
    for (const auto& candidate : candidates)
        if (std::filesystem::is_regular_file(candidate / "vic_man.json")) return candidate;
    throw std::runtime_error("Cannot find decoded victim ACT assets next to the scene package");
}

std::filesystem::path locateGogglesSprites(const SceneData& scene) {
    auto hasGoggleTextures = [](const std::filesystem::path& candidate) {
        return std::filesystem::is_regular_file(candidate / "heatpulse.bmp") &&
               std::filesystem::is_regular_file(candidate / "heatpulse_a.bmp") &&
               std::filesystem::is_regular_file(candidate / "goggtop.bmp") &&
               std::filesystem::is_regular_file(candidate / "goggbott.bmp");
    };
    if (const char* overridePath = std::getenv("FDNY_GAME_DATA_DIR")) {
        const std::filesystem::path overrideRoot = overridePath;
        for (const auto& candidate : {overrideRoot / "sprites", overrideRoot / "Data" / "sprites"})
            if (hasGoggleTextures(candidate)) return candidate;
        throw std::runtime_error("FDNY_GAME_DATA_DIR has no heatpulse texture pair: " + overrideRoot.string());
    }
    if (const char* overridePath = std::getenv("FDNY_ACTOR_ASSETS_DIR")) {
        const std::filesystem::path candidate = std::filesystem::path(overridePath) / "goggles";
        if (hasGoggleTextures(candidate)) return candidate;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {root / "actor_exports" / "goggles",
                                      root.parent_path() / "actor_exports" / "goggles"})
            if (hasGoggleTextures(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    for (auto root = scene.root(); !root.empty();) {
        const auto candidate = root / "inputs" / "original" / "Data" / "sprites";
        if (hasGoggleTextures(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    throw std::runtime_error("Cannot find copied retail goggles textures under actor_exports/goggles or inputs/original/Data/sprites");
}

std::filesystem::path locateOxygenHudSprites(const SceneData& scene) {
    auto hasOxygenTextures = [](const std::filesystem::path& candidate) {
        return std::filesystem::is_regular_file(candidate / "oxygauge.bmp") &&
               std::filesystem::is_regular_file(candidate / "oxygauge_a.bmp") &&
               std::filesystem::is_regular_file(candidate / "oxyneedle.bmp") &&
               std::filesystem::is_regular_file(candidate / "oxyneedle_a.bmp") &&
               std::filesystem::is_regular_file(candidate / "shield.bmp") &&
               std::filesystem::is_regular_file(candidate / "shield_a.bmp");
    };
    if (const char* overridePath = std::getenv("FDNY_GAME_DATA_DIR")) {
        const std::filesystem::path overrideRoot = overridePath;
        for (const auto& candidate : {overrideRoot / "sprites", overrideRoot / "Sprites",
                                      overrideRoot / "Data" / "sprites",
                                      overrideRoot / "Data" / "Sprites"})
            if (hasOxygenTextures(candidate)) return candidate;
        throw std::runtime_error("FDNY_GAME_DATA_DIR has no oxygen HUD/visor texture pairs: " +
                                 overrideRoot.string());
    }
    if (const char* overridePath = std::getenv("FDNY_ACTOR_ASSETS_DIR")) {
        const std::filesystem::path candidate = std::filesystem::path(overridePath) / "oxygen";
        if (hasOxygenTextures(candidate)) return candidate;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {root / "actor_exports" / "oxygen",
                                      root.parent_path() / "actor_exports" / "oxygen"})
            if (hasOxygenTextures(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {
                 root / "inputs" / "original" / "Data" / "Sprites",
                 root / "inputs" / "original" / "Data" / "sprites",
                 root / "inputs" / "original" / "data" / "Sprites",
                 root / "inputs" / "original" / "data" / "sprites"})
            if (hasOxygenTextures(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    throw std::runtime_error("Cannot find copied oxygen HUD/visor textures under "
                             "actor_exports/oxygen or inputs/original/Data/Sprites");
}

std::filesystem::path locateVictimPickupHudSprites(const SceneData& scene) {
    auto hasVictimPickupTexture = [](const std::filesystem::path& candidate) {
        return std::filesystem::is_regular_file(candidate / "rescuing.bmp") &&
               std::filesystem::is_regular_file(candidate / "rescuing_a.bmp");
    };
    if (const char* overridePath = std::getenv("FDNY_GAME_DATA_DIR")) {
        const std::filesystem::path overrideRoot = overridePath;
        for (const auto& candidate : {overrideRoot / "sprites", overrideRoot / "Sprites",
                                      overrideRoot / "Data" / "sprites",
                                      overrideRoot / "Data" / "Sprites"})
            if (hasVictimPickupTexture(candidate)) return candidate;
        throw std::runtime_error("FDNY_GAME_DATA_DIR has no rescuing texture pair: " +
                                 overrideRoot.string());
    }
    if (const char* overridePath = std::getenv("FDNY_ACTOR_ASSETS_DIR")) {
        const std::filesystem::path candidate =
            std::filesystem::path(overridePath) / "victim-feedback";
        if (hasVictimPickupTexture(candidate)) return candidate;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {
                 root / "actor_exports" / "victim-feedback",
                 root.parent_path() / "actor_exports" / "victim-feedback"})
            if (hasVictimPickupTexture(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {
                 root / "inputs" / "original" / "Data" / "Sprites",
                 root / "inputs" / "original" / "Data" / "sprites",
                 root / "inputs" / "original" / "data" / "Sprites",
                 root / "inputs" / "original" / "data" / "sprites"})
            if (hasVictimPickupTexture(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    throw std::runtime_error("Cannot find copied rescuing HUD textures under "
                             "actor_exports/victim-feedback or inputs/original/Data/Sprites");
}

std::filesystem::path locateBonusPointsHudSprites(const SceneData& scene) {
    auto hasBonusPointsTextures = [](const std::filesystem::path& candidate) {
        return std::filesystem::is_regular_file(candidate / "tools.bmp") &&
               std::filesystem::is_regular_file(candidate / "rescbon_a.bmp") &&
               std::filesystem::is_regular_file(candidate / "confont.bmp") &&
               std::filesystem::is_regular_file(candidate / "confont_a.bmp");
    };
    if (const char* overridePath = std::getenv("FDNY_GAME_DATA_DIR")) {
        const std::filesystem::path overrideRoot = overridePath;
        for (const auto& candidate : {overrideRoot / "sprites", overrideRoot / "Sprites",
                                      overrideRoot / "Data" / "sprites",
                                      overrideRoot / "Data" / "Sprites"})
            if (hasBonusPointsTextures(candidate)) return candidate;
        throw std::runtime_error("FDNY_GAME_DATA_DIR has no Bonus Points sprite/font resources: " +
                                 overrideRoot.string());
    }
    if (const char* overridePath = std::getenv("FDNY_ACTOR_ASSETS_DIR")) {
        const std::filesystem::path candidate =
            std::filesystem::path(overridePath) / "bonus-points";
        if (hasBonusPointsTextures(candidate)) return candidate;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {
                 root / "actor_exports" / "bonus-points",
                 root.parent_path() / "actor_exports" / "bonus-points"})
            if (hasBonusPointsTextures(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {
                 root / "inputs" / "original" / "Data" / "Sprites",
                 root / "inputs" / "original" / "Data" / "sprites",
                 root / "inputs" / "original" / "data" / "Sprites",
                 root / "inputs" / "original" / "data" / "sprites"})
            if (hasBonusPointsTextures(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    throw std::runtime_error("Cannot find Bonus Points HUD sprite/font assets under "
                             "actor_exports/bonus-points or inputs/original/Data/Sprites");
}

std::filesystem::path locateFireDirectionSprites(const SceneData& scene) {
    auto hasFireDirectionPair = [](const std::filesystem::path& candidate) {
        return std::filesystem::is_regular_file(candidate / "fireprox.bmp") &&
               std::filesystem::is_regular_file(candidate / "fireprox_a.bmp");
    };
    if (const char* overridePath = std::getenv("FDNY_GAME_DATA_DIR")) {
        const std::filesystem::path overrideRoot = overridePath;
        for (const auto& candidate : {overrideRoot / "sprites", overrideRoot / "Sprites",
                                      overrideRoot / "Data" / "sprites",
                                      overrideRoot / "Data" / "Sprites"})
            if (hasFireDirectionPair(candidate)) return candidate;
        throw std::runtime_error("FDNY_GAME_DATA_DIR has no fireprox texture pair: " + overrideRoot.string());
    }
    if (const char* overridePath = std::getenv("FDNY_ACTOR_ASSETS_DIR")) {
        const std::filesystem::path candidate = std::filesystem::path(overridePath) / "fire-direction";
        if (hasFireDirectionPair(candidate)) return candidate;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {root / "actor_exports" / "fire-direction",
                                      root.parent_path() / "actor_exports" / "fire-direction"})
            if (hasFireDirectionPair(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {
                 root / "inputs" / "original" / "Data" / "Sprites",
                 root / "inputs" / "original" / "Data" / "sprites",
                 root / "inputs" / "original" / "data" / "Sprites",
                 root / "inputs" / "original" / "data" / "sprites"})
            if (hasFireDirectionPair(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    throw std::runtime_error("Cannot find copied fire-direction textures under actor_exports/fire-direction or inputs/original/Data/Sprites");
}

std::filesystem::path locateExtinguisherSpraySprites(
        const SceneData& scene, const std::filesystem::path& actorAssets) {
    auto hasSprayPair = [](const std::filesystem::path& candidate) {
        return std::filesystem::is_regular_file(candidate / "bsmoke.bmp") &&
               std::filesystem::is_regular_file(candidate / "sm_alpha.bmp");
    };
    if (const char* overridePath = std::getenv("FDNY_GAME_DATA_DIR")) {
        const std::filesystem::path root = overridePath;
        for (const auto& candidate : {root / "sprites", root / "Sprites",
                                      root / "Data" / "sprites", root / "Data" / "Sprites"})
            if (hasSprayPair(candidate)) return candidate;
        throw std::runtime_error("FDNY_GAME_DATA_DIR has no resource-14 extinguisher texture pair: " +
                                 root.string());
    }
    const auto bundled = actorAssets / "extinguisher-spray";
    if (hasSprayPair(bundled)) return bundled;
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {
                 root / "actor_exports" / "extinguisher-spray",
                 root.parent_path() / "actor_exports" / "extinguisher-spray",
                 root / "inputs" / "original" / "Data" / "sprites",
                 root / "inputs" / "original" / "Data" / "Sprites"})
            if (hasSprayPair(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    throw std::runtime_error("Cannot find rescue.rrf resource-14 particle textures "
                             "under actor_exports/extinguisher-spray or copied game data");
}

std::filesystem::path locatePlayerHoseStreamSprites(
        const SceneData& scene, const std::filesystem::path& actorAssets) {
    auto hasStreamPair = [](const std::filesystem::path& candidate) {
        return std::filesystem::is_regular_file(candidate / "hdstream.bmp") &&
               std::filesystem::is_regular_file(candidate / "hdstream_a.bmp");
    };
    if (const char* overridePath = std::getenv("FDNY_GAME_DATA_DIR")) {
        const std::filesystem::path root = overridePath;
        for (const auto& candidate : {root / "sprites", root / "Sprites",
                                      root / "Data" / "sprites", root / "Data" / "Sprites"})
            if (hasStreamPair(candidate)) return candidate;
        throw std::runtime_error("FDNY_GAME_DATA_DIR has no hose.rrf resource-3 stream texture pair: " +
                                 root.string());
    }
    const auto bundled = actorAssets / "playerhose-stream";
    if (hasStreamPair(bundled)) return bundled;
    if (const char* overridePath = std::getenv("FDNY_ACTOR_ASSETS_DIR")) {
        const auto candidate = std::filesystem::path(overridePath) / "playerhose-stream";
        if (hasStreamPair(candidate)) return candidate;
    }
    for (auto root = scene.root(); !root.empty();) {
        for (const auto& candidate : {
                 root / "actor_exports" / "playerhose-stream",
                 root.parent_path() / "actor_exports" / "playerhose-stream",
                 root / "inputs" / "original" / "Data" / "sprites",
                 root / "inputs" / "original" / "Data" / "Sprites"})
            if (hasStreamPair(candidate)) return candidate;
        const auto parent = root.parent_path();
        if (parent == root) break;
        root = parent;
    }
    throw std::runtime_error("Cannot find hose.rrf resource-3 stream textures under "
                             "actor_exports/playerhose-stream or copied game data");
}

std::vector<std::string> victimSpawnScripts(J entity) {
    std::vector<std::string> scripts;
    // VictimRescue::Spawn (0045D5B0) dispatches exactly S_Script01..05.
    for (int i = 1; i <= 5; ++i) {
        char key[16];
        std::snprintf(key, sizeof(key), "S_Script%02d", i);
        scripts.push_back(str(field(field(entity, "properties"), key)));
    }
    return scripts;
}

V rotateRoamOffsetY(V value, float degrees) {
    const float radians = degrees * 0.01745329251994329577f;
    const float cosine = std::cos(radians), sine = std::sin(radians);
    return {cosine * value.x + sine * value.z, value.y,
            cosine * value.z - sine * value.x};
}

V normalizedRoamVector(V value) {
    const float magnitude = length(value);
    return magnitude > 0.000855f ? value * (1.f / magnitude) : value;
}

float angleBetweenDegrees(V left, V right) {
    const float leftLength = length(left), rightLength = length(right);
    if (leftLength == 0.f || rightLength == 0.f) return 180.f;
    const double cosine = std::clamp(double(dot(left, right)) /
                                     (double(leftLength) * rightLength), -1.0, 1.0);
    return float(std::acos(cosine) * (180.0 / 3.14159265358979323846));
}

bool victimCandidateValid(const PreviewVictim& victim, V offset, Collision& collision) {
    // 0045AB40: offset from the current target, not an absolute destination.
    constexpr double minimumOffsetSquared = 0.001;
    const V destination = victim.roamTarget + offset;
    if (double(dot(offset, offset)) < minimumOffsetSquared) return false;
    V fromRoamOrigin = destination - victim.roamCenter;
    if (double(dot(fromRoamOrigin, fromRoamOrigin)) >
        double(victim.roamRadius) * victim.roamRadius) return false;

    const V mins{-24.f, -24.f, -24.f}, maxs{24.f, 24.f, 24.f};
    Hit obstruction;
    if (collision.trace(victim.roamTarget, offset, mins, maxs, obstruction)) return false;
    for (const auto& obstacle : victim.roamObstacles) {
        V separation = destination - obstacle.center;
        if (double(dot(separation, separation)) < obstacle.radiusSquared) return false;
    }

    // The source checks floor at the destination offset rotated by both ±15°.
    // Each point must land within 6 units of the victim's current foot height.
    const float footY = victim.roamTarget.y - 24.f;
    for (float yaw : {15.f, -15.f}) {
        const V probe = victim.roamTarget + rotateRoamOffsetY(offset, yaw);
        const V rayStart{probe.x, probe.y + 50.f, probe.z};
        const V rayEnd{probe.x, probe.y - (240.f + 24.f), probe.z};
        Hit floor;
        if (!collision.tracePoint(rayStart, rayEnd, 1, 2, floor)) return false;
        if (std::abs(floor.point.y - footY) > 6.f) return false;
    }
    return true;
}

void acceptVictimRoamTarget(PreviewVictim& victim, V offset) {
    victim.previousRoamTarget = victim.roamTarget;
    victim.roamTarget = victim.roamTarget + offset;
}

std::vector<PreviewRoamObstacle> collectVictimRoamObstacles(
    const std::vector<J>& entities, Collision& collision) {
    std::vector<PreviewRoamObstacle> obstacles;
    for (J entity : entities) {
        if (str(field(entity, "class")) != "RCS_FireRescue") continue;
        FireVisual floorProbe;
        floorProbe.position = parseVector(property(entity, "ZZ_location"),
                                          parseVector(property(entity, "location"), {}));
        // Reuse FireRescue's exact floor-frame helper so victim obstacle
        // centers agree with the fire visual's authored L_FloorMe behavior.
        if (parseFloat(property(entity, "L_FloorMe"), 0.f) != 0.f)
            FireFloor::alignInWorld(floorProbe, collision);
        V center = floorProbe.position;

        // FireRescue virtual +0x0C returns root+0x7C; the native obstacle
        // builder reads its +0x0C, i.e. root+0x88. H_Radius is the matching
        // authored SceneData value used by the existing FireRescue port path.
        // The retail UserData-key-to-field assignment is not yet proven, so
        // retain the map value as a clearly bounded preview proxy.
        float radius = parseFloat(property(entity, "H_Radius"), 24.f);
        if (radius < 24.f || radius > 1200.f) radius = std::clamp(radius, 24.f, 1200.f);
        obstacles.push_back({center, radius});
    }
    return obstacles;
}

std::vector<PreviewFire> loadPreviewFires(const SceneData& scene, Collision& collision,
                                          Random& random, uint32_t now, int& dividerCycle) {
    std::vector<PreviewFire> fires;
    J orderedEntities = scene.entities();
    fires.reserve(size_t(count(orderedEntities)));
    for (int i = 0; i < count(orderedEntities); ++i) {
        J entity = at(orderedEntities, i);
        if (str(field(entity, "class")) != "RCS_FireRescue") continue;
        PreviewFire preview;
        preview.entity = integer(field(entity, "index"));
        preview.authoredName = property(entity, "A_Name");
        if (preview.authoredName == "<null>") preview.authoredName.clear();
        preview.label = preview.authoredName;
        if (preview.label.empty() || preview.label == "<null>")
            preview.label = "FireRescue #" + std::to_string(preview.entity);
        const V authored = parseVector(property(entity, "ZZ_location"),
                                       parseVector(property(entity, "location"), {}));
        const bool floorMe = parseFloat(property(entity, "L_FloorMe"), 0.f) != 0.f;
        preview.floorMe = floorMe;
        // The authored `moveto` fires in MBase and Gas R have no C_Model. In
        // that initializer branch, 00448D60 writes
        // (H_Radius,H_Height,H_Radius) to root[+0x108..+0x110]. Preserve the
        // +0x108 value consumed by the half-threshold gate in 00449340. The
        // model-derived branch is not fully mapped, so leave it on the
        // continuous host update path until its first component is resolved.
        const std::string fireModel = property(entity, "C_Model");
        preview.routeVisualUpdateThresholdMapped = fireModel.empty() || fireModel == "<null>";
        preview.routeVisualUpdateThreshold =
            parseFloat(property(entity, "H_Radius"), 0.f) * 0.5f;
        const bool named = !preview.authoredName.empty();
        // 00448D60 copies UserData A_Name (root+0x78+4) into the embedded
        // subobject string at root+0x48 when the pointer is present. The
        // spread gate reads that string object's length at +4, so an empty
        // name leaves spreading disabled. See fire_name_gate.md.
        preview.spreadNameGateEnabled = named;
        const bool smoke = parseFloat(property(entity, "M_Smoking"), 0.f) != 0.f;
        preview.runtime.initialize(
            random, now, dividerCycle, authored,
            parseFloat(property(entity, "H_Radius"), 24.f),
            parseFloat(property(entity, "H_Height"), 24.f),
            parseFloat(property(entity, "K_SprayTime"), 0.f), named, smoke,
            [&](FireVisual& visual) {
                if (floorMe) FireFloor::alignInWorld(visual, collision);
            },
            [](int) { /* start after SceneAudio loads the bundled FireBurn resources */ });
        preview.runtime.setInitialStrength(std::clamp(
            parseFloat(property(entity, "FDNY_InitialStrength"), 1.f), 0.f, 1.f));
        preview.runtime.visual.sizeUpdate(now);
        fires.push_back(std::move(preview));
    }
    return fires;
}

void startPreviewFireLoopSounds(std::vector<PreviewFire>& fires, SceneAudio& audio,
                               V listener, float yaw) {
    if (!audio.device) return;
    for (auto& fire : fires) {
        const char* sound = fireLoopSoundName(fire.runtime.soundChoice);
        if (!sound || fire.runtime.suppression.state == 5) continue;
        // 00448D60 starts one of the FireBurn 3D loops at FireRescue root +0x7C.
        audio.loopOwned(fire.entity, sound, fire.runtime.origin, listener, yaw, 2);
        fire.loopSoundStarted = true;
        fire.lastLoopSoundSource = fire.runtime.origin;
    }
}

void updatePreviewFireLoopSounds(std::vector<PreviewFire>& fires, SceneAudio& audio,
                                 V listener, float yaw) {
    if (!audio.device) return;
    for (auto& fire : fires) {
        if (!fire.loopSoundStarted) continue;
        const char* sound = fireLoopSoundName(fire.runtime.soundChoice);
        if (!sound) continue;
        if (fire.runtime.suppression.state == 5) {
            audio.stopOwned(fire.entity, sound);
            fire.loopSoundStarted = false;
            continue;
        }
        const V source = fire.runtime.origin;
        if (source.x != fire.lastLoopSoundSource.x ||
            source.y != fire.lastLoopSoundSource.y ||
            source.z != fire.lastLoopSoundSource.z) {
            audio.updateOwned(fire.entity, sound, source, listener, yaw);
            fire.lastLoopSoundSource = source;
        }
    }
}

int drawPreviewFires(std::vector<PreviewFire>& fires, FireRenderer& renderer, uint32_t now) {
    int quads = 0;
    for (auto& fire : fires) quads += renderer.draw(fire.runtime, now, true);
    return quads;
}

void updatePreviewFires(std::vector<PreviewFire>& fires, uint32_t now, float dt) {
    for (auto& fire : fires)
        fire.runtime.think(now, dt, [] {}, [] {}, [] {});
}

void updatePreviewFirePathMotion(std::vector<PreviewFire>& fires, Collision& collision,
                                 uint32_t now, float dt) {
    for (auto& fire : fires) {
        auto& motion = fire.pathMotion;
        if (!motion.active) continue;

        // 00449340 accumulates route distance as frameDelta * root[+0xFC].
        // For the authored no-C_Model FireRescue routes, 00448D60 sets
        // root[+0x108] from H_Radius and 00449340 updates the source visual
        // only after progress advances by more than half that value. Its
        // callback cadence and deferred child-spawn drain remain open.
        motion.progress += std::max(0.f, dt) * motion.rate;
        const bool complete = motion.progress >= motion.distance;
        if (!complete && motion.updateThresholdMapped &&
            motion.progress - motion.lastVisualProgress <= motion.updateThreshold)
            continue;

        V next = complete ? motion.target : motion.start + motion.direction * motion.progress;
        motion.lastVisualProgress = motion.progress;
        const V previousVisual = fire.runtime.visual.position;
        fire.runtime.visual.position = next;
        if (fire.floorMe) FireFloor::alignInWorld(fire.runtime.visual, collision);

        // FireRescue keeps the candidate origin and embedded visual point in
        // separate fields. Preserve their authored offset while moving both.
        const V visualDelta = fire.runtime.visual.position - previousVisual;
        fire.runtime.origin = fire.runtime.origin + visualDelta;
        fire.runtime.visual.sizes.changed = true;
        fire.runtime.visual.sizeUpdate(now);
        if (complete) motion.active = false;
    }
}

V playerRescueCandidateWorldOrigin(const Player& player) {
    // VictimRescue, FireRescue, and PropModel callbacks ask the candidate
    // vtable at +0x0C. PlayerRescue maps that slot through 0049DB90 to the
    // transform-origin getter: local (0, 0, 0) through root transform +0x24,
    // whose world position is the canonical translation at +0x4C..+0x54.
    // Player::p stores that same movement origin in this host.
    return player.p;
}

struct QueuedSpreadFire {
    FireSpreadSpawn spawn;
    int parentEntity = -1;
    float sprayTime = 1.f;
};

void queuePreviewFireSpreadForCandidate(PreviewFire& fire, Collision& collision,
                                        Random& random, V playerPosition, uint32_t now,
                                        int& nearbyCount,
                                        std::vector<QueuedSpreadFire>& queued) {
    if (!fire.spreadNameGateEnabled || fire.runtime.suppression.state == 5) return;
    const int distance = FireSpread::proximityDistance(fire.runtime.origin, playerPosition);
    FireSpread::updateNearbyCount(fire.lastSpreadProximity, distance, nearbyCount);
    fire.lastSpreadProximity = distance;
    if (distance >= 750) return; // 00448320: strict spread-attempt gate.

    FireSpreadAttempt attempt = FireSpread::attempt(
        fire.runtime.visual.sizes.size, fire.runtime.visual.position,
        fire.runtime.suppression.strength, now, fire.nextSpreadAttempt,
        nearbyCount, random,
        [&](V start, V delta, V mins, V maxs, V& point) {
            Hit hit;
            if (!collision.trace(start, delta, mins, maxs, hit)) return false;
            point = hit.point;
            return true;
        });
    if (attempt.result == FireSpreadAttempt::Result::spawned && attempt.child)
        queued.push_back({*attempt.child, fire.entity, fire.runtime.suppression.sprayTime});
}

void drainPreviewFireSpawnQueue(std::vector<PreviewFire>& fires, Collision& collision,
                                Random& random, uint32_t now, int& dividerCycle,
                                int& nextDynamicEntity,
                                const std::vector<QueuedSpreadFire>& queued) {
    // 00429520 drains the staged factory queue after 0049BD20 has completed
    // its contact traversal. Do the same here so a child never mutates the
    // active fire list while it is being visited.
    for (const auto& request : queued) {
        PreviewFire child;
        child.entity = nextDynamicEntity++;
        child.label = "Spread fire from #" + std::to_string(request.parentEntity);
        // 00447A90 zeroes the generated definition's A_Name slot at +4;
        // FireRescue init therefore leaves the embedded name empty and the
        // native spread gate rejects this child as a future parent.
        child.spreadNameGateEnabled = false;

        const float c = std::cos(request.spawn.heading);
        const float s = std::sin(request.spawn.heading);
        const float headingMatrix[9] = {c, 0.f, s, 0.f, 1.f, 0.f, -s, 0.f, c};
        child.runtime.initialize(
            random, now, dividerCycle, request.spawn.position, 0.f, 0.f,
            request.sprayTime, false, false,
            [&](FireVisual& visual) {
                std::copy(headingMatrix, headingMatrix + 9, visual.matrix);
                FireFloor::alignInWorld(visual, collision);
            }, [](int) {});
        child.runtime.origin = request.spawn.position;

        auto& size = child.runtime.visual.sizes;
        size.keys = {{true, now, {0.f, 0.f, 0.f}},
                     {true, request.spawn.growAt, request.spawn.peakSize},
                     {true, request.spawn.collapseAt,
                      {0.f, 0.f, request.spawn.peakSize.z * .25f}}};
        size.first = 0;
        size.next = 1;
        size.size = {};
        size.scale = {1.f, 1.f, 1.f};
        size.changed = false;
        child.runtime.visual.sizeUpdate(now);
        fires.push_back(std::move(child));
    }
}

void startVictimRoam(PreviewVictim& victim, float radius, Collision& collision,
                     Random& random, VictimActorAsset& actor, uint32_t now,
                     const std::vector<PreviewRoamObstacle>& fireObstacles) {
    // 0045C7F0: +0x224 is the authored roam radius; +0x218 remains the
    // pre-randomized origin copied during the first spawn-queue pass.
    victim.roamRadius = radius;
    victim.roamState = 7;
    victim.roaming = false;
    victim.roamTarget = victim.spawn.selectedPosition;
    victim.roamSpeed = 36.f;
    victim.roamObstacles.clear();
    for (const auto& obstacle : fireObstacles) {
        const V separation = obstacle.center - victim.spawn.selectedPosition;
        const float nearDistance = obstacle.radius + radius;
        if (double(dot(separation, separation)) <= double(nearDistance) * nearDistance) {
            const float expandedRadius = obstacle.radius + 48.f;
            victim.roamObstacles.push_back({obstacle.center, expandedRadius * expandedRadius});
        }
    }

    bool hasOpenOffset = false;
    for (V offset : {V{5.f, 0.f, 5.f}, V{5.f, 0.f, -5.f},
                     V{-5.f, 0.f, 5.f}, V{-5.f, 0.f, -5.f}}) {
        if (victimCandidateValid(victim, offset, collision)) {
            hasOpenOffset = true;
            break;
        }
    }
    if (!hasOpenOffset) {
        victim.roamState = 4;
        return;
    }

    // SelectVictimRoamAnimation (00459A50). Six-motion actors can roam;
    // other ACTs select a slow motion and return to state 4.
    const int motionCount = actor.motionCount();
    if (motionCount != 6) {
        const int motion = int(random.bounded(uint32_t(std::max(0, motionCount))));
        const float motionEnd = num(field(at(field(actor.data.get(), "motions"), motion), "end"));
        victim.animation.select(motion, 0.f, motionEnd, 0.2f, now);
        victim.roamState = 4;
        return;
    }
    const uint32_t choice = random.bounded(3);
    const int motion = int(choice) + 3;
    victim.roamSpeed = choice == 0 ? 50.4f : 19.2f;
    const float motionEnd = num(field(at(field(actor.data.get(), "motions"), motion), "end"));
    victim.animation.select(motion, 0.f, motionEnd, 1.f, now);
    victim.roaming = true;
}

void updateVictimRoamTarget(PreviewVictim& victim, Collision& collision,
                            Random& random, V playerPosition, float playerYaw,
                            bool playerCarryingVictim) {
    V targetToActor = victim.roamTarget - victim.spawn.selectedPosition;
    if (double(dot(targetToActor, targetToActor)) > 5184.0) return;

    if (victim.randomRoamDirection) {
        V offset = victim.roamDirection;
        const float magnitude = float(random.range(6.f, 18.f));
        const float oldLength = length(offset);
        if (oldLength > 0.000855f) offset = offset * (magnitude / oldLength);
        else offset = {magnitude, 0.f, 0.f};
        offset = rotateRoamOffsetY(offset, random.unit() * 360.f);
        offset.y = 0.f;
        if (!victimCandidateValid(victim, offset, collision)) return;
        victim.randomRoamDirection = false;
        acceptVictimRoamTarget(victim, offset);
        return;
    }

    V candidate{};
    V playerFromTarget = playerPosition - victim.roamTarget;
    const double playerDistanceSquared = double(dot(playerFromTarget, playerFromTarget));
    if (playerDistanceSquared >= 432.0 * 432.0) {
        candidate = normalizedRoamVector(victim.roamTarget - victim.previousRoamTarget) * 12.f;
    } else {
        const V victimForward{-std::sin(victim.facingDegrees * 0.01745329251994329577f), 0.f,
                               std::cos(victim.facingDegrees * 0.01745329251994329577f)};
        Hit sight;
        const bool blocked = collision.tracePoint(victim.roamTarget, playerPosition, 0x43, 2, sight);
        if (angleBetweenDegrees(victimForward, playerPosition - victim.spawn.selectedPosition) >= 45.f || blocked) {
            candidate = normalizedRoamVector(victim.roamTarget - victim.previousRoamTarget) * 12.f;
        } else {
            candidate = normalizedRoamVector(playerPosition - victim.roamTarget) * 12.f;
            if (playerCarryingVictim || playerDistanceSquared < 2304.0) {
                candidate = playerFromTarget * -1.f;
            } else {
                const V playerForward{-std::sin(playerYaw), 0.f, -std::cos(playerYaw)};
                if (angleBetweenDegrees(playerForward, victim.roamTarget - playerPosition) > 45.f) {
                    candidate = playerFromTarget * -1.f;
                } else if (playerDistanceSquared < 36864.0) {
                    // The executable consumes rand(15) here but discards its result.
                    (void)random.bounded(15);
                }
            }
        }
    }

    if (double(dot(candidate, candidate)) <= 0.0001) {
        victim.roamDirection = victim.roamTarget - victim.spawn.selectedPosition;
        victim.randomRoamDirection = true;
        return;
    }
    candidate.y = 0.f;
    if (victimCandidateValid(victim, candidate, collision)) {
        acceptVictimRoamTarget(victim, candidate);
        return;
    }

    V alternative = candidate * 0.5f;
    alternative = rotateRoamOffsetY(alternative, 30.f);
    if (victimCandidateValid(victim, alternative, collision)) {
        acceptVictimRoamTarget(victim, alternative);
        return;
    }
    alternative = rotateRoamOffsetY(alternative, -60.f);
    if (victimCandidateValid(victim, alternative, collision)) {
        acceptVictimRoamTarget(victim, alternative);
        return;
    }
    victim.roamDirection = victim.roamTarget - victim.spawn.selectedPosition;
    victim.randomRoamDirection = true;
}

std::vector<PreviewVictim> loadPreviewVictims(
    const SceneData& scene, Collision& collision, Random& random, uint32_t now,
    const std::filesystem::path& assetRoot,
    std::map<std::string, std::unique_ptr<VictimActorAsset>>& assets,
    bool prepareRenderers) {
    std::vector<J> entities;
    J sourceEntities = scene.entities();
    entities.reserve(size_t(count(sourceEntities)));
    std::map<int, J> entityByIndex;
    for (int i = 0; i < count(sourceEntities); ++i) {
        J entity = at(sourceEntities, i);
        entities.push_back(entity);
        entityByIndex[integer(field(entity, "index"))] = entity;
    }
    auto spawns = originalVictimCandidates(entities);
    const auto fireObstacles = collectVictimRoamObstacles(entities, collision);
    std::vector<PreviewVictim> victims;
    victims.reserve(spawns.size());
    // First ProcessEntitySpawnQueue pass: every VictimRescue actor is created,
    // resolves its model variant, and consumes its initial-motion draw before
    // any actor chooses a random spawn spot.
    for (auto& spawn : spawns) {
        J entity = entityByIndex.at(spawn.entity);
        PreviewVictim victim;
        // VictimRescue constructs its sound controller before selecting the
        // actor model/motion; its initial deadline consumes one shared draw.
        victim.sounds.initialize(random);
        VictimActorResource resource = resolveVictimActor(spawn.type, random);
        auto assetIt = assets.find(resource.stem);
        if (assetIt == assets.end()) {
            auto loaded = std::make_unique<VictimActorAsset>(assetRoot, resource.stem, prepareRenderers);
            assetIt = assets.emplace(resource.stem, std::move(loaded)).first;
        }
        VictimActorAsset& actor = *assetIt->second;
        spawn.modelId = resource.modelId;
        spawn.initializeMotion(random, actor.motionCount());
        const float end = num(field(at(field(actor.data.get(), "motions"), spawn.initialMotion), "end"));
        victim.spawn = std::move(spawn);
        victim.pickup.entity = victim.spawn.entity;
        victim.runtimeModelId = resource.modelId;
        victim.actorStem = resource.stem;
        victim.label = victim.spawn.name;
        if (victim.label.empty() || victim.label == "<null>")
            victim.label = victim.spawn.type + " #" + std::to_string(victim.spawn.entity);
        victim.animation.select(victim.spawn.initialMotion, 0.f, end,
                                victim.spawn.initialMotionSpeed, now);
        // +0x218 is copied from the authored initial location in the actor
        // creation pass. The later random spawn selection does not overwrite it.
        victim.roamCenter = victim.spawn.candidates.empty()
            ? victim.spawn.selectedPosition : victim.spawn.candidates.front().position;
        J sourceEntity = entityByIndex.at(victim.spawn.entity);
        victim.pickupRadius = parseFloat(property(sourceEntity, "J_Radius"), 60.f);
        if (victim.pickupRadius < 1.f) victim.pickupRadius = 60.f;
        victims.push_back(std::move(victim));
    }

    // Second queue pass: victim spawn selection precedes its S_Script01..05
    // dispatch. Alternate points have already been registered for all victims.
    std::map<int, int> materialVariantByModel;
    for (auto& victim : victims) {
        J entity = entityByIndex.at(victim.spawn.entity);
        VictimActorAsset& actor = *assets.at(victim.actorStem);
        victim.spawn.choose(random);
        victim.spawn.floorPlacementAttempted = true;
        V authoredPosition = victim.spawn.selectedPosition;
        victim.spawn.floorPlacementSucceeded = placeModelOnFloor(
            victim.spawn.selectedPosition, victim.spawn.modelMins, victim.spawn.modelMaxs,
            [&](V start, V delta, V mins, V maxs, V& point) {
                Hit hit;
                if (!collision.trace(start, delta, mins, maxs, hit)) return false;
                point = hit.point;
                return true;
            }, [] {});
        if (!victim.spawn.floorPlacementSucceeded) victim.spawn.selectedPosition = authoredPosition;
        victim.facingDegrees = victim.spawn.selectedHeading * 30.f + 180.f;
        victim.roamTarget = victim.spawn.selectedPosition;
        int& nextAppearance = materialVariantByModel[victim.runtimeModelId];
        victim.spawn.appearanceIndex = actor.materialCount() ? nextAppearance % actor.materialCount() : 0;
        if (actor.materialCount()) nextAppearance++;

        for (const std::string& command : victimSpawnScripts(entity)) {
            std::istringstream tokens(command);
            std::string verb;
            tokens >> verb;
            verb = lower(verb);
            if (verb == "norescuebot") {
                // 0045CDB0 writes the VictimRescue+0x1FD exclusion byte.
                victim.noRescueBot = true;
                continue;
            }
            if (verb != "roam") continue;
            float radius = 0.f;
            if (tokens >> radius) {
                victim.hasRoamCommand = true;
                startVictimRoam(victim, radius, collision, random, actor, now, fireObstacles);
            }
        }
    }
    return victims;
}

void updateRoamingVictims(std::vector<PreviewVictim>& victims, Collision& collision,
                          Random& random, const Player& player, float playerYaw,
                          bool playerCarryingVictim, float dt) {
    for (auto& victim : victims) {
        // Victim state 5 is carried; state 6 is the timed release animation.
        if (!victim.roaming || victim.roamState != 7 ||
            victim.pickup.state == 5 || victim.pickup.state == 6) continue;

        updateVictimRoamTarget(victim, collision, random, player.p, playerYaw,
                               playerCarryingVictim);
        V delta = victim.roamTarget - victim.spawn.selectedPosition;
        const double distanceSquared = victimRoamDistanceSquared(delta);
        if (distanceSquared > 36.0) {
            const float desired = float(std::atan2(-delta.x, delta.z) *
                                        (180.0 / 3.14159265358979323846));
            // Retail 00482AA0 applies 0.5^(frameDelta / 0.3) to the old
            // heading. Its update callback cadence is still unresolved, so
            // dt remains this viewer's rendered-frame delta.
            victim.facingDegrees = blendVictimRoamHeading(
                victim.facingDegrees, desired, dt);
            victim.spawn.selectedHeading = (victim.facingDegrees - 180.f) / 30.f;
        }

        const float stepLength = victim.roamSpeed * dt;
        victim.spawn.selectedPosition = advanceVictimRoamPosition(
            victim.spawn.selectedPosition, victim.roamTarget, stepLength);
    }
}

void drawPreviewVictims(const std::vector<PreviewVictim>& victims,
                        const std::map<std::string, std::unique_ptr<VictimActorAsset>>& assets,
                        uint32_t now) {
    for (const auto& victim : victims) {
        // 0045BEF0 stores zero in the Genesis actor flags on pickup; the
        // normal render pass skips actors unless flags & 0x1400 is nonzero.
        // No Player-relative carried-body transform is source-verified.
        if ((victim.pickup.worldActorFlags & 0x1400u) == 0) continue;
        auto asset = assets.find(victim.actorStem);
        if (asset == assets.end()) continue;
        ActorTransform root = victimActorRoot(victim.spawn);
        asset->second->renderer->draw(victim.spawn, victim.animation, now, root);
    }
}

void drawPreviewPropModels(const std::vector<PropModelInstance>& props,
                           const std::map<int, std::unique_ptr<EquipmentActorAsset>>& assets) {
    for (const auto& prop : props) {
        if (!prop.visible || !prop.modelEnabled) continue;
        auto asset = assets.find(prop.resourceModelId);
        if (asset == assets.end() || !asset->second->renderer) continue;
        asset->second->renderer->drawBindPose(propModelActorRoot(prop));
    }
}

std::vector<uint8_t> composeLightmap(const SceneFaceSpan& span, const LightStyleState& styles) {
    const size_t sampleCount = size_t(span.lightWidth) * size_t(span.lightHeight);
    if (span.lightStyleCount < 1 || span.lightStyleCount > 4 || sampleCount == 0 ||
        span.lightSamples.size() != sampleCount * 3 * size_t(span.lightStyleCount))
        return {};
    for (int plane = 0; plane < span.lightStyleCount; ++plane)
        if (span.lightTypes[size_t(plane)] < 0 || span.lightTypes[size_t(plane)] >= int(styles.scales.size()))
            return {};
    std::vector<uint8_t> composed(sampleCount * 3);
    for (size_t sample = 0; sample < sampleCount; ++sample) {
        for (size_t channel = 0; channel < 3; ++channel) {
            int32_t accumulator = 0;
            for (int plane = 0; plane < span.lightStyleCount; ++plane) {
                const size_t offset = (size_t(plane) * sampleCount + sample) * 3 + channel;
                const int value = span.lightSamples[offset];
                const int style = span.lightTypes[size_t(plane)];
                if (style == 0) {
                    // Genesis copies style-zero RGB into Q8 accumulators.
                    accumulator = value << 8;
                } else if (plane == 0) {
                    accumulator = value * styles.scales[size_t(style)];
                } else {
                    accumulator += value * styles.scales[size_t(style)];
                }
            }
            const int output = std::clamp(accumulator >> 8, 0, 255);
            composed[sample * 3 + channel] = uint8_t(output);
        }
    }
    return composed;
}

std::vector<uint8_t> paddedLightmapPixels(const SceneFaceSpan& span,
                                          const LightStyleState& styles) {
    std::vector<uint8_t> pixels = composeLightmap(span, styles);
    if (pixels.empty()) return {};
    const int paddedWidth = span.lightWidth + 2;
    const int paddedHeight = span.lightHeight + 2;
    std::vector<uint8_t> padded(size_t(paddedWidth) * size_t(paddedHeight) * 3);
    for (int y = -1; y <= span.lightHeight; ++y) {
        const int sourceY = std::clamp(y, 0, span.lightHeight - 1);
        for (int x = -1; x <= span.lightWidth; ++x) {
            const int sourceX = std::clamp(x, 0, span.lightWidth - 1);
            const size_t source = (size_t(sourceY) * size_t(span.lightWidth) + size_t(sourceX)) * 3;
            const size_t destination = (size_t(y + 1) * size_t(paddedWidth) + size_t(x + 1)) * 3;
            std::copy_n(pixels.begin() + source, 3, padded.begin() + destination);
        }
    }
    return padded;
}

void uploadLightmapPixels(SceneFaceSpan& span, GLuint atlasTexture,
                          const LightStyleState& styles, bool initial = false) {
    if (!span.lightmapAtlasAssigned || !atlasTexture) return;
    bool changed = initial || !span.hasUploadedLightStyleScales;
    for (int plane = 0; plane < span.lightStyleCount; ++plane) {
        const int style = span.lightTypes[size_t(plane)];
        const int scale = styles.scales[size_t(style)];
        if (span.uploadedLightStyleScales[size_t(plane)] != scale) changed = true;
        span.uploadedLightStyleScales[size_t(plane)] = scale;
    }
    span.hasUploadedLightStyleScales = true;
    if (!changed) return;
    std::vector<uint8_t> pixels = paddedLightmapPixels(span, styles);
    if (pixels.empty()) return;
    glBindTexture(GL_TEXTURE_2D, atlasTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, span.lightmapAtlasX, span.lightmapAtlasY,
                    span.lightWidth + 2, span.lightHeight + 2,
                    GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        throw std::runtime_error("OpenGL lightmap update failed with error " + std::to_string(error));
}

void buildLightmapAtlas(std::vector<SceneMesh>& meshes, TextureLoadSummary& textureSummary,
                        bool uploadTextures, const LightStyleState& styles) {
    struct Item {
        size_t mesh = 0;
        size_t span = 0;
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;
    };
    std::vector<Item> items;
    uint64_t totalArea = 0;
    int maximumRectWidth = 1;
    for (size_t meshIndex = 0; meshIndex < meshes.size(); ++meshIndex) {
        auto& mesh = meshes[meshIndex];
        for (size_t spanIndex = 0; spanIndex < mesh.faceSpans.size(); ++spanIndex) {
            auto& span = mesh.faceSpans[spanIndex];
            const bool stylesSupported = span.lightStyleCount > 0 && span.lightStyleCount <= 4 &&
                std::all_of(span.lightTypes.begin(),
                    span.lightTypes.begin() + span.lightStyleCount,
                    [&](int style) { return style >= 0 && style < int(styles.scales.size()); });
            const int64_t sampleBytes = int64_t(span.lightWidth) * span.lightHeight * 3 * span.lightStyleCount;
            if (!uploadTextures || !stylesSupported || span.triangleCount <= 0 ||
                span.lightWidth <= 0 || span.lightHeight <= 0 || sampleBytes <= 0 ||
                sampleBytes != int64_t(span.lightSamples.size()) ||
                mesh.lightmapUvs.size() != (mesh.positions.size() / 3) * 2)
                continue;
            Item item{meshIndex, spanIndex, span.lightWidth + 2, span.lightHeight + 2};
            items.push_back(item);
            totalArea += uint64_t(item.width) * uint64_t(item.height);
            maximumRectWidth = std::max(maximumRectWidth, item.width);
        }
    }

    if (!uploadTextures || items.empty()) return;

    auto nextPowerOfTwo = [](uint64_t value) {
        uint64_t result = 1;
        while (result < value && result <= uint64_t(std::numeric_limits<int>::max()) / 2)
            result <<= 1;
        if (result < value) throw std::runtime_error("Lightmap atlas dimension overflow");
        return int(result);
    };
    const uint64_t targetWidth = std::max<uint64_t>(maximumRectWidth + 4,
        uint64_t(std::ceil(std::sqrt(double(totalArea)) * 2.0)));
    const int atlasWidth = nextPowerOfTwo(targetWidth);
    std::sort(items.begin(), items.end(), [](const Item& left, const Item& right) {
        if (left.height != right.height) return left.height > right.height;
        return left.width > right.width;
    });
    int cursorX = 2, cursorY = 2, rowHeight = 0;
    for (auto& item : items) {
        if (item.width + 2 > atlasWidth)
            throw std::runtime_error("A face lightmap exceeds the shared atlas width");
        if (cursorX + item.width > atlasWidth) {
            cursorX = 2;
            cursorY += rowHeight;
            rowHeight = 0;
        }
        item.x = cursorX;
        item.y = cursorY;
        cursorX += item.width;
        rowHeight = std::max(rowHeight, item.height);
    }
    const int atlasHeight = nextPowerOfTwo(uint64_t(cursorY + rowHeight + 2));
    GLint maximumTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maximumTextureSize);
    if (maximumTextureSize <= 0 || atlasWidth > maximumTextureSize || atlasHeight > maximumTextureSize)
        throw std::runtime_error("Face lightmap atlas exceeds the OpenGL texture-size limit");

    std::vector<uint8_t> atlas(size_t(atlasWidth) * size_t(atlasHeight) * 3, 255);
    for (const auto& item : items) {
        auto& mesh = meshes[item.mesh];
        auto& span = mesh.faceSpans[item.span];
        const std::vector<uint8_t> padded = paddedLightmapPixels(span, styles);
        if (padded.size() != size_t(item.width) * size_t(item.height) * 3)
            throw std::runtime_error("Could not compose face lightmap " + std::to_string(span.faceId));
        for (int row = 0; row < item.height; ++row) {
            const size_t source = size_t(row) * size_t(item.width) * 3;
            const size_t destination = (size_t(item.y + row) * size_t(atlasWidth) + size_t(item.x)) * 3;
            std::copy_n(padded.begin() + source, size_t(item.width) * 3, atlas.begin() + destination);
        }
        span.lightmapAtlasX = item.x;
        span.lightmapAtlasY = item.y;
        span.lightmapAtlasAssigned = true;
        mesh.hasLightmappedFaces = true;
        for (int plane = 0; plane < span.lightStyleCount; ++plane)
            span.uploadedLightStyleScales[size_t(plane)] =
                styles.scales[size_t(span.lightTypes[size_t(plane)])];
        span.hasUploadedLightStyleScales = true;
        const size_t firstVertex = size_t(span.firstTriangle) * 3;
        const size_t vertexCount = size_t(span.triangleCount) * 3;
        for (size_t vertex = firstVertex; vertex < firstVertex + vertexCount; ++vertex) {
            const float u = std::clamp(mesh.lightmapUvs[vertex * 2], 0.f, 1.f);
            const float v = std::clamp(mesh.lightmapUvs[vertex * 2 + 1], 0.f, 1.f);
            mesh.lightmapUvs[vertex * 2] = (float(item.x + 1) + u * float(span.lightWidth)) /
                                             float(atlasWidth);
            mesh.lightmapUvs[vertex * 2 + 1] = (float(item.y + 1) + v * float(span.lightHeight)) /
                                                 float(atlasHeight);
        }
        ++textureSummary.uploadedFaceLightmaps;
    }

    // Reserve the atlas origin as a white texel so unlit faces in a mixed
    // mesh group pass through the multiply stage unchanged.
    const float whiteU = .5f / float(atlasWidth);
    const float whiteV = .5f / float(atlasHeight);
    for (auto& mesh : meshes) {
        for (const auto& span : mesh.faceSpans) {
            if (span.lightmapAtlasAssigned) continue;
            const size_t firstVertex = size_t(span.firstTriangle) * 3;
            const size_t vertexCount = size_t(span.triangleCount) * 3;
            for (size_t vertex = firstVertex; vertex < firstVertex + vertexCount; ++vertex) {
                mesh.lightmapUvs[vertex * 2] = whiteU;
                mesh.lightmapUvs[vertex * 2 + 1] = whiteV;
            }
        }
    }

    GLuint atlasTexture = 0;
    glGenTextures(1, &atlasTexture);
    if (!atlasTexture) throw std::runtime_error("OpenGL did not allocate a face lightmap atlas");
    glBindTexture(GL_TEXTURE_2D, atlasTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlasWidth, atlasHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, atlas.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        glDeleteTextures(1, &atlasTexture);
        throw std::runtime_error("OpenGL face lightmap atlas upload failed with error " +
                                 std::to_string(error));
    }
    for (auto& mesh : meshes) mesh.lightmapAtlasTexture = atlasTexture;
}

std::vector<SceneMesh> loadMeshes(const SceneData& scene, TextureLoadSummary& textureSummary,
                                  bool uploadTextures, const LightStyleState& styles) {
    textureSummary.uploadAttempted = uploadTextures;
    std::vector<SceneMesh> meshes;
    std::map<std::string, GLuint> textureCache;
    std::set<std::string> seenFiles;
    auto lightData = readSceneChunkBytes(scene, "lightdata.bin");
    if (lightData.empty()) lightData = readSceneChunkBytes(scene, "chunks/20_lightdata.bin");
    J groups = field(scene.worldMesh(), "groups");
    meshes.reserve(size_t(count(groups)));
    for (int i = 0; i < count(groups); ++i) {
        J group = at(groups, i);
        SceneMesh mesh;
        mesh.model = integer(field(group, "model"));
        J positions = field(group, "positions"), uvs = field(group, "uvs");
        if (count(positions) % 9 != 0 || count(uvs) % 6 != 0 ||
            count(positions) / 3 != count(uvs) / 2)
            throw std::runtime_error("Invalid vertex or UV array in scene mesh group " + std::to_string(i));
        mesh.positions.reserve(size_t(count(positions)));
        mesh.uvs.reserve(size_t(count(uvs)));
        J lightmapUvs = field(group, "lightmap_uvs");
        if (lightmapUvs && (json_object_get_type(lightmapUvs) != json_type_array ||
                           count(lightmapUvs) != count(positions) / 3 * 2))
            throw std::runtime_error("Invalid lightmap UV array in scene mesh group " + std::to_string(i));
        mesh.lightmapUvs.reserve(size_t(count(positions) / 3 * 2));
        for (int k = 0; k < count(positions); ++k) mesh.positions.push_back(num(at(positions, k)));
        for (int k = 0; k < count(uvs); ++k) mesh.uvs.push_back(num(at(uvs, k)));
        for (int k = 0; k < count(positions) / 3 * 2; ++k) {
            J uv = lightmapUvs ? at(lightmapUvs, k) : nullptr;
            if (uv && (!jsonNumber(uv) || !std::isfinite(json_object_get_double(uv))))
                throw std::runtime_error("Nonfinite lightmap UV in scene mesh group " + std::to_string(i));
            mesh.lightmapUvs.push_back(num(uv));
        }
        if (!mesh.positions.empty()) {
            const float infinity = std::numeric_limits<float>::infinity();
            mesh.boundsMin = {infinity, infinity, infinity};
            mesh.boundsMax = {-infinity, -infinity, -infinity};
            for (size_t vertex = 0; vertex + 2 < mesh.positions.size(); vertex += 3) {
                for (int axis = 0; axis < 3; ++axis) {
                    mesh.boundsMin[axis] = std::min(mesh.boundsMin[axis], mesh.positions[vertex + axis]);
                    mesh.boundsMax[axis] = std::max(mesh.boundsMax[axis], mesh.positions[vertex + axis]);
                }
            }
            mesh.boundsCenter = (mesh.boundsMin + mesh.boundsMax) * .5f;
            mesh.boundsRadius = length(mesh.boundsMax - mesh.boundsCenter);
        }

        J faceSpans = field(group, "face_spans");
        if (faceSpans) {
            if (json_object_get_type(faceSpans) != json_type_array)
                throw std::runtime_error("Invalid face_spans array in scene mesh group " + std::to_string(i));
            int expectedFirstTriangle = 0;
            const int triangleCount = count(positions) / 9;
            mesh.faceSpans.reserve(size_t(count(faceSpans)));
            for (int spanIndex = 0; spanIndex < count(faceSpans); ++spanIndex) {
                J source = at(faceSpans, spanIndex);
                SceneFaceSpan span;
                auto requiredInt = [&](const char* key) {
                    J value = field(source, key);
                    if (!value || json_object_get_type(value) != json_type_int)
                        throw std::runtime_error(std::string("Missing/noninteger face span field ") + key);
                    return integer(value);
                };
                auto requiredFloat = [&](const char* key) {
                    J value = field(source, key);
                    if (!jsonNumber(value) || !std::isfinite(json_object_get_double(value)))
                        throw std::runtime_error(std::string("Missing/nonfinite face span field ") + key);
                    float result = num(value);
                    if (!std::isfinite(result))
                        throw std::runtime_error(std::string("Out-of-range face span field ") + key);
                    return result;
                };
                span.faceId = requiredInt("face_id");
                span.firstTriangle = requiredInt("first_triangle");
                span.triangleCount = requiredInt("triangle_count");
                span.texinfoId = requiredInt("texinfo_id");
                span.texinfoFlags = requiredInt("texinfo_flags");
                span.texinfoAlpha = requiredFloat("texinfo_alpha");
                span.faceLight = requiredFloat("face_light");
                span.reflectiveScale = requiredFloat("reflective_scale");
                span.mipMapBias = requiredFloat("mip_map_bias");
                span.lightOffset = requiredInt("light_ofs");
                span.lightWidth = requiredInt("light_width");
                span.lightHeight = requiredInt("light_height");
                J lightTypes = field(source, "light_types");
                if (!lightTypes || json_object_get_type(lightTypes) != json_type_array ||
                    span.faceId < 0 || span.texinfoId < 0 || span.firstTriangle < 0 ||
                    span.triangleCount < 0 ||
                    span.firstTriangle != expectedFirstTriangle ||
                    int64_t(span.firstTriangle) + span.triangleCount > triangleCount || count(lightTypes) != 4)
                    throw std::runtime_error("Invalid or noncontiguous face span in scene mesh group " +
                                             std::to_string(i) + ", span " + std::to_string(spanIndex));
                for (int lightType = 0; lightType < 4; ++lightType) {
                    if (!at(lightTypes, lightType) ||
                        json_object_get_type(at(lightTypes, lightType)) != json_type_int ||
                        integer(at(lightTypes, lightType)) < 0 || integer(at(lightTypes, lightType)) > 255)
                        throw std::runtime_error("Invalid light type in scene mesh group " + std::to_string(i));
                    span.lightTypes[size_t(lightType)] = integer(at(lightTypes, lightType));
                }
                while (span.lightStyleCount < 4 &&
                       span.lightTypes[size_t(span.lightStyleCount)] != 255)
                    ++span.lightStyleCount;
                if (span.lightOffset >= 0 && span.lightStyleCount > 0 &&
                    span.lightWidth > 0 && span.lightHeight > 0 && !lightData.empty()) {
                    const int64_t sampleBytes = int64_t(span.lightWidth) * span.lightHeight * 3 * span.lightStyleCount;
                    const int64_t dataStart = int64_t(span.lightOffset) + 1;
                    if (sampleBytes <= 0 || dataStart < 1 || dataStart > int64_t(lightData.size()) ||
                        sampleBytes > int64_t(lightData.size()) - dataStart ||
                        lightData[size_t(span.lightOffset)] != 0x01)
                        throw std::runtime_error("Invalid LIGHTDATA span for face " + std::to_string(span.faceId));
                    span.lightSamples.assign(lightData.begin() + dataStart,
                                             lightData.begin() + dataStart + sampleBytes);
                }
                expectedFirstTriangle += span.triangleCount;
                mesh.faceSpans.push_back(span);
            }
            if (expectedFirstTriangle != triangleCount)
                throw std::runtime_error("Face spans do not cover every triangle in scene mesh group " +
                                         std::to_string(i));
        }

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
    buildLightmapAtlas(meshes, textureSummary, uploadTextures, styles);
    return meshes;
}

std::vector<PreviewBModelMotion> loadBModelMotions(const SceneData& scene) {
    J exportedMotions = scene.motions();

    std::map<std::string, int> modelIndices;
    J entities = scene.entities();
    for (int i = 0; i < count(entities); ++i) {
        J entity = at(entities, i);
        if (str(field(entity, "class")) != "%Model%") continue;
        const std::string name = property(entity, "%name%");
        const int modelIndex = parseFloat(property(entity, "Model"), -1.f);
        if (!name.empty() && modelIndex >= 0) {
            auto [it, inserted] = modelIndices.emplace(name, modelIndex);
            if (!inserted && it->second != modelIndex)
                throw std::runtime_error("Ambiguous %Model% name in scene export: " + name);
        }
    }

    std::map<int, J> pathsByModel;
    J records = field(exportedMotions, "records");
    for (int i = 0; i < count(records); ++i) {
        J record = at(records, i);
        const int modelIndex = integer(field(record, "model_index"));
        J paths = field(record, "paths");
        if (count(paths) > 1)
            throw std::runtime_error("Retail GBSP BModel motion: multiple PATHs per model are not ported yet (model " +
                                     std::to_string(modelIndex) + ")");
        if (count(paths) == 1) pathsByModel[modelIndex] = at(paths, 0);
    }

    std::map<int, J> collisionModels;
    J exportedCollisionModels = field(scene.collision(), "models");
    for (int i = 0; i < count(exportedCollisionModels); ++i) {
        J model = at(exportedCollisionModels, i);
        collisionModels[integer(field(model, "index"))] = model;
    }

    std::vector<PreviewBModelMotion> result;
    for (int i = 0; i < count(entities); ++i) {
        J entity = at(entities, i);
        if (str(field(entity, "class")) != "RCS_BModel") continue;
        PreviewBModelMotion bmodel;
        bmodel.entity = integer(field(entity, "index"));
        bmodel.authoredName = property(entity, "A_Name");
        bmodel.sourcePosition = parseVector(property(entity, "ZZ_location"),
                                             parseVector(property(entity, "location"), {}));
        bool foundChild = false;
        bool hasMotionRange = false;
        for (int child = 0; child < 64; ++child) {
            const std::string propertyName = "C_AnimModel" + std::to_string(child);
            const std::string modelName = property(entity, propertyName.c_str());
            if (modelName.empty() || modelName == "<null>") continue;
            foundChild = true;
            auto model = modelIndices.find(modelName);
            if (model == modelIndices.end())
                throw std::runtime_error("BModel child has no matching %Model% export: " + modelName);
            auto bounds = collisionModels.find(model->second);
            if (bounds != collisionModels.end()) {
                V lo = vec(field(bounds->second, "mins"));
                V hi = vec(field(bounds->second, "maxs"));
                for (int axis = 0; axis < 3; ++axis) {
                    bmodel.boundsMin[axis] = std::min(bmodel.boundsMin[axis], lo[axis]);
                    bmodel.boundsMax[axis] = std::max(bmodel.boundsMax[axis], hi[axis]);
                }
                bmodel.hasBounds = true;
                const int root = integer(field(bounds->second, "root"));
                J nodes = field(scene.collision(), "nodes");
                if (root >= 0 && root < count(nodes)) {
                    constexpr float sourceBoundsInset = 128.f;
                    J rootNode = at(nodes, root);
                    V openLo = vec(field(rootNode, "mins")) + V{sourceBoundsInset, sourceBoundsInset, sourceBoundsInset};
                    V openHi = vec(field(rootNode, "maxs")) - V{sourceBoundsInset, sourceBoundsInset, sourceBoundsInset};
                    for (int axis = 0; axis < 3; ++axis) {
                        bmodel.openAwayBoundsMin[axis] = std::min(bmodel.openAwayBoundsMin[axis], openLo[axis]);
                        bmodel.openAwayBoundsMax[axis] = std::max(bmodel.openAwayBoundsMax[axis], openHi[axis]);
                    }
                    bmodel.hasOpenAwayBounds = true;
                }
            }
            auto path = pathsByModel.find(model->second);
            if (path == pathsByModel.end()) continue;
            auto prepared = RetailGbspMotion::preparePath(path->second);
            auto includeTimes = [&](const auto& keys) {
                if (keys.empty()) return;
                if (!hasMotionRange) {
                    bmodel.startTime = keys.front().time;
                    bmodel.endTime = keys.back().time;
                    hasMotionRange = true;
                } else {
                    bmodel.startTime = std::min(bmodel.startTime, keys.front().time);
                    bmodel.endTime = std::max(bmodel.endTime, keys.back().time);
                }
            };
            includeTimes(prepared.translation);
            includeTimes(prepared.rotation);
            V rotationalCenter{};
            if (bounds != collisionModels.end())
                rotationalCenter = vec(field(bounds->second, "origin"));
            bmodel.children.push_back({model->second, rotationalCenter, std::move(prepared)});
        }
        if (!foundChild || !bmodel.hasBounds) continue;
        bmodel.currentTime = bmodel.startTime;
        const float authoredReach = parseFloat(property(entity, "E_Reach"), 0.f);
        const int hitPoints = int(parseFloat(property(entity, "F_HitPoints"), 0.f));
        const int liftable = int(parseFloat(property(entity, "G_Liftable"), 0.f));
        bmodel.hitPoints = hitPoints;
        bmodel.liftable = liftable;
        bmodel.contactReach = BModelContact::effectiveReach(
            authoredReach, hitPoints, liftable, hasMotionRange ? bmodel.endTime : 0.f);
        if (bmodel.authoredName.empty() || lower(bmodel.authoredName).rfind("<null>", 0) == 0)
            bmodel.authoredName.clear();
        result.push_back(std::move(bmodel));
    }
    return result;
}

std::vector<PreviewBoundsTrigger> loadBoundsTriggers(const SceneData& scene) {
    std::map<std::string, int> modelIndices;
    J entities = scene.entities();
    for (int i = 0; i < count(entities); ++i) {
        J entity = at(entities, i);
        if (str(field(entity, "class")) != "%Model%") continue;
        const std::string name = property(entity, "%name%");
        const int modelIndex = parseFloat(property(entity, "Model"), -1.f);
        if (!name.empty() && modelIndex >= 0) {
            auto [it, inserted] = modelIndices.emplace(name, modelIndex);
            if (!inserted && it->second != modelIndex)
                throw std::runtime_error("Ambiguous %Model% name in scene export: " + name);
        }
    }

    std::map<int, J> collisionModels;
    J exportedCollisionModels = field(scene.collision(), "models");
    for (int i = 0; i < count(exportedCollisionModels); ++i) {
        J model = at(exportedCollisionModels, i);
        collisionModels[integer(field(model, "index"))] = model;
    }

    std::vector<PreviewBoundsTrigger> result;
    for (int i = 0; i < count(entities); ++i) {
        J entity = at(entities, i);
        if (str(field(entity, "class")) != "RCS_BoundsTrigger") continue;
        const std::string modelName = property(entity, "C_Model");
        const std::string subject = "BoundsTrigger entity " +
            std::to_string(integer(field(entity, "index"))) + " C_Model `" + modelName + "`";
        if (modelName.empty() || modelName == "<null>") {
            // Retail InitializeBoundsTriggerEntity (00441040) copies root
            // fields +0xC8/+0xCC/+0xD0 into both bounds when the model pointer
            // is null. CheckBoundsTriggerContact (00440CE0) also requires the
            // UserData model pointer at +8, so don't invent a contact volume
            // for an export record with no C_Model.
            continue;
        }
        auto modelIndex = modelIndices.find(modelName);
        if (modelIndex == modelIndices.end()) {
            std::cerr << "Unresolved trigger render/contact join: " << subject
                      << " has no matching %Model% record\n";
            continue;
        }
        auto collisionModel = collisionModels.find(modelIndex->second);
        if (collisionModel == collisionModels.end()) {
            std::cerr << "Unresolved trigger render/contact join: " << subject
                      << " resolves to model " << modelIndex->second
                      << " without collision bounds\n";
            continue;
        }

        PreviewBoundsTrigger trigger;
        trigger.entity = integer(field(entity, "index"));
        trigger.modelIndex = modelIndex->second;
        trigger.authoredName = property(entity, "A_Name");
        trigger.modelName = modelName;
        trigger.boundsMin = vec(field(collisionModel->second, "mins"));
        trigger.boundsMax = vec(field(collisionModel->second, "maxs"));
        trigger.contactReach = parseFloat(property(entity, "E_Reach"), 0.f);
        if (trigger.boundsMin.x > trigger.boundsMax.x ||
            trigger.boundsMin.y > trigger.boundsMax.y ||
            trigger.boundsMin.z > trigger.boundsMax.z) {
            std::cerr << "Unresolved trigger render/contact join: " << subject
                      << " has invalid collision bounds\n";
            continue;
        }
        result.push_back(std::move(trigger));
    }
    return result;
}

void sampleBModelMotions(const std::vector<PreviewBModelMotion>& motions, float time,
                         std::map<int, Pose>& poses) {
    for (const auto& bmodel : motions)
        for (const auto& child : bmodel.children)
            poses[child.model] = child.path.sample(time);
}

void sampleBModelState(const PreviewBModelMotion& bmodel, Collision& collision) {
    for (const auto& child : bmodel.children)
        collision.setPose(child.model, child.path.sample(bmodel.currentTime));
}

Q multiplyQuaternion(Q a, Q b) {
    return {
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
    };
}

Q conjugateQuaternion(Q q) { return {q.w, -q.x, -q.y, -q.z}; }

void chooseBModelOpenAway(PreviewBModelMotion& bmodel, V playerPosition) {
    if (!bmodel.hasOpenAwayBounds) return;
    const V corner0{bmodel.openAwayBoundsMin.x, 0.f, bmodel.openAwayBoundsMin.z};
    const V corner1{bmodel.openAwayBoundsMax.x, 0.f, bmodel.openAwayBoundsMax.z};
    for (auto& child : bmodel.children) {
        auto& keys = child.path.rotation;
        if (keys.size() != 2) continue;

        // 0043D6A0 forms transpose(key0) * key1, rotates the horizontal
        // near-to-far owner-bound direction, then transposes key 1 when the
        // result points toward the player. Quaternion multiplication and
        // normalization model the corresponding Genesis XForm operations;
        // their exact float bits have not been compared against the DLL.
        const Q first = normalizeQuaternion(keys[0].value);
        const Q second = normalizeQuaternion(keys[1].value);
        Q relative = normalizeQuaternion(multiplyQuaternion(conjugateQuaternion(first), second));
        const V offset0 = corner0 - child.rotationalCenter;
        const V offset1 = corner1 - child.rotationalCenter;
        V direction = length(offset0) <= length(offset1) ? corner1 - corner0 : corner0 - corner1;
        const float directionLength = length(direction);
        if (directionLength != 0.f) direction = direction * (1.f / directionLength);

        if (dot(rotateQ(direction, relative), playerPosition - child.rotationalCenter) >= 0.f) {
            relative = conjugateQuaternion(relative);
            keys[1].value = normalizeQuaternion(multiplyQuaternion(first, relative));
        }
    }
}

void commandBModel(PreviewBModelMotion& bmodel, const std::string& command, V playerPosition = {}) {
    if (command == "open") {
        if (bmodel.motionState == 4) bmodel.pendingCommand = "open";
        else bmodel.motionState = 3;
    } else if (command == "openaway" || command == "opentowards") {
        if (bmodel.motionState == 4) bmodel.pendingCommand = command;
        else {
            chooseBModelOpenAway(bmodel, playerPosition);
            bmodel.motionState = 3;
        }
    } else if (command == "close") {
        if (bmodel.motionState == 3) bmodel.pendingCommand = "close";
        else bmodel.motionState = 4;
    }
}

void updateBModelStates(std::vector<PreviewBModelMotion>& bmodels, float dt,
                        Collision& collision, V playerPosition = {},
                        const std::function<void(int, size_t, const std::string&)>&
                            dispatchTimedCommand = {}) {
    for (auto& bmodel : bmodels) {
        if (bmodel.pendingCommand) {
            if ((*bmodel.pendingCommand == "openaway" || *bmodel.pendingCommand == "opentowards") &&
                bmodel.motionState == 4) {
                // Native openaway requeues itself while the owner is closing.
                // Keep the request until the close reaches its start endpoint.
            } else {
                if (*bmodel.pendingCommand == "openaway" || *bmodel.pendingCommand == "opentowards") {
                    chooseBModelOpenAway(bmodel, playerPosition);
                    bmodel.motionState = 3;
                } else {
                    bmodel.motionState = *bmodel.pendingCommand == "open" ? 3 : 4;
                }
                bmodel.pendingCommand.reset();
            }
        }
        // UpdateBModelEntity (0043DC60) exits before its timer loop when the
        // command state is zero on entry. A command which reaches state zero
        // while clamping this frame still reaches the timer loop.
        const int motionStateAtUpdate = bmodel.motionState;
        const float previousTime = bmodel.currentTime;
        if (bmodel.motionState == 3) {
            bmodel.currentTime += dt * bmodel.timeScale;
            if (bmodel.currentTime >= bmodel.endTime) {
                bmodel.currentTime = bmodel.endTime;
                bmodel.motionState = 0;
            }
        } else if (bmodel.motionState == 4) {
            bmodel.currentTime -= dt * bmodel.timeScale;
            if (bmodel.currentTime <= bmodel.startTime) {
                bmodel.currentTime = bmodel.startTime;
                bmodel.motionState = 0;
            }
        }
        // Retail samples every child PATH transform before checking this
        // owner's timed command array. Keep those operations in the same
        // per-owner order so a due script sees the updated pose.
        sampleBModelState(bmodel, collision);

        if (motionStateAtUpdate == 0 || !dispatchTimedCommand) continue;
        const float stepDistance = std::abs(bmodel.currentTime - previousTime);
        for (size_t timerIndex = 0; timerIndex < bmodel.timedCommands.size(); ++timerIndex) {
            if (!bmodel.timedCommands[timerIndex].active) continue;
            const float keyDistance = std::abs(
                bmodel.currentTime - bmodel.timedCommands[timerIndex].keyFrame);
            // 0043DC60: fcompp, fnstsw ax, test ah,0x41, jp. The ordered
            // branch is strict stepDistance > keyDistance; equality is not a
            // dispatch. This is the retail distance predicate, not a generic
            // inclusive interval-crossing test.
            if (!(stepDistance > keyDistance)) continue;
            const std::string command = bmodel.timedCommands[timerIndex].command;
            dispatchTimedCommand(bmodel.entity, timerIndex, command);
            // Retail clears the active byte after dispatch returns. Indexing
            // after the callback also tolerates addtimecmd growing this vector.
            bmodel.timedCommands[timerIndex].active = false;
            break; // At most one due record is dispatched per BModel callback.
        }
    }
}

bool hasScriptEvent(const Scripts& scripts, int entity, const std::string& event) {
    auto found = scripts.bindings.find(entity);
    return found != scripts.bindings.end() &&
        std::any_of(found->second.begin(), found->second.end(), [&](const Scripts::Binding& binding) {
            return binding.event == event;
        });
}

bool scriptCallsPlayerCommand(const std::string& raw, const std::string& expectedCommand) {
    const std::string command = unwrap(raw);
    const auto [verb, arguments] = token(command);
    const std::string foldedVerb = lower(verb);
    if (foldedVerb == "cmd") {
        for (const auto& part : splitList(unwrap(arguments)))
            if (scriptCallsPlayerCommand(part, expectedCommand)) return true;
        return false;
    }
    if (foldedVerb != "tell" && foldedVerb != "action") return false;
    const auto [target, body] = token(arguments);
    if (lower(target) != "player") return false;
    const auto [targetCommand, ignored] = token(unwrap(body));
    (void)ignored;
    return lower(targetCommand) == lower(expectedCommand);
}

bool hasPlayerScriptCommand(const Scripts& scripts, int entity, const std::string& event,
                            const std::string& command) {
    auto found = scripts.bindings.find(entity);
    if (found == scripts.bindings.end()) return false;
    return std::any_of(found->second.begin(), found->second.end(), [&](const Scripts::Binding& binding) {
        return binding.event == event && scriptCallsPlayerCommand(binding.command, command);
    });
}

PreviewBModelMotion& bmodelById(std::vector<PreviewBModelMotion>& bmodels, int entity) {
    auto found = std::find_if(bmodels.begin(), bmodels.end(), [&](const PreviewBModelMotion& bmodel) {
        return bmodel.entity == entity;
    });
    if (found == bmodels.end())
        throw std::runtime_error("BModel interaction check could not resolve entity " + std::to_string(entity));
    return *found;
}

void runInteractionCheck(const SceneData& scene, J entities,
                         std::vector<PreviewBModelMotion>& bmodels,
                         const std::vector<PreviewBoundsTrigger>& boundsTriggers,
                         std::vector<std::pair<int, std::string>>& soundDispatches,
                         Scripts* scripts, Collision& collision, Player& player) {
    if (!scripts) throw std::runtime_error("Interaction check requires a bundled EVS source package");
    if (lower(scene.key()) == "subway_a") {
        int doorEntity = -1;
        for (int i = 0; i < count(entities); ++i) {
            J entity = at(entities, i);
            if (str(field(entity, "class")) == "RCS_BModel" &&
                lower(property(entity, "S_Script01")) == "*subdoor") {
                doorEntity = integer(field(entity, "index"));
                break;
            }
        }
        if (doorEntity < 0 || !hasScriptEvent(*scripts, doorEntity, "touch") ||
            !hasScriptEvent(*scripts, doorEntity, "untouch"))
            throw std::runtime_error("Subway A test door is missing the authored touch/untouch bindings");
        auto& door = bmodelById(bmodels, doorEntity);
        const V center = (door.boundsMin + door.boundsMax) * .5f;
        if (!BModelContact::overlapsExpanded(door.boundsMin, door.boundsMax,
                                             center + V{-30.f, -30.f, -30.f},
                                             center + V{30.f, 85.f, 30.f}, door.contactReach))
            throw std::runtime_error("Subway A test player does not overlap the selected door bounds");
        scripts->dispatch(doorEntity, "touch");
        if (door.motionState != 3 || std::abs(door.timeScale - 4.f) > 1.0e-6f)
            throw std::runtime_error("Subway A touch did not apply set-timescale 4 then open");
        const float step = 1.f / 60.f;
        for (int i = 0; i < 2000 && door.motionState != 0; ++i)
            updateBModelStates(bmodels, step, collision);
        if (door.motionState != 0 || std::abs(door.currentTime - door.endTime) > 1.0e-4f)
            throw std::runtime_error("Subway A open command did not reach its authored PATH endpoint");
        scripts->dispatch(doorEntity, "untouch");
        if (door.motionState != 4 || std::abs(door.timeScale - 4.f) > 1.0e-6f)
            throw std::runtime_error("Subway A untouch did not close while retaining time scale 4");
        for (int i = 0; i < 2000 && door.motionState != 0; ++i)
            updateBModelStates(bmodels, step, collision);
        if (door.motionState != 0 || std::abs(door.currentTime - door.startTime) > 1.0e-4f)
            throw std::runtime_error("Subway A close command did not return to the authored PATH start");
        int brokenDoorTrigger = -1;
        for (int i = 0; i < count(entities); ++i) {
            J entity = at(entities, i);
            if (str(field(entity, "class")) == "RCS_BoundsTrigger" &&
                lower(property(entity, "A_Name")) == "brokendoor") {
                brokenDoorTrigger = integer(field(entity, "index"));
                break;
            }
        }
        auto trigger = std::find_if(boundsTriggers.begin(), boundsTriggers.end(), [&](const auto& item) {
            return item.entity == brokenDoorTrigger;
        });
        if (trigger == boundsTriggers.end() || !hasScriptEvent(*scripts, brokenDoorTrigger, "touch"))
            throw std::runtime_error("Subway A broken-door BoundsTrigger is missing its touch binding or model bounds");
        const V triggerCenter = (trigger->boundsMin + trigger->boundsMax) * .5f;
        if (!BModelContact::overlapsExpanded(trigger->boundsMin, trigger->boundsMax,
                                             triggerCenter + V{-30.f, -30.f, -30.f},
                                             triggerCenter + V{30.f, 85.f, 30.f},
                                             trigger->contactReach))
            throw std::runtime_error("Subway A test player does not overlap the broken-door trigger bounds");
        scripts->dispatch(brokenDoorTrigger, "touch");
        const bool playedZap = std::any_of(soundDispatches.begin(), soundDispatches.end(), [&](const auto& sound) {
            return sound.first == brokenDoorTrigger && lower(sound.second) == "zapp3d";
        });
        if (!playedZap)
            throw std::runtime_error("Subway A broken-door touch did not resolve its authored Zapp3d sound command");
        std::cout << "Interaction check: Subway A entity " << doorEntity << " applied touch -> speed 4 -> open, "
                  << "then untouch -> close; PATH range " << door.startTime << ".." << door.endTime
                  << " returned to its start; BoundsTrigger " << brokenDoorTrigger
                  << " touch dispatched Zapp3d.\n";
        return;
    }
    if (lower(scene.key()) == "bank") {
        int consoleEntity = -1;
        int vaultEntity = -1;
        int contactDoorEntity = -1;
        float contactDoorSpawnDistanceSquared = std::numeric_limits<float>::infinity();
        const V playerSpawn = player.p;
        auto distanceSquaredToBounds = [](V point, V lo, V hi) {
            float distanceSquared = 0.f;
            for (int axis = 0; axis < 3; ++axis) {
                const float nearest = std::clamp(point[axis], lo[axis], hi[axis]);
                const float delta = point[axis] - nearest;
                distanceSquared += delta * delta;
            }
            return distanceSquared;
        };
        for (int i = 0; i < count(entities); ++i) {
            J entity = at(entities, i);
            if (str(field(entity, "class")) != "RCS_BModel") continue;
            if (lower(property(entity, "S_Script02")) == "*vaultconsole")
                consoleEntity = integer(field(entity, "index"));
            if (lower(property(entity, "A_Name")) == "vault")
                vaultEntity = integer(field(entity, "index"));
            if (lower(property(entity, "S_Script01")) == "*door") {
                const int candidateEntity = integer(field(entity, "index"));
                if (!hasScriptEvent(*scripts, candidateEntity, "touch") ||
                    !hasScriptEvent(*scripts, candidateEntity, "untouch")) continue;
                const auto& candidate = bmodelById(bmodels, candidateEntity);
                const float distanceSquared = distanceSquaredToBounds(
                    playerSpawn, candidate.boundsMin, candidate.boundsMax);
                if (distanceSquared < contactDoorSpawnDistanceSquared) {
                    contactDoorEntity = candidateEntity;
                    contactDoorSpawnDistanceSquared = distanceSquared;
                }
            }
        }
        if (contactDoorEntity < 0 || !hasScriptEvent(*scripts, contactDoorEntity, "touch") ||
            !hasScriptEvent(*scripts, contactDoorEntity, "untouch"))
            throw std::runtime_error("Bank authored *Door touch/untouch fixture near the player spawn is missing");
        auto& contactDoor = bmodelById(bmodels, contactDoorEntity);
        const V doorCenter = (contactDoor.boundsMin + contactDoor.boundsMax) * .5f;
        const float step = 1.f / 60.f;
        for (int frame = 0; frame < 120; ++frame)
            player.step(step, 0.f, 0.f, 0.f, false, false, false, collision);
        const float approachDistance = length(V{doorCenter.x - player.p.x, 0.f, doorCenter.z - player.p.z});
        if (approachDistance == 0.f)
            throw std::runtime_error("Bank approach fixture starts at the selected *Door bounds");
        V approachDirection{doorCenter.x - player.p.x, 0.f, doorCenter.z - player.p.z};
        approachDirection = approachDirection * (1.f / length(approachDirection));
        const float approachYaw = std::atan2(-approachDirection.x, -approachDirection.z);
        bool doorContact = false;
        bool touchedByApproach = false;
        bool openedByApproach = false;
        bool crossedDoorPlane = false;
        V approachTouchPosition{};
        const float clearDistance = 50.f;
        int simulationFrame = 0;
        auto approachFrame = [&](float forward, float side) {
            const uint32_t now = SDL_GetTicks() + uint32_t(simulationFrame++ * 17);
            updateBModelStates(bmodels, step, collision, player.p);
            if (!contactDoor.children.empty() && contactDoor.motionState == 0 &&
                std::abs(contactDoor.currentTime - contactDoor.endTime) <= 1.0e-4f &&
                collision.poses.count(contactDoor.children.front().model))
                openedByApproach = true;
            player.step(step, forward, side, approachYaw, false, false, false, collision);
            const V actorMin = player.p + V{-30.f, -30.f, -30.f};
            const V actorMax = player.p + player.maxs();
            const bool inside = BModelContact::overlapsExpanded(
                contactDoor.boundsMin, contactDoor.boundsMax, actorMin, actorMax,
                contactDoor.contactReach);
            if (inside && !doorContact) {
                scripts->dispatch(contactDoorEntity, "touch");
                touchedByApproach = true;
                approachTouchPosition = player.p;
            }
            if (inside) scripts->dispatch(contactDoorEntity, "touching");
            if (!inside && doorContact) scripts->dispatch(contactDoorEntity, "untouch");
            doorContact = inside;
            scripts->advanceEntity(contactDoorEntity, now);
            crossedDoorPlane = dot(player.p - doorCenter, approachDirection) > clearDistance;
        };

        // First record a continuous approach from the same grounded start used
        // by the control route below: no wait at the door, centering, or M/N
        // preview input. Restore the simulated runtime state afterwards so the
        // existing endpoint/alignment control remains an independent check.
        const Player unassistedStartPlayer = player;
        const PreviewBModelMotion unassistedStartDoor = contactDoor;
        const auto unassistedStartPoses = collision.poses;
        const auto unassistedStartAngles = collision.angles;
        const auto unassistedStartQueues = scripts->actorQueues;
        const auto unassistedStartVariables = scripts->variables;
        const auto unassistedStartOnce = scripts->once;
        const uint32_t unassistedStartScriptTime = scripts->timeMs;
        const size_t unassistedStartSounds = soundDispatches.size();
        int unassistedFrames = 0;
        int stalledFrames = 0;
        V previousPosition = player.p;
        for (; unassistedFrames < 480 && !crossedDoorPlane; ++unassistedFrames) {
            approachFrame(1.f, 0.f);
            const float moved = length(player.p - previousPosition);
            stalledFrames = moved < .05f ? stalledFrames + 1 : 0;
            previousPosition = player.p;
            if (stalledFrames >= 45) break;
        }
        const bool unassistedTouched = touchedByApproach;
        const bool unassistedOpened = openedByApproach;
        const bool unassistedCrossed = crossedDoorPlane;
        const V unassistedTouchPosition = approachTouchPosition;
        const V unassistedStopPosition = player.p;
        const float unassistedPathTime = contactDoor.currentTime;
        const size_t unassistedOpenSounds = std::count_if(
            soundDispatches.begin() + std::min(unassistedStartSounds, soundDispatches.size()),
            soundDispatches.end(), [&](const auto& sound) {
                return sound.first == contactDoorEntity && lower(sound.second) == "opendoor";
            });
        Hit unassistedObstruction;
        const bool unassistedTraceBlocked = collision.trace(
            player.p, approachDirection * 180.f, {-30.f, -30.f, -30.f},
            player.maxs(), unassistedObstruction);

        player = unassistedStartPlayer;
        contactDoor = unassistedStartDoor;
        collision.poses = unassistedStartPoses;
        collision.angles = unassistedStartAngles;
        scripts->actorQueues = unassistedStartQueues;
        scripts->variables = unassistedStartVariables;
        scripts->once = unassistedStartOnce;
        scripts->timeMs = unassistedStartScriptTime;
        soundDispatches.resize(unassistedStartSounds);
        doorContact = false;
        touchedByApproach = false;
        openedByApproach = false;
        crossedDoorPlane = false;
        approachTouchPosition = {};
        simulationFrame = 0;

        std::cout << "Bank unassisted door route: "
                  << (unassistedCrossed ? "crossed" : "did not cross")
                  << " after " << unassistedFrames << " continuous forward frames; touch="
                  << (unassistedTouched ? "yes" : "no") << ", endpoint="
                  << (unassistedOpened ? "yes" : "no") << ", OpenDoor cue="
                  << (unassistedOpenSounds ? "yes" : "no")
                  << ", PATH=" << unassistedPathTime << "/" << contactDoor.endTime
                  << ", door-center x=" << doorCenter.x << ", x offset="
                  << unassistedStopPosition.x - doorCenter.x
                  << ", touch position=(" << unassistedTouchPosition.x << ", "
                  << unassistedTouchPosition.y << ", " << unassistedTouchPosition.z
                  << "), stop position=(" << unassistedStopPosition.x << ", "
                  << unassistedStopPosition.y << ", " << unassistedStopPosition.z << ")";
        if (!unassistedCrossed && unassistedTraceBlocked)
            std::cout << ", next swept trace hit model " << unassistedObstruction.model
                      << " at (" << unassistedObstruction.point.x << ", "
                      << unassistedObstruction.point.y << ", " << unassistedObstruction.point.z
                      << ") normal (" << unassistedObstruction.normal.x << ", "
                      << unassistedObstruction.normal.y << ", " << unassistedObstruction.normal.z << ")";
        std::cout << ".\n";

        int approachFrames = 0;
        for (; approachFrames < 300 && !touchedByApproach; ++approachFrames)
            approachFrame(1.f, 0.f);
        if (!touchedByApproach)
            throw std::runtime_error("Bank player approach did not dispatch the selected *Door touch before collision; player stopped at (" +
                std::to_string(player.p.x) + ", " + std::to_string(player.p.y) + ", " +
                std::to_string(player.p.z) + ")");
        for (int waitFrame = 0; waitFrame < 180 && !openedByApproach; ++waitFrame)
            approachFrame(0.f, 0.f);
        const float alignSide = doorCenter.x > player.p.x ? .75f : -.75f;
        const int alignBurstFrames = std::clamp(
            int(std::round(std::abs(doorCenter.x - player.p.x) / 4.f)), 1, 30);
        for (int alignFrame = 0; alignFrame < alignBurstFrames; ++alignFrame)
            approachFrame(0.f, alignSide);
        approachFrame(0.f, -alignSide);
        for (int stopFrame = 0; stopFrame < 30; ++stopFrame)
            approachFrame(0.f, 0.f);
        int passFrames = 0;
        for (; passFrames < 300 && !crossedDoorPlane; ++passFrames)
            approachFrame(1.f, 0.f);
        const V controlCrossPosition = player.p;
        if (doorContact) {
            scripts->dispatch(contactDoorEntity, "untouch");
            doorContact = false;
            scripts->advanceEntity(contactDoorEntity, SDL_GetTicks() + uint32_t(simulationFrame++ * 17));
        }
        if (!crossedDoorPlane) {
            Hit obstruction;
            const bool traceBlocked = collision.trace(player.p, approachDirection * 180.f,
                {-30.f, -30.f, -30.f}, player.maxs(), obstruction);
            std::string collisionDetail = traceBlocked
                ? "model " + std::to_string(obstruction.model) + " at (" +
                    std::to_string(obstruction.point.x) + ", " + std::to_string(obstruction.point.y) + ", " +
                    std::to_string(obstruction.point.z) + ") normal (" +
                    std::to_string(obstruction.normal.x) + ", " + std::to_string(obstruction.normal.y) + ", " +
                    std::to_string(obstruction.normal.z) + ")"
                : "no obstruction on the next 180 units";
            throw std::runtime_error("Bank door received a player-approach touch and opened to PATH time " +
                std::to_string(contactDoor.currentTime) + "/" + std::to_string(contactDoor.endTime) +
                ", but Player::step could not cross the door plane; stopped at (" +
                std::to_string(player.p.x) + ", " + std::to_string(player.p.y) + ", " +
                std::to_string(player.p.z) + "); touch occurred at (" +
                std::to_string(approachTouchPosition.x) + ", " +
                std::to_string(approachTouchPosition.y) + ", " +
                std::to_string(approachTouchPosition.z) + "); door center x=" +
                std::to_string(doorCenter.x) + ", final x offset=" +
                std::to_string(player.p.x - doorCenter.x) + "; next collision trace hits " + collisionDetail);
        }
        const bool playedOpen = std::any_of(soundDispatches.begin(), soundDispatches.end(), [&](const auto& sound) {
            return sound.first == contactDoorEntity && lower(sound.second) == "opendoor";
        });
        if (!playedOpen || !openedByApproach ||
            std::abs(contactDoor.timeScale - 3.f) > 1.0e-6f)
            throw std::runtime_error("Bank *Door touch did not run openaway and its authored OpenDoor cue");
        if (!openedByApproach ||
            contactDoor.children.empty() || !collision.poses.count(contactDoor.children.front().model))
            throw std::runtime_error("Bank *Door openaway did not sample its PATH into the shared collision/render pose");
        scripts->advanceEntity(contactDoorEntity, 16);
        scripts->advanceEntity(contactDoorEntity, 32);
        const bool playedClose = std::any_of(soundDispatches.begin(), soundDispatches.end(), [&](const auto& sound) {
            return sound.first == contactDoorEntity && lower(sound.second) == "closedoor";
        });
        if (contactDoor.motionState != 4 || !playedClose)
            throw std::runtime_error("Bank *Door untouch queue did not run close and its CloseDoor cue");
        for (int i = 0; i < 1000 && contactDoor.motionState != 0; ++i)
            updateBModelStates(bmodels, step, collision, player.p);
        if (contactDoor.motionState != 0 || std::abs(contactDoor.currentTime - contactDoor.startTime) > 1.0e-4f)
            throw std::runtime_error("Bank *Door close command did not return to the authored PATH start");

        if (consoleEntity < 0 || vaultEntity < 0 || !hasScriptEvent(*scripts, consoleEntity, "use"))
            throw std::runtime_error("Bank console or named vault ScriptActor binding is missing");
        auto& console = bmodelById(bmodels, consoleEntity);
        auto& vault = bmodelById(bmodels, vaultEntity);
        const V center = (console.boundsMin + console.boundsMax) * .5f;
        if (!BModelContact::overlapsExpanded(console.boundsMin, console.boundsMax,
                                             center + V{-30.f, -30.f, -30.f},
                                             center + V{30.f, 85.f, 30.f}, console.contactReach))
            throw std::runtime_error("Bank test player does not overlap the console bounds");
        scripts->dispatch(consoleEntity, "touch");
        scripts->dispatch(consoleEntity, "use");
        if (scripts->variables["VaultUnlocked"] != "1" || vault.motionState != 3 ||
            std::abs(vault.timeScale - 1.f) > 1.0e-6f)
            throw std::runtime_error("Bank console use did not unlock and tell the named vault to open");
        for (int i = 0; i < 10000 && vault.motionState != 0; ++i)
            updateBModelStates(bmodels, step, collision);
        if (vault.motionState != 0 || std::abs(vault.currentTime - vault.endTime) > 1.0e-4f)
            throw std::runtime_error("Bank named open command did not reach the vault PATH endpoint");
        std::cout << "Interaction check: Bank nearest authored *Door entity " << contactDoorEntity
                  << " (" << std::sqrt(contactDoorSpawnDistanceSquared)
                  << " units from the authored spawn bounds)"
                  << " received touch after " << approachFrames << " Player::step approach frames, "
                  << "opened with its authored sound and shared PATH pose; after a lateral alignment, "
                  << "Player::step crossed the door plane in " << passFrames << " forward frames at ("
                  << controlCrossPosition.x << ", " << controlCrossPosition.y << ", "
                  << controlCrossPosition.z << "); "
                  << "untouch then closed it with the authored sound; "
                  << "console entity " << consoleEntity << " unlocked VaultUnlocked, "
                  << "resolved `tell vault open` to entity " << vaultEntity << ", and sampled the vault PATH to "
                  << vault.currentTime << " at default speed 1.\n";
        return;
    }
    int ladderEntity = -1;
    for (const auto& bmodel : bmodels) {
        if (hasPlayerScriptCommand(*scripts, bmodel.entity, "touch", "climb") &&
            hasPlayerScriptCommand(*scripts, bmodel.entity, "untouch", "unclimb")) {
            ladderEntity = bmodel.entity;
            break;
        }
    }
    if (ladderEntity < 0)
        throw std::runtime_error("--interaction-check could not find an authored BModel ladder with Player climb/untouch bindings");

    auto& ladder = bmodelById(bmodels, ladderEntity);
    const V ladderCenter = (ladder.boundsMin + ladder.boundsMax) * .5f;
    const V playerMins{-30.f, -30.f, -30.f};
    const V playerMaxs{30.f, 85.f, 30.f};
    bool foundClearContact = false;
    V contactPosition{};
    float bestContactDistanceSquared = std::numeric_limits<float>::infinity();
    std::vector<float> xOffsets;
    std::vector<float> zOffsets;
    for (int offset = -120; offset <= 120; offset += 20) {
        xOffsets.push_back(float(offset));
        zOffsets.push_back(float(offset));
    }
    // Sample positions just beyond each horizontal face of the ladder model.
    // The actor hull then clears the solid model by a small margin while its
    // bounds still overlap the source-mapped E_Reach-expanded contact volume.
    const float contactMargin = std::min(2.f, std::max(0.f, ladder.contactReach * 0.5f));
    xOffsets.push_back(ladder.boundsMin.x - playerMaxs.x - contactMargin - ladderCenter.x);
    xOffsets.push_back(ladder.boundsMax.x - playerMins.x + contactMargin - ladderCenter.x);
    zOffsets.push_back(ladder.boundsMin.z - playerMaxs.z - contactMargin - ladderCenter.z);
    zOffsets.push_back(ladder.boundsMax.z - playerMins.z + contactMargin - ladderCenter.z);
    for (float offsetX : xOffsets) {
        for (float offsetZ : zOffsets) {
            for (float offsetY : {ladderCenter.y,
                                  ladder.boundsMin.y + 30.f,
                                  (ladder.boundsMin.y + ladder.boundsMax.y) * .5f - 30.f,
                                  ladder.boundsMax.y - 80.f}) {
                V candidate{ladderCenter.x + offsetX, offsetY,
                            ladderCenter.z + offsetZ};
                const bool overlaps = BModelContact::overlapsExpanded(
                    ladder.boundsMin, ladder.boundsMax, candidate + playerMins,
                    candidate + playerMaxs, ladder.contactReach);
                if (!overlaps || collision.solid(candidate, playerMins, playerMaxs)) continue;
                const float distanceX = candidate.x - ladderCenter.x;
                const float distanceZ = candidate.z - ladderCenter.z;
                const float distanceSquared = distanceX * distanceX + distanceZ * distanceZ;
                if (distanceSquared < bestContactDistanceSquared) {
                    contactPosition = candidate;
                    bestContactDistanceSquared = distanceSquared;
                    foundClearContact = true;
                }
            }
        }
    }
    if (!foundClearContact)
        throw std::runtime_error("--interaction-check found authored ladder entity " +
                                 std::to_string(ladderEntity) +
                                 " but could not find a clear player position inside its contact bounds");

    player.p = contactPosition;
    player.v = {};
    player.climbing = false;
    const bool initialContact = BModelContact::overlapsExpanded(
        ladder.boundsMin, ladder.boundsMax, player.p + playerMins, player.p + playerMaxs,
        ladder.contactReach);
    if (!initialContact) throw std::runtime_error("Authored ladder contact position did not overlap its BModel bounds");
    scripts->dispatch(ladderEntity, "touch");
    if (!player.climbing)
        throw std::runtime_error("Authored ladder touch did not enable Player climbing");

    const float step = 1.f / 60.f;
    const float startingHeight = player.p.y;
    player.step(step, 1.f, 0.f, 0.f, false, false, false, collision,
                1.57079632679489661923f);
    const float climbedHeight = player.p.y - startingHeight;
    if (climbedHeight <= 0.1f)
        throw std::runtime_error("Player climbing state did not produce upward movement on authored ladder entity " +
                                 std::to_string(ladderEntity));

    V outsidePosition = player.p;
    const int exitAxis = ladder.boundsMax.x - ladder.boundsMin.x <=
        ladder.boundsMax.z - ladder.boundsMin.z ? 0 : 2;
    outsidePosition[exitAxis] = ladder.boundsMax[exitAxis] + ladder.contactReach + 31.f;
    const bool outsideContact = BModelContact::overlapsExpanded(
        ladder.boundsMin, ladder.boundsMax, outsidePosition + playerMins,
        outsidePosition + playerMaxs, ladder.contactReach);
    if (outsideContact)
        throw std::runtime_error("Ladder untouch fixture did not leave the authored BModel contact bounds");
    scripts->dispatch(ladderEntity, "untouch");
    if (player.climbing) throw std::runtime_error("Authored ladder untouch did not disable Player climbing");

    std::cout << "Interaction check: authored ladder entity " << ladderEntity
              << " at (" << contactPosition.x << ", " << contactPosition.y << ", "
              << contactPosition.z << ") touch enabled Player climbing, upward Player::step moved " << climbedHeight
              << " units, and untouch disabled climbing.\n";
}

void clearTextures(std::vector<SceneMesh>& meshes) {
    std::set<GLuint> textures;
    for (auto& mesh : meshes) {
        if (mesh.texture) textures.insert(mesh.texture);
        if (mesh.lightmapAtlasTexture) textures.insert(mesh.lightmapAtlasTexture);
    }
    for (GLuint texture : textures) glDeleteTextures(1, &texture);
}

std::filesystem::path saveRenderCheckFrame(const std::filesystem::path& package,
                                           const std::vector<uint8_t>& bottomUpPixels,
                                           int width, int height,
                                           const std::string& suffix = ".render-check.bmp") {
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
    output += suffix;
    int saveResult = SDL_SaveBMP(surface, output.string().c_str());
    std::string error = SDL_GetError();
    SDL_FreeSurface(surface);
    if (saveResult != 0)
        throw std::runtime_error("Cannot save render-check image " + output.string() + ": " + error);
    return output;
}

struct RenderCounters {
    size_t submittedGroups = 0;
    size_t culledGroups = 0;
    size_t submittedTriangles = 0;
};

void renderScene(std::vector<SceneMesh>& meshes, const SceneData& scene,
                 const Collision& collision, const std::map<int, Pose>& inspectionPoses,
                 const PreviewWorldModelFlags& worldModelFlags,
                 const RetailFogState& fogState,
                 LightStyleState& lightStyles,
                 V camera, float yaw, float pitch,
                 int width, int height, RenderCounters* counters = nullptr,
                 const FrameTransform* cameraFrame = nullptr) {
    lightStyles.advance();
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double nearPlane = 1.0;
    const double farPlane = fogState.cameraFarClip();
    const double top = nearPlane * std::tan(60.0 * 3.141592653589793 / 360.0);
    const double right = top * (double(width) / double(std::max(height, 1)));
    glFrustum(-right, right, -top, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (cameraFrame) {
        // Frame axes are local-to-world columns. Their transpose is the
        // camera's world-to-view rotation, matching the retail view frame.
        const auto& axis = cameraFrame->axis;
        const GLfloat viewRotation[16] = {
            axis[0].x, axis[1].x, axis[2].x, 0.f,
            axis[0].y, axis[1].y, axis[2].y, 0.f,
            axis[0].z, axis[1].z, axis[2].z, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
        glMultMatrixf(viewRotation);
    } else {
        glRotatef(-pitch * 57.29577951308232f, 1.f, 0.f, 0.f);
        glRotatef(-yaw * 57.29577951308232f, 0.f, 1.f, 0.f);
    }
    glTranslatef(-camera.x, -camera.y, -camera.z);

    glClearColor(.08f, .10f, .12f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (fogState.renderEnabled()) {
        const auto color = fogState.normalizedColor();
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, color.data());
        glFogf(GL_FOG_START, fogState.current.start);
        const float fogEnd = std::abs(fogState.current.end - fogState.current.start) < 0.001f
            ? fogState.current.start + 0.001f : fogState.current.end;
        glFogf(GL_FOG_END, fogEnd);
        glEnable(GL_FOG);
    } else {
        glDisable(GL_FOG);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glEnableClientState(GL_VERTEX_ARRAY);
    bool textureCoordinatesEnabled = false;
    const float yawCos = std::cos(yaw), yawSin = std::sin(yaw);
    const float pitchCos = std::cos(pitch), pitchSin = std::sin(pitch);
    const float tanVertical = std::tan(60.f * 3.14159265358979323846f / 360.f);
    const float tanHorizontal = tanVertical * (float(width) / float(std::max(height, 1)));
    const float horizontalPlaneScale = 1.f / std::sqrt(1.f + tanHorizontal * tanHorizontal);
    const float verticalPlaneScale = 1.f / std::sqrt(1.f + tanVertical * tanVertical);
    for (auto& mesh : meshes) {
        if (!mesh.lightmapAtlasTexture) continue;
        for (auto& span : mesh.faceSpans)
            uploadLightmapPixels(span, mesh.lightmapAtlasTexture, lightStyles);
    }
    auto outsideView = [&](const SceneMesh& mesh) {
        const V offset = mesh.boundsCenter - camera;
        const float viewX = cameraFrame
            ? dot(offset, cameraFrame->axis[0])
            : yawCos * offset.x - yawSin * offset.z;
        const float viewY = cameraFrame
            ? dot(offset, cameraFrame->axis[1])
            : pitchCos * offset.y + pitchSin * (yawSin * offset.x + yawCos * offset.z);
        const float viewDepth = cameraFrame
            ? -dot(offset, cameraFrame->axis[2])
            : pitchSin * offset.y - pitchCos * (yawSin * offset.x + yawCos * offset.z);
        const float radius = mesh.boundsRadius;
        if (viewDepth + radius < float(nearPlane) || viewDepth - radius > float(farPlane)) return true;
        if ((viewX + viewDepth * tanHorizontal) * horizontalPlaneScale < -radius) return true;
        if ((viewDepth * tanHorizontal - viewX) * horizontalPlaneScale < -radius) return true;
        if ((viewY + viewDepth * tanVertical) * verticalPlaneScale < -radius) return true;
        if ((viewDepth * tanVertical - viewY) * verticalPlaneScale < -radius) return true;
        return false;
    };
    for (auto& mesh : meshes) {
        if (!worldModelFlags.rendersNormally(mesh.model)) continue;
        float angle = 0.f;
        auto angleIt = collision.angles.find(mesh.model);
        if (angleIt != collision.angles.end()) angle = angleIt->second;
        auto runtimePoseIt = collision.poses.find(mesh.model);
        auto inspectionPoseIt = inspectionPoses.find(mesh.model);
        const bool hasInspectionPose = inspectionPoseIt != inspectionPoses.end();
        const bool hasRuntimePose = runtimePoseIt != collision.poses.end();
        const bool movedModel = angle != 0.f || hasRuntimePose || hasInspectionPose;
        // Moving BModels use a runtime transform that is not represented by
        // the export-time bounds, so keep them on the uncullled render path.
        if (!movedModel && outsideView(mesh)) {
            if (counters) ++counters->culledGroups;
            continue;
        }
        if (counters) {
            ++counters->submittedGroups;
            counters->submittedTriangles += mesh.positions.size() / 9;
        }
        glPushMatrix();
        // A detached M-key PATH preview must not mask the live gameplay pose.
        // Runtime BModel poses also feed Collision::trace, so render the same
        // state the player is colliding against whenever it exists.
        if (movedModel) {
            V pivot{};
            auto origin = collision.modelOrigins.find(mesh.model);
            if (origin != collision.modelOrigins.end()) pivot = origin->second;
            Pose pose;
            if (runtimePoseIt != collision.poses.end()) pose = runtimePoseIt->second;
            else if (hasInspectionPose) pose = inspectionPoseIt->second;
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
        glVertexPointer(3, GL_FLOAT, 0, mesh.positions.data());
        if (mesh.texture && mesh.uvs.size() >= (mesh.positions.size() / 3) * 2) {
            if (!textureCoordinatesEnabled) {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                textureCoordinatesEnabled = true;
            }
            glTexCoordPointer(2, GL_FLOAT, 0, mesh.uvs.data());
        } else {
            if (textureCoordinatesEnabled) {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                textureCoordinatesEnabled = false;
            }
        }
        glDrawArrays(GL_TRIANGLES, 0, GLsizei(mesh.positions.size() / 3));
        if (mesh.hasLightmappedFaces && mesh.lightmapAtlasTexture &&
            mesh.lightmapUvs.size() >= (mesh.positions.size() / 3) * 2) {
            if (!textureCoordinatesEnabled) {
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                textureCoordinatesEnabled = true;
            }
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh.lightmapAtlasTexture);
            glColor3f(1.f, 1.f, 1.f);
            glVertexPointer(3, GL_FLOAT, 0, mesh.positions.data());
            glTexCoordPointer(2, GL_FLOAT, 0, mesh.lightmapUvs.data());
            glEnable(GL_BLEND);
            glBlendFunc(GL_ZERO, GL_SRC_COLOR);
            glDrawArrays(GL_TRIANGLES, 0, GLsizei(mesh.positions.size() / 3));
            glDisable(GL_BLEND);
        }
        glPopMatrix();
    }
    if (textureCoordinatesEnabled) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void printInventory(const SceneData& scene, const std::vector<SceneMesh>& meshes, const TextureLoadSummary& textures) {
    J report = field(scene.report(), "counts");
    size_t triangles = 0;
    size_t faceSpans = 0;
    size_t lightmapFaces = 0;
    size_t lightmapPlanes = 0;
    size_t lightmapBytes = 0;
    size_t uploadedFaceLightmaps = 0;
    for (const auto& mesh : meshes) triangles += mesh.positions.size() / 9;
    for (const auto& mesh : meshes) {
        faceSpans += mesh.faceSpans.size();
        for (const auto& span : mesh.faceSpans) {
            if (span.lightOffset < 0) continue;
            ++lightmapFaces;
            lightmapPlanes += size_t(span.lightStyleCount);
            lightmapBytes += span.lightSamples.size();
            if (span.lightmapAtlasAssigned) ++uploadedFaceLightmaps;
        }
    }
    std::cout << "Scene: " << scene.key() << " (" << scene.sourceFile() << ")\n"
              << "BSP SHA-256: " << scene.sourceSha256() << "\n"
              << "Entities: " << count(scene.entities())
              << ", render groups: " << meshes.size() << "/" << count(field(scene.worldMesh(), "groups"))
              << ", collision models: " << count(field(scene.collision(), "models"))
              << ", nodes: " << count(field(scene.collision(), "nodes"))
              << ", leaves: " << count(field(scene.collision(), "leafs")) << "\n"
              << "BSP faces: " << integer(field(report, "faces"))
              << ", render triangles: " << triangles
              << ", face spans: " << faceSpans
              << ", lightmap faces: " << lightmapFaces
              << ", raw lightmap planes: " << lightmapPlanes
              << ", lightmap bytes: " << lightmapBytes
              << ", face lightmaps uploaded: " << uploadedFaceLightmaps
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
    std::cout << "Disabled collision models: ";
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

void drawRescuePlayerHoseStreamPreview(const std::vector<V>& vertices, float pressure) {
    glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT |
                 GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    // Rescue PlayerRescue HoseSpray has a separate particle path and remains
    // an untextured preview here; dedicated PlayerHose uses the source-bound
    // resource-3 renderer below.
    glColor4f(.36f, .74f, 1.f, .3f * std::clamp(pressure, 0.f, 1.f));
    glBegin(GL_QUADS);
    for (const V& point : vertices) glVertex3f(point.x, point.y, point.z);
    glEnd();
    glPopAttrib();
}

int run(const std::filesystem::path& package, bool inspectOnly, bool renderCheck,
        bool interactionCheck, bool rescueCheck, bool ambientAudioCheck,
        int triggerRenderCheckEntity,
        bool hintRenderCheck, bool fireDirectionRenderCheck, bool oxygenHudRenderCheck,
        bool victimPickupHudRenderCheck,
        bool fireCameraAudit,
        const std::string& equipmentOrientationCheck,
        std::string* nextRescueStage = nullptr, bool performanceAudit = false,
        const SceneAudio::MusicSelection* inheritedMusic = nullptr,
        std::optional<float> levelField10 = std::nullopt,
        std::optional<int> hoseModeIndex = std::nullopt) {
    const bool rescueDiagnostic = rescueCheck || ambientAudioCheck;
    auto logRescueStage = [&](const char* stage) {
        if (rescueCheck) std::cerr << "rescue-check: " << stage << std::endl;
        else if (ambientAudioCheck) std::cerr << "scene-audio-check: " << stage << std::endl;
    };
    logRescueStage("enter");
    SceneData scene = SceneData::load(package);
    PlayerHoseStage playerHoseStage = loadPlayerHoseStage(scene);
    const bool dedicatedPlayerHose = playerHoseStage.present;
    if (dedicatedPlayerHose) {
        size_t hotspots = 0, targets = 0;
        for (const auto& path : playerHoseStage.pathNodes) {
            if (!path.isHotspot) continue;
            ++hotspots;
            targets += path.targets.size();
        }
        const int selectedModeIndex = hoseModeIndex.value_or(2);
        if (!inspectOnly && !playerHoseStage.start(selectedModeIndex))
            throw std::runtime_error("Cannot start PlayerHose route in " + scene.key() + ": " +
                                     playerHoseStage.transition);
        std::cout << "Dedicated PlayerHose route: " << scene.key()
                  << ", path nodes=" << playerHoseStage.pathNodes.size()
                  << ", authored hotspots=" << hotspots
                  << ", WindowFire target references=" << targets
                  << ", mode index=" << selectedModeIndex
                  << ", spot-time multiplier=" << PlayerHoseStage::modeTimeMultiplier(selectedModeIndex)
                  << ". Static retail order/data and default mouse steering scale/signs are mapped; input-poll cadence and live parity remain open.\n";
    }
    logRescueStage("scene loaded");
#if defined(__APPLE__)
    if (!inspectOnly && !renderCheck && !interactionCheck && !rescueDiagnostic && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
#else
    if (!inspectOnly && !interactionCheck && !rescueDiagnostic && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
#endif
        throw std::runtime_error(SDL_GetError());
    if (rescueDiagnostic && SDL_Init(SDL_INIT_TIMER) != 0)
        throw std::runtime_error(SDL_GetError());
    logRescueStage("SDL initialized");
    if (!inspectOnly && !renderCheck && !interactionCheck && !rescueDiagnostic) {
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    }

    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
#if defined(__APPLE__)
    HeadlessOpenGL headlessContext;
#endif
    LightStyleState lightStyles;
    std::vector<SceneMesh> meshes;
    std::map<std::string, std::unique_ptr<VictimActorAsset>> victimAssets;
    std::map<std::string, std::unique_ptr<EquipmentActorAsset>> equipmentAssets;
    std::map<int, std::unique_ptr<EquipmentActorAsset>> propModelAssets;
    std::vector<PreviewVictim> victims;
    std::vector<PropModelInstance> propModels;
    std::map<int, size_t> propModelByEntity;
    std::vector<PreviewFire> fires;
    int fireDividerCycle = 0;
    int nearbySpreadFireCount = 0;
    int nextDynamicFireEntity = 1000000;
    FireRenderer fireRenderer;
    bool fireRendererLoaded = false;
    ExtinguisherSprayRenderer extinguisherSprayRenderer;
    bool extinguisherSprayRendererLoaded = false;
    PlayerHoseStreamRenderer playerHoseStreamRenderer;
    bool playerHoseStreamRendererLoaded = false;
    FireDirectionRenderer fireDirectionRenderer;
    bool fireDirectionRendererLoaded = false;
    GogglesRenderer gogglesRenderer;
    bool gogglesRendererLoaded = false;
    OxygenHudRenderer oxygenHudRenderer;
    bool oxygenHudRendererLoaded = false;
    VictimPickupHudRenderer victimPickupHudRenderer;
    bool victimPickupHudRendererLoaded = false;
    BonusPointsHudRenderer bonusPointsHudRenderer;
    bool bonusPointsHudRendererLoaded = false;
    std::filesystem::path actorAssetRoot;
    VictimTransactionState rescueState;
    RescueBotState rescueBotState;
    auto& carriedVictim = rescueState.player;
    try {
#if defined(__APPLE__)
        if (renderCheck) {
            headlessContext.create(1280, 800);
        } else
#endif
        if (!inspectOnly && !interactionCheck && !rescueDiagnostic) {
            window = SDL_CreateWindow(("FDNY Firefighters: American Hero — " + scene.key()).c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            if (!window) throw std::runtime_error(SDL_GetError());
            context = SDL_GL_CreateContext(window);
            if (!context) throw std::runtime_error(SDL_GetError());
            SDL_GL_SetSwapInterval(1);
        }

        TextureLoadSummary textureSummary;
        logRescueStage("loading meshes");
        meshes = loadMeshes(scene, textureSummary,
                            !inspectOnly && !interactionCheck && !rescueDiagnostic, lightStyles);
        logRescueStage("meshes loaded");
        Collision collision(scene.collision());
        PropModelPvs propModelPvs(scene.collision(),
            readSceneChunkBytes(scene, "chunks/05_clusters.bin"),
            readSceneChunkBytes(scene, "chunks/21_visdata.bin"));
        auto bmodelMotions = loadBModelMotions(scene);
        auto boundsTriggers = loadBoundsTriggers(scene);
        PreviewWorldModelFlags worldModelFlags;
        // InitializeBoundsTriggerEntity (00441040) sends ModelSetFlags(model, 0)
        // after resolving each valid authored trigger model.
        for (const auto& trigger : boundsTriggers)
            worldModelFlags.flags[trigger.modelIndex] = 0;
        std::map<int, Pose> inspectionPoses;
        float bmodelMotionStart = 0.f, bmodelMotionEnd = 0.f;
        bool foundBModelMotionRange = false;
        std::map<int, size_t> bmodelByEntity;
        int animatedBModels = 0;
        for (size_t i = 0; i < bmodelMotions.size(); ++i) {
            auto& bmodel = bmodelMotions[i];
            bmodelByEntity[bmodel.entity] = i;
            if (bmodel.children.empty()) continue;
            ++animatedBModels;
            if (!foundBModelMotionRange) {
                bmodelMotionStart = bmodel.startTime;
                bmodelMotionEnd = bmodel.endTime;
                foundBModelMotionRange = true;
            } else {
                bmodelMotionStart = std::min(bmodelMotionStart, bmodel.startTime);
                bmodelMotionEnd = std::max(bmodelMotionEnd, bmodel.endTime);
            }
        }
        for (const auto& bmodel : bmodelMotions)
            sampleBModelState(bmodel, collision);
        const uint32_t gameplayStart = SDL_GetTicks();
        Random gameplayRandom(gameplayStart);
        if (!inspectOnly && (!interactionCheck || rescueDiagnostic)) {
            logRescueStage("loading FireRescue actors");
            fires = loadPreviewFires(scene, collision, gameplayRandom, gameplayStart, fireDividerCycle);
            nearbySpreadFireCount = 0;
            logRescueStage("FireRescue actors loaded");
            logRescueStage("loading victim actors");
            actorAssetRoot = locateVictimAssets(scene);
            victims = loadPreviewVictims(scene, collision, gameplayRandom, gameplayStart, actorAssetRoot,
                                         victimAssets, !rescueDiagnostic);
            for (const std::string stem : {"axe", "axel", "ext"}) {
                const auto exportPath = actorAssetRoot / "equipment" / (stem + ".json");
                if (std::filesystem::is_regular_file(exportPath)) {
                    equipmentAssets.emplace(stem, std::make_unique<EquipmentActorAsset>(
                        actorAssetRoot / "equipment", stem, !rescueDiagnostic));
                } else {
                    std::cerr << "Equipment ACT export not found: " << exportPath
                              << " (run tools/export_equipment_actors.py)\n";
                }
            }
            propModels = loadPropModelInstances(scene.entities());
            std::set<int> propResourceIds;
            for (const auto& prop : propModels) propResourceIds.insert(prop.resourceModelId);
            for (int modelId : propResourceIds) {
                const std::string stem = "model_" + std::to_string(modelId);
                const auto exportPath = actorAssetRoot / "props" / (stem + ".json");
                if (!std::filesystem::is_regular_file(exportPath)) {
                    std::cerr << "PropModel ACT export not found: " << exportPath << "\n";
                    continue;
                }
                propModelAssets.emplace(modelId, std::make_unique<EquipmentActorAsset>(
                    actorAssetRoot / "props", stem, !rescueDiagnostic));
            }
            for (size_t propIndex = 0; propIndex < propModels.size(); ++propIndex) {
                auto& prop = propModels[propIndex];
                prop.worldLeafIndex = propModelPvs.worldLeaf(prop.position);
                propModelByEntity[prop.entity] = propIndex;
            }
            logRescueStage("victim actors loaded");
        }
        if (!inspectOnly && !interactionCheck && !rescueDiagnostic) {
            fireRenderer.load(actorAssetRoot / "fire-sprites");
            fireRendererLoaded = true;
            extinguisherSprayRenderer.load(
                locateExtinguisherSpraySprites(scene, actorAssetRoot));
            extinguisherSprayRendererLoaded = true;
            if (dedicatedPlayerHose) {
                playerHoseStreamRenderer.load(
                    locatePlayerHoseStreamSprites(scene, actorAssetRoot));
                playerHoseStreamRendererLoaded = true;
            }
            fireDirectionRenderer.load(locateFireDirectionSprites(scene));
            fireDirectionRendererLoaded = true;
            gogglesRenderer.load(locateGogglesSprites(scene));
            gogglesRendererLoaded = true;
            if (oxygenHudRenderCheck) {
                oxygenHudRenderer.load(locateOxygenHudSprites(scene));
                oxygenHudRendererLoaded = true;
            }
            if (victimPickupHudRenderCheck) {
                victimPickupHudRenderer.load(locateVictimPickupHudSprites(scene));
                victimPickupHudRendererLoaded = true;
            }
        }
        V spawn{};
        float yaw = 0.f;
        bool foundPlayer = false;
        int playerEntityId = -1;
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
                playerEntityId = integer(field(entity, "index"));
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
        Player player;
        player.p = spawn;
        player.checkpointPosition = spawn;
        player.checkpointYaw = yaw;
        PlayerPassoutState playerFireState;
        PlayerOxygenState playerOxygen;
        PlayerOxygenVisorEffect playerOxygenVisorEffect;
        PlayerBurnCoolingState playerBurnState;
        uint32_t playerPainDeadline = 0;
        uint32_t playerSeriousBurns = 0;
        int lastPainChoice = -1, lastPassoutChoice = -1;
        bool playerIncapacitated = false;
        PreviewPlayerLoadout playerLoadout;
        PreviewExtinguisher extinguisher;
        PreviewRescueHoseSpray rescueHoseSpray;
        AxeActionCallbacks::State axeActionState;
        ItemViewPose::Result axeItemViewPose;
        bool axeSwingActive = false;
        bool axeActionStarted = false;
        float axeSwingElapsed = 0.f;
        GogglesScan gogglesScan;
        bool gogglesScanPending = false;
        ExtinguisherContacts extinguisherContacts;
        FireMessageQueue fireMessages;
        FireMessageQueue victimMessages;
        EntityIdRegistry rescueMessageRoutes;
        EntityIdRegistry::Handle playerRescueMessageRoute{};
        bool hasPlayerRescueMessageRoute = false;
        std::map<std::string, int> scriptActorIds;
        std::map<int, V> actorPositions;
        for (int i = 0; i < count(entities); ++i) {
            J entity = at(entities, i);
            const int entityId = integer(field(entity, "index"));
            std::string actorName = property(entity, "A_Name");
            if (!actorName.empty() && lower(actorName).rfind("<null>", 0) != 0)
                scriptActorIds.emplace(lower(actorName), entityId);
            actorPositions[entityId] = parseVector(property(entity, "ZZ_location"),
                parseVector(property(entity, "location"), {}));
        }
        if (playerEntityId >= 0) scriptActorIds.emplace("player", playerEntityId);
        SceneAudio sceneAudio;
        if (!inspectOnly && !renderCheck && !interactionCheck && !rescueDiagnostic) {
            sceneAudio.open();
            sceneAudio.load(scene, inheritedMusic);
        }
        if (ambientAudioCheck) {
            if (!sceneAudio.open())
                throw std::runtime_error("Scene audio check could not open SDL's dummy audio device");
            sceneAudio.load(scene, inheritedMusic);
        }
        auto startExtinguisherActionAudio = [&](bool middleAlreadyActive) {
            sceneAudio.playOwned(playerEntityId, "ExtStart", player.p, player.p, yaw, 2);
            if (!middleAlreadyActive)
                sceneAudio.loopOwned(playerEntityId, "ExtMiddle", player.p, player.p, yaw, 2);
        };
        auto stopExtinguisherActionAudio = [&] {
            sceneAudio.stopOwned(playerEntityId, "ExtMiddle");
            sceneAudio.playOwned(playerEntityId, "ExtEnd", player.p, player.p, yaw, 2);
        };
        startPreviewFireLoopSounds(fires, sceneAudio, player.p, yaw);
        struct AmbientLoop {
            int owner = 0;
            std::string name;
            V source{};
        };
        std::vector<AmbientLoop> ambientLoops;
        int nextAmbientLoopOwner = -1000000;
        std::string activeHint;
        float activeHintRemaining = 0.f;
        if (dedicatedPlayerHose) {
            activeHint = playerHoseStage.transition;
            activeHintRemaining = 2.f;
        }
        HintOverlay hintOverlay;
        RetailHintLifetime authoredHintLifetime;
        std::string authoredHintText;
        if (!inspectOnly && !interactionCheck && !rescueDiagnostic)
            hintOverlay.load(actorAssetRoot / "hints");
        std::vector<std::pair<int, std::string>> soundDispatches;
        TrainingZones zones(TrainingZones::LabelMode::CaseSensitive);
        VictimTransactionServices rescueServices;
        std::unique_ptr<VictimTransactions> rescueTransactions;
        std::map<std::string, SourceStatisticRecord> rescueStatistics{
            {"Bonus Points", {0, 0}},
            {"Rescues", {0, 0}},
            {"Longest Rescue Time", {2, 0}},
            {"Shortest Rescue Time", {1, std::numeric_limits<uint32_t>::max()}},
            {"Fires Doused", {0, 0}}
        };
        std::set<int> enabledExitTriggers;
        std::string rescueStageRequest;
        if (nextRescueStage) nextRescueStage->clear();
        auto updateBoundsTriggerModelFlags = [&](uint32_t clockMs) {
            const uint32_t phase = clockMs % 1000u;
            // DispatchBoundsTriggerContactEvents (00440DC0) applies this
            // source-derived pulse only when BoundsTrigger mode +0x165 is set.
            // The viewer samples SDL ticks per frame because the retail update
            // callback cadence and clock source are not yet established.
            const bool pulse = (phase >= 500u && phase < 600u) ||
                               (phase >= 700u && phase < 800u) ||
                               (phase >= 900u);
            for (const auto& trigger : boundsTriggers) {
                // 00440F50 only sets the embedded ScriptActor render pulse
                // when BoundsTrigger root +0x164 was initialized from an
                // authored `tell player escape` binding. Final-exit membership
                // alone does not enable this visual state.
                const bool enabled = trigger.carryModeGate &&
                    (trigger.carryMode || enabledExitTriggers.count(trigger.entity) != 0);
                worldModelFlags.flags[trigger.modelIndex] = enabled && pulse ? 1u : 0u;
            }
        };
        FrameTransform rescuePlayerFrame;
        auto setActorFrame = [](FrameTransform& frame, V position, float degrees) {
            const float angle = degrees * (3.14159265358979323846f / 180.f);
            const float cosine = std::cos(angle), sine = std::sin(angle);
            frame.position = position;
            frame.axis = {V{cosine, 0.f, -sine}, V{0.f, 1.f, 0.f}, V{sine, 0.f, cosine}};
        };
        for (auto& victim : victims) {
            VictimRuntimeActor actor;
            actor.entity = victim.spawn.entity;
            actor.state = victim.pickup.state;
            actor.worldActorFlags = victim.pickup.worldActorFlags;
            actor.animation = victim.animation;
            actor.motionCount = uint32_t(victimAssets.at(victim.actorStem)->motionCount());
            setActorFrame(actor.frame, victim.spawn.selectedPosition, victim.facingDegrees);
            rescueState.actors.emplace(actor.entity, std::move(actor));
        }
        auto syncVictimsToRescueState = [&] {
            for (const auto& victim : victims) {
                auto found = rescueState.actors.find(victim.spawn.entity);
                if (found == rescueState.actors.end()) continue;
                auto& actor = found->second;
                actor.state = victim.pickup.state;
                actor.worldActorFlags = victim.pickup.worldActorFlags;
                actor.animation = victim.animation;
                setActorFrame(actor.frame, victim.spawn.selectedPosition, victim.facingDegrees);
            }
        };
        auto syncRescueStateToVictims = [&] {
            for (auto& victim : victims) {
                auto found = rescueState.actors.find(victim.spawn.entity);
                if (found == rescueState.actors.end()) continue;
                const auto& actor = found->second;
                victim.pickup.state = actor.state;
                victim.pickup.worldActorFlags = actor.worldActorFlags;
                victim.animation = actor.animation;
                victim.spawn.selectedPosition = actor.frame.position;
                victim.facingDegrees = std::atan2(-actor.frame.axis[0].z, actor.frame.axis[0].x) *
                    (180.f / 3.14159265358979323846f);
                victim.spawn.selectedHeading = (victim.facingDegrees - 180.f) / 30.f;
                if (actor.state == 6) victim.roaming = false;
            }
        };
        auto buildGogglesScanCandidates = [&] {
            std::vector<GogglesScanCandidate> candidates;
            for (int i = 0; i < count(entities); ++i) {
                J entity = at(entities, i);
                const int entityId = integer(field(entity, "index"));
                const std::string entityClass = str(field(entity, "class"));
                if (entityClass == "RCS_FireRescue") {
                    auto fire = std::find_if(fires.begin(), fires.end(), [&](const PreviewFire& item) {
                        return item.entity == entityId;
                    });
                    if (fire != fires.end())
                        candidates.push_back({entityId, 6,
                                              uint32_t(fire->runtime.suppression.state),
                                              fire->runtime.visual.position});
                } else if (entityClass == "RCS_VictimRescue") {
                    auto victim = std::find_if(victims.begin(), victims.end(), [&](const PreviewVictim& item) {
                        return item.spawn.entity == entityId;
                    });
                    if (victim != victims.end())
                        candidates.push_back({entityId, 2, victim->pickup.state,
                                              victim->spawn.selectedPosition});
                }
            }
            return candidates;
        };
        auto noteGogglesSelection = [&](const std::string& previousSelection) {
            const std::string selected = playerLoadout.selectedName();
            if (previousSelection == "goggles" && selected != "goggles")
                gogglesScan.deactivate(); // 00462660(0): deactivate each scan-effect child.
            if (previousSelection != "goggles" && selected == "goggles")
                gogglesScanPending = true; // 00462660(1) calls the goggles ACTION scan.
        };
        RetailFogState fogState;
        RescueQuake rescueQuake;
        std::unique_ptr<Scripts> scriptRuntime;
        WorldStartup worldStartup;
        bool worldStartupReady = false;
        if (scene.scripts()) {
            scriptRuntime = std::make_unique<Scripts>(scene.scripts());
            scriptRuntime->timeMs = SDL_GetTicks();
            scriptRuntime->variables["LEVEL"] = scene.key();
            scriptRuntime->hint = [&](std::string text, float seconds) {
                activeHint = std::move(text);
                activeHintRemaining = seconds;
                authoredHintText = activeHint;
                authoredHintLifetime.begin(scriptRuntime->timeMs, seconds);
                std::cout << "Hint: " << activeHint << "\n";
            };
            scriptRuntime->findEntity = [&](std::string name) {
                auto found = scriptActorIds.find(lower(std::move(name)));
                return found == scriptActorIds.end() ? -1 : found->second;
            };
            scriptRuntime->host = [&](int entityId, std::string command) {
                const size_t split = command.find_first_of(" \t\r\n");
                const std::string verb = lower(command.substr(0, split));
                const std::string arguments = split == std::string::npos
                    ? std::string{} : trim(command.substr(split + 1));
                auto owner = bmodelByEntity.find(entityId);
                if (verb == "setlighttable") {
                    std::istringstream styleArguments(arguments);
                    int style = 0;
                    std::string sequence;
                    if (!(styleArguments >> style >> sequence) || style < 1 || style > 19)
                        return false;
                    lightStyles.set(style, std::move(sequence));
                    return true;
                }
                if (verb == "fogenable") {
                    std::string fogArguments = arguments;
                    if (!fogArguments.empty() && fogArguments.front() == '*') {
                        auto value = scriptRuntime->variables.find(fogArguments.substr(1));
                        if (value == scriptRuntime->variables.end()) return false;
                        fogArguments = value->second;
                    }
                    std::array<int32_t, 5> values{};
                    if (!parseRetailFogEnableArguments(fogArguments, values)) return false;
                    fogState.enable(values[0], values[1], values[2], values[3], values[4],
                                    scriptRuntime->timeMs);
                    return true;
                }
                if (verb == "fogdisable") {
                    fogState.disable(scriptRuntime->timeMs);
                    return true;
                }
                if (entityId == playerEntityId && verb == "addbonus") {
                    char* end = nullptr;
                    const long long parsed = std::strtoll(arguments.c_str(), &end, 10);
                    if (end == arguments.c_str() ||
                        parsed < std::numeric_limits<int32_t>::min() ||
                        parsed > std::numeric_limits<int32_t>::max()) return false;
                    while (*end && std::isspace(static_cast<unsigned char>(*end))) ++end;
                    if (*end != '\0') return false;

                    // 0044EF10 adds its signed int argument to the global
                    // "Bonus Points" statistic through 00486980. The source
                    // stores the total as a wrapping 32-bit word.
                    addSourceStatistic(&rescueStatistics["Bonus Points"],
                                       static_cast<uint32_t>(static_cast<int32_t>(parsed)));
                    if (rescueStatistics["Bonus Points"].value != 0)
                        bonusPointsHudRenderer.begin(scriptRuntime->timeMs);
                    else
                        bonusPointsHudRenderer.hide();
                    return true;
                }
                if (verb == "quake") {
                    std::istringstream quakeArguments(arguments);
                    float strength = 0.f, durationSeconds = 0.f;
                    std::string trailing;
                    if (!(quakeArguments >> strength >> durationSeconds) ||
                        (quakeArguments >> trailing) || !std::isfinite(strength) ||
                        !std::isfinite(durationSeconds)) return false;
                    rescueQuake.trigger(scriptRuntime->timeMs, strength, durationSeconds);
                    return true;
                }
                if (entityId == playerEntityId && verb == "climb") {
                    if (!player.climbing) {
                        player.climbing = true;
                        player.v = {};
                    }
                    return true;
                }
                if (entityId == playerEntityId && verb == "unclimb") {
                    player.climbing = false;
                    return true;
                }
                if (entityId == playerEntityId && verb == "oxyon") {
                    startPlayerOxygenDrain(playerOxygen, scriptRuntime->timeMs);
                    startPlayerOxygenVisorEffect(
                        playerOxygenVisorEffect, scriptRuntime->timeMs, true);
                    sceneAudio.playOwned(playerEntityId, "VisorUpDown", player.p, player.p, yaw, 1);
                    return true;
                }
                if (entityId == playerEntityId && verb == "oxyoff") {
                    stopPlayerOxygenDrain(playerOxygen);
                    startPlayerOxygenVisorEffect(
                        playerOxygenVisorEffect, scriptRuntime->timeMs, false);
                    sceneAudio.playOwned(playerEntityId, "VisorUpDown", player.p, player.p, yaw, 1);
                    sceneAudio.stopOwned(playerEntityId, "Breathe");
                    return true;
                }
                if (entityId == playerEntityId && verb == "equip") {
                    const std::string previousSelection = playerLoadout.selectedName();
                    const bool equipped = playerLoadout.equip(arguments);
                    if (equipped) noteGogglesSelection(previousSelection);
                    return equipped;
                }
                if (entityId == playerEntityId && verb == "touchhose") {
                    char* end = nullptr;
                    const long parsed = std::strtol(arguments.c_str(), &end, 10);
                    if (end == arguments.c_str() || parsed < std::numeric_limits<int>::min() ||
                        parsed > std::numeric_limits<int>::max()) return false;
                    while (*end && std::isspace(static_cast<unsigned char>(*end))) ++end;
                    if (*end != '\0') return false;
                    const int argument = static_cast<int>(parsed);
                    const auto result = playerLoadout.touchhose(argument);
                    if (result == PreviewPlayerLoadout::TouchHoseResult::AlreadyHaveHose) {
                        activeHint = "Already have hose";
                        activeHintRemaining = 2.f;
                        return true;
                    }
                    activeHint = "Hose connected";
                    activeHintRemaining = 2.f;
                    std::cout << "PlayerRescue touchhose " << argument
                              << " connected slot 4; attachment records="
                              << playerLoadout.rescueHoseAttachment.count << "\n";
                    return true;
                }
                if (entityId == playerEntityId && verb == "select") {
                    const std::string previousSelection = playerLoadout.selectedName();
                    const bool selected = playerLoadout.select(arguments);
                    if (selected) noteGogglesSelection(previousSelection);
                    return selected;
                }
                if (entityId == playerEntityId && verb == "setashdensity") {
                    char* end = nullptr;
                    const float value = std::strtof(arguments.c_str(), &end);
                    if (end == arguments.c_str()) return false;
                    player.setAshDensity(value);
                    return true;
                }
                if (entityId == playerEntityId && verb == "setrescuebot") {
                    char* end = nullptr;
                    const long value = std::strtol(arguments.c_str(), &end, 0);
                    if (end == arguments.c_str()) return false;
                    player.rescueBotEnabled = value != 0;
                    return true;
                }
                if (verb == "norescuebot") {
                    auto victim = std::find_if(victims.begin(), victims.end(),
                        [&](const PreviewVictim& item) { return item.spawn.entity == entityId; });
                    if (victim == victims.end()) return false;
                    victim->noRescueBot = true;
                    return true;
                }
                if (entityId == playerEntityId && verb == "toolcmd" &&
                    lower(arguments) == "extinguisher refill") {
                    extinguisher.charge = 1.f;
                    return true;
                }
                if (entityId == playerEntityId && verb == "escape") {
                    // 004513A0 snapshots the current Player transform and
                    // heading only while the transient collision latch is set
                    // and oxygen is strictly above 0.5.
                    if (player.collisionLatch && playerOxygen.resource > 0.5f) {
                        player.checkpointPosition = player.p;
                        player.checkpointYaw = yaw;
                    }
                    if (rescueTransactions) {
                        syncVictimsToRescueState();
                        if (rescueTransactions->escape()) {
                            syncRescueStateToVictims();
                            if (!rescueState.exits.prompt.empty()) {
                                activeHint = rescueState.exits.prompt;
                                activeHintRemaining = 8.f;
                            } else {
                                activeHint = "Survivor rescued";
                                activeHintRemaining = 3.f;
                            }
                        }
                    }
                    return true;
                }
                if (entityId == playerEntityId && verb == "finalexit") {
                    const uint32_t total = rescueState.exits.total == std::numeric_limits<uint32_t>::max()
                        ? uint32_t(rescueState.actors.size()) : rescueState.exits.total;
                    const uint64_t accounted = uint64_t(rescueState.exits.field3530) +
                        uint64_t(rescueState.exits.rescues);
                    const uint32_t remaining = accounted >= total ? 0u :
                        total - uint32_t(accounted);
                    if (remaining == 0) {
                        rescueState.exits.messageGate = 1;
                        if (rescueServices.refreshSelectedItem)
                            rescueServices.refreshSelectedItem();
                        return true;
                    }
                    if (remaining == 1) {
                        activeHint = "There is still one survivor remaining.";
                    } else {
                        activeHint = "There are still " + std::to_string(remaining) + " survivors remaining.";
                    }
                    activeHintRemaining = 7.f;
                    return true;
                }
                if (entityId == playerEntityId && verb == "finalexitloadmap") {
                    const uint32_t total = rescueState.exits.total == std::numeric_limits<uint32_t>::max()
                        ? uint32_t(rescueState.actors.size()) : rescueState.exits.total;
                    const uint64_t accounted = uint64_t(rescueState.exits.field3530) +
                        uint64_t(rescueState.exits.rescues);
                    const uint32_t remaining = accounted >= total ? 0u :
                        total - uint32_t(accounted);
                    if (remaining != 0) {
                        activeHint = remaining == 1
                            ? "There is still one survivor remaining."
                            : "There are still " + std::to_string(remaining) + " survivors remaining.";
                        activeHintRemaining = 7.f;
                        return true;
                    }

                    // Retail validates data\\maps\\<key>.bsp before queuing a
                    // pending level request. The viewer's scene packages sit
                    // beside one another, so the authored key selects a sibling
                    // package here. The saved Ghidra trace has not proven that
                    // retail's separate forwarded context equals this key.
                    const auto [rawStageKey, ignored] = token(arguments);
                    (void)ignored;
                    std::string stageKey = lower(rawStageKey);
                    const bool safeKey = !stageKey.empty() &&
                        std::all_of(stageKey.begin(), stageKey.end(), [](unsigned char ch) {
                            return std::isalnum(ch) || ch == '_';
                        });
                    const auto stagePackage = package.parent_path() / stageKey;
                    if (!safeKey || !std::filesystem::is_regular_file(stagePackage / "entities.json") ||
                        !std::filesystem::is_regular_file(stagePackage / "collision.json")) {
                        activeHint = "Next rescue stage is not bundled: " + rawStageKey;
                        activeHintRemaining = 6.f;
                        std::cerr << "Rescue stage request could not be resolved to a sibling package: "
                                  << rawStageKey << "\n";
                        return true;
                    }
                    rescueStageRequest = stageKey;
                    if (nextRescueStage) *nextRescueStage = stageKey;
                    activeHint = "All survivors rescued. Loading " + stageKey + "...";
                    activeHintRemaining = 6.f;
                    std::cout << "Rescue stage request: " << scene.key() << " -> " << stageKey
                              << " (authored command key; viewer sibling-package adapter)\n";
                    return true;
                }
                if (verb == "playsnd" || verb == "loopsnd" || verb == "stopsnd") {
                    const size_t nameEnd = arguments.find_first_of(" \t\r\n");
                    const std::string soundName = arguments.substr(0, nameEnd);
                    if (soundName.empty()) return false;
                    V source = actorPositions.count(entityId) ? actorPositions.at(entityId) : player.p;
                    if (owner != bmodelByEntity.end()) source = bmodelMotions[owner->second].sourcePosition;
                    if (verb == "playsnd") {
                        soundDispatches.emplace_back(entityId, soundName);
                        sceneAudio.playOwned(entityId, soundName, source, player.p, yaw, 2);
                    } else if (verb == "loopsnd") {
                        sceneAudio.loopOwned(entityId, soundName, source, player.p, yaw, 2);
                    } else {
                        sceneAudio.stopOwned(entityId, soundName);
                    }
                    return true;
                }
                if (verb == "ambientsnd") {
                    // 0047A1F0 reads three coordinate operands and passes their
                    // integer-cast values to 00475210. That helper converts the
                    // integers back to float world coordinates and passes
                    // optional mode 2; the sound dispatcher maps a non-null
                    // mode to FMOD's normal-loop flag.
                    std::string normalized = arguments;
                    std::replace(normalized.begin(), normalized.end(), ',', ' ');
                    std::istringstream values(normalized);
                    std::string soundName, xText, yText, zText;
                    if (!(values >> soundName >> xText >> yText >> zText) || soundName.empty())
                        return false;
                    auto retailCoordinate = [](const std::string& text, float& result) {
                        char* end = nullptr;
                        const float parsed = std::strtof(text.c_str(), &end);
                        if (end == text.c_str() || !std::isfinite(parsed)) return false;
                        const double integral = std::trunc(static_cast<double>(parsed));
                        if (integral < double(std::numeric_limits<int32_t>::min()) ||
                            integral > double(std::numeric_limits<int32_t>::max())) return false;
                        result = static_cast<float>(static_cast<int32_t>(integral));
                        return true;
                    };
                    V source;
                    if (!retailCoordinate(xText, source.x) ||
                        !retailCoordinate(yText, source.y) ||
                        !retailCoordinate(zText, source.z)) return false;
                    const int ambientOwner = nextAmbientLoopOwner--;
                    ambientLoops.push_back({ambientOwner, soundName, source});
                    sceneAudio.loopOwned(ambientOwner, soundName, source, player.p, yaw, 2);
                    return true;
                }
                auto propOwner = propModelByEntity.find(entityId);
                if (propOwner != propModelByEntity.end()) {
                    auto& prop = propModels[propOwner->second];
                    if (verb == "setscale") {
                        V scale;
                        if (!parsePropScale(arguments, scale)) return false;
                        prop.scale = scale;
                        return true;
                    }
                    if (verb == "show") {
                        prop.visible = true;
                        prop.modelEnabled = true;
                        return true;
                    }
                    if (verb == "hide") {
                        prop.visible = false;
                        prop.modelEnabled = false;
                        return true;
                    }
                    if (verb == "makevalve") {
                        prop.makeValve = true;
                        return true;
                    }
                }
                if (verb == "makezone") {
                    auto trigger = std::find_if(boundsTriggers.begin(), boundsTriggers.end(),
                        [&](const PreviewBoundsTrigger& candidate) { return candidate.entity == entityId; });
                    const std::string zoneName = unwrap(arguments);
                    if (trigger == boundsTriggers.end() || zoneName.empty()) return false;
                    zones.addTrigger(zoneName, entityId, trigger->boundsMin, trigger->boundsMax);
                    return true;
                }
                if (verb == "setsize") {
                    auto fire = std::find_if(fires.begin(), fires.end(), [&](const PreviewFire& candidate) {
                        return candidate.entity == entityId;
                    });
                    V requestedSize;
                    float duration = 0.f;
                    if (fire == fires.end() || !parseFireRescueSizeCommand(arguments, requestedSize, duration))
                        return false;
                    const uint32_t now = scriptRuntime ? scriptRuntime->timeMs : SDL_GetTicks();
                    const double targetClock = std::nearbyint(double(now) + double(duration) * 1000.0);
                    if (!std::isfinite(targetClock) ||
                        targetClock < double(std::numeric_limits<int32_t>::min()) ||
                        targetClock > double(std::numeric_limits<int32_t>::max())) return false;
                    fire->runtime.visual.sizes.setMotion(
                        requestedSize, uint32_t(int32_t(targetClock)), now);
                    return true;
                }
                if (verb == "moveto") {
                    auto fire = std::find_if(fires.begin(), fires.end(), [&](const PreviewFire& candidate) {
                        return candidate.entity == entityId;
                    });
                    std::string targetName;
                    float divisor = 0.f, firstKeyOffset = 0.f, secondKeyOffset = 0.f;
                    if (fire == fires.end() ||
                        !parseFireRescueMoveCommand(arguments, targetName, divisor,
                                                    firstKeyOffset, secondKeyOffset))
                        return false;

                    // 00470C50 resolves the shared ScriptActor name registry
                    // case-insensitively. Keep its first registered-name
                    // behavior, then require that actor to be a PathPoint.
                    auto registered = scriptActorIds.find(lower(targetName));
                    if (registered == scriptActorIds.end()) return false;
                    const int targetEntity = registered->second;
                    bool isPathPoint = false;
                    for (int i = 0; i < count(entities); ++i) {
                        J candidate = at(entities, i);
                        if (integer(field(candidate, "index")) == targetEntity) {
                            isPathPoint = str(field(candidate, "class")) == "RCS_PathPoint";
                            break;
                        }
                    }
                    auto targetPosition = actorPositions.find(targetEntity);
                    if (!isPathPoint || targetPosition == actorPositions.end()) return false;

                    auto& motion = fire->pathMotion;
                    if (motion.targetEntity == targetEntity) {
                        std::cerr << "ERROR! Recursive PathPoint.\n";
                        return true;
                    }
                    motion.targetEntity = targetEntity;
                    motion.start = fire->runtime.visual.position;
                    motion.target = targetPosition->second;
                    const V displacement = motion.target - motion.start;
                    const double distanceSquared =
                        (double(displacement.x) * displacement.x +
                         double(displacement.y) * displacement.y) +
                        double(displacement.z) * displacement.z;
                    motion.distance = float(std::sqrt(distanceSquared));
                    motion.direction = motion.distance > 0.f
                        ? displacement * (1.f / motion.distance) : V{};
                    motion.progress = 0.f;
                    motion.lastVisualProgress = 0.f;
                    motion.updateThreshold = fire->routeVisualUpdateThreshold;
                    motion.updateThresholdMapped = fire->routeVisualUpdateThresholdMapped;
                    motion.rate = divisor > 0.f ? motion.distance / divisor : -divisor;
                    motion.firstKeyOffset = firstKeyOffset;
                    motion.secondKeyOffset = secondKeyOffset;
                    if (!std::isfinite(motion.distance) || !std::isfinite(motion.rate)) {
                        motion = {};
                        return false;
                    }
                    motion.active = true;
                    return true;
                }
                if (owner == bmodelByEntity.end()) return false;
                auto& bmodel = bmodelMotions[owner->second];
                if (verb == "addtimecmd") {
                    const auto [keyText, commandText] = token(arguments);
                    if (keyText.empty() || commandText.empty()) return false;
                    char* parseEnd = nullptr;
                    const float keyFrame = std::strtof(keyText.c_str(), &parseEnd);
                    if (parseEnd == keyText.c_str() || *parseEnd != '\0' ||
                        !std::isfinite(keyFrame)) return false;
                    const std::string timedCommand = unwrap(commandText);
                    if (timedCommand.empty()) return false;
                    auto slot = std::find_if(bmodel.timedCommands.begin(), bmodel.timedCommands.end(),
                        [](const PreviewBModelTimedCommand& timer) { return !timer.active; });
                    if (slot == bmodel.timedCommands.end()) {
                        bmodel.timedCommands.push_back({true, keyFrame, timedCommand});
                    } else {
                        *slot = {true, keyFrame, timedCommand};
                    }
                    return true;
                }
                if (verb == "removealltimecmds") {
                    for (auto& timer : bmodel.timedCommands) timer.active = false;
                    return true;
                }
                if (verb == "open" || verb == "close" || verb == "openaway" || verb == "opentowards") {
                    commandBModel(bmodel, verb, player.p);
                    return true;
                }
                if (verb == "settimescale") {
                    bmodel.timeScale = parseFloat(arguments, bmodel.timeScale);
                    return true;
                }
                if (verb == "sethitpoints") {
                    bmodel.hitPoints = int(parseFloat(arguments, float(bmodel.hitPoints)));
                    return true;
                }
                return false;
            };
            scriptRuntime->loadLevel(scene.key() + ".evs", -1, true);
            for (int i = 0; i < count(entities); ++i) {
                J entity = at(entities, i);
                const std::string entityClass = str(field(entity, "class"));
                if (entityClass == "RCS_BModel" || entityClass == "RCS_BoundsTrigger" ||
                    entityClass == "RCS_PlayerRescue" || entityClass == "RCS_PropModel" ||
                    (dedicatedPlayerHose && (entityClass == "RCS_PlayerHose" ||
                                             entityClass == "RCS_HosePath")))
                    scriptRuntime->startupEntity(entity);
            }
        }
        auto advanceWorldStartup = [&](uint32_t now) {
            if (!scriptRuntime || worldStartup.complete) return;
            scriptRuntime->timeMs = now;
            worldStartup.update(
                [&] { scriptRuntime->loadLevel(scene.key() + ".evs", -1, false); },
                [&] { worldStartupReady = true; });
            if (worldStartupReady)
                std::cout << "World startup EVS: delayed full pass completed after four host World updates; "
                          << "retail callback cadence remains unverified.\n";
        };
        const int rescueGlobalMode = 0;
        if (playerEntityId >= 0 && !dedicatedPlayerHose) {
            for (int i = 0; i < count(entities); ++i) {
                J entity = at(entities, i);
                const int id = integer(field(entity, "index"));
                const std::string entityClass = str(field(entity, "class"));
                if (entityClass == "RCS_VictimRescue" && rescueState.actors.count(id)) {
                    rescueState.entities.push_back({id, 2, false, {}});
                } else if (entityClass == "RCS_BoundsTrigger") {
                    auto trigger = std::find_if(boundsTriggers.begin(), boundsTriggers.end(),
                        [&](const PreviewBoundsTrigger& item) { return item.entity == id; });
                    if (trigger == boundsTriggers.end()) continue;
                    bool matchesCarryExit = false;
                    bool matchesFinalExit = false;
                    if (scriptRuntime) {
                        auto bindings = scriptRuntime->bindings.find(id);
                        if (bindings != scriptRuntime->bindings.end()) {
                            for (const Scripts::Binding& binding : bindings->second) {
                                if (binding.event != "touch") continue;
                                matchesCarryExit = matchesCarryExit ||
                                    scriptCallsPlayerCommand(binding.command, "escape");
                                matchesFinalExit = matchesFinalExit ||
                                    scriptCallsPlayerCommand(binding.command, "finalexit") ||
                                    scriptCallsPlayerCommand(binding.command, "finalexitloadmap");
                            }
                        }
                    }
                    trigger->carryModeGate = matchesCarryExit;
                    rescueState.entities.push_back({id, 8, matchesFinalExit,
                        (trigger->boundsMin + trigger->boundsMax) * .5f});
                }
            }

            hasPlayerRescueMessageRoute = rescueMessageRoutes.registerObject(
                &rescueState.player, playerRescueMessageRoute);
            rescueServices.playerFrame = [&]() -> const FrameTransform& {
                const float cosine = std::cos(yaw), sine = std::sin(yaw);
                rescuePlayerFrame.position = player.p;
                rescuePlayerFrame.axis = {
                    V{cosine, 0.f, -sine}, V{0.f, 1.f, 0.f}, V{sine, 0.f, cosine}
                };
                return rescuePlayerFrame;
            };
            rescueServices.traceFloor = [&](V start, V delta, V mins, V maxs, V& point) {
                Hit hit;
                if (!collision.trace(start, delta, mins, maxs, hit)) return false;
                point = hit.point;
                return true;
            };
            rescueServices.reportFloorFailure = [&] {
                std::cerr << "Rescue release floor placement found no BSP surface\n";
            };
            rescueServices.readClock = [] { return uint32_t(SDL_GetTicks()); };
            rescueServices.readPlayerField18C = [&] { return carriedVictim.carriedVictimEntity ? 4 : 3; };
            rescueServices.readGlobalMode = [rescueGlobalMode] { return rescueGlobalMode; };
            // Retail initializes Player+0x343C from active level +0x10,
            // applies the mode multiplier, and uses 600.0f when no level is active.
            rescueServices.readPlayerField343C = [levelField10, rescueGlobalMode] {
                return initializePlayerDeadlineInput(levelField10, rescueGlobalMode);
            };
            rescueServices.setActorFlags = [&](int entity, uint32_t flags) {
                auto actor = rescueState.actors.find(entity);
                if (actor == rescueState.actors.end()) return;
                actor->second.worldActorFlags = flags;
                auto victim = std::find_if(victims.begin(), victims.end(), [&](const PreviewVictim& item) {
                    return item.spawn.entity == entity;
                });
                if (victim != victims.end()) victim->pickup.worldActorFlags = flags;
            };
            rescueServices.dispatchVictimEvent = [&](int entity, const char* event) {
                if (scriptRuntime) scriptRuntime->dispatch(entity, event);
            };
            rescueServices.setEntityCarryMode = [&](bool mode) {
                for (auto& trigger : boundsTriggers) {
                    if (!trigger.carryModeGate) continue;
                    trigger.carryMode = mode;
                    if (!mode) {
                        // 00440F50(false) immediately calls
                        // geWorld_ModelSetFlags(model, 0) for this gate.
                        worldModelFlags.flags[trigger.modelIndex] = 0;
                    }
                }
            };
            rescueServices.updateVictimStatus = [&](int action) {
                if (action == 1)
                    victimPickupHudRenderer.begin(rescueServices.readClock());
                if (action != 2) return;
                victimPickupHudRenderer.hide();
                if (!carriedVictim.carriedVictimEntity) return;
                const int entity = *carriedVictim.carriedVictimEntity;
                auto actor = rescueState.actors.find(entity);
                if (actor == rescueState.actors.end()) return;
                auto victim = std::find_if(victims.begin(), victims.end(), [&](const PreviewVictim& item) {
                    return item.spawn.entity == entity;
                });
                if (victim != victims.end()) {
                    victim->pickup.state = actor->second.state;
                    victim->animation = actor->second.animation;
                }
            };
            rescueServices.addStatistic = [&](const char* name, uint32_t value) {
                addSourceStatistic(&rescueStatistics[name], value);
            };
            rescueServices.updateStatistic = [&](const char* name, uint32_t value) {
                updateSourceStatistic(&rescueStatistics[name], value);
            };
            rescueServices.setRescueStatisticTotal = [&](uint32_t total) {
                rescueState.exits.total = total;
            };
            rescueServices.reportUnzonedVictim = [](int) {};
            rescueServices.reportUnzonedExit = [](int) {};
            rescueServices.enableExitTrigger = [&](int entity) {
                enabledExitTriggers.insert(entity);
                activeHint = rescueState.exits.prompt;
                activeHintRemaining = 8.f;
            };
            rescueServices.refreshSelectedItem = [&] {
                const std::string previousSelection = playerLoadout.selectedName();
                HoseItemSelection::Result selectionResult;
                HoseItemSelection::reselectFirst(playerLoadout.items, selectionResult);
                noteGogglesSelection(previousSelection);
            };
            rescueServices.passout.readPlayerPosition3400 = [&] {
                return player.safePosition3400;
            };
            rescueServices.passout.writePlayerPosition4c = [&](V position) {
                player.p = position;
            };
            rescueServices.passout.setVictimMotion = [&](int entity, uint32_t motion, float speed) {
                auto runtimeActor = rescueState.actors.find(entity);
                auto previewVictim = std::find_if(victims.begin(), victims.end(), [&](const PreviewVictim& item) {
                    return item.spawn.entity == entity;
                });
                if (runtimeActor == rescueState.actors.end() || previewVictim == victims.end())
                    throw std::runtime_error("Passout victim is missing its live actor adapter");
                VictimActorAsset& actorAsset = *victimAssets.at(previewVictim->actorStem);
                J motionData = at(field(actorAsset.data.get(), "motions"), int(motion));
                const float end = num(field(motionData, "end"));
                runtimeActor->second.animation.select(int(motion), 0.f, end, speed,
                                                        uint32_t(SDL_GetTicks()));
                previewVictim->animation = runtimeActor->second.animation;
            };
            rescueServices.passout.readCurrentEntities = [&] {
                std::vector<PlayerHeroDownEntity> current;
                current.reserve(rescueState.entities.size());
                for (const auto& entity : rescueState.entities) {
                    if (entity.type != 2) continue;
                    auto actor = rescueState.actors.find(entity.entity);
                    auto victim = std::find_if(victims.begin(), victims.end(), [&](const PreviewVictim& item) {
                        return item.spawn.entity == entity.entity;
                    });
                    if (actor == rescueState.actors.end() || victim == victims.end()) continue;
                    current.push_back({entity.type, victim->spawn.name, int(actor->second.state)});
                }
                return current;
            };
            // The viewer's current global mode adapter is source-default mode
            // 0, so the mode-2 HeroDown spawn/count branch is unreachable.
            // Keep an explicit failure if that unsupported mode is enabled.
            rescueServices.passout.spawnHeroDown = [](const PlayerHeroDownSpawn&) {
                throw std::runtime_error("Mode-2 HeroDown actor spawning is not mapped in this viewer");
            };
            rescueServices.passout.logFirefightersDown = [](int) {
                throw std::runtime_error("Mode-2 firefighters-down logging is not mapped in this viewer");
            };
            rescueServices.passout.readCleanupState = [] { return uint32_t(0); };
            rescueServices.passout.readCleanupCollectionCount = [] { return uint32_t(0); };
            rescueServices.passout.reportCleanupArrayBounds = [] {
                std::cerr << "Passout cleanup array bound reported by retail path\n";
            };
            rescueServices.passout.cleanupSubobjectMode = [] { return 0; };
            rescueServices.passout.transitionCleanupSubobjectTo2 = [] {};
            rescueServices.passout.refreshSelection = [&] {
                rescueServices.refreshSelectedItem();
            };
            rescueServices.passout.resetPlayerView = [&] {
                player.p = player.checkpointPosition;
                player.safePosition3400 = player.checkpointPosition;
                yaw = player.checkpointYaw;
                player.v = {};
                player.delta = {};
                player.cooldown = 0.f;
                player.ground = false;
                player.crouch = false;
                player.climbing = false;
                player.collisionLatch = false;
                playerFireState.state = 3;
                playerIncapacitated = false;
                placeModelOnFloor(player.p, {-30.f, -30.f, -30.f}, player.maxs(),
                    [&](V start, V delta, V mins, V maxs, V& point) {
                        Hit hit;
                        if (!collision.trace(start, delta, mins, maxs, hit)) return false;
                        point = hit.point;
                        return true;
                    }, [&] { std::cerr << "Passout reset could not find floor placement\n"; });
            };
            rescueServices.passout.finishRescueStatus = [&] {
                if (rescueState.exits.total == std::numeric_limits<uint32_t>::max()) {
                    rescueState.exits.total = uint32_t(rescueState.actors.size());
                    rescueServices.setRescueStatisticTotal(rescueState.exits.total);
                }
                const uint32_t remaining = rescueState.exits.total -
                    rescueState.exits.field3530 - rescueState.exits.rescues;
                activeHint = remaining == 1
                    ? "There's still one survivor remaining!"
                    : "There are " + std::to_string(int32_t(remaining)) + " survivors left!";
                activeHintRemaining = 5.f;
                playerOxygen.displayedResource = 0.f;
                playerOxygen.lowWarningPosted = true;
            };
            rescueTransactions.reset(new VictimTransactions{
                rescueState, rescueServices, zones, gameplayRandom});
        }
        auto advanceRescueBot = [&](uint32_t now) {
            if (!player.rescueBotEnabled.value_or(false) || playerIncapacitated ||
                !rescueTransactions) return;
            if (!rescueBotState.schedulesInitialized) {
                initializeRescueBotSchedules(
                    rescueBotState, now,
                    initializePlayerDeadlineInput(levelField10, rescueGlobalMode),
                    rescueGlobalMode, gameplayRandom);
                return;
            }

            syncVictimsToRescueState();
            if (rescueBotState.activeVictim && rescueBotDeadlineDue(
                    now, rescueBotState.activeDeadline)) {
                // 00455580 refreshes final-exit availability at bot drop-off.
                rescueTransactions->refreshFinalExitAvailability();
                std::cout << "RESCUEBOT: Victim dropped off\n";
                rescueBotState.activeVictim.reset();
                postponeRescueBotAttemptIfFuture(rescueBotState, now);
                rescueBotState.nextMessageDeadline =
                    nextRescueBotMessageDeadline(now, gameplayRandom);
            }

            const int sourcePlayerState = playerIncapacitated ? 6 :
                (player.climbing ? 5 : (player.crouch ? 4 : 3));
            const bool regularPlayerState = sourcePlayerState == 3 ||
                sourcePlayerState == 4 || sourcePlayerState == 5;
            if (!rescueBotState.activeVictim && regularPlayerState &&
                rescueBotDeadlineDue(now, rescueBotState.nextAttemptDeadline)) {
                const int32_t maximum = rescueBotMaximum(
                    rescueState.exits.total, rescueGlobalMode);
                if (int32_t(rescueState.exits.field3530) < maximum) {
                    std::vector<RescueBotCandidate> candidates;
                    candidates.reserve(rescueState.actors.size());
                    for (const auto& entity : rescueState.entities) {
                        if (entity.type != 2) continue;
                        auto actor = rescueState.actors.find(entity.entity);
                        auto victim = std::find_if(victims.begin(), victims.end(),
                            [&](const PreviewVictim& item) {
                                return item.spawn.entity == entity.entity;
                            });
                        if (actor == rescueState.actors.end() || victim == victims.end()) continue;
                        candidates.push_back({entity.entity, actor->second.state,
                            victim->noRescueBot, victim->spawn.selectedPosition});
                    }
                    auto target = selectRescueBotCandidate(
                        candidates, player.p, [&](V start, V end) {
                            Hit sight;
                            return collision.tracePoint(start, end, 0x43, 2, sight);
                        });
                    if (target) {
                        auto actor = rescueState.actors.find(*target);
                        auto victim = std::find_if(victims.begin(), victims.end(),
                            [&](const PreviewVictim& item) {
                                return item.spawn.entity == *target;
                            });
                        if (actor != rescueState.actors.end() && victim != victims.end()) {
                            static const char* reports[] = {
                                "I've located %s, and am heading for the nearest exit.",
                                "I just located %s.  I'm heading to an exit.",
                                "[Cough cough]  I've located %s, proceeding to exit.",
                                "I found %s.  Heading to the nearest exit...",
                                "I just found %s.  Let's get these people to safety!",
                                "I've spotted %s here.  I'll take care of this one!",
                                "I'm taking %s to safety.  Over and out.",
                                "I just spotted %s here.  I'll handle this one!"
                            };
                            const uint32_t reportIndex = gameplayRandom.bounded(8u);
                            std::string name = victim->spawn.name;
                            if (name.empty() || name == "<null>") name = "survivor";
                            activeHint = reports[reportIndex];
                            const size_t marker = activeHint.find("%s");
                            if (marker != std::string::npos)
                                activeHint.replace(marker, 2, name);
                            activeHintRemaining = 6.f;

                            ++rescueState.exits.field3530;
                            actor->second.state = 6;
                            victim->pickup.state = 6;
                            if (scriptRuntime) scriptRuntime->dispatch(*target, "botrescue");
                            victim->botRescueField1F0 = std::numeric_limits<uint32_t>::max();
                            actor->second.animation.playing = false;
                            victim->animation.playing = false;
                            victim->roaming = false;
                            rescueServices.setActorFlags(*target, 0);
                            rescueBotState.activeVictim = *target;
                            rescueBotState.activeDeadline =
                                startRescueBotCarryDeadline(now, gameplayRandom);
                            std::cout << "RESCUEBOT: Rescuing " << name << " (entity "
                                      << *target << ")\n";
                        }
                    }
                }
                // 00455580 schedules another attempt after every due attempt,
                // including a maximum-count or no-candidate result.
                setPlayerRescueDeadline(rescueBotState.nextAttemptDeadline,
                    initializePlayerDeadlineInput(levelField10, rescueGlobalMode),
                    rescueGlobalMode, now, gameplayRandom);
            }

            if (rescueBotDeadlineDue(now, rescueBotState.nextMessageDeadline)) {
                static const char* prompts[] = {
                    "Let's move!",
                    "Let's get this fire under control!",
                    "Come on, let's hussle!",
                    "We're counting on you!",
                    "Hurry up, we're running out of time!"
                };
                activeHint = prompts[gameplayRandom.bounded(5u)];
                activeHintRemaining = 6.f;
                postponeRescueBotAttemptIfFuture(rescueBotState, now);
                rescueBotState.nextMessageDeadline =
                    nextRescueBotMessageDeadline(now, gameplayRandom);
            }
            syncRescueStateToVictims();
        };
        if ((interactionCheck || rescueDiagnostic || renderCheck) && scriptRuntime) {
            // These diagnostics do not enter the interactive frame loop. Drive
            // the source-mapped four WorldEntity callbacks so their EVS state
            // matches the point after PlayerRescue registration.
            for (int update = 0; update < 4; ++update)
                advanceWorldStartup(uint32_t(SDL_GetTicks()));
            if (!worldStartupReady)
                throw std::runtime_error("Delayed full EVS pass did not complete after four World updates");
        }
        // A static render capture should show the authored settled fog target;
        // the interactive viewer advances the same transition in real time.
        if (renderCheck && fogState.transitioning)
            fogState.update(fogState.transitionStartMs + RetailFogState::TransitionDurationMs);
        if (rescueDiagnostic) {
            RetailFogState fogTimingAudit;
            fogTimingAudit.enable(0, 600, 50, 50, 50, 100);
            fogTimingAudit.update(2100);
            fogTimingAudit.disable(2100);
            fogTimingAudit.update(4099);
            const bool disableRampsBeforeTurningOff = fogTimingAudit.renderEnabled() &&
                fogTimingAudit.transitioning &&
                std::abs(fogTimingAudit.target.start - 1990.f) < 0.001f &&
                std::abs(fogTimingAudit.target.end - 2000.f) < 0.001f &&
                std::abs(fogTimingAudit.target.red - 50.f) < 0.001f;
            fogTimingAudit.update(4100);
            if (!disableRampsBeforeTurningOff || fogTimingAudit.renderEnabled() ||
                fogTimingAudit.transitioning)
                throw std::runtime_error("Fog disable did not stay enabled through its two-second transition");

            if (scene.key() == "gas_r" && scriptRuntime) {
                int insideFogTrigger = -1;
                int outsideFogTrigger = -1;
                for (const auto& [entity, bindings] : scriptRuntime->bindings) {
                    for (const Scripts::Binding& binding : bindings) {
                        const std::string command = lower(trim(binding.command));
                        if (binding.event != "touch") continue;
                        if (command == "fogenable 5 320 60 40 40") insideFogTrigger = entity;
                        if (command == "fogdisable") outsideFogTrigger = entity;
                    }
                }
                if (insideFogTrigger < 0 || outsideFogTrigger < 0)
                    throw std::runtime_error("Gas R fog trigger bindings are missing");

                const RetailFogState savedFog = fogState;
                const float savedAshDensity = player.ashDensity;
                const uint32_t savedScriptTime = scriptRuntime->timeMs;
                scriptRuntime->dispatch(insideFogTrigger, "touch");
                const uint32_t enabledAt = scriptRuntime->timeMs;
                const bool insideTargetReached = fogState.targetEnabled && fogState.transitioning &&
                    std::abs(fogState.target.start - 1.f) < 0.001f &&
                    std::abs(fogState.target.end - 320.f) < 0.001f &&
                    std::abs(fogState.target.red - 60.f) < 0.001f &&
                    std::abs(fogState.target.green - 40.f) < 0.001f &&
                    std::abs(fogState.target.blue - 40.f) < 0.001f;
                fogState.update(enabledAt + RetailFogState::TransitionDurationMs);
                const auto gasFogColor = fogState.normalizedColor();
                const bool quantizedColorMatches =
                    std::abs(gasFogColor[0] * 255.f - 48.f) < 0.01f &&
                    std::abs(gasFogColor[1] * 255.f - 32.f) < 0.01f &&
                    std::abs(gasFogColor[2] * 255.f - 32.f) < 0.01f;
                scriptRuntime->timeMs = enabledAt + RetailFogState::TransitionDurationMs;
                scriptRuntime->dispatch(outsideFogTrigger, "touch");
                const bool outsideTargetReached = !fogState.targetEnabled && fogState.transitioning &&
                    std::abs(fogState.target.start - 1990.f) < 0.001f &&
                    std::abs(fogState.target.end - 2000.f) < 0.001f &&
                    std::abs(fogState.target.red - 60.f) < 0.001f &&
                    std::abs(fogState.target.green - 40.f) < 0.001f &&
                    std::abs(fogState.target.blue - 40.f) < 0.001f;
                fogState.update(enabledAt + 2 * RetailFogState::TransitionDurationMs - 1);
                const bool enabledUntilTarget = fogState.renderEnabled();
                fogState.update(enabledAt + 2 * RetailFogState::TransitionDurationMs);
                const bool disabledAtTarget = !fogState.renderEnabled();
                fogState = savedFog;
                player.setAshDensity(savedAshDensity);
                scriptRuntime->timeMs = savedScriptTime;
                if (!insideTargetReached || !quantizedColorMatches || !outsideTargetReached ||
                    !enabledUntilTarget || !disabledAtTarget)
                    throw std::runtime_error("Gas R fog trigger did not preserve the retail fog transition");
                std::cout << "Gas R fog trigger check: inside RGB quantized to 48/32/32; "
                             "outside keeps fog active until the two-second transition ends.\n";
            }

            const bool expectsGoggles = scene.key() != "training" && scene.key() != "gas_r";
            const bool expectsExtinguisher = scene.key() == "gas_r" ||
                                             scene.key() == "oil_rig_b" ||
                                             scene.key() == "oil_rig_c" ||
                                             scene.key() == "oil_rig_d";
            if (!playerLoadout.items.slots[1].present ||
                playerLoadout.items.slots[2].present != expectsExtinguisher ||
                playerLoadout.items.slots[3].present != expectsGoggles)
                throw std::runtime_error("Delayed startup EVS did not produce the authored rescue loadout");
            std::optional<float> expectedFogEnd;
            if (scene.key() == "bank") expectedFogEnd = 600.f;
            else if (scene.key() == "boat_a") expectedFogEnd = 1000.f;
            else if (scene.key() == "boat_b") expectedFogEnd = 250.f;
            if (expectedFogEnd && (!fogState.configured || !fogState.targetEnabled ||
                !fogState.transitioning || std::abs(fogState.target.start - 1.f) > 0.001f ||
                std::abs(fogState.target.end - *expectedFogEnd) > 0.001f ||
                std::abs(fogState.target.red - 50.f) > 0.001f ||
                std::abs(fogState.target.green - 50.f) > 0.001f ||
                std::abs(fogState.target.blue - 50.f) > 0.001f))
                throw std::runtime_error("Authored map fog command did not reach its expected target");
            std::cout << "Startup loadout check: axe="
                      << (playerLoadout.items.slots[1].present ? "yes" : "no")
                      << ", extinguisher=" << (playerLoadout.items.slots[2].present ? "yes" : "no")
                      << ", goggles=" << (playerLoadout.items.slots[3].present ? "yes" : "no")
                      << ", selected=" << playerLoadout.selectedName()
                      << " (controlled host check; retail callback order remains open).\n";
            if (expectedFogEnd)
                std::cout << "Startup fog check: end=" << *expectedFogEnd
                          << ", RGB=50/50/50, two-second transition active.\n";
        }
        printInventory(scene, meshes, textureSummary);
        if (!inspectOnly && (!interactionCheck || rescueDiagnostic)) {
            size_t roaming = std::count_if(victims.begin(), victims.end(), [](const PreviewVictim& victim) {
                return victim.roaming;
            });
            std::cout << "Rescue actors: " << victims.size() << " loaded, " << roaming
                      << " with authored roam scripts; decoded ACT types: " << victimAssets.size() << "\n";
            std::cout << "FireRescue actors: " << fires.size()
                      << " initialized from authored positions; extinguisher suppression, contact-driven spread, and Player exposure/passout are wired; exposure volume and cadence remain provisional\n";
        }
        if (foundPlayer) std::cout << "Spawn: " << spawn.x << ", " << spawn.y << ", " << spawn.z << "\n";
        else std::cout << "Spawn: fallback near world-model bounds (no player entity found)\n";
        int touchBindings = 0, useBindings = 0;
        if (scriptRuntime) {
            for (const auto& [entity, bindings] : scriptRuntime->bindings) {
                for (const auto& binding : bindings) {
                    if (binding.event == "touch") ++touchBindings;
                    if (binding.event == "use") ++useBindings;
                }
            }
        }
        std::cout << "BModel actors: " << bmodelMotions.size() << " loaded, " << animatedBModels
                  << " with retail PATHs; EVS: " << (scriptRuntime ? "loaded" : "not bundled")
                  << ", touch bindings=" << touchBindings << ", use bindings=" << useBindings
                  << ", sound resources=" << (scene.sounds() ? count(field(scene.sounds(), "resources")) : 0) << "\n";
        std::cout << "Bounds triggers: " << boundsTriggers.size() << " loaded from authored C_Model bounds\n";
        std::cout << "Ambient sound loops: " << ambientLoops.size()
                  << " fixed-source EVS requests registered (loop and spatial rolloff use the shared host mixer)\n";
        std::cout << "Scene package path: " << scene.root().string() << "\n";
        std::cout << "Named zones: " << zones.groups.size() << " built from BoundsTrigger makezone commands\n";
        logRescueStage("scene startup complete");
        if (ambientAudioCheck) {
            const size_t expectedFireLoops = size_t(std::count_if(
                fires.begin(), fires.end(), [](const PreviewFire& fire) {
                    return fire.runtime.soundChoice >= 0;
                }));
            if (ambientLoops.empty() && expectedFireLoops == 0)
                throw std::runtime_error("Scene audio check found no authored loop sources");
            const V testListener = player.p + V{17.f, -2.f, 9.f};
            const float testYaw = yaw + .25f;
            sceneAudio.updateOwnedListeners(testListener, testYaw);
            for (const auto& ambient : ambientLoops)
                sceneAudio.updateOwned(ambient.owner, ambient.name, ambient.source,
                                       testListener, testYaw);
            size_t verifiedVoices = 0;
            size_t spatialVoices = 0;
            size_t verifiedFireLoops = 0;
            if (!sceneAudio.applyOxygenVisorCategorySelector(4, playerOxygenVisorGroup2Gain))
                throw std::runtime_error("Scene audio check could not apply oxygen-on sound selector 4");
            SceneAudio::Voice categoryOneProbe;
            categoryOneProbe.channelGroup = 1;
            SceneAudio::Voice categoryTwoProbe;
            categoryTwoProbe.channelGroup = 2;
            bool verifiedGroupMix = false;
            SDL_LockAudioDevice(sceneAudio.device);
            verifiedGroupMix = std::abs(sceneAudio.soundGroupGainForVoice(categoryOneProbe) - 1.f) <= .001f &&
                               std::abs(sceneAudio.soundGroupGainForVoice(categoryTwoProbe) -
                                        playerOxygenVisorGroup2Gain) <= .001f;
            for (const auto& ambient : ambientLoops) {
                auto voice = std::find_if(sceneAudio.voices.begin(), sceneAudio.voices.end(),
                    [&](const SceneAudio::Voice& candidate) {
                        return candidate.ownerAware && candidate.owner == ambient.owner &&
                               candidate.foldedName == SceneAudio::fold(ambient.name) &&
                               candidate.sample && candidate.looping;
                    });
                if (voice == sceneAudio.voices.end()) continue;
                if (voice->channelGroup != 2 ||
                    std::abs(sceneAudio.soundGroupGainForVoice(*voice) -
                             playerOxygenVisorGroup2Gain) > .001f) {
                    SDL_UnlockAudioDevice(sceneAudio.device);
                    throw std::runtime_error("Scene audio check found an incorrect retail sound group");
                }
                if (voice->sample->spatial) ++spatialVoices;
                const auto near = [](float left, float right) {
                    return std::abs(left - right) <= .001f;
                };
                if (!near(voice->source.x, ambient.source.x) ||
                    !near(voice->source.y, ambient.source.y) ||
                    !near(voice->source.z, ambient.source.z) ||
                    !near(voice->listener.x, testListener.x) ||
                    !near(voice->listener.y, testListener.y) ||
                    !near(voice->listener.z, testListener.z) ||
                    !near(voice->yaw, testYaw)) {
                    SDL_UnlockAudioDevice(sceneAudio.device);
                    throw std::runtime_error("Scene audio check found an incorrect source or listener update");
                }
                ++verifiedVoices;
            }
            for (const auto& fire : fires) {
                const char* sound = fireLoopSoundName(fire.runtime.soundChoice);
                if (!sound) continue;
                auto voice = std::find_if(sceneAudio.voices.begin(), sceneAudio.voices.end(),
                    [&](const SceneAudio::Voice& candidate) {
                        return candidate.ownerAware && candidate.owner == fire.entity &&
                               candidate.foldedName == SceneAudio::fold(sound) &&
                               candidate.sample && candidate.looping;
                    });
                if (voice == sceneAudio.voices.end() || !voice->sample->spatial ||
                    voice->channelGroup != 2 || voice->sample->minimumDistance != 200.f ||
                    voice->sample->maximumDistance != 1200.f || !fire.loopSoundStarted ||
                    std::abs(voice->source.x - fire.runtime.origin.x) > .001f ||
                    std::abs(voice->source.y - fire.runtime.origin.y) > .001f ||
                    std::abs(voice->source.z - fire.runtime.origin.z) > .001f ||
                    std::abs(voice->listener.x - testListener.x) > .001f ||
                    std::abs(voice->listener.y - testListener.y) > .001f ||
                    std::abs(voice->listener.z - testListener.z) > .001f ||
                    std::abs(voice->yaw - testYaw) > .001f) {
                    SDL_UnlockAudioDevice(sceneAudio.device);
                    throw std::runtime_error("Scene audio check found an invalid authored FireRescue loop");
                }
                ++verifiedFireLoops;
            }
            SDL_UnlockAudioDevice(sceneAudio.device);
            if (!verifiedGroupMix)
                throw std::runtime_error("Scene audio check found an incorrect category 1/2 mix gain");
            if (verifiedVoices != ambientLoops.size())
                throw std::runtime_error("Scene audio check could not resolve every ambient loop to a bundled voice");
            if (spatialVoices != verifiedVoices)
                throw std::runtime_error("Scene audio check found a non-3D sound resource for an ambient loop");
            if (verifiedFireLoops != expectedFireLoops)
                throw std::runtime_error("Scene audio check could not resolve each named fire to its bundled loop");
            if (!sceneAudio.applyOxygenVisorCategorySelector(5, playerOxygenVisorGroup2Gain))
                throw std::runtime_error("Scene audio check could not apply oxygen-off sound selector 5");
            SDL_LockAudioDevice(sceneAudio.device);
            const bool verifiedRestoredMix =
                std::abs(sceneAudio.soundGroupGainForVoice(categoryOneProbe) - 1.f) <= .001f &&
                std::abs(sceneAudio.soundGroupGainForVoice(categoryTwoProbe) - 1.f) <= .001f;
            SDL_UnlockAudioDevice(sceneAudio.device);
            if (!verifiedRestoredMix)
                throw std::runtime_error("Scene audio check found that selector 5 did not restore category gains");
            std::cout << "Authored FireRescue audio: " << verifiedFireLoops
                      << " selected 3D loops verified with source, listener, and retail distance metadata.\n";
            bool verifiedBoatTimedAudio = false;
            if (scene.key() == "boat_b") {
                auto button = std::find_if(bmodelMotions.begin(), bmodelMotions.end(),
                    [](const PreviewBModelMotion& candidate) {
                        return lower(candidate.authoredName) == "button";
                    });
                if (button == bmodelMotions.end() || !scriptRuntime)
                    throw std::runtime_error("Boat B ambient audio check could not resolve the authored button BModel");
                const int buttonEntity = button->entity;
                const auto hasTimer = [&](float keyFrame, const std::string& command) {
                    return std::any_of(button->timedCommands.begin(), button->timedCommands.end(),
                        [&](const PreviewBModelTimedCommand& timer) {
                            return timer.active && std::abs(timer.keyFrame - keyFrame) <= .001f &&
                                   lower(timer.command) == command;
                        });
                };
                if (!hasTimer(.1f, "loopsnd ambient2") ||
                    !hasTimer(29.f, "stopsnd ambient2") || button->endTime <= 29.f)
                    throw std::runtime_error("Boat B button did not register its authored Ambient2 PATH timers");

                std::vector<PreviewBModelMotion> timedProbe{*button};
                timedProbe.front().currentTime = timedProbe.front().startTime;
                timedProbe.front().timeScale = 1.f;
                timedProbe.front().motionState = 3;
                auto dispatchTimedAudio = [&](int entityId, size_t timerIndex,
                                              const std::string& command) {
                    scriptRuntime->run(command, entityId,
                        "BModel " + std::to_string(entityId) + "/timer " +
                            std::to_string(timerIndex));
                };
                updateBModelStates(timedProbe, .15f, collision, player.p, dispatchTimedAudio);
                auto hasBoatAmbientVoice = [&] {
                    SDL_LockAudioDevice(sceneAudio.device);
                    const bool found = std::any_of(sceneAudio.voices.begin(), sceneAudio.voices.end(),
                        [&](const SceneAudio::Voice& voice) {
                            return voice.ownerAware && voice.owner == buttonEntity &&
                                   voice.foldedName == "ambient2" && voice.looping && voice.sample;
                        });
                    SDL_UnlockAudioDevice(sceneAudio.device);
                    return found;
                };
                if (!hasBoatAmbientVoice() || timedProbe.front().timedCommands[0].active)
                    throw std::runtime_error("Boat B 0.1 PATH timer did not start its Ambient2 loop");
                updateBModelStates(timedProbe, 29.1f, collision, player.p, dispatchTimedAudio);
                if (hasBoatAmbientVoice() || timedProbe.front().timedCommands[1].active)
                    throw std::runtime_error("Boat B 29 PATH timer did not stop its Ambient2 loop");
                verifiedBoatTimedAudio = true;
            }
            std::cout << "Scene audio check: " << verifiedVoices << " ambient and "
                      << verifiedFireLoops << " FireRescue looping 3D voices passed";
            if (verifiedBoatTimedAudio)
                std::cout << "; Boat B Ambient2 PATH start/stop timers passed";
            std::cout << ".\n";
            sceneAudio.close();
            SDL_Quit();
            return 0;
        }
        if (interactionCheck) {
            runInteractionCheck(scene, entities, bmodelMotions, boundsTriggers,
                                soundDispatches, scriptRuntime.get(), collision, player);
            return 0;
        }
        if (rescueCheck) {
            if (!scriptRuntime || !rescueTransactions || victims.empty())
                throw std::runtime_error("Rescue check requires EVS, a PlayerRescue entity, and at least one victim");
            int exitTrigger = -1;
            const PreviewBoundsTrigger* triggerBounds = nullptr;
            std::vector<std::pair<int, const PreviewBoundsTrigger*>> finalExitTriggers;
            std::vector<std::pair<int, const PreviewBoundsTrigger*>> stageExitTriggers;
            for (const auto& trigger : boundsTriggers) {
                auto bindings = scriptRuntime->bindings.find(trigger.entity);
                if (bindings == scriptRuntime->bindings.end()) continue;
                const bool escapeBinding = std::any_of(bindings->second.begin(), bindings->second.end(),
                    [](const Scripts::Binding& binding) {
                        return binding.event == "touch" &&
                            scriptCallsPlayerCommand(binding.command, "escape");
                    });
                if (escapeBinding && exitTrigger < 0) {
                    exitTrigger = trigger.entity;
                    triggerBounds = &trigger;
                }
                const bool finalExitBinding = std::any_of(bindings->second.begin(), bindings->second.end(),
                    [](const Scripts::Binding& binding) {
                        return binding.event == "touch" &&
                            scriptCallsPlayerCommand(binding.command, "finalexit");
                    });
                if (finalExitBinding) finalExitTriggers.emplace_back(trigger.entity, &trigger);
                const bool stageBinding = std::any_of(bindings->second.begin(), bindings->second.end(),
                    [](const Scripts::Binding& binding) {
                        return binding.event == "touch" &&
                            scriptCallsPlayerCommand(binding.command, "finalexitloadmap");
                    });
                if (stageBinding) stageExitTriggers.emplace_back(trigger.entity, &trigger);
            }
            if (exitTrigger < 0 || !triggerBounds)
                throw std::runtime_error("Rescue check could not find an authored `touch: tell player escape` trigger");
            const V center = (triggerBounds->boundsMin + triggerBounds->boundsMax) * .5f;
            syncVictimsToRescueState();
            for (size_t index = 0; index < victims.size(); ++index) {
                auto& victim = victims[index];
                auto actor = rescueState.actors.find(victim.spawn.entity);
                if (actor == rescueState.actors.end())
                    throw std::runtime_error("Rescue check could not resolve victim actor " +
                                             std::to_string(victim.spawn.entity));
                player.p = actor->second.frame.position;
                const VictimPickupBox playerBox = victimPickupBox(player, player.p);
                const VictimProximityResult publication = publishVictimProximity(
                    actor->second.state, 1, &actor->second, victimMessages,
                    [&]() -> std::optional<VictimPickupBox> { return playerBox; },
                    [&] { return victim.pickupRadius; },
                    [&] { return actor->second.frame.position; },
                    [&] { return playerRescueMessageRoute; },
                    [] { std::cerr << "Victim message mapper has no free records\n"; });
                VictimPickupMessageResult pickupResult = VictimPickupMessageResult::ignoredMessage;
                victimMessages.drain(
                    [&](uint32_t route, uint32_t generation) -> FireMessageQueue::Receiver {
                        if (rescueMessageRoutes.resolve(route, generation) != &rescueState.player)
                            return {};
                        return [&](uint32_t message, const void* payload) {
                            if (message == 0x7532)
                                pickupResult = rescueTransactions->receivePickupPayload(payload);
                        };
                    }, [](const FireMessageQueue::Delivery&) {});
                if (publication != VictimProximityResult::queued ||
                    pickupResult != VictimPickupMessageResult::accepted)
                    throw std::runtime_error("Mapped Player pickup message was not accepted for victim " +
                                             std::to_string(victim.spawn.entity));
                syncRescueStateToVictims();
                if (index == 0) {
                    // Exercise the newly wired timed recovery adapter once in
                    // this controlled check: the safe snapshot is an explicit
                    // test input, and the elapsed value crosses 1000 ms.
                    player.safePosition3400 = player.p;
                    const uint32_t passoutNow = uint32_t(SDL_GetTicks());
                    playerFireState.state = 6;
                    playerFireState.failedAt = passoutNow - 1001u;
                    playerIncapacitated = true;
                    if (playerPassoutRecoveryDue(playerFireState, passoutNow - 1u) ||
                        !playerPassoutRecoveryDue(playerFireState, passoutNow))
                        throw std::runtime_error("Passout recovery deadline did not use strict >1000 ms");
                    if (rescueTransactions->passout(0) != VictimPassoutResult::dropped)
                        throw std::runtime_error("Timed passout adapter did not drop the carried victim");
                    syncRescueStateToVictims();
                    if (carriedVictim.carriedVictimEntity || playerIncapacitated ||
                        playerFireState.state != 3 ||
                        actor->second.state != 4 || actor->second.worldActorFlags != 0x400u)
                        throw std::runtime_error("Passout adapter did not restore Player and victim state");
                    player.p = actor->second.frame.position;
                    const VictimPickupBox retryPlayerBox = victimPickupBox(player, player.p);
                    const VictimProximityResult retryPublication = publishVictimProximity(
                        actor->second.state, 1, &actor->second, victimMessages,
                        [&]() -> std::optional<VictimPickupBox> { return retryPlayerBox; },
                        [&] { return victim.pickupRadius; },
                        [&] { return actor->second.frame.position; },
                        [&] { return playerRescueMessageRoute; },
                        [] { std::cerr << "Victim message mapper has no free records\n"; });
                    VictimPickupMessageResult retryResult = VictimPickupMessageResult::ignoredMessage;
                    victimMessages.drain(
                        [&](uint32_t route, uint32_t generation) -> FireMessageQueue::Receiver {
                            if (rescueMessageRoutes.resolve(route, generation) != &rescueState.player)
                                return {};
                            return [&](uint32_t message, const void* payload) {
                                if (message == 0x7532)
                                    retryResult = rescueTransactions->receivePickupPayload(payload);
                            };
                        }, [](const FireMessageQueue::Delivery&) {});
                    if (retryPublication != VictimProximityResult::queued ||
                        retryResult != VictimPickupMessageResult::accepted)
                        throw std::runtime_error("Dropped victim could not be picked up again");
                    syncRescueStateToVictims();
                }
                player.p = center;
                if (!BModelContact::overlapsExpanded(triggerBounds->boundsMin, triggerBounds->boundsMax,
                        center + V{-30.f, -30.f, -30.f}, center + V{30.f, 85.f, 30.f},
                        triggerBounds->contactReach))
                    throw std::runtime_error("Rescue check player box does not overlap the selected exit bounds");
                scriptRuntime->dispatch(exitTrigger, "touch");
                const uint32_t expectedRescues = uint32_t(index + 1);
                if (carriedVictim.carriedVictimEntity || rescueState.exits.rescues != expectedRescues ||
                    actor->second.state != 6 || actor->second.worldActorFlags != 0x400u)
                    throw std::runtime_error("Authored exit touch did not release and count victim " +
                                             std::to_string(victim.spawn.entity));
                if (scene.key() == "mbase" && index + 1 < victims.size() &&
                    rescueState.exits.messageGate != 0)
                    throw std::runtime_error("MBase exit gate closed before every victim was rescued");
                if (index == 0 && index + 1 < victims.size()) {
                    const uint32_t remaining = uint32_t(victims.size()) - expectedRescues;
                    const std::string expectedFinalExitHint = remaining == 1
                        ? "There is still one survivor remaining."
                        : "There are still " + std::to_string(remaining) + " survivors remaining.";
                    for (const auto& [finalExitEntity, finalExitBounds] : finalExitTriggers) {
                        const V finalExitCenter =
                            (finalExitBounds->boundsMin + finalExitBounds->boundsMax) * .5f;
                        player.p = finalExitCenter;
                        if (!BModelContact::overlapsExpanded(
                                finalExitBounds->boundsMin, finalExitBounds->boundsMax,
                                finalExitCenter + V{-30.f, -30.f, -30.f},
                                finalExitCenter + V{30.f, 85.f, 30.f},
                                finalExitBounds->contactReach))
                            throw std::runtime_error("Final-exit fixture does not overlap an authored trigger");
                        scriptRuntime->dispatch(finalExitEntity, "touch");
                        if (activeHint != expectedFinalExitHint ||
                            std::abs(activeHintRemaining - 7.f) > 0.001f ||
                            rescueState.exits.messageGate != 0)
                            throw std::runtime_error("Incomplete final exit did not show the seven-second rescue-count hint");
                    }
                    for (const auto& [stageEntity, stageBounds] : stageExitTriggers) {
                        const V stageCenter = (stageBounds->boundsMin + stageBounds->boundsMax) * .5f;
                        player.p = stageCenter;
                        if (!BModelContact::overlapsExpanded(
                                stageBounds->boundsMin, stageBounds->boundsMax,
                                stageCenter + V{-30.f, -30.f, -30.f},
                                stageCenter + V{30.f, 85.f, 30.f}, stageBounds->contactReach))
                            throw std::runtime_error("Stage-gate fixture does not overlap an authored trigger");
                        scriptRuntime->dispatch(stageEntity, "touch");
                        if (!rescueStageRequest.empty() || activeHint != expectedFinalExitHint ||
                            std::abs(activeHintRemaining - 7.f) > 0.001f)
                            throw std::runtime_error("Stage exit bypassed the remaining-victim gate");
                    }
                }
            }
            const bool allReleased = std::all_of(victims.begin(), victims.end(), [&](const PreviewVictim& victim) {
                auto actor = rescueState.actors.find(victim.spawn.entity);
                return actor != rescueState.actors.end() && actor->second.state == 6 &&
                       actor->second.worldActorFlags == 0x400u;
            });
            if (!allReleased || carriedVictim.carriedVictimEntity ||
                rescueState.exits.rescues != victims.size() ||
                rescueState.exits.total != victims.size())
                throw std::runtime_error("Rescue check did not release every authored victim");
            if (scene.key() == "mbase" &&
                (rescueState.exits.messageGate != 1 || !enabledExitTriggers.empty()))
                throw std::runtime_error("MBase did not reach its no-final-exit gate after all rescues");
            for (const auto& [finalExitEntity, finalExitBounds] : finalExitTriggers) {
                const V finalExitCenter =
                    (finalExitBounds->boundsMin + finalExitBounds->boundsMax) * .5f;
                player.p = finalExitCenter;
                if (!BModelContact::overlapsExpanded(
                        finalExitBounds->boundsMin, finalExitBounds->boundsMax,
                        finalExitCenter + V{-30.f, -30.f, -30.f},
                        finalExitCenter + V{30.f, 85.f, 30.f}, finalExitBounds->contactReach))
                    throw std::runtime_error("Final-exit fixture does not overlap an authored trigger");
                int firstOwnedSlot = -1;
                int alternateOwnedSlot = -1;
                for (int slot = 0; slot < playerLoadout.items.count && slot < 5; ++slot) {
                    if (!playerLoadout.items.slots[size_t(slot)].present) continue;
                    if (firstOwnedSlot < 0) firstOwnedSlot = slot;
                    else if (slot != firstOwnedSlot) alternateOwnedSlot = slot;
                }
                if (firstOwnedSlot < 0 || alternateOwnedSlot < 0)
                    throw std::runtime_error("Final-exit check needs two owned items to verify inventory refresh");
                if (!playerLoadout.select(PreviewPlayerLoadout::nameForSlot(alternateOwnedSlot)))
                    throw std::runtime_error("Final-exit check could not select its alternate owned item");
                scriptRuntime->dispatch(finalExitEntity, "touch");
                if (rescueState.exits.messageGate != 1 ||
                    playerLoadout.items.selected != firstOwnedSlot)
                    throw std::runtime_error("Successful final exit did not open the gate and reselect the first owned item");
            }
            for (const auto& [stageEntity, stageBounds] : stageExitTriggers) {
                const V stageCenter = (stageBounds->boundsMin + stageBounds->boundsMax) * .5f;
                player.p = stageCenter;
                if (!BModelContact::overlapsExpanded(
                        stageBounds->boundsMin, stageBounds->boundsMax,
                        stageCenter + V{-30.f, -30.f, -30.f},
                        stageCenter + V{30.f, 85.f, 30.f}, stageBounds->contactReach))
                    throw std::runtime_error("Stage-completion fixture does not overlap its authored trigger");
                rescueStageRequest.clear();
                scriptRuntime->dispatch(stageEntity, "touch");
                if (rescueStageRequest.empty())
                    throw std::runtime_error("All-rescues stage command did not queue a linked package");
                const auto nextPackage = package.parent_path() / rescueStageRequest;
                SceneData nextScene = SceneData::load(nextPackage);
                if (nextScene.key() != rescueStageRequest)
                    throw std::runtime_error("Linked stage package key does not match its authored request");
                std::cout << "Stage completion check: " << scene.key() << " trigger "
                          << stageEntity << " -> " << nextScene.key() << " after all "
                          << rescueState.exits.rescues
                          << " rescues; linked package loaded (touch is controlled).\n";
            }
            std::cout << "Rescue check: " << victims.size()
                      << " authored pickup/release transactions passed through trigger " << exitTrigger
                      << "; rescued=" << rescueState.exits.rescues << "/" << rescueState.exits.total
                      << "; one timed passout drop/reset/re-pickup passed";
            if (scene.key() == "mbase")
                std::cout << "; no-final-exit gate=1 (outro semantics unverified)";
            if (!finalExitTriggers.empty())
                std::cout << "; incomplete final-exit hints and successful inventory refresh passed";
            std::cout << ". This is a synthetic transaction check, not natural travel or retail parity.\n";
            return 0;
        }
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
            updateBoundsTriggerModelFlags(SDL_GetTicks());
            if (!equipmentOrientationCheck.empty()) {
                const std::string stem = equipmentOrientationCheck == "extinguisher"
                    ? "ext" : equipmentOrientationCheck;
                auto asset = equipmentAssets.find(stem);
                if (asset == equipmentAssets.end() || !asset->second->renderer)
                    throw std::runtime_error("Equipment orientation check could not load the selected ACT renderer");
                const V cameraPosition{spawn.x, spawn.y + 70.f, spawn.z};
                FrameTransform cameraFrame;
                const float cosine = std::cos(yaw), sine = std::sin(yaw);
                cameraFrame.position = cameraPosition;
                cameraFrame.axis = {V{cosine, 0.f, -sine}, V{0.f, 1.f, 0.f}, V{sine, 0.f, cosine}};
                const bool axeFamily = stem == "axe" || stem == "axel";
                const V itemPosition = axeFamily
                    ? cameraRelativePoint(cameraFrame, 35.f, -8.f, 140.f)
                    : cameraRelativePoint(cameraFrame, 42.f, -5.f, 145.f);
                auto captureTool = [&](bool halfTurn, float motionTime) {
                    renderScene(meshes, scene, collision, inspectionPoses, worldModelFlags,
                                fogState, lightStyles,
                                cameraPosition, yaw, 0.f, width, height);
                    drawPreviewPropModels(propModels, propModelAssets);
                    drawPreviewFires(fires, fireRenderer, SDL_GetTicks());
                    drawPreviewVictims(victims, victimAssets, SDL_GetTicks());
                    glClear(GL_DEPTH_BUFFER_BIT);
                    ActorTransform root = halfTurn
                        ? halfTurnedToolRootFromCameraFrame(cameraFrame)
                        : rawToolRootFromCameraFrame(cameraFrame);
                    root.t = itemPosition;
                    if (stem == "ext") scaleActorRoot(root, .25f);
                    const auto& actor = *asset->second;
                    const int triangles = actor.renderer->draw(root,
                        axeFamily ? actor.swingMotion : -1, motionTime);
                    glFinish();
#if defined(__APPLE__)
                    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
#else
                    glReadBuffer(GL_BACK);
#endif
                    glPixelStorei(GL_PACK_ALIGNMENT, 1);
                    std::vector<uint8_t> pixels(size_t(width) * size_t(height) * 4);
                    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
                    return std::pair<std::vector<uint8_t>, int>{std::move(pixels), triangles};
                };
                const auto raw = captureTool(false, 0.f);
                const auto halfTurn = captureTool(true, 0.f);
                const auto rawPath = saveRenderCheckFrame(package, raw.first, width, height,
                                                           "." + stem + "-camera-root.bmp");
                const auto halfPath = saveRenderCheckFrame(package, halfTurn.first, width, height,
                                                            "." + stem + "-half-turn.bmp");
                std::pair<std::vector<uint8_t>, int> swingFrame;
                std::filesystem::path swingPath;
                size_t swingChangedPixels = 0;
                if (axeFamily) {
                    // The retail hit gate is crossed at 0.275 seconds. Sample
                    // the exported ACT just beyond that point and compare it
                    // against bind time to ensure the swing is visibly posed.
                    swingFrame = captureTool(false, 0.30f);
                    swingPath = saveRenderCheckFrame(package, swingFrame.first, width, height,
                                                       "." + stem + "-swing-300ms.bmp");
                    for (size_t i = 0; i + 3 < raw.first.size(); i += 4) {
                        if (raw.first[i] != swingFrame.first[i] ||
                            raw.first[i + 1] != swingFrame.first[i + 1] ||
                            raw.first[i + 2] != swingFrame.first[i + 2] ||
                            raw.first[i + 3] != swingFrame.first[i + 3])
                            ++swingChangedPixels;
                    }
                }
                size_t changedPixels = 0;
                for (size_t i = 0; i + 3 < raw.first.size(); i += 4) {
                    if (raw.first[i] != halfTurn.first[i] || raw.first[i + 1] != halfTurn.first[i + 1] ||
                        raw.first[i + 2] != halfTurn.first[i + 2] || raw.first[i + 3] != halfTurn.first[i + 3])
                        ++changedPixels;
                }
                const GLenum glError = glGetError();
                std::cout << "Equipment orientation check: " << stem << " ACT, " << raw.second
                          << " triangles; camera-root and half-turn frames differ at " << changedPixels
                          << " pixels; saved " << rawPath.string() << " and " << halfPath.string();
                if (axeFamily)
                    std::cout << "; 300 ms swing differs from bind frame at " << swingChangedPixels
                              << " pixels; saved " << swingPath.string();
#if defined(__APPLE__)
                std::cout << "; OpenGL " << (headlessContext.version() ? headlessContext.version() : "unknown");
#endif
                if (glError != GL_NO_ERROR) std::cout << "; OpenGL error " << glError;
                std::cout << "\n";
                clearTextures(meshes);
                for (auto& victim : victimAssets) victim.second->renderer->release();
                for (auto& equipment : equipmentAssets)
                    if (equipment.second->renderer) equipment.second->renderer->release();
                for (auto& prop : propModelAssets)
                    if (prop.second->renderer) prop.second->renderer->release();
                fireRenderer.release();
                extinguisherSprayRenderer.release();
                if (playerHoseStreamRendererLoaded) playerHoseStreamRenderer.release();
                fireDirectionRenderer.release();
                fireRendererLoaded = false;
                extinguisherSprayRendererLoaded = false;
                playerHoseStreamRendererLoaded = false;
                fireDirectionRendererLoaded = false;
#if defined(__APPLE__)
                headlessContext.destroy();
#endif
                SDL_GL_DeleteContext(context);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return glError == GL_NO_ERROR && changedPixels > 0 &&
                    (!axeFamily || swingChangedPixels > 0) ? 0 : 1;
            }
            if (triggerRenderCheckEntity >= 0) {
                auto target = std::find_if(boundsTriggers.begin(), boundsTriggers.end(),
                    [&](const PreviewBoundsTrigger& trigger) {
                        return trigger.entity == triggerRenderCheckEntity;
                    });
                if (target == boundsTriggers.end())
                    throw std::runtime_error("Trigger render check cannot find authored BoundsTrigger entity " +
                                             std::to_string(triggerRenderCheckEntity));

                V camera = (target->boundsMin + target->boundsMax) * .5f;
                const V playerEye{spawn.x, spawn.y + 70.f, spawn.z};
                if (playerEye.x >= target->boundsMin.x && playerEye.x <= target->boundsMax.x &&
                    playerEye.y >= target->boundsMin.y && playerEye.y <= target->boundsMax.y &&
                    playerEye.z >= target->boundsMin.z && playerEye.z <= target->boundsMax.z)
                    camera = playerEye;

                auto captureTriggerFrame = [&](uint32_t flags) {
                    worldModelFlags.flags[target->modelIndex] = flags;
                    renderScene(meshes, scene, collision, inspectionPoses, worldModelFlags,
                                fogState, lightStyles,
                                camera, yaw, 0.f, width, height);
                    drawPreviewPropModels(propModels, propModelAssets);
                    drawPreviewFires(fires, fireRenderer, SDL_GetTicks());
                    drawPreviewVictims(victims, victimAssets, SDL_GetTicks());
                    glFinish();
#if defined(__APPLE__)
                    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
#else
                    glReadBuffer(GL_BACK);
#endif
                    glPixelStorei(GL_PACK_ALIGNMENT, 1);
                    std::vector<uint8_t> pixels(size_t(width) * size_t(height) * 4);
                    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
                    return pixels;
                };

                const auto visibleFrame = captureTriggerFrame(1u);
                const auto visiblePath = saveRenderCheckFrame(package, visibleFrame, width, height,
                                                               ".trigger-visible.bmp");
                const auto hiddenFrame = captureTriggerFrame(0u);
                const auto hiddenPath = saveRenderCheckFrame(package, hiddenFrame, width, height,
                                                              ".trigger-hidden.bmp");
                size_t changedPixels = 0;
                for (size_t i = 0; i + 3 < visibleFrame.size(); i += 4) {
                    if (visibleFrame[i] != hiddenFrame[i] || visibleFrame[i + 1] != hiddenFrame[i + 1] ||
                        visibleFrame[i + 2] != hiddenFrame[i + 2] || visibleFrame[i + 3] != hiddenFrame[i + 3])
                        ++changedPixels;
                }
                const GLenum glError = glGetError();
                std::cout << "Trigger render check: entity " << target->entity << " / model "
                          << target->modelIndex << "; forced-visible versus ModelSetFlags(0) frame changed "
                          << changedPixels << " pixels; frames saved to " << visiblePath.string()
                          << " and " << hiddenPath.string();
#if defined(__APPLE__)
                std::cout << "; OpenGL " << (headlessContext.version() ? headlessContext.version() : "unknown");
#endif
                if (glError != GL_NO_ERROR) std::cout << "; OpenGL error " << glError;
                std::cout << "\n";
                clearTextures(meshes);
                for (auto& asset : victimAssets) asset.second->renderer->release();
                for (auto& prop : propModelAssets)
                    if (prop.second->renderer) prop.second->renderer->release();
                fireRenderer.release();
                extinguisherSprayRenderer.release();
                if (playerHoseStreamRendererLoaded) playerHoseStreamRenderer.release();
                fireDirectionRenderer.release();
                fireRendererLoaded = false;
                extinguisherSprayRendererLoaded = false;
                playerHoseStreamRendererLoaded = false;
                fireDirectionRendererLoaded = false;
#if defined(__APPLE__)
                headlessContext.destroy();
#endif
                SDL_GL_DeleteContext(context);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return glError == GL_NO_ERROR && changedPixels > 0 ? 0 : 1;
            }
            renderScene(meshes, scene, collision, inspectionPoses, worldModelFlags,
                        fogState, lightStyles,
                        {spawn.x, spawn.y + 70.f, spawn.z}, yaw, 0.f, width, height);
            drawPreviewPropModels(propModels, propModelAssets);
            const int fireQuads = drawPreviewFires(fires, fireRenderer, SDL_GetTicks());
            drawPreviewVictims(victims, victimAssets, SDL_GetTicks());
            auto readFramebuffer = [&]() {
                glFinish();
#if defined(__APPLE__)
                glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
#else
                glReadBuffer(GL_BACK);
#endif
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                std::vector<uint8_t> pixels(size_t(width) * size_t(height) * 4);
                glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
                return pixels;
            };
            const std::vector<uint8_t> frameBeforeOxygen =
                oxygenHudRenderCheck ? readFramebuffer() : std::vector<uint8_t>{};
            glDisable(GL_FOG);
            int oxygenVisorEffectLayers = 0;
            size_t oxygenVisorEffectChangedPixels = 0;
            std::vector<uint8_t> frameBeforeOxygenHud;
            if (oxygenHudRenderCheck) {
                const float oxygenWarningAtThreshold =
                    playerOxygenWarningOverlayAmount(0.25f);
                const float oxygenWarningAtHalfThreshold =
                    playerOxygenWarningOverlayAmount(0.125f);
                const float oxygenWarningAtZero =
                    playerOxygenWarningOverlayAmount(0.f);
                if (oxygenWarningAtThreshold != 0.f ||
                    std::abs(oxygenWarningAtHalfThreshold - 0.05f) > 0.000001f ||
                    std::abs(oxygenWarningAtZero - 0.1f) > 0.000001f)
                    throw std::runtime_error("Oxygen warning layer amount did not match retail thresholds");
                PlayerOxygenVisorEffect opening;
                startPlayerOxygenVisorEffect(opening, 1000u, true);
                int32_t openingY = 0;
                if (!samplePlayerOxygenVisorEffect(opening, 1495u, openingY) || openingY != 240 ||
                    samplePlayerOxygenVisorEffect(opening, opening.startAt, openingY) ||
                    samplePlayerOxygenVisorEffect(opening, opening.endAt, openingY) ||
                    completePlayerOxygenVisorEffect(opening, opening.endAt) !=
                        PlayerOxygenVisorEffectCompletion::oxygenOn)
                    throw std::runtime_error("Oxygen-on visor animation timing did not match retail bounds");
                PlayerOxygenVisorEffect closing;
                startPlayerOxygenVisorEffect(closing, 2000u, false);
                int32_t closingY = 0;
                if (!samplePlayerOxygenVisorEffect(
                        closing, closing.startAt + (closing.endAt - closing.startAt) / 2u, closingY) ||
                    closingY != 240 || completePlayerOxygenVisorEffect(closing, closing.endAt) !=
                        PlayerOxygenVisorEffectCompletion::oxygenOff)
                    throw std::runtime_error("Oxygen-off visor animation timing did not match retail bounds");
                PlayerOxygenVisorEffect renderSample;
                startPlayerOxygenVisorEffect(renderSample, 1000u, true);
                oxygenVisorEffectLayers = oxygenHudRenderer.drawVisorEffect(
                    renderSample, 1495u, width, height);
                frameBeforeOxygenHud = readFramebuffer();
            }
            const int oxygenHudLayers = oxygenHudRenderCheck
                ? oxygenHudRenderer.draw(
                    0.125f, playerOxygenWarningOverlayAmount(0.125f), width, height) : 0;
            const std::vector<uint8_t> frameBeforeVictimPickupHud =
                victimPickupHudRenderCheck ? readFramebuffer() : std::vector<uint8_t>{};
            int victimPickupHudLayers = 0;
            size_t victimPickupHudChangedPixels = 0;
            if (victimPickupHudRenderCheck) {
                const auto start = VictimPickupHudRenderer::sample(0.f);
                const auto midpoint = VictimPickupHudRenderer::sample(0.05f);
                const auto expanded = VictimPickupHudRenderer::sample(0.1f);
                const float halfwaySampleTime = float(uint32_t(1050)) * 0.001f;
                const auto halfwayToCorner = VictimPickupHudRenderer::sample(halfwaySampleTime);
                const auto settled = VictimPickupHudRenderer::sample(1.1f);
                const auto afterEnd = VictimPickupHudRenderer::sample(1.2f);
                if (start.left != 320 || start.right != 320 || start.top != 240 || start.bottom != 240 ||
                    midpoint.left != 288 || midpoint.right != 352 || midpoint.top != 208 || midpoint.bottom != 272 ||
                    expanded.left != 256 || expanded.right != 384 || expanded.top != 176 || expanded.bottom != 304 ||
                    halfwayToCorner.left != 379 || halfwayToCorner.right != 475 ||
                    halfwayToCorner.top != 90 || halfwayToCorner.bottom != 186 ||
                    settled.left != 502 || settled.right != 566 || settled.top != 5 || settled.bottom != 69 ||
                    afterEnd.left != settled.left || afterEnd.right != settled.right ||
                    afterEnd.top != settled.top || afterEnd.bottom != settled.bottom)
                    throw std::runtime_error("Victim pickup HUD keyframe interpolation did not match the retail track");
                victimPickupHudRenderer.begin(0);
                if (victimPickupHudRenderer.drawAtSeconds(0.f, width, height) != 0)
                    throw std::runtime_error("Victim pickup HUD should begin at its zero-sized center keyframe");
                victimPickupHudLayers = victimPickupHudRenderer.drawAtSeconds(
                    halfwaySampleTime, width, height);
                const auto frame = readFramebuffer();
                for (size_t i = 0; i + 3 < frame.size(); i += 4) {
                    if (frame[i] != frameBeforeVictimPickupHud[i] ||
                        frame[i + 1] != frameBeforeVictimPickupHud[i + 1] ||
                        frame[i + 2] != frameBeforeVictimPickupHud[i + 2] ||
                        frame[i + 3] != frameBeforeVictimPickupHud[i + 3])
                        ++victimPickupHudChangedPixels;
                }
                victimPickupHudRenderer.hide();
                if (victimPickupHudRenderer.drawAtSeconds(1.1f, width, height) != 0)
                    throw std::runtime_error("Victim pickup HUD remained visible after status action 2");
            }
            std::vector<uint8_t> frameWithoutHint;
            if (hintRenderCheck) frameWithoutHint = readFramebuffer();
            int hintGlyphs = 0;
            if (fireDirectionRenderCheck) {
                glDisable(GL_FOG);
                const auto before = readFramebuffer();
                const std::array<uint8_t, 4> allSectors{{1, 1, 1, 1}};
                const int iconsDrawn = fireDirectionRenderer.draw(allSectors, width, height);
                const auto frame = readFramebuffer();
                size_t changedPixels = 0;
                for (size_t i = 0; i + 3 < frame.size(); i += 4) {
                    if (frame[i] != before[i] || frame[i + 1] != before[i + 1] ||
                        frame[i + 2] != before[i + 2] || frame[i + 3] != before[i + 3])
                        ++changedPixels;
                }
                const auto imagePath = saveRenderCheckFrame(
                    package, frame, width, height, ".fire-direction-render-check.bmp");
                const GLenum glError = glGetError();
                std::cout << "Fire direction render check: " << iconsDrawn
                          << "/4 source-mapped HUD sectors drawn; overlay changed "
                          << changedPixels << " pixels; frame saved to " << imagePath.string();
#if defined(__APPLE__)
                std::cout << "; OpenGL " << (headlessContext.version() ? headlessContext.version() : "unknown");
#endif
                if (glError != GL_NO_ERROR) std::cout << "; OpenGL error " << glError;
                std::cout << "\n";
                clearTextures(meshes);
                for (auto& asset : victimAssets) asset.second->renderer->release();
                for (auto& asset : equipmentAssets)
                    if (asset.second->renderer) asset.second->renderer->release();
                for (auto& prop : propModelAssets)
                    if (prop.second->renderer) prop.second->renderer->release();
                fireRenderer.release();
                extinguisherSprayRenderer.release();
                if (playerHoseStreamRendererLoaded) playerHoseStreamRenderer.release();
                fireDirectionRenderer.release();
                gogglesRenderer.release();
                hintOverlay.release();
                fireRendererLoaded = false;
                extinguisherSprayRendererLoaded = false;
                playerHoseStreamRendererLoaded = false;
                fireDirectionRendererLoaded = false;
#if defined(__APPLE__)
                headlessContext.destroy();
#endif
                SDL_GL_DeleteContext(context);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return glError == GL_NO_ERROR && iconsDrawn == 4 && changedPixels > 0 ? 0 : 1;
            }
            if (hintRenderCheck) {
                glDisable(GL_FOG);
                RetailHintLifetime timingCheck;
                timingCheck.begin(100, 1.f);
                if (timingCheck.visible(100) || !timingCheck.visible(101) ||
                    !timingCheck.visible(1099) || timingCheck.visible(1100))
                    throw std::runtime_error("Retail hint visibility did not use strict start/end bounds");
                const auto markupSample = HintOverlay::parse("^(228,0,0)TOO HOT!\nCOOL OFF FAST!");
                if (markupSample.size() != 23 || markupSample[0].value != 'T' ||
                    markupSample[0].color != std::array<uint8_t, 3>{{228, 0, 0}} ||
                    markupSample[8].value != '\n' || markupSample[9].value != 'C')
                    throw std::runtime_error("Retail hint color markup or newline parsing failed");
                hintGlyphs = hintOverlay.draw("^(228,0,0)TOO HOT!\nCOOL OFF FAST!", width, height);
            }
            auto frame = readFramebuffer();
            GLenum glError = glGetError();
            size_t hintChangedPixels = 0;
            size_t oxygenHudChangedPixels = 0;
            if (oxygenHudRenderCheck) {
                for (size_t i = 0; i + 3 < frameBeforeOxygenHud.size(); i += 4) {
                    if (frameBeforeOxygenHud[i] != frameBeforeOxygen[i] ||
                        frameBeforeOxygenHud[i + 1] != frameBeforeOxygen[i + 1] ||
                        frameBeforeOxygenHud[i + 2] != frameBeforeOxygen[i + 2] ||
                        frameBeforeOxygenHud[i + 3] != frameBeforeOxygen[i + 3])
                        ++oxygenVisorEffectChangedPixels;
                }
            }
            if (oxygenHudRenderCheck) {
                for (size_t i = 0; i + 3 < frame.size(); i += 4) {
                    if (frame[i] != frameBeforeOxygenHud[i] ||
                        frame[i + 1] != frameBeforeOxygenHud[i + 1] ||
                        frame[i + 2] != frameBeforeOxygenHud[i + 2] ||
                        frame[i + 3] != frameBeforeOxygenHud[i + 3])
                        ++oxygenHudChangedPixels;
                }
            }
            if (hintRenderCheck) {
                for (size_t i = 0; i + 3 < frame.size(); i += 4) {
                    if (frame[i] != frameWithoutHint[i] || frame[i + 1] != frameWithoutHint[i + 1] ||
                        frame[i + 2] != frameWithoutHint[i + 2] || frame[i + 3] != frameWithoutHint[i + 3])
                        ++hintChangedPixels;
                }
            }
            std::set<uint32_t> colors;
            size_t nonBackgroundPixels = 0;
            for (size_t i = 0; i + 3 < frame.size(); i += 4) {
                uint32_t rgba = (uint32_t(frame[i]) << 24) | (uint32_t(frame[i + 1]) << 16) |
                                (uint32_t(frame[i + 2]) << 8) | uint32_t(frame[i + 3]);
                colors.insert(rgba);
                if (rgba != 0x141a1fff) ++nonBackgroundPixels;
            }
            auto imagePath = saveRenderCheckFrame(package, frame, width, height,
                victimPickupHudRenderCheck ? ".victim-pickup-hud-render-check.bmp" :
                oxygenHudRenderCheck ? ".oxygen-hud-render-check.bmp" :
                hintRenderCheck ? ".hint-render-check.bmp" : ".render-check.bmp");
            std::cout << "Render check: " << textureSummary.uploadedFiles << "/"
                      << textureSummary.uniqueFiles << " textures uploaded; "
                      << colors.size() << " distinct framebuffer colors; " << nonBackgroundPixels
                      << " non-background pixels; FireRescue=" << fires.size() << " ("
                      << fireQuads << " sprite quads)";
            if (hintRenderCheck)
                std::cout << "; hint overlay drew " << hintGlyphs << " atlas glyphs and changed "
                          << hintChangedPixels << " pixels";
            if (oxygenHudRenderCheck)
                std::cout << "; oxygen visor effect drew " << oxygenVisorEffectLayers
                          << " layer and changed " << oxygenVisorEffectChangedPixels
                          << " pixels; oxygen HUD drew " << oxygenHudLayers
                          << " layers and changed " << oxygenHudChangedPixels << " pixels";
            if (victimPickupHudRenderCheck)
                std::cout << "; victim pickup HUD drew " << victimPickupHudLayers
                          << " layer at t=1.05s and changed " << victimPickupHudChangedPixels
                          << " pixels; source keyframes and hide action checked";
            std::cout << "; frame saved to " << imagePath.string();
#if defined(__APPLE__)
            std::cout << "; OpenGL " << (headlessContext.version() ? headlessContext.version() : "unknown");
#endif
            if (glError != GL_NO_ERROR) std::cout << "; OpenGL error " << glError;
            std::cout << "\n";
            clearTextures(meshes);
            for (auto& asset : victimAssets) asset.second->renderer->release();
            for (auto& prop : propModelAssets)
                if (prop.second->renderer) prop.second->renderer->release();
            fireRenderer.release();
            extinguisherSprayRenderer.release();
            if (playerHoseStreamRendererLoaded) playerHoseStreamRenderer.release();
            fireDirectionRenderer.release();
            if (oxygenHudRendererLoaded) oxygenHudRenderer.release();
            if (victimPickupHudRendererLoaded) victimPickupHudRenderer.release();
            hintOverlay.release();
            fireRendererLoaded = false;
            extinguisherSprayRendererLoaded = false;
            playerHoseStreamRendererLoaded = false;
            fireDirectionRendererLoaded = false;
            oxygenHudRendererLoaded = false;
#if defined(__APPLE__)
            headlessContext.destroy();
#endif
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return glError == GL_NO_ERROR && textureSummary.failedFiles == 0 &&
                   textureSummary.uploadedFiles == textureSummary.uniqueFiles &&
                   (!hintRenderCheck || (hintGlyphs > 0 && hintChangedPixels > 0)) &&
                   (!oxygenHudRenderCheck ||
                    (oxygenVisorEffectLayers == 1 && oxygenVisorEffectChangedPixels > 0 &&
                     oxygenHudLayers == 3 && oxygenHudChangedPixels > 0)) &&
                   (!victimPickupHudRenderCheck ||
                    (victimPickupHudLayers == 1 && victimPickupHudChangedPixels > 0)) ? 0 : 1;
        }

        oxygenHudRenderer.load(locateOxygenHudSprites(scene));
        oxygenHudRendererLoaded = true;
        victimPickupHudRenderer.load(locateVictimPickupHudSprites(scene));
        victimPickupHudRendererLoaded = true;
        bonusPointsHudRenderer.load(locateBonusPointsHudSprites(scene));
        bonusPointsHudRendererLoaded = true;
        bool running = true, captured = true, jumpEdge = false;
        bool useEdge = false, sprayHeld = false, sprayPressed = false, sprayReleased = false;
        float pitch = 0.f;
        Follower playerViewFollower;
        bool playerViewInitialized = false;
        bool previewMotionsPlaying = false;
        bool fireCameraAuditExposure = false;
        int fireCameraAuditEntity = -1;
        uint32_t fireCameraAuditNextSample = 0;
        uint32_t fireCameraAuditNextPoseSample = 0;
        uint32_t extinguisherAuditNextSample = 0;
        float fireCameraAuditYawDelta = 0.f;
        float fireCameraAuditPitchDelta = 0.f;
        uint32_t fireCameraAuditMouseEvents = 0;
        float fireCameraAuditSampleYaw = yaw;
        float fireCameraAuditSamplePitch = pitch;
        float previewMotionTime = bmodelMotionStart;
        int nearestUseEntity = -1;
        std::string previousEquippedItem = playerLoadout.selectedName();
        std::map<int, bool> bmodelContacts;
        std::map<int, bool> boundsTriggerContacts;
        std::map<int, bool> propModelContacts;
        std::map<int, float> lastLoggedBModelPathTimes;
        std::map<int, int> lastLoggedBModelStates;
        bool axeStrikePending = false;
        uint32_t axeWorldImpactCount = 0;
        std::string axeFeedback;
        float yawInput = yaw;
        uint32_t previous = SDL_GetTicks();
        uint32_t previousTitleUpdate = previous;
        using PerformanceClock = std::chrono::steady_clock;
        const auto performanceAverage = [](const std::vector<double>& samples) {
            if (samples.empty()) return 0.0;
            double total = 0.0;
            for (double sample : samples) total += sample;
            return total / double(samples.size());
        };
        const auto performanceP95 = [](std::vector<double> samples) {
            if (samples.empty()) return 0.0;
            std::sort(samples.begin(), samples.end());
            const size_t index = std::min(samples.size() - 1,
                size_t(std::ceil(double(samples.size()) * .95)) - 1);
            return samples[index];
        };
        auto performanceWindowStart = PerformanceClock::now();
        std::vector<double> performanceFrameMs, performanceUpdateMs,
                            performanceSceneMs, performanceOverlayMs,
                            performanceSwapMs;
        size_t performanceSubmittedGroups = 0, performanceCulledGroups = 0,
               performanceSubmittedTriangles = 0;
        if (performanceAudit) {
            performanceFrameMs.reserve(90);
            performanceUpdateMs.reserve(90);
            performanceSceneMs.reserve(90);
            performanceOverlayMs.reserve(90);
            performanceSwapMs.reserve(90);
        }
        SDL_SetRelativeMouseMode(SDL_TRUE);
        if (dedicatedPlayerHose) {
            std::cout << "PlayerHose: the authored camera rail follows the hose path. Move the mouse to steer the stream; left-click and hold to douse the selected WindowFire target. Escape releases the mouse.\n";
        } else {
            std::cout << "WASD move, mouse look, Space/right mouse jump, C crouch, Escape releases the mouse.\n"
                      << "Press F to snap to the nearest walkable collision floor below you.\n"
                      << "Rescue victims render from exported ACT assets; authored roam commands move them around the map.\n"
                          "Authored FireRescue sprites animate; left-click and hold with the extinguisher selected to suppress fires.\n"
                          "Victims are picked up automatically on contact. Authored BModel touch events open and close doors.\n"
                          "PlayerRescue authored loadouts initialize; press 1/2/3 to select equipped axe/extinguisher/goggles.\n"
                          "Extinguisher contacts use the recovered tool pose and fire falloff; left-click with goggles to scan nearby actors.\n"
                          "The extinguisher pose follows this viewer's camera transform; contact cadence is a host approximation.\n"
                          "Tool selection keys 1/2/3 are viewer controls.\n"
                          "Press E or Space near a scripted console to use it. M/N inspect PATH timing; live door poses stay in control.\n"
                          "Doors respond to authored touch events; PATH preview does not activate a door or change collision.\n"
                          "Gameplay BModel poses drive both rendering and collision; retail callback cadence remains unknown.\n";
        }
        if (fireCameraAudit)
            std::cout << "Fire camera audit is active: it logs mouse events, 250 ms view/movement samples, and extinguisher contact points and delivery while spraying.\n";
        if (performanceAudit)
            std::cout << "Performance audit is active: one-second CPU frame, world, overlay, swap, and visibility summaries will be printed.\n";
        while (running) {
            const auto performanceFrameStart = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                if (event.type == SDL_WINDOWEVENT &&
                    event.window.event == SDL_WINDOWEVENT_FOCUS_LOST && captured) {
                    // Preserve the user's capture preference, but release SDL's
                    // relative mode while the window is unfocused so stale
                    // motion cannot pass through the relative-look path.
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    if (fireCameraAudit)
                        std::cout << "fire-camera-audit focus=lost capture-wanted=yes relative=no\n";
                }
                if (event.type == SDL_WINDOWEVENT &&
                    event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED && captured) {
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    if (fireCameraAudit)
                        std::cout << "fire-camera-audit focus=gained capture-wanted=yes relative="
                                  << (SDL_GetRelativeMouseMode() ? "yes" : "no") << "\n";
                }
                if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        captured = !captured;
                        SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
                    }
                    if (event.key.keysym.sym == SDLK_SPACE) jumpEdge = true;
                    if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_e) useEdge = true;
                    if (!dedicatedPlayerHose && event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_4) {
                        const int slot = int(event.key.keysym.sym - SDLK_0);
                        const char* name = PreviewPlayerLoadout::nameForSlot(slot);
                        const std::string previousSelection = playerLoadout.selectedName();
                        const bool selected = playerLoadout.select(name);
                        if (fireCameraAudit)
                            std::cout << "fire-camera-audit selection t=" << SDL_GetTicks()
                                      << " requested=" << name
                                      << " result=" << (selected ? "selected" : "not-equipped")
                                      << " previous=" << previousSelection
                                      << " current=" << playerLoadout.selectedName()
                                      << "\n";
                        if (selected) {
                            noteGogglesSelection(previousSelection);
                            activeHint = std::string("Selected ") + name;
                        }
                        else activeHint = std::string("Not equipped: ") + name;
                        activeHintRemaining = 1.5f;
                    }
                    if (event.key.keysym.sym == SDLK_m && bmodelMotionEnd > 0.f) {
                        if (previewMotionTime >= bmodelMotionEnd) previewMotionTime = bmodelMotionStart;
                        previewMotionsPlaying = !previewMotionsPlaying;
                        std::cout << "BModel PATH inspection " << (previewMotionsPlaying ? "playing" : "paused")
                                  << " at retail path time " << previewMotionTime << "/" << bmodelMotionEnd
                                  << "; gameplay and collision poses are unchanged\n";
                    }
                    if (event.key.keysym.sym == SDLK_n) {
                        previewMotionTime = bmodelMotionStart;
                        previewMotionsPlaying = false;
                        inspectionPoses.clear();
                        std::cout << "BModel path inspection cleared; gameplay owner motion is unchanged\n";
                    }
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
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && !sprayHeld) {
                    sprayHeld = true;
                    sprayPressed = true;
                    if (fireCameraAudit) {
                        const Uint32 flags = SDL_GetWindowFlags(window);
                        std::cout << "fire-camera-audit action-input t=" << SDL_GetTicks()
                                  << " edge=down selected=" << playerLoadout.selectedName()
                                  << " focus=" << ((flags & SDL_WINDOW_INPUT_FOCUS) ? "yes" : "no")
                                  << " relative=" << (SDL_GetRelativeMouseMode() ? "yes" : "no")
                                  << "\n";
                    }
                }
                if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && sprayHeld) {
                    sprayHeld = false;
                    sprayReleased = true;
                    if (fireCameraAudit)
                        std::cout << "fire-camera-audit action-input t=" << SDL_GetTicks()
                                  << " edge=up selected=" << playerLoadout.selectedName()
                                  << "\n";
                }
                if (event.type == SDL_MOUSEMOTION) {
                    const Uint32 windowFlags = SDL_GetWindowFlags(window);
                    const bool hasFocus = (windowFlags & SDL_WINDOW_INPUT_FOCUS) != 0;
                    const bool relativeMode = SDL_GetRelativeMouseMode() == SDL_TRUE;
                    if (!captured || !relativeMode || !hasFocus) {
                        if (fireCameraAudit && captured && (event.motion.xrel || event.motion.yrel))
                            std::cout << "fire-camera-audit mouse-ignored t=" << SDL_GetTicks()
                                      << " captured=" << (captured ? "yes" : "no")
                                      << " relative=" << (relativeMode ? "yes" : "no")
                                      << " focus=" << (hasFocus ? "yes" : "no")
                                      << " xrel=" << event.motion.xrel
                                      << " yrel=" << event.motion.yrel << "\n";
                        continue;
                    }
                    // Retail's fire passout switches to Player state 6, whose
                    // update owns the timed view transition instead of normal
                    // mouse-look input. Do not let SDL motion keep steering
                    // yaw/pitch after that state change.
                    if (playerIncapacitated) {
                        if (fireCameraAudit && (event.motion.xrel || event.motion.yrel))
                            std::cout << "fire-camera-audit mouse-ignored t=" << SDL_GetTicks()
                                      << " reason=passout captured=yes relative=yes focus=yes"
                                      << " xrel=" << event.motion.xrel
                                      << " yrel=" << event.motion.yrel << "\n";
                        continue;
                    }
                    if (dedicatedPlayerHose) {
                        // PlayerHose reads relative MOUSE_X/Y through the
                        // STEER_YAW/PITCH actions. Consume summed deltas with
                        // the retail scale in the stage update.
                        playerHoseStage.queueSteeringMouseDelta(
                            event.motion.xrel, event.motion.yrel);
                    } else {
                        // Keep the existing native workbench camera mapping.
                        const float yawBefore = yawInput;
                        const float pitchBefore = pitch;
                        yawInput -= event.motion.xrel * .0025f;
                        yaw = yawInput;
                        pitch -= event.motion.yrel * (.1f * 0.01745329251994329577f);
                        // Fresh retail profiles default to DifficultyLevel 1,
                        // which selects the +/-40 degree pitch limit in 00452570.
                        constexpr float freshProfilePitchLimit =
                            40.f * 0.01745329251994329577f;
                        pitch = std::clamp(pitch, -freshProfilePitchLimit,
                                           freshProfilePitchLimit);
                        if (fireCameraAudit) {
                            fireCameraAuditYawDelta += yaw - yawBefore;
                            fireCameraAuditPitchDelta += pitch - pitchBefore;
                            ++fireCameraAuditMouseEvents;
                            std::cout << "fire-camera-audit mouse t=" << SDL_GetTicks()
                                      << " exposed=" << (fireCameraAuditExposure ? "yes" : "no")
                                      << " entity=" << fireCameraAuditEntity
                                      << " captured=" << (captured ? "yes" : "no")
                                      << " relative=" << (SDL_GetRelativeMouseMode() ? "yes" : "no")
                                      << " focus=" << ((windowFlags & SDL_WINDOW_INPUT_FOCUS) ? "yes" : "no")
                                      << " xrel=" << event.motion.xrel
                                      << " yrel=" << event.motion.yrel
                                      << " yaw=" << yawBefore << "->" << yaw
                                      << " pitch=" << pitchBefore << "->" << pitch
                                      << " player=" << player.p.x << "," << player.p.y << "," << player.p.z
                                      << "\n";
                        }
                    }
                }
            }
            uint32_t now = SDL_GetTicks();
            // Retail 0042bf70 converts elapsed milliseconds with 0.001,
            // waits in 1 ms steps until dt > 1/60 second, then caps dt at
            // 0.1 second. Keep the same gameplay pacing with SDL's shared tick.
            float dt = float(double(now - previous) * 0.001);
            while (double(dt) <= (1.0 / 60.0)) {
                SDL_Delay(1);
                now = SDL_GetTicks();
                dt = float(double(now - previous) * 0.001);
            }
            previous = now;
            if (double(dt) > 0.1) dt = 0.1f;
            extinguisherSprayRenderer.prune(now);
            if (scriptRuntime) scriptRuntime->timeMs = now;
            fogState.update(now);
            advanceWorldStartup(now);
            bool passoutRecoveredThisFrame = false;
            if (!playerIncapacitated) {
                PreviewPlayerFireOps oxygenOps;
                oxygenOps.now = now;
                oxygenOps.dt = dt;
                oxygenOps.playerPosition = player.p;
                oxygenOps.yaw = yaw;
                oxygenOps.pitch = pitch;
                oxygenOps.cameraAudit = fireCameraAudit;
                oxygenOps.lastPassoutChoice = &lastPassoutChoice;
                oxygenOps.selectedItemIndex = uint32_t(playerLoadout.items.selected);
                oxygenOps.incapacitated = &playerIncapacitated;
                oxygenOps.activeHint = &activeHint;
                oxygenOps.activeHintRemaining = &activeHintRemaining;
                oxygenOps.audio = &sceneAudio;
                oxygenOps.random = &gameplayRandom;
                oxygenOps.releaseSelectedItem = [&] {
                    if (playerLoadout.selectedName() == "extinguisher") {
                        if (extinguisher.action.active && extinguisher.hasWorldFrame) {
                            extinguisher.action = ExtinguisherPose::endWindow(
                                extinguisher.action, now, extinguisher.worldFrame);
                            stopExtinguisherActionAudio();
                        }
                        sprayHeld = false;
                        sprayPressed = false;
                        sprayReleased = false;
                    }
                };
                updatePlayerOxygen(playerOxygen, now, dt,
                    [&] {
                        sceneAudio.playOwned(playerEntityId, "Breathe", player.p, player.p, yaw, 1);
                    },
                    [&] {
                        sceneAudio.playOwned(playerEntityId, "OxyRelease", player.p, player.p, yaw, 2);
                    },
                    [&] {
                        oxygenOps.triggerPassout(playerFireState, 0);
                    });
            }
            const auto visorCompletion =
                completePlayerOxygenVisorEffect(playerOxygenVisorEffect, now);
            if (visorCompletion == PlayerOxygenVisorEffectCompletion::oxygenOn) {
                sceneAudio.playOwned(playerEntityId, "VisorLock", player.p, player.p, yaw, 1);
                sceneAudio.applyOxygenVisorCategorySelector(4, playerOxygenVisorGroup2Gain);
            } else if (visorCompletion == PlayerOxygenVisorEffectCompletion::oxygenOff) {
                sceneAudio.applyOxygenVisorCategorySelector(5, playerOxygenVisorGroup2Gain);
            }
            // Retail calls the Player status updater before its state-6 branch,
            // so the HUD keeps easing toward the last resource value during
            // passout even though ordinary oxygen drain has stopped.
            updatePlayerOxygenDisplay(playerOxygen);
            if (updatePlayerOxygenWarning(playerOxygen)) {
                // 00458400 raises RC_OxygenLow through 004897C0 with the
                // sequential chatter path (00489110). The authored set has
                // one sound, so this maps directly to its sole RRF resource.
                sceneAudio.play("RC_OxygenLow0", player.p, player.p, yaw, 1);
                std::cout << "Player status key RC_OxygenLow asserted at "
                          << playerOxygen.displayedResource << "\n";
            }
            if (playerPassoutRecoveryDue(playerFireState, now) && rescueTransactions) {
                // Retail 00451080 keeps state 6 through elapsed=1000 and calls
                // 0044FF10(player, 0) on the first later Player update.
                syncVictimsToRescueState();
                rescueTransactions->passout(0);
                syncRescueStateToVictims();
                passoutRecoveredThisFrame = true;
            }
            updateBoundsTriggerModelFlags(now);
            updatePreviewFires(fires, now, dt);
            updatePreviewFirePathMotion(fires, collision, now, dt);
            if (previewMotionsPlaying) {
                previewMotionTime = std::min(bmodelMotionEnd, previewMotionTime + dt);
                sampleBModelMotions(bmodelMotions, previewMotionTime, inspectionPoses);
                if (previewMotionTime >= bmodelMotionEnd) previewMotionsPlaying = false;
            }
            updateBModelStates(bmodelMotions, dt, collision, player.p,
                [&](int entityId, size_t timerIndex, const std::string& command) {
                    if (!scriptRuntime) return;
                    scriptRuntime->run(command, entityId,
                        "BModel " + std::to_string(entityId) + "/timer " +
                            std::to_string(timerIndex));
                });
            for (const auto& bmodel : bmodelMotions) {
                const float previousPathTime = lastLoggedBModelPathTimes.count(bmodel.entity)
                    ? lastLoggedBModelPathTimes[bmodel.entity] : bmodel.startTime;
                const int previousState = lastLoggedBModelStates.count(bmodel.entity)
                    ? lastLoggedBModelStates[bmodel.entity] : 0;
                const bool stateChanged = previousState != bmodel.motionState;
                const bool advancedEnough = std::abs(bmodel.currentTime - previousPathTime) >= .1f;
                if (stateChanged || ((bmodel.motionState != 0 || previousState != 0) && advancedEnough)) {
                    std::cout << "BModel live PATH: entity " << bmodel.entity
                              << (bmodel.authoredName.empty() ? "" : " (" + bmodel.authoredName + ")")
                              << ", state " << bmodel.motionState << ", time " << bmodel.currentTime
                              << "/" << bmodel.endTime << ", speed " << bmodel.timeScale << "\n";
                }
                lastLoggedBModelPathTimes[bmodel.entity] = bmodel.currentTime;
                lastLoggedBModelStates[bmodel.entity] = bmodel.motionState;
            }
            const Uint8* keys = SDL_GetKeyboardState(nullptr);
            float forward = captured ? float(keys[SDL_SCANCODE_W] - keys[SDL_SCANCODE_S]) : 0.f;
            float side = captured ? .75f * float(keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]) : 0.f;
            bool crouchEdge = false;
            static bool crouchWasDown = false;
            bool crouchDown = keys[SDL_SCANCODE_C] != 0;
            crouchEdge = crouchDown && !crouchWasDown;
            crouchWasDown = crouchDown;
            const int useTargetBeforeMove = nearestUseEntity;
            const bool usingConsole = useEdge && useTargetBeforeMove >= 0;
            if (playerIncapacitated) forward = side = 0.f;
            const bool jumpDown = !playerIncapacitated && !usingConsole && (keys[SDL_SCANCODE_SPACE] != 0 ||
                (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)));
            if (!dedicatedPlayerHose && !playerIncapacitated && !passoutRecoveredThisFrame) {
                player.step(dt, forward, side, yaw, jumpDown,
                    jumpEdge && !usingConsole, crouchEdge, collision, pitch);
            }
            // Player::Think calls the RescueBot callback after movement and
            // collision. Host frame cadence remains provisional.
            if (!dedicatedPlayerHose && !playerIncapacitated && !passoutRecoveredThisFrame)
                advanceRescueBot(now);
            sceneAudio.updateOwnedListeners(player.p, yaw);
            updatePreviewFireLoopSounds(fires, sceneAudio, player.p, yaw);
            jumpEdge = false;
            const V playerContactPoint = playerRescueCandidateWorldOrigin(player);
            if (!playerIncapacitated && !passoutRecoveredThisFrame) {
                updatePlayerBurnCooling(playerBurnState, dt, player.collisionLatch,
                    playerOxygen.resource,
                    [&](const auto&, const auto&) {
                        // 00452570's 00441E90 copies the three translation
                        // DWORDs into the distinct +0x3400 safe snapshot.
                        player.safePosition3400 = player.p;
                    });
            }
            std::vector<QueuedSpreadFire> pendingSpreadChildren;
            std::vector<std::pair<int, float>> pendingFireExposureMessages;
            // 004483A0 updates spread proximity and queues Player exposure in
            // the same FireRescue callback. Defer this callback until after
            // the earlier registered BModel, BoundsTrigger, and PropModel
            // contact candidates have been collected below.
            auto runFireContactPass = [&] {
                const size_t fireContactCandidateCount = fires.size();
                int exposureRecords = 0;
                int firstExposureFire = -1;
                float firstExposureInfluence = 0.f;
                const V candidatePosition = playerContactPoint;
                for (size_t fireIndex = 0; fireIndex < fireContactCandidateCount; ++fireIndex) {
                    auto& fire = fires[fireIndex];
                    queuePreviewFireSpreadForCandidate(
                        fire, collision, gameplayRandom, playerContactPoint, now,
                        nearbySpreadFireCount, pendingSpreadChildren);
                    // Retail's four-record gate is FireRescue+0xB8, so each fire
                    // owns its own candidate count. This host evaluates one
                    // Player candidate per fire; a frame-wide cap would
                    // incorrectly discard later FireRescue instances.
                    if (fire.runtime.suppression.state == 5) continue;
                    // 0044A7A0 reads first flame sprite +0xD0. Fire Think
                    // writes that alpha from strength and clamps it to 0.999.
                    // Its pre-first-update value and callback order remain open.
                    const float emitterAlpha = fire.runtime.alpha;
                    const auto unobstructedContact = PlayerFireContact::evaluate(
                        fire.runtime.visual.position,
                        fire.runtime.visual.sizes.size.x,
                        fire.runtime.visual.sizes.size.z,
                        0.f, // Retail PlayerRescue initializes getter buffer +0x0C's fourth float (+0x74) to zero.
                        emitterAlpha, fire.runtime.suppression.strength,
                        candidatePosition, false);
                    // Retail 004483A0 first rejects candidates beyond its
                    // 500-unit max-axis gate, then computes volume falloff,
                    // and only performs the world-visibility query when the
                    // resulting influence passes its cutoff. Doing LOS before
                    // this cheap test made Bank trace all 72 FireRescue
                    // volumes every frame, including fires nowhere near the
                    // Player. An unobstructed result is an upper bound: a
                    // failed visibility query can only reduce its influence.
                    if (unobstructedContact.preVisibilityInfluence <
                        kPlayerFireExposureInfluenceCutoff) continue;
                    Hit sight;
                    const bool blocked = collision.tracePoint(
                        fire.runtime.visual.position, candidatePosition, 0x43, 2, sight);
                    const auto contact = blocked
                        ? PlayerFireContact::evaluate(
                            fire.runtime.visual.position,
                            fire.runtime.visual.sizes.size.x,
                            fire.runtime.visual.sizes.size.z,
                            0.f, // Retail PlayerRescue initializes getter buffer +0x0C's fourth float (+0x74) to zero.
                            emitterAlpha, fire.runtime.suppression.strength,
                            candidatePosition, true)
                        : unobstructedContact;
                    // Retail applies the influence cutoff before visibility.
                    // An obstructed result is then attenuated and still sent
                    // even if that lower value falls below the cutoff.
                    if (exposureRecords == 0) {
                        firstExposureFire = fire.entity;
                        firstExposureInfluence = contact.influence;
                    }
                    ++exposureRecords;
                    // 004483A0 submits message 0x7531 during the contact pass;
                    // 00429520 delivers mapped messages only after the contact
                    // traversal and entity-spawn queue have finished.
                    pendingFireExposureMessages.emplace_back(fire.entity, contact.influence);
                }
                if (fireCameraAudit) {
                    const bool exposedNow = exposureRecords != 0;
                    if (exposedNow != fireCameraAuditExposure) {
                        std::cout << "fire-camera-audit exposure=" << (exposedNow ? "enter" : "leave")
                                  << " t=" << now
                                  << " entity=" << (exposedNow ? firstExposureFire : fireCameraAuditEntity)
                                  << " influence=" << (exposedNow ? firstExposureInfluence : 0.f)
                                  << " yaw=" << yaw << " pitch=" << pitch
                                  << " player=" << player.p.x << "," << player.p.y << "," << player.p.z
                                  << "\n";
                    }
                    fireCameraAuditExposure = exposedNow;
                    fireCameraAuditEntity = exposedNow ? firstExposureFire : -1;
                    if (fireCameraAuditNextSample == 0 ||
                        static_cast<int32_t>(now - fireCameraAuditNextSample) >= 0) {
                        fireCameraAuditNextSample = now + 250;
                        const V auditCameraPosition{
                            player.p.x, player.p.y + (player.crouch ? 10.f : 70.f), player.p.z};
                        std::cout << "fire-camera-audit frame t=" << now
                                  << " exposed=" << (exposedNow ? "yes" : "no")
                                  << " entity=" << (exposedNow ? firstExposureFire : -1)
                                  << " influence=" << (exposedNow ? firstExposureInfluence : 0.f)
                                  << " yaw=" << yaw << " pitch=" << pitch
                                  << " viewDelta=" << (yaw - fireCameraAuditSampleYaw) << ","
                                  << (pitch - fireCameraAuditSamplePitch)
                                  << " mouseAngleDelta=" << fireCameraAuditYawDelta << ","
                                  << fireCameraAuditPitchDelta
                                  << " mouseEvents=" << fireCameraAuditMouseEvents
                                  << " captured=" << (captured ? "yes" : "no")
                                  << " relative=" << (SDL_GetRelativeMouseMode() ? "yes" : "no")
                                  << " focus=" << ((SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) ? "yes" : "no")
                                  << " player=" << player.p.x << "," << player.p.y << "," << player.p.z
                                  << " velocity=" << player.v.x << "," << player.v.y << "," << player.v.z
                                  << " delta=" << player.delta.x << "," << player.delta.y << "," << player.delta.z
                                  << " ground=" << (player.ground ? "yes" : "no")
                                  << " crouch=" << (player.crouch ? "yes" : "no")
                                  << " incapacitated=" << (playerIncapacitated ? "yes" : "no")
                                  << " input=" << forward << "," << side
                                  << " cameraBase=" << auditCameraPosition.x << ","
                                  << auditCameraPosition.y << "," << auditCameraPosition.z << "\n";
                        fireCameraAuditYawDelta = 0.f;
                        fireCameraAuditPitchDelta = 0.f;
                        fireCameraAuditMouseEvents = 0;
                        fireCameraAuditSampleYaw = yaw;
                        fireCameraAuditSamplePitch = pitch;
                    }
                }
            };
            const std::string selectedItem = playerLoadout.selectedName();
            if (dedicatedPlayerHose) playerHoseStage.advanceCameraRail(dt);
            FrameTransform cameraFrame = ViewUpdate::directEye(player.p, yaw, pitch, player.crouch);
            FrameTransform hoseAimFrame = cameraFrame;
            std::vector<V> hoseStreamVertices;
            V hoseStreamEnd{};
            bool hoseStreamVisible = false;
            float hoseStreamDrawPressure = 0.f;
            if (dedicatedPlayerHose) {
                const PlayerHoseCameraPose hosePose = playerHoseStage.cameraPose();
                if (hosePose.valid) {
                    Hit floorHit;
                    const V floorStart{hosePose.railPosition.x, 50.f, hosePose.railPosition.z};
                    const V floorEnd{hosePose.railPosition.x, -2000.f, hosePose.railPosition.z};
                    const float floorY = collision.tracePoint(floorStart, floorEnd, 1, 2, floorHit)
                        ? floorHit.point.y : 0.f;
                    const V cameraPosition{
                        hosePose.railPosition.x, floorY + 32.f, hosePose.railPosition.z};
                    player.p = cameraPosition;
                    player.v = {};
                    const V lookDirection = ViewUpdate::normalize(hosePose.lookAt - cameraPosition);
                    const float routeYaw = std::atan2(-lookDirection.x, -lookDirection.z);
                    const float routePitch = std::asin(std::clamp(lookDirection.y, -1.f, 1.f));
                    playerHoseStage.applySteeringMouseDeltas(routePitch);
                    auto frameForAngles = [](V position, float frameYaw, float framePitch) {
                        FrameTransform frame;
                        const float cy = std::cos(frameYaw), sy = std::sin(frameYaw);
                        const float cp = std::cos(framePitch), sp = std::sin(framePitch);
                        frame.position = position;
                        frame.axis = {V{cy, 0.f, -sy}, V{sy * sp, cp, cy * sp},
                                      V{sy * cp, -sp, cy * cp}};
                        return frame;
                    };
                    cameraFrame = frameForAngles(cameraPosition, routeYaw, routePitch);
                    // 00466780 copies the rail-derived frame, rotates its
                    // basis around axis 0 by pitch, then around the updated
                    // axis 1 by negative yaw. The input scale/sign mapping is
                    // source-backed; host input-poll cadence remains provisional.
                    auto rotateVectorAroundAxis = [](V vector, V axis, float angleDegrees) {
                        constexpr float degree = 0.01745329251994329577f;
                        axis = ViewUpdate::normalize(axis);
                        const float radians = angleDegrees * degree;
                        const float cosine = std::cos(radians);
                        const float sine = std::sin(radians);
                        // 004476B0 is called as Cross(axis, basis) but returns
                        // basis x axis, so preserve that argument convention.
                        return vector * cosine + ViewUpdate::cross(vector, axis) * sine +
                            axis * (dot(axis, vector) * (1.f - cosine));
                    };
                    auto rotateFrameAroundAxis = [&](FrameTransform& frame, V axis, float angleDegrees) {
                        for (V& basis : frame.axis)
                            basis = rotateVectorAroundAxis(basis, axis, angleDegrees);
                    };
                    hoseAimFrame = cameraFrame;
                    const float hosePitchDegrees = playerHoseStage.steerPitchDegrees;
                    const float hoseYawDegrees = -playerHoseStage.steerYawDegrees;
                    rotateFrameAroundAxis(hoseAimFrame, hoseAimFrame.axis[0], hosePitchDegrees);
                    rotateFrameAroundAxis(hoseAimFrame, hoseAimFrame.axis[1], hoseYawDegrees);
                }
                const std::string previousTransition = playerHoseStage.transition;
                bool streamHasWorldContact = false;
                const bool hoseActionActive = sprayHeld &&
                    playerHoseStage.result == PlayerHoseStage::Result::Running;
                playerHoseStage.updateStreamPressure(hoseActionActive);
                hoseStreamDrawPressure = playerHoseStage.streamPressure;
                // 00465EF0 offsets the source from the entity frame; the aim
                // direction comes from 00466780's temporary rotated frame.
                const V hoseStreamStart = cameraFrame.position + cameraFrame.axis[2] * 10.f +
                    cameraFrame.axis[1] * 15.f - cameraFrame.axis[0] * 10.f;
                const V hoseStreamDirection = hoseAimFrame.axis[2] * -1.f;
                const auto hoseCurve = playerHoseStage.streamCurve(
                    dt, hoseStreamStart, hoseStreamDirection);
                if (hoseCurve.size() > 1)
                    hoseStreamVertices.reserve((hoseCurve.size() - 1) * 4);
                if (!hoseCurve.empty()) hoseStreamEnd = hoseCurve.front().position;
                for (size_t i = 1; i < hoseCurve.size(); ++i) {
                    if (hoseCurve[i].shape <= 1.f) continue;
                    // 0046C5A0 orders the quad as previous main point,
                    // previous secondary point, current secondary point,
                    // current main point.
                    hoseStreamVertices.push_back(hoseCurve[i - 1].position);
                    hoseStreamVertices.push_back(hoseCurve[i - 1].secondaryPosition);
                    hoseStreamVertices.push_back(hoseCurve[i].secondaryPosition);
                    hoseStreamVertices.push_back(hoseCurve[i].position);
                    const V segmentStart = hoseCurve[i - 1].position;
                    const V segmentDelta = hoseCurve[i].position - segmentStart;
                    Hit streamHit;
                    if (collision.tracePoint(segmentStart, segmentDelta, 0x43, 2, streamHit)) {
                        hoseStreamEnd = streamHit.point;
                        streamHasWorldContact = true;
                        hoseStreamVisible = true;
                        break;
                    }
                    hoseStreamEnd = hoseCurve[i].position;
                    hoseStreamVisible = true;
                }
                bool targetInContactVolume = false;
                if (hoseActionActive && streamHasWorldContact) {
                    if (const auto* target = playerHoseStage.target()) {
                        targetInContactVolume = collision.modelBoundsOverlapPoint(
                            target->model, hoseStreamEnd, playerHoseStage.streamContactRadius());
                    }
                }
                playerHoseStage.update(dt, hoseActionActive, targetInContactVolume);
                switch (playerHoseStage.takeChatterRequest()) {
                    case PlayerHoseStage::ChatterRequest::LevelStart:
                        // The retail one-entry RC_HoseLvlStart set reaches the
                        // same 2D audio backend as other Player chatter cues.
                        sceneAudio.play("RC_HoseLvlStart0", player.p, player.p, yaw, 1);
                        break;
                    case PlayerHoseStage::ChatterRequest::LevelWin:
                        sceneAudio.play("RC_HoseLvlWin0", player.p, player.p, yaw, 1);
                        break;
                    case PlayerHoseStage::ChatterRequest::LevelLose:
                        sceneAudio.play("RC_HoseLvlLose0", player.p, player.p, yaw, 1);
                        break;
                    case PlayerHoseStage::ChatterRequest::None:
                        break;
                }
                if (playerHoseStage.transition != previousTransition &&
                    !playerHoseStage.transition.empty()) {
                    activeHint = playerHoseStage.transition;
                    activeHintRemaining = 2.f;
                    std::cout << "PlayerHose: " << playerHoseStage.transition << "\n";
                }
            }
            if (!dedicatedPlayerHose) {
                const bool hoseSelected = selectedItem == "hose";
                const int hoseMode = playerLoadout.items.hoseMode;
                const bool hoseModeUpdates = hoseMode == 1 || hoseMode == 3;
                if (hoseSelected && hoseModeUpdates && sprayPressed &&
                    !rescueHoseSpray.action.active) {
                    HoseSprayAction::begin(rescueHoseSpray.action, now);
                    // Retail action-start requests the owned 2D HoseSpray
                    // instance. The host loops it while held; loop policy has
                    // not been measured from the original audio backend.
                    sceneAudio.loopOwned(playerEntityId, "HoseSpray", player.p,
                                         player.p, yaw, 2);
                }
                if (rescueHoseSpray.action.active &&
                    (sprayReleased || !sprayHeld || !hoseSelected || !hoseModeUpdates)) {
                    HoseSprayAction::end(rescueHoseSpray.action);
                    sceneAudio.stopOwned(playerEntityId, "HoseSpray");
                }

                if (hoseSelected && hoseModeUpdates) {
                    // 00463EF0 consumes the component hit from the preceding
                    // 0046D1B0/0046C5A0 refresh, then the current update refreshes
                    // the point and padding for its next invocation.
                    if (rescueHoseSpray.action.active &&
                        rescueHoseSpray.stream.contactGate) {
                        for (auto& fire : fires) {
                            fire.runtime.visual.sizeUpdate(now);
                            const float influence = FireContact::influence(
                                fire.runtime.visual.sizes.size,
                                fire.runtime.visual.position,
                                rescueHoseSpray.stream.contactPoint,
                                rescueHoseSpray.stream.contactPadding,
                                fire.runtime.alpha);
                            if (influence < 0.01f) continue;
                            fire.runtime.spray(
                                dt, influence, true,
                                [&] {
                                    ++rescueHoseSpray.dousedFireCount;
                                    // 00448970 increments this only on the
                                    // finalize=1 transition into FireRescue
                                    // mode 5. The extinguisher route uses
                                    // finalize=0 and must not count here.
                                    addSourceStatistic(&rescueStatistics["Fires Doused"], 1);
                                },
                                [&] {
                                    const char* sound = fireLoopSoundName(
                                        fire.runtime.soundChoice);
                                    if (sound && fire.loopSoundStarted) {
                                        sceneAudio.stopOwned(fire.entity, sound);
                                        fire.loopSoundStarted = false;
                                    }
                                });
                            ExtinguisherEffects::SpawnRequest effectSpawn;
                            if (RescueHoseSprayEffects::afterContact(
                                    gameplayRandom, now,
                                    fire.runtime.suppression.strength,
                                    rescueHoseSpray.action.effectDeadline,
                                    effectSpawn)) {
                                // HoseSpray and Extinguisher both configure
                                // resource 14. The child render binding is not
                                // proven, so reuse the preview particle path.
                                // The initial HoseSpray template scalar is 3.0;
                                // mapping it to a quad side length is provisional.
                                extinguisherSprayRenderer.spawnHoseSpray(
                                    effectSpawn, fire.runtime.origin, now);
                            }
                        }
                    }

                    rescueHoseSpray.stream.clearContact();
                    rescueHoseSpray.stream.updatePressure(
                        rescueHoseSpray.action.active);
                    // Retail transforms local nozzle point (0,0,8) through
                    // HoseSpray +0x58 and adds 5 units of transformed aim while
                    // active. The host uses its camera frame for that item frame
                    // until attachment composition is recovered.
                    V source = cameraRelativePoint(cameraFrame, 0.f, 0.f, 8.f);
                    const V direction = hoseAimFrame.axis[2] * -1.f;
                    if (rescueHoseSpray.action.active) source = source + direction * 5.f;
                    const auto hoseCurve = rescueHoseSpray.stream.update(
                        dt, source, direction);
                    if (hoseCurve.size() > 1)
                        hoseStreamVertices.reserve((hoseCurve.size() - 1) * 4);
                    for (size_t i = 1; i < hoseCurve.size(); ++i) {
                        if (hoseCurve[i].shape <= 1.f) continue;
                        hoseStreamVertices.push_back(hoseCurve[i - 1].position);
                        hoseStreamVertices.push_back(hoseCurve[i - 1].secondaryPosition);
                        hoseStreamVertices.push_back(hoseCurve[i].secondaryPosition);
                        hoseStreamVertices.push_back(hoseCurve[i].position);
                        const V segmentStart = hoseCurve[i - 1].position;
                        const V segmentDelta = hoseCurve[i].position - segmentStart;
                        Hit streamHit;
                        if (collision.tracePoint(segmentStart, segmentDelta, 0x43, 2,
                                                streamHit)) {
                            rescueHoseSpray.stream.contactGate = true;
                            rescueHoseSpray.stream.contactPoint = streamHit.point;
                            rescueHoseSpray.stream.contactPadding =
                                rescueHoseSpray.stream.collisionPadding();
                            hoseStreamEnd = streamHit.point;
                            hoseStreamVisible = true;
                            break;
                        }
                        hoseStreamEnd = hoseCurve[i].position;
                        hoseStreamVisible = true;
                    }
                    hoseStreamDrawPressure = rescueHoseSpray.stream.pressure;
                } else if (!hoseSelected) {
                    rescueHoseSpray.stream.reset();
                }
            }
            const float cameraCosYaw = std::cos(yaw), cameraSinYaw = std::sin(yaw);
            const bool playerMoving = !playerIncapacitated &&
                std::sqrt(forward * forward + side * side) > 1e-7f;
            const int playerViewState = playerIncapacitated
                ? playerFireState.state : (player.crouch ? 4 : 3);
            const bool usePassoutCamera = playerFireState.state == 6;
            // The ACT swing is finite, but axeActionStarted used to remain
            // set after its endpoint. That kept advancing ItemViewPose's
            // one-second wrapping clock forever, feeding its hand offsets
            // back into ViewUpdate and making the stationary camera cycle.
            // The renderer clears axeSwingActive after the ACT endpoint; on
            // the following frame, return the hand pose and action state to
            // idle before passing its offsets into ViewUpdate.
            if (selectedItem == "axe" && axeActionStarted && !axeSwingActive) {
                axeStrikePending = false;
                axeActionStarted = false;
                axeSwingElapsed = 0.f;
                axeItemViewPose = ItemViewPose::Result{};
            }
            const V followerStartPosition = playerViewFollower.position;
            const V cameraBasePosition = cameraFrame.position;
            FrameTransform playerFrame;
            playerFrame.position = player.p;
            playerFrame.axis = {
                V{cameraCosYaw, 0.f, -cameraSinYaw},
                V{0.f, 1.f, 0.f},
                V{cameraSinYaw, 0.f, cameraCosYaw}
            };
            const V itemViewOffset = selectedItem == "axe" ? axeItemViewPose.viewOffset : V{};
            const V itemAimOffset = selectedItem == "axe" ? axeItemViewPose.aimOffset : V{};
            // ViewUpdate builds the Player frame's +Z backward basis. A
            // positive input pitch means looking up, so its local-X roll must
            // use the opposite sign to produce the same camera frame.
            if (usePassoutCamera) {
                if (!playerViewInitialized) {
                    playerViewFollower = Follower{};
                    playerViewFollower.flags = 0x2e;
                    playerViewFollower.threshold = {20.f, 1000.f, 20.f};
                    playerViewFollower.settling = {.05f, 0.f, .05f};
                    playerViewFollower.smoothing = {0.f, .05f, 0.f};
                    playerViewFollower.held = true;
                    playerViewFollower.position = cameraFrame.position;
                    playerViewFollower.previous = cameraFrame.position;
                    playerViewFollower.basis = cameraFrame.axis;
                    playerViewInitialized = true;
                }
                const float fraction = playerPassoutViewFactor(playerFireState, now);
                const auto view = ViewUpdate::transition(
                    playerFrame, {},
                    itemViewOffset, itemAimOffset, {0.f, -20.f, -30.f},
                    {-60.f, 1000.f, 0.f}, 70.f, fraction,
                    -pitch * 57.29577951308232f, dt, playerViewFollower, false);
                playerViewFollower = view.follower;
            } else {
                // Use only directEye above. Item animation offsets and the
                // view follower must never move the ordinary camera.
                playerViewInitialized = false;
            }
            if (usePassoutCamera) {
                cameraFrame.position = playerViewFollower.position;
                cameraFrame.axis = playerViewFollower.basis;
            }
            if (fireCameraAudit &&
                (fireCameraAuditNextPoseSample == 0 ||
                 static_cast<int32_t>(now - fireCameraAuditNextPoseSample) >= 0)) {
                fireCameraAuditNextPoseSample = now + 250;
                const V viewRay = cameraFrame.axis[2] * -1.f;
                std::cout << "fire-camera-audit camera-pose t=" << now
                          << " dt=" << dt
                          << " selected=" << selectedItem
                          << " moving=" << (playerMoving ? "yes" : "no")
                          << " playerState=" << playerViewState
                          << " headBob=disabled"
                          << " player=" << player.p.x << "," << player.p.y << "," << player.p.z
                          << " velocity=" << player.v.x << "," << player.v.y << "," << player.v.z
                          << " input=" << forward << "," << side
                          << " baseEye=" << cameraBasePosition.x << ","
                          << cameraBasePosition.y << "," << cameraBasePosition.z
                          << " followerBefore=" << followerStartPosition.x << ","
                          << followerStartPosition.y << "," << followerStartPosition.z
                          << " camera=" << cameraFrame.position.x << ","
                          << cameraFrame.position.y << "," << cameraFrame.position.z
                          << " cameraDelta=" << cameraFrame.position.x - cameraBasePosition.x << ","
                          << cameraFrame.position.y - cameraBasePosition.y << ","
                          << cameraFrame.position.z - cameraBasePosition.z
                          << " cameraRight=" << cameraFrame.axis[0].x << ","
                          << cameraFrame.axis[0].y << "," << cameraFrame.axis[0].z
                          << " cameraUp=" << cameraFrame.axis[1].x << ","
                          << cameraFrame.axis[1].y << "," << cameraFrame.axis[1].z
                          << " cameraBack=" << cameraFrame.axis[2].x << ","
                          << cameraFrame.axis[2].y << "," << cameraFrame.axis[2].z
                          << " viewRay=" << viewRay.x << "," << viewRay.y << "," << viewRay.z
                          << " axeAction=" << (axeActionStarted ? "yes" : "no")
                          << " axeAnimation=" << (axeSwingActive ? "yes" : "no")
                          << " axeTime=" << axeSwingElapsed
                          << " itemPoseTime=" << axeItemViewPose.currentTime
                          << " itemViewOffset=" << axeItemViewPose.viewOffset.x << ","
                          << axeItemViewPose.viewOffset.y << "," << axeItemViewPose.viewOffset.z
                          << " itemAimOffset=" << axeItemViewPose.aimOffset.x << ","
                          << axeItemViewPose.aimOffset.y << "," << axeItemViewPose.aimOffset.z;
                if (selectedItem == "axe") {
                    auto axeAsset = equipmentAssets.find("axe");
                    if (axeAsset != equipmentAssets.end() && axeAsset->second->renderer) {
                        const auto& actor = *axeAsset->second;
                        const bool animate = axeSwingActive && actor.swingMotion >= 0 &&
                            axeSwingElapsed <= actor.swingEnd;
                        const int motion = animate ? actor.swingMotion : -1;
                        const float motionTime = animate ? axeSwingElapsed : 0.f;
                        const auto currentJoints = actor.renderer->pose.joints(motion, motionTime);
                        const auto bindJoints = actor.renderer->pose.joints(-1, 0.f);
                        const J bones = field(actor.renderer->asset, "bones");
                        int rootBone = -1;
                        for (int index = 0; index < count(bones); ++index) {
                            if (integer(field(at(bones, index), "parent")) < 0) {
                                rootBone = index;
                                break;
                            }
                        }
                        if (rootBone >= 0) {
                            const V localDelta = currentJoints[size_t(rootBone)].t -
                                bindJoints[size_t(rootBone)].t;
                            const ActorTransform previewRoot = rawToolRootFromCameraFrame(cameraFrame);
                            const V worldDelta = previewRoot.rotate(localDelta);
                            const V previewOrigin = cameraRelativePoint(cameraFrame, 35.f, -8.f, 140.f);
                            const auto currentVertices = actor.renderer->pose.vertices(motion, motionTime);
                            const auto bindVertices = actor.renderer->pose.vertices(-1, 0.f);
                            double vertexAlongRayMin = 0.0, vertexAlongRayMax = 0.0,
                                   vertexAlongRaySum = 0.0;
                            double vertexTowardCameraMin = 0.0, vertexTowardCameraMax = 0.0,
                                   vertexTowardCameraSum = 0.0;
                            if (currentVertices.size() == bindVertices.size() && !currentVertices.empty()) {
                                vertexAlongRayMin = std::numeric_limits<double>::infinity();
                                vertexAlongRayMax = -std::numeric_limits<double>::infinity();
                                vertexTowardCameraMin = std::numeric_limits<double>::infinity();
                                vertexTowardCameraMax = -std::numeric_limits<double>::infinity();
                                for (size_t vertex = 0; vertex < currentVertices.size(); ++vertex) {
                                    const V delta = previewRoot.rotate(
                                        currentVertices[vertex] - bindVertices[vertex]);
                                    // viewRay points from the eye into the scene. Positive
                                    // projection is away from the camera; its negation is
                                    // motion toward the camera.
                                    const double alongViewRay = double(delta.x) * viewRay.x +
                                        double(delta.y) * viewRay.y + double(delta.z) * viewRay.z;
                                    const double towardCamera = -alongViewRay;
                                    vertexAlongRayMin = std::min(vertexAlongRayMin, alongViewRay);
                                    vertexAlongRayMax = std::max(vertexAlongRayMax, alongViewRay);
                                    vertexAlongRaySum += alongViewRay;
                                    vertexTowardCameraMin = std::min(vertexTowardCameraMin, towardCamera);
                                    vertexTowardCameraMax = std::max(vertexTowardCameraMax, towardCamera);
                                    vertexTowardCameraSum += towardCamera;
                                }
                            }
                            std::cout << " axeModel=" << actor.stem
                                      << " axeMotion=" << motion
                                      << " axeMotionEnd=" << actor.swingEnd
                                      << " axeRootLocal=" << currentJoints[size_t(rootBone)].t.x << ","
                                      << currentJoints[size_t(rootBone)].t.y << ","
                                      << currentJoints[size_t(rootBone)].t.z
                                      << " axeRootDeltaLocal=" << localDelta.x << ","
                                      << localDelta.y << "," << localDelta.z
                                      << " axeRootDeltaAlongViewRay=" << (worldDelta.x * viewRay.x +
                                          worldDelta.y * viewRay.y + worldDelta.z * viewRay.z)
                                      << " axeRootDeltaTowardCamera=" << -(worldDelta.x * viewRay.x +
                                          worldDelta.y * viewRay.y + worldDelta.z * viewRay.z)
                                      << " axeVertexDeltaAlongViewRay=" << vertexAlongRayMin << ","
                                      << vertexAlongRayMax << ","
                                      << (currentVertices.empty() ? 0.0 :
                                          vertexAlongRaySum / double(currentVertices.size()))
                                      << " axeVertexDeltaTowardCamera=" << vertexTowardCameraMin << ","
                                      << vertexTowardCameraMax << ","
                                      << (currentVertices.empty() ? 0.0 :
                                          vertexTowardCameraSum / double(currentVertices.size()))
                                      << " axePreviewOrigin=" << previewOrigin.x << ","
                                      << previewOrigin.y << "," << previewOrigin.z
                                      << " axeViewRayIntoScene=" << viewRay.x << ","
                                      << viewRay.y << "," << viewRay.z;
                        }
                    }
                }
                std::cout << "\n";
            }
            if (selectedItem == "axe" && sprayPressed) {
                // Retail Axe ACTION press (0045F8F0) sets global clock +100.
                // SDL ticks stand in for the retail clock, and this left-click
                // edge is the viewer's ACTION mapping. The retail child starts
                // after that deadline. Its Think path advances +0x33c, copies
                // the prior value to +0x340, and strikes only on the strict
                // old < 0.275 < new animation-time crossing.
                AxeActionCallbacks::Result axeActionResult;
                AxeActionCallbacks::press(axeActionState, now, axeActionResult);
                axeSwingActive = false;
                axeActionStarted = false;
                axeSwingElapsed = 0.f;
                axeItemViewPose = ItemViewPose::update(0.f, 1.f, 0.f, 7);
                axeStrikePending = true;
                axeFeedback = "waiting for action gate";
                activeHint = "Axe: action queued";
                activeHintRemaining = 1.2f;
            }
            if (selectedItem == "goggles" && sprayPressed)
                gogglesScanPending = true; // 0044F340 ACTION press -> goggles +0x14.
            if (selectedItem != "goggles" && gogglesScan.effectsActive)
                gogglesScan.deactivate();
            if (selectedItem != previousEquippedItem) {
                if (previousEquippedItem == "extinguisher" && extinguisher.action.active &&
                    extinguisher.hasWorldFrame) {
                    extinguisher.action = ExtinguisherPose::endWindow(
                        extinguisher.action, now, extinguisher.worldFrame);
                    stopExtinguisherActionAudio();
                }
                previousEquippedItem = selectedItem;
            }
            if (selectedItem != "axe") {
                axeStrikePending = false;
                axeActionStarted = false;
                axeSwingActive = false;
                axeSwingElapsed = 0.f;
                axeItemViewPose = ItemViewPose::Result{};
            }
            axeActionState.primaryEnabled =
                playerLoadout.items.axeGate.primaryChildEnabled ? 1 : 0;
            bool axeStrikeReady = false;
            bool axeStartedThisFrame = false;
            if (selectedItem == "axe" && axeStrikePending && !axeActionStarted &&
                static_cast<int32_t>(now - axeActionState.deadline) > 0) {
                axeActionStarted = true;
                axeSwingActive = true;
                axeSwingElapsed = 0.f;
                axeItemViewPose = ItemViewPose::update(0.f, 1.f, 0.f, 7);
                axeStartedThisFrame = true;
                sceneAudio.playOwned(playerEntityId, "AxeSwing", player.p, player.p, yaw, 2);
                axeFeedback = "swinging";
                activeHint = "Axe: swinging";
                activeHintRemaining = 1.2f;
            }
            if (selectedItem == "axe" && axeActionStarted && !axeStartedThisFrame) {
                const float priorAnimationTime = axeSwingElapsed;
                axeItemViewPose = ItemViewPose::update(
                    dt, 1.f, axeItemViewPose.currentTime,
                    axeItemViewPose.animationFlag);
                // The base item view-pose clock wraps after one second.
                // The exported Axe swing ACT tracks last 2-3 seconds, so keep
                // its animation and strike clock monotonic in a separate value.
                axeSwingElapsed += dt;
                // 0045FB90 stores prior time at Axe+0x340 and advanced time at
                // +0x33c. 00460B90's paired comparisons admit exactly the
                // sampled strict threshold crossing.
                axeStrikeReady = axeStrikePending && priorAnimationTime < 0.275f &&
                    axeSwingElapsed > 0.275f;
                if (axeStrikeReady) axeStrikePending = false;
            }
            if (axeStrikeReady) {

                auto toStrikeVec = [](V value) -> AxeStrikePose::Vec3 {
                    return {value.x, value.y, value.z};
                };
                AxeStrikePose::Inputs strikeInputs;
                strikeInputs.playerFrame04 = toStrikeVec(cameraFrame.axis[0]);
                strikeInputs.playerFrame10 = toStrikeVec(cameraFrame.axis[1]);
                // The OpenGL camera basis stores +Z backward; the strike's
                // Player view-frame third column points along the viewed ray.
                // This sign conversion is a viewer mapping assumption pending
                // a live ActorInfo/frame capture from retail.
                strikeInputs.playerFrame1c = toStrikeVec(cameraFrame.axis[2] * -1.f);
                strikeInputs.playerAt34cc = toStrikeVec(cameraFrame.position);
                strikeInputs.axeFlag1fc = axeActionState.primaryEnabled ? 1 : 0;
                const AxeStrikePose::Query query = AxeStrikePose::build(strikeInputs);

                AxeWorldCollision::State collisionState;
                collisionState.worldAtPlus8 = 1; // Selected scene package's active world.
                AxeWorldCollision::Inputs collisionInputs;
                collisionInputs.param1 = query.start;
                collisionInputs.param2 = query.mins;
                collisionInputs.param3 = query.maxs;
                collisionInputs.param4 = query.displacement;
                const auto worldResult = AxeWorldCollision::invoke(
                    collisionState, collisionInputs,
                    [&](const AxeWorldCollision::EngineRequest& request,
                        AxeWorldCollision::EngineResult& engineResult) {
                        if (request.contentsMask != 0x43 || request.collisionType != 2)
                            return false;
                        const V start{request.start[0], request.start[1], request.start[2]};
                        const V end{request.end[0], request.end[1], request.end[2]};
                        const V mins{request.mins[0], request.mins[1], request.mins[2]};
                        const V maxs{request.maxs[0], request.maxs[1], request.maxs[2]};
                        Hit hit;
                        if (!collision.trace(start, end - start, mins, maxs, hit)) return false;
                        engineResult.position = {hit.point.x, hit.point.y, hit.point.z};
                        engineResult.normal = {hit.normal.x, hit.normal.y, hit.normal.z};
                        return true;
                    });

                if (!worldResult.hit) {
                    axeFeedback = "miss: no world hit";
                    activeHint = "Axe: no world hit";
                    activeHintRemaining = 1.5f;
                    std::cout << "Axe swing: no world collision on the provisional strike sweep.\n";
                } else {
                    // Retail 00460B90 stops the registered AxeSwing instance
                    // after a world hit before it dispatches impact sounds.
                    sceneAudio.stopOwned(playerEntityId, "AxeSwing");
                    ++axeWorldImpactCount;
                    const V contactPoint{worldResult.position[0], worldResult.position[1],
                                         worldResult.position[2]};
                    const V candidatePoint = contactPoint +
                        V{strikeInputs.playerFrame1c[0], strikeInputs.playerFrame1c[1],
                          strikeInputs.playerFrame1c[2]} * 6.f;
                    bool chopped = false;
                    for (auto& bmodel : bmodelMotions) {
                        const AxeCandidateBounds::Vec3 boundsMin{
                            bmodel.boundsMin.x, bmodel.boundsMin.y, bmodel.boundsMin.z};
                        const AxeCandidateBounds::Vec3 boundsMax{
                            bmodel.boundsMax.x, bmodel.boundsMax.y, bmodel.boundsMax.z};
                        const AxeCandidateBounds::Vec3 strikePoint{
                            candidatePoint.x, candidatePoint.y, candidatePoint.z};
                        if (!AxeCandidateBounds::overlaps(boundsMin, boundsMax,
                                                         strikePoint, 8.f)) continue;

                        BModelAxeHit::State hitState;
                        hitState.hitpoints = uint32_t(std::max(0, bmodel.hitPoints));
                        hitState.liftable = uint32_t(std::max(0, bmodel.liftable));
                        hitState.strikeTime = bmodel.endTime; // max child motion extent at root +0x1EC
                        hitState.rootAddress = uint32_t(bmodel.entity);
                        BModelAxeHit::Services hitServices;
                        hitServices.constructString00402790 = [](const char*) { return uint32_t{1}; };
                        hitServices.scriptEvent004705B0 = [&](uint32_t, uint32_t, const char* command) {
                            if (!scriptRuntime) return false;
                            auto found = scriptRuntime->bindings.find(bmodel.entity);
                            if (found == scriptRuntime->bindings.end()) return false;
                            const bool present = std::any_of(found->second.begin(), found->second.end(),
                                [&](const Scripts::Binding& binding) {
                                    return lower(binding.event) == lower(command);
                                });
                            scriptRuntime->dispatch(bmodel.entity, command);
                            return present;
                        };
                        hitServices.destroy0043d660 = [&](uint32_t) {
                            commandBModel(bmodel, "open", player.p);
                        };
                        const auto hitResult = BModelAxeHit::invoke(hitState, hitServices);
                        if (hitResult.flags == 0) continue;
                        if ((hitResult.flags & 4u) == 0) {
                            if ((hitResult.flags & 1u) != 0)
                                sceneAudio.playOwned(playerEntityId, "WoodDoorBreak", player.p, player.p, yaw, 2);
                            else if ((hitResult.flags & 2u) != 0)
                                sceneAudio.playOwned(playerEntityId, "AxeHitWood", player.p, player.p, yaw, 2);
                        }
                        bmodel.hitPoints = int(hitState.hitpoints);
                        std::cout << "Axe chopped BModel " << bmodel.entity
                                  << (bmodel.authoredName.empty() ? "" : " (" + bmodel.authoredName + ")")
                                  << ": " << bmodel.hitPoints << " hit points remain";
                        if (bmodel.hitPoints == 0) std::cout << "; break event dispatched and PATH opened";
                        std::cout << "\n";
                        axeFeedback = bmodel.authoredName.empty()
                            ? "chopped entity " + std::to_string(bmodel.entity)
                            : "chopped " + bmodel.authoredName;
                        activeHint = "Axe: " + axeFeedback;
                        activeHintRemaining = 2.f;
                        chopped = true;
                        break; // Retail stops at the first accepted type-4 candidate.
                    }
                    if (!chopped) {
                        // When retail's breakable candidate traversal exhausts,
                        // 00460B90 plays the AxeHitOther instance.
                        sceneAudio.playOwned(playerEntityId, "AxeHitOther", player.p, player.p, yaw, 2);
                        axeFeedback = "world hit: no eligible breakable";
                        activeHint = "Axe: no eligible breakable in reach";
                        activeHintRemaining = 1.5f;
                        std::cout << "Axe struck world geometry; no eligible BModel was within the retail 8-unit candidate box.\n";
                    }
                }
            }
            const bool extinguisherSelected = selectedItem == "extinguisher";
            if (extinguisherSelected) {
                if (!extinguisher.followerInitialized) {
                    extinguisher.follower.position = cameraFrame.position;
                    extinguisher.follower.previous = cameraFrame.position;
                    extinguisher.follower.basis = cameraFrame.axis;
                    extinguisher.followerInitialized = true;
                }
                if (!extinguisher.hasWorldFrame) {
                    extinguisher.worldFrame = ExtinguisherPose::world(
                        extinguisher.localPose, extinguisher.follower);
                    extinguisher.hasWorldFrame = true;
                }
                if (sprayPressed && extinguisher.hasWorldFrame) {
                    const bool middleAlreadyActive = extinguisher.action.active;
                    extinguisher.action = ExtinguisherPose::beginWindow(
                        extinguisher.action, now, extinguisher.worldFrame);
                    startExtinguisherActionAudio(middleAlreadyActive);
                }
                if (sprayReleased && extinguisher.action.active) {
                    extinguisher.action = ExtinguisherPose::endWindow(
                        extinguisher.action, now, extinguisher.worldFrame);
                    stopExtinguisherActionAudio();
                }
            }
            sprayPressed = sprayReleased = false;
            if (extinguisherSelected) {
                extinguisher.follower.update(cameraFrame.position, dt);
                extinguisher.follower.basis = cameraFrame.axis;
                // 00461F80 samples the existing ActorInfo frame, then writes
                // the next blended pose with 004612B0 at the end of Think.
                // Keep that same local frame for both the mesh and contacts.
                extinguisher.worldFrame = ExtinguisherPose::world(
                    extinguisher.localPose, extinguisher.follower);
                extinguisher.hasWorldFrame = true;
            }
            if (extinguisherSelected && extinguisher.action.active) {
                const FrameTransform contactFrame = extinguisher.worldFrame;
                const bool traceExtinguisherFrame = fireCameraAudit &&
                    (extinguisherAuditNextSample == 0 ||
                     static_cast<int32_t>(now - extinguisherAuditNextSample) >= 0);
                if (traceExtinguisherFrame) {
                    extinguisherAuditNextSample = now + 250;
                    std::cout << "fire-camera-audit extinguisher-frame t=" << now
                              << " action=active held=" << (sprayHeld ? "yes" : "no")
                              << " dt=" << dt << " charge=" << extinguisher.charge
                              << " origin=" << contactFrame.position.x << ","
                              << contactFrame.position.y << "," << contactFrame.position.z
                              << " forward=" << -contactFrame.axis[1].x << ","
                              << -contactFrame.axis[1].y << "," << -contactFrame.axis[1].z
                              << " axis0=" << contactFrame.axis[0].x << ","
                              << contactFrame.axis[0].y << "," << contactFrame.axis[0].z
                              << " axis1=" << contactFrame.axis[1].x << ","
                              << contactFrame.axis[1].y << "," << contactFrame.axis[1].z
                              << " axis2=" << contactFrame.axis[2].x << ","
                              << contactFrame.axis[2].y << "," << contactFrame.axis[2].z
                              << "\n";
                }

                extinguisherContacts.reset();
                float sampleMatrix[9];
                for (int row = 0; row < 3; ++row)
                    for (int column = 0; column < 3; ++column)
                        sampleMatrix[row * 3 + column] = contactFrame.axis[column][row];
                const auto contactPoints = ExtinguisherSamples::points(
                    sampleMatrix, contactFrame.position);
                for (auto& fire : fires) {
                    fire.runtime.visual.sizeUpdate(now);
                    for (size_t sample = 0; sample < contactPoints.size(); ++sample) {
                        const V point = contactPoints[sample];
                        const float influence = FireContact::influence(
                            fire.runtime.visual.sizes.size, fire.runtime.visual.position,
                            point, 60.f, fire.runtime.alpha);
                        const double deliveredInfluence = double(influence) + double(influence);
                        const double suppressionResponse = 1.0 -
                            (1.0 - deliveredInfluence) * (1.0 - deliveredInfluence);
                        if (traceExtinguisherFrame) {
                            std::cout << "fire-camera-audit extinguisher-sample t=" << now
                                      << " fire=" << fire.entity << " sample=" << sample
                                      << " point=" << point.x << "," << point.y << "," << point.z
                                      << " fireOrigin=" << fire.runtime.visual.position.x << ","
                                      << fire.runtime.visual.position.y << ","
                                      << fire.runtime.visual.position.z
                                      << " size=" << fire.runtime.visual.sizes.size.x << ","
                                      << fire.runtime.visual.sizes.size.y << ","
                                      << fire.runtime.visual.sizes.size.z
                                      << " alpha=" << fire.runtime.alpha
                                      << " influence=" << influence
                                      << " deliveredInfluence=" << deliveredInfluence
                                      << " suppressionResponse=" << suppressionResponse
                                      << " expectedStrengthDelta=" <<
                                          (fire.runtime.suppression.state == 5 ? 0.0 :
                                           -double(dt) / double(fire.runtime.suppression.sprayTime) *
                                               suppressionResponse)
                                      << " eligible=" << (influence >= .001f ? "yes" : "no")
                                      << " strength=" << fire.runtime.suppression.strength
                                      << "\n";
                        }
                        if (!(influence >= .001f)) continue;
                        const float strength = float(double(influence) + double(influence));
                        const bool appended = extinguisherContacts.append(
                            uint32_t(fire.entity), uint32_t(fire.entity), 1, point, strength,
                            [&](const ExtinguisherContacts::Dispatch& event) {
                                fireMessages.enqueue(event.route0, event.route1,
                                                     event.message, event.payload);
                            });
                        if (appended) {
                            // The native contact is message 30000; effect deadlines
                            // and emitter offsets share the game's RNG stream.
                            ExtinguisherEffects::SpawnRequest spraySpawn;
                            ExtinguisherEffects::afterContact(
                                gameplayRandom, now, fire.runtime.suppression.strength,
                                extinguisher.action.cooldown, true, spraySpawn);
                            if (spraySpawn.requested) {
                                // FUN_00461F80 emits at FireRescue's +0x300 frame
                                // origin. FireEntity::origin is the shared port's
                                // current equivalent; the texture/quad mapping is
                                // renderer-level and documented as provisional.
                                extinguisherSprayRenderer.spawn(
                                    spraySpawn, fire.runtime.origin, now);
                            }
                        }
                    }
                }
                int extinguisherDelivered = 0;
                fireMessages.drain(
                    [&](uint32_t route, uint32_t generation) -> FireMessageQueue::Receiver {
                        if (generation != 1) return {};
                        auto target = std::find_if(fires.begin(), fires.end(), [&](const PreviewFire& fire) {
                            return uint32_t(fire.entity) == route;
                        });
                        if (target == fires.end()) return {};
                        return [&, target](uint32_t message, const void* payload) {
                            const float before = target->runtime.suppression.strength;
                            ExtinguisherEffects::deliverContact(
                                message, payload, dt, target->runtime);
                            ++extinguisherDelivered;
                            if (traceExtinguisherFrame) {
                                std::cout << "fire-camera-audit extinguisher-delivery t=" << now
                                          << " fire=" << target->entity << " message=" << message
                                          << " strength=" << before << "->"
                                          << target->runtime.suppression.strength
                                          << " alpha=" << target->runtime.alpha << "\n";
                            }
                        };
                    }, [](const FireMessageQueue::Delivery&) {});
                if (traceExtinguisherFrame) {
                    std::cout << "fire-camera-audit extinguisher-summary t=" << now
                              << " contacts=" << extinguisherContacts.count
                              << " delivered=" << extinguisherDelivered << "\n";
                }
                if (ExtinguisherPose::consumeCharge(extinguisher.charge, dt)) {
                    extinguisher.action = ExtinguisherPose::endWindow(
                        extinguisher.action, now, contactFrame);
                    stopExtinguisherActionAudio();
                    extinguisher.charge = 0.f;
                }
                const auto activePose = ExtinguisherPose::activeMotion(
                    gameplayRandom, dt, extinguisher.phase, {-20.f, -20.f, 30.f});
                extinguisher.phase = activePose.phase;
                extinguisher.activePose = activePose;
                const float progress = ExtinguisherPose::blendProgress(
                    now, extinguisher.action.start, extinguisher.action.end);
                extinguisher.localPose = extinguisher.action.active
                    ? (progress < 1.f
                        ? ExtinguisherPose::interpolate(
                            ExtinguisherPose::idleMotion(), activePose, progress)
                        : activePose)
                    : ExtinguisherPose::interpolate(
                        activePose, ExtinguisherPose::idleMotion(), progress);
                extinguisher.worldFrame = ExtinguisherPose::world(
                    extinguisher.localPose, extinguisher.follower);
            } else if (extinguisherSelected) {
                const uint32_t returnDuration = extinguisher.action.end - extinguisher.action.start;
                const int32_t returnElapsed = static_cast<int32_t>(now - extinguisher.action.start);
                if (returnDuration != 0 && returnElapsed >= 0 &&
                    uint32_t(returnElapsed) < returnDuration) {
                    const float progress = ExtinguisherPose::blendProgress(
                        now, extinguisher.action.start, extinguisher.action.end);
                    extinguisher.localPose = ExtinguisherPose::interpolate(
                        extinguisher.activePose, ExtinguisherPose::idleMotion(), progress);
                } else {
                    extinguisher.localPose = ExtinguisherPose::idleMotion();
                }
                extinguisher.worldFrame = ExtinguisherPose::world(
                    extinguisher.localPose, extinguisher.follower);
            }
            std::map<int, bool> currentBModelContacts;
            std::vector<std::pair<int, std::string>> pendingBModelContactEvents;
            nearestUseEntity = -1;
            float nearestUseDistanceSquared = std::numeric_limits<float>::infinity();
            const V playerMinimum = player.p + V{-30.f, -30.f, -30.f};
            const V playerMaximum = player.p + player.maxs();
            for (const auto& bmodel : bmodelMotions) {
                const bool inside = BModelContact::overlapsExpanded(
                    bmodel.boundsMin, bmodel.boundsMax, playerMinimum, playerMaximum,
                    bmodel.contactReach);
                const bool wasInside = bmodelContacts[bmodel.entity];
                if (scriptRuntime) {
                    if (inside && !wasInside) {
                        std::cout << "BModel contact touch: entity " << bmodel.entity
                                  << (bmodel.authoredName.empty() ? "" : " (" + bmodel.authoredName + ")")
                                  << " at PATH time " << bmodel.currentTime << "/" << bmodel.endTime << "\n";
                        pendingBModelContactEvents.emplace_back(bmodel.entity, "touch");
                    }
                    if (inside)
                        pendingBModelContactEvents.emplace_back(bmodel.entity, "touching");
                    if (!inside && wasInside)
                        pendingBModelContactEvents.emplace_back(bmodel.entity, "untouch");
                    auto foundBindings = scriptRuntime->bindings.find(bmodel.entity);
                    bool hasUse = foundBindings != scriptRuntime->bindings.end() &&
                        std::any_of(foundBindings->second.begin(), foundBindings->second.end(),
                                    [](const Scripts::Binding& binding) { return binding.event == "use"; });
                    if (inside && hasUse) {
                        const V center = (bmodel.boundsMin + bmodel.boundsMax) * .5f;
                        const V difference = center - player.p;
                        const float distanceSquared = dot(difference, difference);
                        if (distanceSquared < nearestUseDistanceSquared) {
                            nearestUseDistanceSquared = distanceSquared;
                            nearestUseEntity = bmodel.entity;
                        }
                    }
                }
                currentBModelContacts[bmodel.entity] = inside;
            }
            bmodelContacts = std::move(currentBModelContacts);
            std::map<int, bool> currentBoundsTriggerContacts;
            std::vector<const PreviewBoundsTrigger*> boundsTriggerTouches;
            std::vector<const PreviewBoundsTrigger*> boundsTriggerTouching;
            std::vector<const PreviewBoundsTrigger*> boundsTriggerUntouches;
            // Retail final-exit availability changes trigger model state via
            // 00440F50; it does not gate 00440CE0 contact production. Keep
            // pair edges independent: an already-overlapping trigger stays
            // `touching` after enablement, with no synthetic second `touch`.
            for (const auto& trigger : boundsTriggers) {
                const bool inside = BModelContact::overlapsExpanded(
                    trigger.boundsMin, trigger.boundsMax, playerMinimum, playerMaximum,
                    trigger.contactReach);
                const bool wasInside = boundsTriggerContacts[trigger.entity];
                if (inside && !wasInside) boundsTriggerTouches.push_back(&trigger);
                else if (inside) boundsTriggerTouching.push_back(&trigger);
                else if (wasInside) boundsTriggerUntouches.push_back(&trigger);
                currentBoundsTriggerContacts[trigger.entity] = inside;
            }
            boundsTriggerContacts = std::move(currentBoundsTriggerContacts);
            std::map<int, bool> currentPropModelContacts;
            std::vector<const PropModelInstance*> propModelTouches;
            std::vector<const PropModelInstance*> propModelTouching;
            std::vector<const PropModelInstance*> propModelUntouches;
            const int playerWorldLeaf = propModels.empty() ? -1 : propModelPvs.worldLeaf(playerContactPoint);
            for (const auto& prop : propModels) {
                // Retail 00443700 checks the Player point against the authored
                // radius, then gates the PropModel center through the world
                // leaf visibility state. The cluster PVS rejection is exact;
                // refresh the Player leaf once per frame and cache each static
                // PropModel leaf. Current-view area-matrix state and Player
                // frame alignment remain unresolved.
                const bool inside = propModelTouchCandidate(prop, playerContactPoint) &&
                                    propModelPvs.maySeeLeaves(playerWorldLeaf, prop.worldLeafIndex);
                const bool wasInside = propModelContacts[prop.entity];
                if (inside && !wasInside) propModelTouches.push_back(&prop);
                else if (inside) propModelTouching.push_back(&prop);
                else if (wasInside) propModelUntouches.push_back(&prop);
                currentPropModelContacts[prop.entity] = inside;
            }
            propModelContacts = std::move(currentPropModelContacts);
            // The registered contact classes are traversed in this order by
            // retail's contact manager: BModel, BoundsTrigger, PropModel,
            // FireRescue, then VictimRescue. Keep the event drains below the
            // contact pass; their retail scheduling cadence remains open.
            runFireContactPass();
            if (rescueTransactions && hasPlayerRescueMessageRoute) {
                // VictimRescue is registered after FireRescue in the retail
                // contact manager. Submit its 0x7532 records here, before the
                // separate ScriptActor pair drains below. The relative timing
                // of victim roam updates and this owner callback is unresolved.
                syncVictimsToRescueState();
                const VictimPickupBox playerBox{
                    playerContactPoint, {-30.f, -30.f, -30.f}, player.maxs()};
                for (auto& victim : victims) {
                    auto actor = rescueState.actors.find(victim.spawn.entity);
                    if (actor == rescueState.actors.end()) continue;
                    publishVictimProximity(
                        actor->second.state, 1, &actor->second, victimMessages,
                        [&]() -> std::optional<VictimPickupBox> { return playerBox; },
                        [&] { return victim.pickupRadius; },
                        [&] { return actor->second.frame.position; },
                        [&] { return playerRescueMessageRoute; },
                        [] { std::cerr << "Victim message mapper has no free records\n"; });
                }
            }
            if (scriptRuntime) {
                for (const auto& [entity, event] : pendingBModelContactEvents) {
                    scriptRuntime->dispatch(entity, event);
                    if (event == "touch") {
                        const int eventEntity = entity;
                        auto bmodel = std::find_if(bmodelMotions.begin(), bmodelMotions.end(),
                            [eventEntity](const PreviewBModelMotion& candidate) {
                                return candidate.entity == eventEntity;
                            });
                        if (bmodel != bmodelMotions.end())
                            std::cout << "BModel contact result: state " << bmodel->motionState
                                      << ", speed " << bmodel->timeScale << ", PATH time "
                                      << bmodel->currentTime << "/" << bmodel->endTime << "\n";
                    }
                }
                // 00470840 dispatches accumulated BoundsTrigger contacts in
                // three event-type passes. Contact production is sampled per
                // host frame; the retail updater cadence remains unresolved.
                for (const auto* trigger : boundsTriggerTouches)
                    scriptRuntime->dispatch(trigger->entity, "touch");
                for (const auto* trigger : boundsTriggerTouching)
                    scriptRuntime->dispatch(trigger->entity, "touching");
                for (const auto* trigger : boundsTriggerUntouches)
                    scriptRuntime->dispatch(trigger->entity, "untouch");
                // 00470840 drains pair deltas in three passes: newly touching,
                // still touching, then no longer touching. Keep that ordering
                // even when a Player overlaps more than one PropModel.
                for (const auto* prop : propModelTouches) {
                    std::cout << "PropModel contact touch: entity " << prop->entity
                              << (prop->name.empty() ? "" : " (" + prop->name + ")")
                              << " at authored point radius " << prop->touchRadius << "\n";
                    scriptRuntime->dispatch(prop->entity, "touch");
                }
                for (const auto* prop : propModelTouching)
                    scriptRuntime->dispatch(prop->entity, "touching");
                for (const auto* prop : propModelUntouches)
                    scriptRuntime->dispatch(prop->entity, "untouch");
            }
            if (useEdge && scriptRuntime && useTargetBeforeMove >= 0 &&
                bmodelContacts[useTargetBeforeMove]) {
                scriptRuntime->dispatch(useTargetBeforeMove, "use");
                activeHintRemaining = std::max(activeHintRemaining, 0.25f);
            }
            useEdge = false;
            if (scriptRuntime) {
                for (const auto& bmodel : bmodelMotions)
                    scriptRuntime->advanceEntity(bmodel.entity, now);
                for (const auto& trigger : boundsTriggers)
                    scriptRuntime->advanceEntity(trigger.entity, now);
                for (const auto& prop : propModels)
                    scriptRuntime->advanceEntity(prop.entity, now);
            }
            if (!rescueStageRequest.empty()) running = false;
            if (!authoredHintText.empty() && activeHint != authoredHintText)
                authoredHintText.clear();
            if (authoredHintText == activeHint && authoredHintLifetime.configured) {
                if (now > authoredHintLifetime.startTick && !authoredHintLifetime.visible(now)) {
                    activeHint.clear();
                    activeHintRemaining = 0.f;
                    authoredHintText.clear();
                }
            } else {
                activeHintRemaining = std::max(0.f, activeHintRemaining - dt);
                if (activeHintRemaining == 0.f) activeHint.clear();
            }
            updateRoamingVictims(victims, collision, gameplayRandom, player, yaw,
                                 carriedVictim.carriedVictimEntity.has_value(), dt);
            if (gogglesScan.effectsActive)
                gogglesScan.followTargets(buildGogglesScanCandidates());
            if (gogglesScanPending) {
                if (playerLoadout.selectedName() == "goggles") {
                    const auto candidates = buildGogglesScanCandidates();
                    gogglesScan.scan(player.p, candidates, now);
                    const size_t victimsDetected = std::count_if(
                        gogglesScan.targets.begin(), gogglesScan.targets.end(),
                        [](const GogglesScanTarget& target) { return target.kind == 2; });
                    activeHint = "Goggles scan: " + std::to_string(victimsDetected) +
                                 " survivors, " +
                                 std::to_string(gogglesScan.targets.size() - victimsDetected) +
                                 " fire targets";
                    activeHintRemaining = 1.5f;
                    std::cout << "Goggles scan recorded " << gogglesScan.targets.size()
                              << " authored kind-2/kind-6 targets\n";
                }
                gogglesScanPending = false;
            }
            // 00429520 finishes contact-manager callbacks before it drains the
            // entity-spawn queue and then the mapped-message queue. FireRescue
            // child creation and both message deliveries therefore wait until
            // after the represented contact candidates. FireRescue and
            // VictimRescue submissions now follow their registered class
            // order; ScriptActor event-drain timing and render-frame cadence
            // remain unresolved.
            drainPreviewFireSpawnQueue(fires, collision, gameplayRandom, now,
                                       fireDividerCycle, nextDynamicFireEntity,
                                       pendingSpreadChildren);
            for (const auto& [fireEntity, influence] : pendingFireExposureMessages) {
                const int queuedFireEntity = fireEntity;
                auto fire = std::find_if(fires.begin(), fires.end(), [&](const PreviewFire& candidate) {
                    return candidate.entity == queuedFireEntity;
                });
                if (fire == fires.end()) continue;
                PreviewPlayerFireOps fireOps;
                fireOps.now = now;
                fireOps.dt = dt;
                fireOps.playerPosition = player.p;
                fireOps.fireSource = fire->runtime.visual.position;
                fireOps.yaw = yaw;
                fireOps.pitch = pitch;
                fireOps.cameraAudit = fireCameraAudit;
                fireOps.directionalIndicators = &playerBurnState.directionalIndicators;
                fireOps.seriousBurns = &playerSeriousBurns;
                fireOps.lastPainChoice = &lastPainChoice;
                fireOps.lastPassoutChoice = &lastPassoutChoice;
                fireOps.selectedItemIndex = uint32_t(playerLoadout.items.selected);
                fireOps.incapacitated = &playerIncapacitated;
                fireOps.activeHint = &activeHint;
                fireOps.activeHintRemaining = &activeHintRemaining;
                fireOps.audio = &sceneAudio;
                fireOps.random = &gameplayRandom;
                fireOps.releaseSelectedItem = [&] {
                    // The retail selected-item +0x1C callback is a no-op
                    // for the Axe's child action state, and the preview
                    // exposes no persistent state for goggles' release.
                    // Extinguisher +0x1C is 00461590 and clears ACTION.
                    if (playerLoadout.selectedName() == "extinguisher") {
                        if (extinguisher.action.active && extinguisher.hasWorldFrame)
                            extinguisher.action = ExtinguisherPose::endWindow(
                                extinguisher.action, now, extinguisher.worldFrame);
                        sprayHeld = false;
                        sprayPressed = false;
                        sprayReleased = false;
                    }
                };
                // Keep draining queued records after passout: the retail
                // Player handler itself ignores subsequent messages in state 6.
                handlePlayerFireMessage7531(playerFireState, playerPainDeadline,
                                            playerBurnState.accumulator, influence,
                                            gameplayRandom, fireOps);
            }
            if (rescueTransactions && hasPlayerRescueMessageRoute) {
                victimMessages.drain(
                    [&](uint32_t route, uint32_t generation) -> FireMessageQueue::Receiver {
                        if (rescueMessageRoutes.resolve(route, generation) != &rescueState.player ||
                            !rescueTransactions) return {};
                        return [&](uint32_t message, const void* payload) {
                            if (message != 0x7532) return;
                            const auto* actor = static_cast<const VictimRuntimeActor*>(payload);
                            if (rescueTransactions->receivePickupPayload(payload) !=
                                VictimPickupMessageResult::accepted) return;
                            syncRescueStateToVictims();
                            auto picked = std::find_if(victims.begin(), victims.end(),
                                [&](const PreviewVictim& candidate) {
                                    return candidate.spawn.entity == actor->entity;
                                });
                            if (picked != victims.end())
                                std::cout << "Picked up survivor: " << picked->label << "\n";
                        };
                    }, [](const FireMessageQueue::Delivery&) {});
            }
            if (now - previousTitleUpdate >= 500) {
                std::string title;
                if (dedicatedPlayerHose) {
                    title = "FDNY Firefighters — " + scene.key() + " | Hose stage";
                    if (const auto* hotspot = playerHoseStage.hotspot()) {
                        title += " | hotspot " + std::to_string(playerHoseStage.hotspotOrdinal);
                        if (const auto* target = playerHoseStage.target()) {
                            title += " | target " + std::to_string(playerHoseStage.activeTarget + 1) + "/" +
                                std::to_string(hotspot->targets.size()) + " " + target->modelName;
                            title += " | cycle " + std::to_string(hotspot->numCycles -
                                playerHoseStage.cyclesRemaining + 1) + "/" +
                                std::to_string(hotspot->numCycles);
                        }
                        title += " | time " + std::to_string(int(std::ceil(std::max(0.f,
                            playerHoseStage.countdown))));
                    }
                    if (playerHoseStage.result == PlayerHoseStage::Result::Won)
                        title += " | stage complete";
                    else if (playerHoseStage.result == PlayerHoseStage::Result::Lost)
                        title += " | stage failed";
                    title += " | xyz " + std::to_string(int(player.p.x)) + "," +
                        std::to_string(int(player.p.y)) + "," + std::to_string(int(player.p.z));
                } else {
                    title = "FDNY scene preview — " + scene.key() + " | " +
                        std::to_string(victims.size()) + " survivors | " +
                        (player.ground ? "grounded" : "falling") + " | xyz " +
                        std::to_string(int(player.p.x)) + "," + std::to_string(int(player.p.y)) + "," +
                        std::to_string(int(player.p.z));
                    title += " | rescued " + std::to_string(rescueState.exits.rescues) + "/" +
                        std::to_string(rescueState.actors.size());
                    title += " | tool " + playerLoadout.selectedName();
                    if (!axeFeedback.empty()) title += " | axe " + axeFeedback;
                    if (axeWorldImpactCount != 0)
                        title += " | axe impacts " + std::to_string(axeWorldImpactCount);
                    if (gogglesScan.effectWindowOpen(now))
                        title += " | scan targets " + std::to_string(gogglesScan.targets.size());
                    if (carriedVictim.carriedVictimEntity) {
                        auto held = std::find_if(victims.begin(), victims.end(), [&](const PreviewVictim& victim) {
                            return victim.spawn.entity == *carriedVictim.carriedVictimEntity;
                        });
                        if (held != victims.end()) title += " | carrying " + held->label;
                    }
                }
                if (!activeHint.empty()) title += " | " + activeHint;
                else if (nearestUseEntity >= 0) title += " | E/Space: use";
                SDL_SetWindowTitle(window, title.c_str());
                previousTitleUpdate = now;
            }
            const auto performanceUpdateEnd = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            const V quakeOffset = rescueQuake.update(now, dt, gameplayRandom);
            if (quakeOffset.x != 0.f || quakeOffset.y != 0.f) {
                // 0049CD80 updates CameraRescue's +0x4c/+0x50 vector. Until
                // its final geCamera consumer is traced, use those values as
                // a provisional local-right/local-up presentation offset.
                const V localOffset = ViewUpdate::add(
                    ViewUpdate::scale(cameraFrame.axis[0], quakeOffset.x),
                    ViewUpdate::scale(cameraFrame.axis[1], quakeOffset.y));
                cameraFrame.position = ViewUpdate::add(cameraFrame.position, localOffset);
            }
            int width = 1, height = 1;
            SDL_GL_GetDrawableSize(window, &width, &height);
            RenderCounters renderCounters;
            const auto performanceSceneStart = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            renderScene(meshes, scene, collision, inspectionPoses, worldModelFlags,
                        fogState, lightStyles, cameraFrame.position, yaw, pitch, width, height,
                        performanceAudit ? &renderCounters : nullptr,
                        &cameraFrame);
            if (hoseStreamVisible) {
                if (dedicatedPlayerHose)
                    playerHoseStreamRenderer.draw(
                        hoseStreamVertices, playerHoseStage.streamClockSeconds);
                else
                    drawRescuePlayerHoseStreamPreview(
                        hoseStreamVertices, hoseStreamDrawPressure);
            }
            drawPreviewPropModels(propModels, propModelAssets);
            const auto performanceSceneEnd = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            drawPreviewFires(fires, fireRenderer, now);
            extinguisherSprayRenderer.draw(now, cameraFrame);
            drawPreviewVictims(victims, victimAssets, now);
            // Goggles target billboards still use world depth. Draw them before
            // clearing depth for the first-person equipment models below.
            gogglesRenderer.draw(gogglesScan, now, yaw, pitch);
            // Retail first equip leaves the Axe root +0x1FC gate clear. Its
            // secondary child (+0x648) then uses model 1 (axe.act); model 6
            // (axel.act) is the primary child and is updated only after the
            // duplicate-equip path sets that gate. The two children can both
            // be active after that path.
            auto axeSecondaryAsset = equipmentAssets.find("axe");
            auto axePrimaryAsset = equipmentAssets.find("axel");
            if (selectedItem == "axe" && axeSecondaryAsset != equipmentAssets.end() &&
                axeSecondaryAsset->second->renderer) {
                // Retail's complete ActorInfo matrix is not mapped into camera
                // coordinates yet, so the preview root remains provisional.
                ActorTransform axeRoot = rawToolRootFromCameraFrame(cameraFrame);
                axeRoot.t = cameraRelativePoint(cameraFrame, 35.f, -8.f, 140.f);
                glClear(GL_DEPTH_BUFFER_BIT); // Held models render over the opaque world, with self-depth retained.
                auto drawAxeChild = [&](EquipmentActorAsset& actor) {
                    const bool animate = axeSwingActive && actor.swingMotion >= 0 &&
                        axeSwingElapsed <= actor.swingEnd;
                    actor.renderer->draw(axeRoot, animate ? actor.swingMotion : -1,
                                         animate ? axeSwingElapsed : 0.f);
                    return animate;
                };
                bool anyEnabledAxeMotion = drawAxeChild(*axeSecondaryAsset->second);
                if (playerLoadout.items.axeGate.primaryChildEnabled &&
                    axePrimaryAsset != equipmentAssets.end() && axePrimaryAsset->second->renderer)
                    anyEnabledAxeMotion = drawAxeChild(*axePrimaryAsset->second) || anyEnabledAxeMotion;
                if (axeSwingActive && !anyEnabledAxeMotion) axeSwingActive = false;
            }
            auto extinguisherAsset = equipmentAssets.find("ext");
            if (selectedItem == "extinguisher" && extinguisher.hasWorldFrame &&
                extinguisherAsset != equipmentAssets.end() && extinguisherAsset->second->renderer) {
                // Retail samples fire from the extinguisher's transformed
                // ActorInfo frame. Share that action frame with the visible
                // ACT so the pictured nozzle and contact line move together.
                // Component scale and camera-space attachment still need a
                // retail runtime comparison.
                ActorTransform toolRoot = actorRootFromFrame(extinguisher.worldFrame);
                scaleActorRoot(toolRoot, .25f);
                glClear(GL_DEPTH_BUFFER_BIT);
                extinguisherAsset->second->renderer->drawBindPose(toolRoot);
            }
            glDisable(GL_FOG);
            gogglesRenderer.drawVisor(selectedItem == "goggles", width, height);
            fireDirectionRenderer.draw(playerBurnState.directionalIndicators, width, height);
            oxygenHudRenderer.drawVisorEffect(playerOxygenVisorEffect, now, width, height);
            oxygenHudRenderer.draw(
                playerOxygen.displayedResource,
                playerOxygenWarningOverlayAmount(playerOxygen.displayedResource),
                width, height);
            victimPickupHudRenderer.draw(uint32_t(SDL_GetTicks()), width, height);
            const int32_t bonusPoints = static_cast<int32_t>(
                SourceStatisticRecord::signedWord(rescueStatistics["Bonus Points"].value));
            bonusPointsHudRenderer.draw(uint32_t(SDL_GetTicks()), bonusPoints, width, height);
            if (activeHint == authoredHintText && authoredHintLifetime.configured) {
                if (authoredHintLifetime.visible(now))
                    hintOverlay.draw(activeHint, width, height);
            } else if (activeHintRemaining > 0.f) {
                hintOverlay.draw(activeHint, width, height);
            }
            const auto performanceOverlayEnd = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            const auto performanceSwapStart = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            SDL_GL_SwapWindow(window);
            const auto performanceSwapEnd = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            if (performanceAudit) {
                const auto performanceFrameEnd = PerformanceClock::now();
                performanceFrameMs.push_back(std::chrono::duration<double, std::milli>(
                    performanceFrameEnd - performanceFrameStart).count());
                performanceUpdateMs.push_back(std::chrono::duration<double, std::milli>(
                    performanceUpdateEnd - performanceFrameStart).count());
                performanceSceneMs.push_back(std::chrono::duration<double, std::milli>(
                    performanceSceneEnd - performanceSceneStart).count());
                performanceOverlayMs.push_back(std::chrono::duration<double, std::milli>(
                    performanceOverlayEnd - performanceSceneEnd).count());
                performanceSwapMs.push_back(std::chrono::duration<double, std::milli>(
                    performanceSwapEnd - performanceSwapStart).count());
                performanceSubmittedGroups += renderCounters.submittedGroups;
                performanceCulledGroups += renderCounters.culledGroups;
                performanceSubmittedTriangles += renderCounters.submittedTriangles;
                const double elapsedSeconds = std::chrono::duration<double>(
                    performanceFrameEnd - performanceWindowStart).count();
                if (elapsedSeconds >= 1.0) {
                    const double samples = double(performanceFrameMs.size());
                    std::cout << std::fixed << std::setprecision(1)
                              << "performance scene=" << scene.key()
                              << " fps=" << samples / elapsedSeconds
                              << " frame_ms avg/p95/max=" << performanceAverage(performanceFrameMs)
                              << "/" << performanceP95(performanceFrameMs) << "/"
                              << *std::max_element(performanceFrameMs.begin(), performanceFrameMs.end())
                              << " update_ms avg=" << performanceAverage(performanceUpdateMs)
                              << " world_ms avg=" << performanceAverage(performanceSceneMs)
                              << " overlays_ms avg=" << performanceAverage(performanceOverlayMs)
                              << " swap_ms avg=" << performanceAverage(performanceSwapMs)
                              << " groups/frame drawn/cull="
                              << double(performanceSubmittedGroups) / samples << "/"
                              << double(performanceCulledGroups) / samples
                              << " triangles/frame="
                              << double(performanceSubmittedTriangles) / samples << "\n"
                              << std::defaultfloat;
                    performanceFrameMs.clear();
                    performanceUpdateMs.clear();
                    performanceSceneMs.clear();
                    performanceOverlayMs.clear();
                    performanceSwapMs.clear();
                    performanceSubmittedGroups = performanceCulledGroups =
                        performanceSubmittedTriangles = 0;
                    performanceWindowStart = performanceFrameEnd;
                }
            }
        }
        clearTextures(meshes);
        for (auto& asset : victimAssets) asset.second->renderer->release();
        for (auto& asset : equipmentAssets)
            if (asset.second->renderer) asset.second->renderer->release();
        for (auto& prop : propModelAssets)
            if (prop.second->renderer) prop.second->renderer->release();
        if (fireRendererLoaded) fireRenderer.release();
        if (extinguisherSprayRendererLoaded) extinguisherSprayRenderer.release();
        if (playerHoseStreamRendererLoaded) playerHoseStreamRenderer.release();
        if (fireDirectionRendererLoaded) fireDirectionRenderer.release();
        if (gogglesRendererLoaded) gogglesRenderer.release();
        if (oxygenHudRendererLoaded) oxygenHudRenderer.release();
        if (victimPickupHudRendererLoaded) victimPickupHudRenderer.release();
        if (bonusPointsHudRendererLoaded) bonusPointsHudRenderer.release();
        hintOverlay.release();
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        sceneAudio.close();
        SDL_Quit();
        return 0;
    } catch (...) {
        for (auto& asset : equipmentAssets)
            if (asset.second->renderer) asset.second->renderer->release();
        for (auto& prop : propModelAssets)
            if (prop.second->renderer) prop.second->renderer->release();
        fireRenderer.release();
        extinguisherSprayRenderer.release();
        if (playerHoseStreamRendererLoaded) playerHoseStreamRenderer.release();
        fireDirectionRenderer.release();
        gogglesRenderer.release();
        oxygenHudRenderer.release();
        victimPickupHudRenderer.release();
        bonusPointsHudRenderer.release();
        if (context) SDL_GL_DeleteContext(context);
        if (window) SDL_DestroyWindow(window);
        if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_Quit();
        throw;
    }
}

int runVictimRoamMotionCheck() {
    const auto near = [](float actual, float expected) {
        return std::abs(actual - expected) <= 1.0e-5f;
    };
    const auto require = [](bool condition, const char* message) {
        if (!condition) throw std::runtime_error(message);
    };

    require(victimRoamStepReachesTarget({0.05f, 0.f, 0.f}, 0.1f),
            "positive sub-0.2 step should snap when it is longer than the remaining distance");
    require(!victimRoamStepReachesTarget({0.1f, 0.f, 0.f}, 0.1f),
            "arrival gate must use strict less-than at equal distance");
    require(!victimRoamStepReachesTarget({0.05f, 0.f, 0.f}, 0.f),
            "zero movement step must not use the snap branch");

    const V closeArrival = advanceVictimRoamPosition({}, {0.05f, 0.f, 0.f}, 0.1f);
    require(near(closeArrival.x, 0.05f),
            "sub-0.2 positive step failed to snap to a nearby target");
    const V partialStep = advanceVictimRoamPosition({}, {1.f, 0.f, 0.f}, 0.1f);
    require(near(partialStep.x, 0.1f), "ordinary normalized victim step is incorrect");
    const V zeroStep = advanceVictimRoamPosition({}, {1.f, 0.f, 0.f}, 0.f);
    require(near(zeroStep.x, 0.f), "zero victim step moved the actor");

    require(near(blendVictimRoamHeading(0.f, 90.f, 0.f), 0.f),
            "zero-delta turn changed the old heading");
    require(near(blendVictimRoamHeading(0.f, 90.f, 0.3f), 45.f),
            "0.3-second turn did not apply the retail half-weight");
    require(near(blendVictimRoamHeading(350.f, 10.f, 0.3f), 360.f),
            "turn blend did not follow the shortest wrapped heading");

    std::cout << "Victim roam motion check: strict retail arrival gate, normalized movement, "
                 "and exponential heading blend passed.\n";
    return 0;
}

int runRescueSceneChain(const std::filesystem::path& initialPackage,
                        bool performanceAudit = false,
                        std::optional<float> levelField10 = std::nullopt,
                        std::optional<int> hoseModeIndex = std::nullopt) {
    const SceneData initialScene = SceneData::load(initialPackage);
    if (loadPlayerHoseStage(initialScene).present) {
        const std::string musicFile = SceneData::stringField(initialScene.music(), "file");
        const std::string musicTrack = SceneData::stringField(initialScene.music(), "track");
        std::optional<SceneAudio::MusicSelection> hoseMusic;
        if (!musicFile.empty() && !musicTrack.empty())
            hoseMusic = SceneAudio::MusicSelection{musicTrack, initialScene.resolveAsset(musicFile)};
        // Dedicated hose maps are standalone stages; do not feed their result
        // messages into the rescue-only linked-stage chain.
        return run(initialPackage, false, false, false, false, false, -1,
                   false, false, false, false, false, {}, nullptr, performanceAudit,
                   hoseMusic ? &*hoseMusic : nullptr, levelField10, hoseModeIndex);
    }
    if (hoseModeIndex)
        throw std::runtime_error("--hose-mode-index requires a dedicated PlayerHose scene");
    const std::string musicFile = SceneData::stringField(initialScene.music(), "file");
    const std::string musicTrack = SceneData::stringField(initialScene.music(), "track");
    std::optional<SceneAudio::MusicSelection> inheritedMusic;
    // Retail copies the active level record's +0x0C music string into each
    // pending linked-stage record before selector 2 starts the new level.
    if (!musicFile.empty() && !musicTrack.empty())
        inheritedMusic = SceneAudio::MusicSelection{musicTrack, initialScene.resolveAsset(musicFile)};

    std::filesystem::path currentPackage = initialPackage;
    std::set<std::string> visited;
    for (int stageCount = 0; stageCount < 16; ++stageCount) {
        std::string currentKey = lower(currentPackage.filename().string());
        if (!visited.insert(currentKey).second) {
            std::cerr << "Rescue stage cycle stopped at " << currentKey << ".\n";
            return 0;
        }
        std::string nextStage;
        const int result = run(currentPackage, false, false, false, false, false, -1,
                               false, false, false, false, false, {}, &nextStage, performanceAudit,
                               inheritedMusic ? &*inheritedMusic : nullptr, levelField10);
        if (result != 0 || nextStage.empty()) return result;
        currentPackage = currentPackage.parent_path() / nextStage;
        std::cout << "Loading linked rescue package: " << currentPackage << "\n";
    }
    throw std::runtime_error("Rescue stage chain exceeded 16 linked packages");
}

int runRescueMissionSelectionLoop(const std::filesystem::path& resourcesRoot,
                                  bool performanceAudit = false) {
    const auto choices = loadRescueMissionChoices(resourcesRoot);
    std::cout << "Rescue chooser loaded " << choices.size()
              << " retail .lev entries from " << resourcesRoot << ".\n";
    size_t selectedTitleAction = 0;
    for (;;) {
        const auto action = chooseRescueTitleMenu(
            resourcesRoot / "menu" / "main.bmp",
            resourcesRoot / "menu" / "mcontrols.bmp",
            resourcesRoot / "menu" / "mcontrols_a.bmp",
            selectedTitleAction);
        if (!action || *action == RescueTitleAction::Exit) return 0;
        if (*action == RescueTitleAction::Game) {
            const auto selected = chooseRescueMission(choices);
            if (!selected) continue;
            const auto& choice = choices[*selected];
            std::cout << "Selected rescue mission catalog ordinal " << choice.catalogOrdinal
                      << ": " << choice.key << " (" << choice.title << ")\n";
            runRescueSceneChain(choice.scenePackage, performanceAudit, choice.levelField10);
        }
    }
}

int runRescueMissionCatalogCheck(const std::filesystem::path& resourcesRoot) {
    const auto choices = loadRescueMissionChoices(resourcesRoot);
    size_t retailCount = 0;
    size_t customCount = 0;
    for (const auto& choice : choices) {
        const SceneData scene = SceneData::load(choice.scenePackage);
        if (scene.key() != choice.key)
            throw std::runtime_error("Bundled rescue key does not match scene package: " + choice.key);
        std::cout << "Mission package check: ";
        if (choice.catalogOrdinal >= 0) {
            ++retailCount;
            std::cout << "ordinal " << choice.catalogOrdinal << " " << choice.key
                      << " (" << choice.title << "), entities="
                      << count(scene.entities()) << ", meshes="
                      << count(field(scene.worldMesh(), "groups"))
                      << ", level+0x10=" << choice.levelField10.value_or(1200.f)
                      << ", Player+0x343C(mode 0)="
                      << initializePlayerDeadlineInput(choice.levelField10, 0) << "\n";
        } else {
            ++customCount;
            std::cout << "custom " << choice.key << " (" << choice.title
                      << "), entities=" << count(scene.entities())
                      << ", meshes=" << count(field(scene.worldMesh(), "groups"))
                      << ", Player+0x343C(no level record, mode 0)="
                      << initializePlayerDeadlineInput(choice.levelField10, 0) << "\n";
        }
    }
    std::cout << "Mission catalog check: all " << retailCount
              << " retail rescue entries";
    if (customCount) std::cout << " and " << customCount << " custom packages";
    std::cout << " load through SceneData.\n";
    return 0;
}

int runRescueMissionMenuRenderCheck(const std::filesystem::path& resourcesRoot) {
#if defined(__APPLE__)
    const auto choices = loadRescueMissionChoices(resourcesRoot);
    constexpr int width = 1120;
    constexpr int height = 800;
    HeadlessOpenGL headless;
    headless.create(width, height);
    const GLuint titleBackground = loadRetailMenuBitmap(resourcesRoot / "menu" / "main.bmp");
    const GLuint titleControls = loadRetailMenuAlphaBitmap(
        resourcesRoot / "menu" / "mcontrols.bmp", resourcesRoot / "menu" / "mcontrols_a.bmp");
    std::vector<uint8_t> previous;
    std::vector<uint8_t> pixels(size_t(width) * size_t(height) * 4);
    size_t changedTransitions = 0;
    drawRescueTitleMenu(width, height, 0, titleBackground, titleControls);
    glFinish();
    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    saveRenderCheckFrame("/private/tmp/fdny-rescue-title-menu", pixels,
                         width, height, ".bmp");
    previous = pixels;
    drawRescueTitleMenu(width, height, 1, titleBackground, titleControls);
    glFinish();
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    size_t titleChanged = 0;
    for (size_t i = 0; i + 3 < pixels.size(); i += 4)
        if (pixels[i] != previous[i] || pixels[i + 1] != previous[i + 1] ||
            pixels[i + 2] != previous[i + 2] || pixels[i + 3] != previous[i + 3])
            ++titleChanged;
    if (!titleChanged)
        throw std::runtime_error("Title menu selection highlight did not change the frame");
    previous.clear();
    for (size_t selected = 0; selected < choices.size(); ++selected) {
        drawRescueMissionChooser(choices, selected, width, height);
        glFinish();
        glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        if (selected == 0) {
            saveRenderCheckFrame("/private/tmp/fdny-rescue-mission-menu", pixels,
                                 width, height, ".bmp");
        } else {
            size_t changed = 0;
            for (size_t i = 0; i + 3 < pixels.size(); i += 4)
                if (pixels[i] != previous[i] || pixels[i + 1] != previous[i + 1] ||
                    pixels[i + 2] != previous[i + 2] || pixels[i + 3] != previous[i + 3])
                    ++changed;
            if (!changed)
                throw std::runtime_error("Mission chooser selection highlight did not change the frame");
            ++changedTransitions;
        }
        previous = pixels;
    }
    const GLenum error = glGetError();
    glDeleteTextures(1, &titleControls);
    glDeleteTextures(1, &titleBackground);
    headless.destroy();
    if (error != GL_NO_ERROR)
        throw std::runtime_error("Mission chooser render check OpenGL error: " + std::to_string(error));
    std::cout << "Mission menu render check: title menu and " << choices.size() << " mission rows; "
              << "title selection changed " << titleChanged << " pixels; "
              << changedTransitions << " selected-row transitions changed the frame; "
              << "saved /private/tmp/fdny-rescue-title-menu.bmp and "
              << "/private/tmp/fdny-rescue-mission-menu.bmp.\n";
    return 0;
#else
    (void)resourcesRoot;
    throw std::runtime_error("--mission-menu-render-check currently requires the macOS CGL offscreen path");
#endif
}
} // namespace

int main(int argc, char** argv) {
#if defined(SDL_MAIN_HANDLED)
    SDL_SetMainReady();
#endif
    try {
        bool inspectOnly = false;
        bool renderCheck = false;
        bool interactionCheck = false;
        bool rescueCheck = false;
        bool ambientAudioCheck = false;
        bool hintRenderCheck = false;
        bool fireDirectionRenderCheck = false;
        bool oxygenHudRenderCheck = false;
        bool victimPickupHudRenderCheck = false;
        bool fireCameraAudit = false;
        bool victimRoamMathCheck = false;
        bool missionSelect = false;
        bool performanceAudit = false;
        bool missionCatalogCheck = false;
        bool missionMenuRenderCheck = false;
        std::string equipmentOrientationCheck;
        int triggerRenderCheckEntity = -1;
        std::optional<int> hoseModeIndex;
        std::filesystem::path package;
        for (int i = 1; i < argc; ++i) {
            std::string argument(argv[i]);
            if (argument == "--help") {
                std::cout << "Usage: fdny-scene-viewer [scene-export-directory] [--inspect | --render-check]\n"
                          << "       fdny-scene-viewer [scene-export-directory] --hint-render-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --fire-direction-render-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --oxygen-hud-render-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --victim-pickup-hud-render-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --fire-camera-audit\n"
                          << "       fdny-scene-viewer [scene-export-directory] --performance-audit\n"
                          << "       fdny-scene-viewer [scene-export-directory] --tool-orientation-check axe|axel|extinguisher\n"
                          << "       fdny-scene-viewer [scene-export-directory] --trigger-render-check ENTITY\n"
                          << "       fdny-scene-viewer [scene-export-directory] --interaction-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --rescue-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --scene-audio-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --ambient-audio-check (alias)\n"
                          << "       fdny-scene-viewer --mission-select\n"
                          << "       fdny-scene-viewer --mission-catalog-check\n"
                          << "       fdny-scene-viewer --mission-menu-render-check\n"
                          << "       fdny-scene-viewer <hose-scene-directory> --hose-mode-index 0|1|2\n"
                          << "       fdny-scene-viewer --victim-roam-math-check\n"
                          << "With a rescue-all bundle, no directory opens the rescue mission chooser.\n"
                          << "Otherwise, no directory loads the bundled scene (or .local/exports/mbase in the project).\n";
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
            if (argument == "--hint-render-check") {
                renderCheck = true;
                hintRenderCheck = true;
                continue;
            }
            if (argument == "--fire-direction-render-check") {
                renderCheck = true;
                fireDirectionRenderCheck = true;
                continue;
            }
            if (argument == "--oxygen-hud-render-check") {
                renderCheck = true;
                oxygenHudRenderCheck = true;
                continue;
            }
            if (argument == "--victim-pickup-hud-render-check") {
                renderCheck = true;
                victimPickupHudRenderCheck = true;
                continue;
            }
            if (argument == "--fire-camera-audit") {
                fireCameraAudit = true;
                continue;
            }
            if (argument == "--performance-audit") {
                performanceAudit = true;
                continue;
            }
            if (argument == "--hose-mode-index") {
                if (i + 1 >= argc)
                    throw std::runtime_error("--hose-mode-index requires a retail mode index (0, 1, or 2)");
                const std::string value(argv[++i]);
                size_t parsed = 0;
                const int modeIndex = std::stoi(value, &parsed);
                if (parsed != value.size() || modeIndex < 0 || modeIndex > 2)
                    throw std::runtime_error("--hose-mode-index accepts only 0, 1, or 2");
                hoseModeIndex = modeIndex;
                continue;
            }
            if (argument == "--tool-orientation-check") {
                if (i + 1 >= argc) throw std::runtime_error("--tool-orientation-check requires axe, axel, or extinguisher");
                equipmentOrientationCheck = lower(argv[++i]);
                if (equipmentOrientationCheck != "axe" && equipmentOrientationCheck != "axel" &&
                    equipmentOrientationCheck != "extinguisher")
                    throw std::runtime_error("--tool-orientation-check accepts axe, axel, or extinguisher");
                renderCheck = true;
                continue;
            }
            if (argument == "--trigger-render-check") {
                if (i + 1 >= argc) throw std::runtime_error("--trigger-render-check requires an entity index");
                triggerRenderCheckEntity = std::stoi(argv[++i]);
                renderCheck = true;
                continue;
            }
            if (argument == "--interaction-check") {
                interactionCheck = true;
                continue;
            }
            if (argument == "--rescue-check") {
                rescueCheck = true;
                continue;
            }
            if (argument == "--scene-audio-check" || argument == "--ambient-audio-check") {
                ambientAudioCheck = true;
                continue;
            }
            if (argument == "--victim-roam-math-check") {
                victimRoamMathCheck = true;
                continue;
            }
            if (argument == "--mission-select") {
                missionSelect = true;
                continue;
            }
            if (argument == "--mission-catalog-check") {
                missionCatalogCheck = true;
                continue;
            }
            if (argument == "--mission-menu-render-check") {
                missionMenuRenderCheck = true;
                continue;
            }
            if (!package.empty()) throw std::runtime_error("Only one scene export directory may be supplied.");
            package = argument;
        }
        if (performanceAudit && (inspectOnly || renderCheck || interactionCheck || rescueCheck || ambientAudioCheck ||
            hintRenderCheck || fireDirectionRenderCheck || !equipmentOrientationCheck.empty() ||
            oxygenHudRenderCheck || victimPickupHudRenderCheck || triggerRenderCheckEntity >= 0 || victimRoamMathCheck || missionCatalogCheck ||
            missionMenuRenderCheck))
            throw std::runtime_error("--performance-audit requires an interactive scene or mission selection");
        if (victimRoamMathCheck) {
            if (!package.empty() || inspectOnly || renderCheck || interactionCheck || rescueCheck || ambientAudioCheck ||
                hintRenderCheck || fireCameraAudit || !equipmentOrientationCheck.empty() ||
                fireDirectionRenderCheck || oxygenHudRenderCheck || victimPickupHudRenderCheck || triggerRenderCheckEntity >= 0)
                throw std::runtime_error("--victim-roam-math-check cannot be combined with a scene or another diagnostic mode.");
            return runVictimRoamMotionCheck();
        }
        std::filesystem::path bundledResources;
        if (package.empty() || missionSelect || missionCatalogCheck || missionMenuRenderCheck) {
            char* basePath = SDL_GetBasePath();
            if (basePath) {
                const std::filesystem::path baseDirectory(basePath);
                SDL_free(basePath);
                const std::vector<std::filesystem::path> candidates = {
                    baseDirectory,
                    baseDirectory / "Resources",
                    baseDirectory.parent_path() / "Resources",
                };
                for (const auto& candidate : candidates) {
                    if (std::filesystem::is_directory(candidate / "scene") ||
                        (std::filesystem::is_regular_file(candidate / "rescue_missions.json") &&
                         std::filesystem::is_directory(candidate / "scenes"))) {
                        bundledResources = candidate;
                        break;
                    }
                }
            }
            if (!missionSelect && !missionCatalogCheck && !missionMenuRenderCheck &&
                std::filesystem::is_regular_file(bundledResources / "rescue_missions.json") &&
                std::filesystem::is_directory(bundledResources / "scenes"))
                missionSelect = true;
            if (!missionSelect && !missionCatalogCheck && !missionMenuRenderCheck) {
                auto bundledScene = bundledResources / "scene";
                if (std::filesystem::is_directory(bundledScene)) package = bundledScene;
            }
            if (package.empty() && !missionSelect && !missionCatalogCheck && !missionMenuRenderCheck) {
                auto projectScene = std::filesystem::current_path() / ".local" / "exports" / "mbase";
                if (std::filesystem::is_directory(projectScene)) package = projectScene;
            }
        }
        if (missionSelect) {
            if (hoseModeIndex)
                throw std::runtime_error("--hose-mode-index is only available when directly launching a PlayerHose scene");
            if (!package.empty() || inspectOnly || renderCheck || interactionCheck || rescueCheck || ambientAudioCheck ||
                hintRenderCheck || fireDirectionRenderCheck || fireCameraAudit ||
                oxygenHudRenderCheck || victimPickupHudRenderCheck || !equipmentOrientationCheck.empty() || triggerRenderCheckEntity >= 0 ||
                missionCatalogCheck || missionMenuRenderCheck ||
                victimRoamMathCheck)
                throw std::runtime_error("--mission-select cannot be combined with a scene or another mode");
            if (bundledResources.empty())
                throw std::runtime_error("--mission-select requires the rescue mission catalog bundled next to the executable");
            return runRescueMissionSelectionLoop(bundledResources, performanceAudit);
        }
        if (missionCatalogCheck || missionMenuRenderCheck) {
            if (hoseModeIndex)
                throw std::runtime_error("--hose-mode-index is only available when directly launching a PlayerHose scene");
            if (!package.empty() || inspectOnly || renderCheck || interactionCheck || rescueCheck || ambientAudioCheck ||
                hintRenderCheck || fireDirectionRenderCheck || fireCameraAudit ||
                oxygenHudRenderCheck || victimPickupHudRenderCheck || !equipmentOrientationCheck.empty() || triggerRenderCheckEntity >= 0 ||
                (missionCatalogCheck && missionMenuRenderCheck) || victimRoamMathCheck)
                throw std::runtime_error("Mission catalog diagnostics cannot be combined with another scene or mode");
            if (bundledResources.empty())
                throw std::runtime_error("Mission catalog diagnostics require bundled rescue resources next to the executable");
            return missionCatalogCheck
                ? runRescueMissionCatalogCheck(bundledResources)
                : runRescueMissionMenuRenderCheck(bundledResources);
        }
        if (package.empty()) {
            throw std::runtime_error("No scene supplied. Pass a scene-export directory or launch the bundled app with its MBase data.");
        }
        const int diagnosticModes = int(inspectOnly) + int(renderCheck) + int(interactionCheck) +
                                    int(rescueCheck) + int(ambientAudioCheck);
        if (hoseModeIndex && (diagnosticModes != 0 || hintRenderCheck || fireDirectionRenderCheck ||
                              oxygenHudRenderCheck || victimPickupHudRenderCheck || fireCameraAudit ||
                              !equipmentOrientationCheck.empty() || triggerRenderCheckEntity >= 0))
            throw std::runtime_error("--hose-mode-index is only available for an interactive PlayerHose launch");
        if (diagnosticModes > 1)
            throw std::runtime_error("Choose only one diagnostic mode: --inspect, --render-check, --interaction-check, --rescue-check, or --scene-audio-check.");
        if (ambientAudioCheck && (hintRenderCheck || fireDirectionRenderCheck ||
                                  oxygenHudRenderCheck || victimPickupHudRenderCheck || fireCameraAudit ||
                                  !equipmentOrientationCheck.empty() || triggerRenderCheckEntity >= 0))
            throw std::runtime_error("--scene-audio-check cannot be combined with render or camera diagnostics.");
        if (fireDirectionRenderCheck && (hintRenderCheck || !equipmentOrientationCheck.empty() ||
                                         triggerRenderCheckEntity >= 0 || victimPickupHudRenderCheck))
            throw std::runtime_error("--fire-direction-render-check cannot be combined with hint, equipment, or trigger render checks.");
        if (oxygenHudRenderCheck && (hintRenderCheck || fireDirectionRenderCheck ||
                                     victimPickupHudRenderCheck || !equipmentOrientationCheck.empty() || triggerRenderCheckEntity >= 0))
            throw std::runtime_error("--oxygen-hud-render-check cannot be combined with another specialized render check.");
        if (victimPickupHudRenderCheck && (hintRenderCheck || fireDirectionRenderCheck || oxygenHudRenderCheck ||
                                     !equipmentOrientationCheck.empty() || triggerRenderCheckEntity >= 0))
            throw std::runtime_error("--victim-pickup-hud-render-check cannot be combined with another specialized render check.");
        const bool normalInteractiveRun = diagnosticModes == 0 &&
            triggerRenderCheckEntity < 0 && !hintRenderCheck &&
            !fireDirectionRenderCheck && !oxygenHudRenderCheck && !victimPickupHudRenderCheck && !fireCameraAudit &&
            equipmentOrientationCheck.empty();
        if (normalInteractiveRun)
            return runRescueSceneChain(package, performanceAudit, std::nullopt, hoseModeIndex);
        if (ambientAudioCheck && SDL_setenv("SDL_AUDIODRIVER", "dummy", 1) != 0)
            throw std::runtime_error("Could not select SDL's dummy audio driver for --scene-audio-check");
        return run(package, inspectOnly, renderCheck, interactionCheck, rescueCheck, ambientAudioCheck,
                   triggerRenderCheckEntity, hintRenderCheck, fireDirectionRenderCheck,
                   oxygenHudRenderCheck, victimPickupHudRenderCheck, fireCameraAudit,
                   equipmentOrientationCheck, nullptr, performanceAudit);
    } catch (const std::exception& error) {
        std::cerr << "Scene viewer: " << error.what() << "\n";
        return 1;
    }
}
