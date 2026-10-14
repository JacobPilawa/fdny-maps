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
#include "fire_floor.h"
#include "fire_renderer.h"
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
#include "extinguisher_pose.h"
#include "extinguisher_samples.h"
#include "extinguisher_contacts.h"
#include "extinguisher_effects.h"
#include "goggles_scan.h"
#include "goggles_renderer.h"
#include "scripts.h"
#include "scene_audio.h"
#include "hose_item_selection.h"

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
    std::array<int, 4> lightTypes{};
};

struct SceneMesh {
    int model = 0;
    std::vector<float> positions, uvs;
    std::vector<SceneFaceSpan> faceSpans;
    GLuint texture = 0;
    V boundsCenter{}, boundsMin{}, boundsMax{};
    float boundsRadius = 0.f;
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

std::string lower(std::string value);

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
                    yaw);
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
            void playPassoutMotion(uint32_t playerSubobjectOffset) {
                // The viewer has no Player ACT pose to drive yet. Preserve the
                // source-mapped +0x454 / +0x430 choice without inventing one.
                if (host.cameraAudit)
                    std::cout << "passout motion adapter open offset=0x" << std::hex
                              << playerSubobjectOffset << std::dec << "\n";
            }
            void playChatterSet(const char* setName, bool enabled) {
                if (!enabled || std::string(setName) != "RC_PassOut" ||
                    !host.audio || !host.random || !host.lastPassoutChoice) return;
                *host.lastPassoutChoice = host.random->soundIndex(
                    3, *host.lastPassoutChoice);
                host.audio->play("RC_PassOut" + std::to_string(*host.lastPassoutChoice),
                                 host.playerPosition, host.playerPosition, host.yaw);
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
    int roamState = 4;
    float roamRadius = 0.f, roamSpeed = 36.f, pickupRadius = 60.f;
    float facingDegrees = 0.f;
    V roamCenter{}, roamTarget{}, previousRoamTarget{}, roamDirection{};
    struct Obstacle { V center{}; float radiusSquared = 0.f; };
    std::vector<Obstacle> roamObstacles;
};

struct PreviewFire {
    int entity = -1;
    std::string label;
    std::string authoredName;
    // 0046F930 initializes the root +0x48 name string empty. FireRescue init
    // fills it from A_Name, so an absent or empty name keeps this gate closed.
    bool spreadNameGateEnabled = false;
    uint32_t nextSpreadAttempt = 0;
    FireEntity runtime;
};

struct PreviewPlayerLoadout {
    HoseItemSelection::State items;

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
            default: return "none";
        }
    }

    bool equip(const std::string& name) {
        const int slot = slotForName(name);
        if (slot == 0) return false;
        auto& item = items.slots[size_t(slot)];
        if (item.present && slot != 1) return true;
        item.present = true;
        if (slot == 1) items.axeGate.primaryChildEnabled = true;
        HoseItemSelection::Result result;
        HoseItemSelection::selectSlot(items, slot, false, result);
        return true;
    }

    bool select(const std::string& name) {
        const int slot = slotForName(name);
        if (slot == 0) return false;
        HoseItemSelection::Result result;
        return HoseItemSelection::selectSlot(items, slot, false, result);
    }

    std::string selectedName() const {
        return nameForSlot(items.selected);
    }
};

struct PreviewExtinguisher {
    Follower follower;
    FrameTransform worldFrame;
    ExtinguisherPose::CallbackState action;
    float phase = 0.f;
    float charge = 1.f;
    bool followerInitialized = false;
    bool hasWorldFrame = false;
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
            [](int) { /* named loop selection is source-mapped; audio routing remains open */ });
        preview.runtime.visual.sizeUpdate(now);
        fires.push_back(std::move(preview));
    }
    return fires;
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

V playerRescueCandidateWorldOrigin(const Player& player) {
    // VictimRescue, FireRescue, and PropModel callbacks ask the candidate
    // vtable at +0x0C. PlayerRescue maps that slot through 0049DB90 to the
    // transform-origin getter. Player::p remains the current host adapter for
    // that world point until its live frame alignment is recovered.
    return player.p;
}

struct QueuedSpreadFire {
    FireSpreadSpawn spawn;
    int parentEntity = -1;
    float sprayTime = 1.f;
};

void updatePreviewFireSpread(std::vector<PreviewFire>& fires, Collision& collision,
                             Random& random, V playerPosition, uint32_t now,
                             int& dividerCycle, int& nextDynamicEntity) {
    // The native manager's global count tracks eligible fires that crossed
    // the 750-unit boundary. Rebuilding it from the active host list is
    // equivalent for this one-player scene and avoids stale pooled entries.
    int nearbyCount = 0;
    for (const auto& fire : fires) {
        if (!fire.spreadNameGateEnabled || fire.runtime.suppression.state == 5) continue;
        if (FireSpread::proximityDistance(fire.runtime.origin, playerPosition) <= 750)
            ++nearbyCount;
    }

    std::vector<QueuedSpreadFire> queued;
    for (auto& fire : fires) {
        if (!fire.spreadNameGateEnabled || fire.runtime.suppression.state == 5) continue;
        const int distance = FireSpread::proximityDistance(fire.runtime.origin, playerPosition);
        if (distance >= 750) continue; // 00448320: strict spread-attempt gate.

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
            if (lower(verb) != "roam") continue;
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
        if (count(positions) % 9 != 0 || count(uvs) % 6 != 0 ||
            count(positions) / 3 != count(uvs) / 2)
            throw std::runtime_error("Invalid vertex or UV array in scene mesh group " + std::to_string(i));
        mesh.positions.reserve(size_t(count(positions)));
        mesh.uvs.reserve(size_t(count(uvs)));
        for (int k = 0; k < count(positions); ++k) mesh.positions.push_back(num(at(positions, k)));
        for (int k = 0; k < count(uvs); ++k) mesh.uvs.push_back(num(at(uvs, k)));
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
    for (int offsetX = -120; offsetX <= 120; offsetX += 20) {
        for (int offsetZ = -120; offsetZ <= 120; offsetZ += 20) {
            for (float offsetY : {ladder.boundsMin.y + 30.f,
                                  (ladder.boundsMin.y + ladder.boundsMax.y) * .5f - 30.f,
                                  ladder.boundsMax.y - 80.f}) {
                V candidate{ladderCenter.x + float(offsetX), offsetY,
                            ladderCenter.z + float(offsetZ)};
                const bool overlaps = BModelContact::overlapsExpanded(
                    ladder.boundsMin, ladder.boundsMax, candidate + playerMins,
                    candidate + playerMaxs, ladder.contactReach);
                if (!overlaps || collision.solid(candidate, playerMins, playerMaxs)) continue;
                const float distanceSquared = float(offsetX * offsetX + offsetZ * offsetZ);
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
              << " touch enabled Player climbing, upward Player::step moved " << climbedHeight
              << " units, and untouch disabled climbing.\n";
}

void clearTextures(std::vector<SceneMesh>& meshes) {
    std::set<GLuint> textures;
    for (const auto& mesh : meshes) if (mesh.texture) textures.insert(mesh.texture);
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

void renderScene(const std::vector<SceneMesh>& meshes, const SceneData& scene,
                 const Collision& collision, const std::map<int, Pose>& inspectionPoses,
                 const PreviewWorldModelFlags& worldModelFlags,
                 V camera, float yaw, float pitch,
                 int width, int height, RenderCounters* counters = nullptr) {
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
    glEnableClientState(GL_VERTEX_ARRAY);
    bool textureCoordinatesEnabled = false;
    const float yawCos = std::cos(yaw), yawSin = std::sin(yaw);
    const float pitchCos = std::cos(pitch), pitchSin = std::sin(pitch);
    const float tanVertical = std::tan(60.f * 3.14159265358979323846f / 360.f);
    const float tanHorizontal = tanVertical * (float(width) / float(std::max(height, 1)));
    const float horizontalPlaneScale = 1.f / std::sqrt(1.f + tanHorizontal * tanHorizontal);
    const float verticalPlaneScale = 1.f / std::sqrt(1.f + tanVertical * tanVertical);
    auto outsideView = [&](const SceneMesh& mesh) {
        const V offset = mesh.boundsCenter - camera;
        const float viewX = yawCos * offset.x - yawSin * offset.z;
        const float yawDepth = yawSin * offset.x + yawCos * offset.z;
        const float viewY = pitchCos * offset.y + pitchSin * yawDepth;
        const float viewDepth = pitchSin * offset.y - pitchCos * yawDepth;
        const float radius = mesh.boundsRadius;
        if (viewDepth + radius < 1.f || viewDepth - radius > 30000.f) return true;
        if ((viewX + viewDepth * tanHorizontal) * horizontalPlaneScale < -radius) return true;
        if ((viewDepth * tanHorizontal - viewX) * horizontalPlaneScale < -radius) return true;
        if ((viewY + viewDepth * tanVertical) * verticalPlaneScale < -radius) return true;
        if ((viewDepth * tanVertical - viewY) * verticalPlaneScale < -radius) return true;
        return false;
    };
    for (const auto& mesh : meshes) {
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
        glPopMatrix();
    }
    if (textureCoordinatesEnabled) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void printInventory(const SceneData& scene, const std::vector<SceneMesh>& meshes, const TextureLoadSummary& textures) {
    J report = field(scene.report(), "counts");
    size_t triangles = 0;
    size_t faceSpans = 0;
    for (const auto& mesh : meshes) triangles += mesh.positions.size() / 9;
    for (const auto& mesh : meshes) faceSpans += mesh.faceSpans.size();
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

int run(const std::filesystem::path& package, bool inspectOnly, bool renderCheck,
        bool interactionCheck, bool rescueCheck, int triggerRenderCheckEntity,
        bool hintRenderCheck, bool fireDirectionRenderCheck, bool fireCameraAudit,
        const std::string& equipmentOrientationCheck,
        std::string* nextRescueStage = nullptr, bool performanceAudit = false) {
    auto logRescueStage = [&](const char* stage) {
        if (rescueCheck) std::cerr << "rescue-check: " << stage << std::endl;
    };
    logRescueStage("enter");
    SceneData scene = SceneData::load(package);
    logRescueStage("scene loaded");
#if defined(__APPLE__)
    if (!inspectOnly && !renderCheck && !interactionCheck && !rescueCheck && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
#else
    if (!inspectOnly && !interactionCheck && !rescueCheck && SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
#endif
        throw std::runtime_error(SDL_GetError());
    if (rescueCheck && SDL_Init(SDL_INIT_TIMER) != 0)
        throw std::runtime_error(SDL_GetError());
    logRescueStage("SDL initialized");
    if (!inspectOnly && !renderCheck && !interactionCheck && !rescueCheck) {
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
    std::map<std::string, std::unique_ptr<VictimActorAsset>> victimAssets;
    std::map<std::string, std::unique_ptr<EquipmentActorAsset>> equipmentAssets;
    std::map<int, std::unique_ptr<EquipmentActorAsset>> propModelAssets;
    std::vector<PreviewVictim> victims;
    std::vector<PropModelInstance> propModels;
    std::map<int, size_t> propModelByEntity;
    std::vector<PreviewFire> fires;
    int fireDividerCycle = 0;
    int nextDynamicFireEntity = 1000000;
    FireRenderer fireRenderer;
    bool fireRendererLoaded = false;
    FireDirectionRenderer fireDirectionRenderer;
    bool fireDirectionRendererLoaded = false;
    GogglesRenderer gogglesRenderer;
    bool gogglesRendererLoaded = false;
    std::filesystem::path actorAssetRoot;
    VictimTransactionState rescueState;
    auto& carriedVictim = rescueState.player;
    try {
#if defined(__APPLE__)
        if (renderCheck) {
            headlessContext.create(1280, 800);
        } else
#endif
        if (!inspectOnly && !interactionCheck && !rescueCheck) {
            window = SDL_CreateWindow(("FDNY scene preview — " + scene.key()).c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            if (!window) throw std::runtime_error(SDL_GetError());
            context = SDL_GL_CreateContext(window);
            if (!context) throw std::runtime_error(SDL_GetError());
            SDL_GL_SetSwapInterval(1);
        }

        TextureLoadSummary textureSummary;
        logRescueStage("loading meshes");
        meshes = loadMeshes(scene, textureSummary, !inspectOnly && !interactionCheck && !rescueCheck);
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
        if (!inspectOnly && (!interactionCheck || rescueCheck)) {
            logRescueStage("loading FireRescue actors");
            fires = loadPreviewFires(scene, collision, gameplayRandom, gameplayStart, fireDividerCycle);
            logRescueStage("FireRescue actors loaded");
            logRescueStage("loading victim actors");
            actorAssetRoot = locateVictimAssets(scene);
            victims = loadPreviewVictims(scene, collision, gameplayRandom, gameplayStart, actorAssetRoot,
                                         victimAssets, !rescueCheck);
            for (const std::string stem : {"axe", "axel", "ext"}) {
                const auto exportPath = actorAssetRoot / "equipment" / (stem + ".json");
                if (std::filesystem::is_regular_file(exportPath)) {
                    equipmentAssets.emplace(stem, std::make_unique<EquipmentActorAsset>(
                        actorAssetRoot / "equipment", stem, !rescueCheck));
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
                    actorAssetRoot / "props", stem, !rescueCheck));
            }
            for (size_t propIndex = 0; propIndex < propModels.size(); ++propIndex) {
                auto& prop = propModels[propIndex];
                prop.worldLeafIndex = propModelPvs.worldLeaf(prop.position);
                propModelByEntity[prop.entity] = propIndex;
            }
            logRescueStage("victim actors loaded");
        }
        if (!inspectOnly && !interactionCheck && !rescueCheck) {
            fireRenderer.load(actorAssetRoot / "fire-sprites");
            fireRendererLoaded = true;
            fireDirectionRenderer.load(locateFireDirectionSprites(scene));
            fireDirectionRendererLoaded = true;
            gogglesRenderer.load(locateGogglesSprites(scene));
            gogglesRendererLoaded = true;
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
        PlayerPassoutState playerFireState;
        PlayerOxygenState playerOxygen;
        PlayerBurnCoolingState playerBurnState;
        uint32_t playerPainDeadline = 0;
        uint32_t playerSeriousBurns = 0;
        int lastPainChoice = -1, lastPassoutChoice = -1;
        bool playerIncapacitated = false;
        PreviewPlayerLoadout playerLoadout;
        PreviewExtinguisher extinguisher;
        AxeActionCallbacks::State axeActionState;
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
        if (!inspectOnly && !renderCheck && !interactionCheck && !rescueCheck) {
            sceneAudio.open();
            sceneAudio.load(scene);
        }
        std::string activeHint;
        float activeHintRemaining = 0.f;
        HintOverlay hintOverlay;
        std::vector<std::pair<int, std::string>> soundDispatches;
        TrainingZones zones(TrainingZones::LabelMode::CaseSensitive);
        VictimTransactionServices rescueServices;
        std::unique_ptr<VictimTransactions> rescueTransactions;
        std::map<std::string, SourceStatisticRecord> rescueStatistics{
            {"Rescues", {0, 0}},
            {"Longest Rescue Time", {2, 0}},
            {"Shortest Rescue Time", {1, std::numeric_limits<uint32_t>::max()}}
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
        std::unique_ptr<Scripts> scriptRuntime;
        if (scene.scripts()) {
            scriptRuntime = std::make_unique<Scripts>(scene.scripts());
            scriptRuntime->timeMs = SDL_GetTicks();
            scriptRuntime->variables["LEVEL"] = scene.key();
            scriptRuntime->hint = [&](std::string text, float seconds) {
                activeHint = std::move(text);
                activeHintRemaining = seconds;
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
                    return true;
                }
                if (entityId == playerEntityId && verb == "oxyoff") {
                    stopPlayerOxygenDrain(playerOxygen);
                    return true;
                }
                if (entityId == playerEntityId && verb == "equip") {
                    const std::string previousSelection = playerLoadout.selectedName();
                    const bool equipped = playerLoadout.equip(arguments);
                    if (equipped) noteGogglesSelection(previousSelection);
                    return equipped;
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
                if (entityId == playerEntityId && verb == "toolcmd" &&
                    lower(arguments) == "extinguisher refill") {
                    extinguisher.charge = 1.f;
                    return true;
                }
                if (entityId == playerEntityId && verb == "escape") {
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
                    if (remaining == 0 && !enabledExitTriggers.empty()) {
                        rescueState.exits.messageGate = 1;
                        activeHint = "All survivors rescued. Mission exit reached.";
                    } else if (remaining == 1) {
                        activeHint = "There is still one survivor remaining.";
                    } else {
                        activeHint = "There are still " + std::to_string(remaining) + " survivors remaining.";
                    }
                    activeHintRemaining = 6.f;
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
                        activeHintRemaining = 6.f;
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
                        sceneAudio.playOwned(entityId, soundName, source, player.p, yaw);
                    } else if (verb == "loopsnd") {
                        sceneAudio.loopOwned(entityId, soundName, source, player.p, yaw);
                    } else {
                        sceneAudio.stopOwned(entityId, soundName);
                    }
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
                    entityClass == "RCS_PlayerRescue" || entityClass == "RCS_PropModel")
                    scriptRuntime->startupEntity(entity);
            }
        }
        if (playerEntityId >= 0 && !victims.empty()) {
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
            rescueServices.readGlobalMode = [] { return 0; };
            // The scene package does not yet carry the selected catalog time
            // word; 600 seconds is the retail initializer's null-field fallback.
            rescueServices.readPlayerField343C = [] { return 600.f; };
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
                if (action != 2 || !carriedVictim.carriedVictimEntity) return;
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
            rescueServices.setRescueStatisticTotal = [](uint32_t) {};
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
            rescueTransactions.reset(new VictimTransactions{
                rescueState, rescueServices, zones, gameplayRandom});
        }
        printInventory(scene, meshes, textureSummary);
        if (!inspectOnly && (!interactionCheck || rescueCheck)) {
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
        std::cout << "Scene package path: " << scene.root().string() << "\n";
        std::cout << "Named zones: " << zones.groups.size() << " built from BoundsTrigger makezone commands\n";
        logRescueStage("scene startup complete");
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
            for (const auto& trigger : boundsTriggers) {
                auto bindings = scriptRuntime->bindings.find(trigger.entity);
                if (bindings == scriptRuntime->bindings.end()) continue;
                const bool escapeBinding = std::any_of(bindings->second.begin(), bindings->second.end(),
                    [](const Scripts::Binding& binding) {
                        return binding.event == "touch" &&
                            scriptCallsPlayerCommand(binding.command, "escape");
                    });
                if (escapeBinding) {
                    exitTrigger = trigger.entity;
                    triggerBounds = &trigger;
                    break;
                }
            }
            if (exitTrigger < 0 || !triggerBounds)
                throw std::runtime_error("Rescue check could not find an authored `touch: tell player escape` trigger");
            auto& victim = victims.front();
            auto actor = rescueState.actors.find(victim.spawn.entity);
            if (actor == rescueState.actors.end())
                throw std::runtime_error("Rescue check could not resolve the first victim actor");
            const V center = (triggerBounds->boundsMin + triggerBounds->boundsMax) * .5f;
            syncVictimsToRescueState();
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
                throw std::runtime_error("Mapped Player pickup message was not accepted");
            syncRescueStateToVictims();
            player.p = center;
            if (!BModelContact::overlapsExpanded(triggerBounds->boundsMin, triggerBounds->boundsMax,
                    center + V{-30.f, -30.f, -30.f}, center + V{30.f, 85.f, 30.f},
                    triggerBounds->contactReach))
                throw std::runtime_error("Rescue check player box does not overlap the selected exit bounds");
            scriptRuntime->dispatch(exitTrigger, "touch");
            if (carriedVictim.carriedVictimEntity || rescueState.exits.rescues != 1 ||
                actor->second.state != 6 || actor->second.worldActorFlags != 0x400u)
                throw std::runtime_error("Authored exit touch did not release and count the carried victim");
            std::cout << "Rescue check: trigger " << exitTrigger << " dispatched `tell player escape`; victim "
                      << actor->second.entity << " released at " << actor->second.frame.position.x << ", "
                      << actor->second.frame.position.y << ", " << actor->second.frame.position.z
                      << "; rescued=" << rescueState.exits.rescues << "/" << rescueState.exits.total << ".\n";
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
                fireDirectionRenderer.release();
                fireRendererLoaded = false;
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
                fireDirectionRenderer.release();
                fireRendererLoaded = false;
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
            std::vector<uint8_t> frameWithoutHint;
            if (hintRenderCheck) frameWithoutHint = readFramebuffer();
            if (fireDirectionRenderCheck) {
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
                fireDirectionRenderer.release();
                gogglesRenderer.release();
                fireRendererLoaded = false;
                fireDirectionRendererLoaded = false;
#if defined(__APPLE__)
                headlessContext.destroy();
#endif
                SDL_GL_DeleteContext(context);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return glError == GL_NO_ERROR && iconsDrawn == 4 && changedPixels > 0 ? 0 : 1;
            }
            if (hintRenderCheck)
                hintOverlay.draw("Press E or Space to use this console.", width, height);
            auto frame = readFramebuffer();
            GLenum glError = glGetError();
            size_t hintChangedPixels = 0;
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
                hintRenderCheck ? ".hint-render-check.bmp" : ".render-check.bmp");
            std::cout << "Render check: " << textureSummary.uploadedFiles << "/"
                      << textureSummary.uniqueFiles << " textures uploaded; "
                      << colors.size() << " distinct framebuffer colors; " << nonBackgroundPixels
                      << " non-background pixels; FireRescue=" << fires.size() << " ("
                      << fireQuads << " sprite quads)";
            if (hintRenderCheck)
                std::cout << "; hint overlay changed " << hintChangedPixels << " pixels";
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
            fireDirectionRenderer.release();
            fireRendererLoaded = false;
            fireDirectionRendererLoaded = false;
#if defined(__APPLE__)
            headlessContext.destroy();
#endif
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return glError == GL_NO_ERROR && textureSummary.failedFiles == 0 &&
                   textureSummary.uploadedFiles == textureSummary.uniqueFiles &&
                   (!hintRenderCheck || hintChangedPixels > 0) ? 0 : 1;
        }

        bool running = true, captured = true, jumpEdge = false;
        bool useEdge = false, sprayHeld = false, sprayPressed = false, sprayReleased = false;
        float pitch = 0.f;
        bool previewMotionsPlaying = false;
        bool fireCameraAuditExposure = false;
        int fireCameraAuditEntity = -1;
        uint32_t fireCameraAuditNextSample = 0;
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
        std::cout << "WASD move, mouse look, Space/right mouse jump, C crouch, Escape releases the mouse.\n"
                  << "Press F to snap to the nearest walkable collision floor below you.\n"
                  << "Rescue victims render from exported ACT assets; authored roam commands move them around the map.\n"
                  << "Authored FireRescue sprites animate; left-click and hold with the extinguisher selected to suppress fires.\n"
                  << "Victims are picked up automatically on contact. Authored BModel touch events open and close doors.\n"
                  << "PlayerRescue authored loadouts initialize; press 1/2/3 to select equipped axe/extinguisher/goggles.\n"
                  << "Extinguisher contacts use the recovered tool pose and fire falloff; left-click with goggles to scan nearby actors.\n"
                  << "The extinguisher pose follows this viewer's camera transform; contact cadence is a host approximation.\n"
                  << "Tool selection keys 1/2/3 are viewer controls.\n"
                  << "Press E or Space near a scripted console to use it. M/N inspect PATH timing; live door poses stay in control.\n"
                  << "Doors respond to authored touch events; PATH preview does not activate a door or change collision.\n"
                  << "Gameplay BModel poses drive both rendering and collision; retail callback cadence remains unknown.\n";
        if (fireCameraAudit)
            std::cout << "Fire camera audit is active: it logs mouse events and 250 ms view, movement, focus, and relative-mode samples.\n";
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
                    if (event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_3) {
                        const int slot = int(event.key.keysym.sym - SDLK_0);
                        const char* name = PreviewPlayerLoadout::nameForSlot(slot);
                        const std::string previousSelection = playerLoadout.selectedName();
                        if (playerLoadout.select(name)) {
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
                }
                if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && sprayHeld) {
                    sprayHeld = false;
                    sprayReleased = true;
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
                    // Keep the same input scaling as the existing native workbench.
                    const float yawBefore = yawInput;
                    const float pitchBefore = pitch;
                    yawInput -= event.motion.xrel * .0025f;
                    yaw = yawInput;
                    pitch -= event.motion.yrel * (.1f * .01745329251994329577f);
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
            uint32_t now = SDL_GetTicks();
            if (scriptRuntime) scriptRuntime->timeMs = now;
            float dt = std::min(.1f, float(now - previous) * .001f);
            previous = now;
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
                        if (extinguisher.action.active && extinguisher.hasWorldFrame)
                            extinguisher.action = ExtinguisherPose::endWindow(
                                extinguisher.action, now, extinguisher.worldFrame);
                        sprayHeld = false;
                        sprayPressed = false;
                        sprayReleased = false;
                    }
                };
                updatePlayerOxygen(playerOxygen, now, dt, [&] {
                    oxygenOps.triggerPassout(playerFireState, 0);
                });
                if (updatePlayerOxygenWarning(playerOxygen))
                    std::cout << "Player status key RC_OxygenLow asserted at "
                              << playerOxygen.resource << "\n";
            }
            updateBoundsTriggerModelFlags(now);
            updatePreviewFires(fires, now, dt);
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
            player.step(dt, forward, side, yaw, jumpDown,
                jumpEdge && !playerIncapacitated && !usingConsole, crouchEdge, collision, pitch);
            jumpEdge = false;
            const V playerContactPoint = playerRescueCandidateWorldOrigin(player);
            if (!playerIncapacitated) {
                updatePlayerBurnCooling(playerBurnState, dt, false, 0.f,
                                        [](const auto&, const auto&) {});
            }
            updatePreviewFireSpread(fires, collision, gameplayRandom,
                                    playerContactPoint, now,
                                    fireDividerCycle, nextDynamicFireEntity);
            int exposureRecords = 0;
            int firstExposureFire = -1;
            float firstExposureInfluence = 0.f;
            if (!playerIncapacitated) {
                const V candidatePosition = playerContactPoint;
                for (auto& fire : fires) {
                    // Retail's four-record gate is FireRescue+0xB8, so each
                    // fire owns its own candidate count. This host evaluates
                    // one Player candidate per fire; a frame-wide cap would
                    // incorrectly discard later FireRescue instances.
                    if (fire.runtime.suppression.state == 5) continue;
                    const auto unobstructedContact = PlayerFireContact::evaluate(
                        fire.runtime.visual.position,
                        fire.runtime.visual.sizes.size.x,
                        fire.runtime.visual.sizes.size.z,
                        30.f, // Host PlayerRescue hull radius; retail getter +0x0C's fourth float is open.
                        1.f, fire.runtime.suppression.strength,
                        candidatePosition, false);
                    // Retail 004483A0 first rejects candidates beyond its
                    // 500-unit max-axis gate, then computes volume falloff,
                    // and only performs the world-visibility query when the
                    // resulting influence passes its cutoff. Doing LOS before
                    // this cheap test made Bank trace all 72 FireRescue
                    // volumes every frame, including fires nowhere near the
                    // Player. An unobstructed result is an upper bound: a
                    // failed visibility query can only reduce its influence.
                    if (unobstructedContact.influence < .0001f) continue;
                    Hit sight;
                    const bool blocked = collision.tracePoint(
                        fire.runtime.visual.position, candidatePosition, 0x43, 2, sight);
                    const auto contact = blocked
                        ? PlayerFireContact::evaluate(
                            fire.runtime.visual.position,
                            fire.runtime.visual.sizes.size.x,
                            fire.runtime.visual.sizes.size.z,
                            30.f, // Host PlayerRescue hull radius; retail getter +0x0C's fourth float is open.
                            1.f, fire.runtime.suppression.strength,
                            candidatePosition, true)
                        : unobstructedContact;
                    if (contact.influence < .0001f) continue; // Retail cutoff constant remains under review.
                    if (exposureRecords == 0) {
                        firstExposureFire = fire.entity;
                        firstExposureInfluence = contact.influence;
                    }
                    ++exposureRecords;
                    PreviewPlayerFireOps fireOps;
                    fireOps.now = now;
                    fireOps.dt = dt;
                    fireOps.playerPosition = player.p;
                    fireOps.fireSource = fire.runtime.visual.position;
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
                    handlePlayerFireMessage7531(playerFireState, playerPainDeadline,
                                                playerBurnState.accumulator, contact.influence,
                                                gameplayRandom, fireOps);
                    if (playerIncapacitated) break;
                }
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
                              << " camera=" << auditCameraPosition.x << ","
                              << auditCameraPosition.y << "," << auditCameraPosition.z << "\n";
                    fireCameraAuditYawDelta = 0.f;
                    fireCameraAuditPitchDelta = 0.f;
                    fireCameraAuditMouseEvents = 0;
                    fireCameraAuditSampleYaw = yaw;
                    fireCameraAuditSamplePitch = pitch;
                }
            }
            FrameTransform cameraFrame;
            const float cameraCosYaw = std::cos(yaw), cameraSinYaw = std::sin(yaw);
            const float cameraCosPitch = std::cos(pitch), cameraSinPitch = std::sin(pitch);
            cameraFrame.position = {player.p.x, player.p.y + (player.crouch ? 10.f : 70.f), player.p.z};
            cameraFrame.axis = {
                V{cameraCosYaw, 0.f, -cameraSinYaw},
                V{cameraSinYaw * cameraSinPitch, cameraCosPitch, cameraCosYaw * cameraSinPitch},
                V{cameraSinYaw * cameraCosPitch, -cameraSinPitch, cameraCosYaw * cameraCosPitch}
            };
            const std::string selectedItem = playerLoadout.selectedName();
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
                }
                previousEquippedItem = selectedItem;
            }
            if (selectedItem != "axe") {
                axeStrikePending = false;
                axeActionStarted = false;
                axeSwingActive = false;
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
                axeStartedThisFrame = true;
                axeFeedback = "swinging";
                activeHint = "Axe: swinging";
                activeHintRemaining = 1.2f;
            }
            if (selectedItem == "axe" && axeActionStarted && !axeStartedThisFrame) {
                const float priorAnimationTime = axeSwingElapsed;
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
                    extinguisher.worldFrame = ExtinguisherPose::world(extinguisher.follower);
                    extinguisher.hasWorldFrame = true;
                }
                if (sprayPressed && extinguisher.hasWorldFrame)
                    extinguisher.action = ExtinguisherPose::beginWindow(
                        extinguisher.action, now, extinguisher.worldFrame);
                if (sprayReleased && extinguisher.action.active)
                    extinguisher.action = ExtinguisherPose::endWindow(
                        extinguisher.action, now, extinguisher.worldFrame);
            }
            sprayPressed = sprayReleased = false;
            if (extinguisherSelected) {
                extinguisher.follower.update(cameraFrame.position, dt);
                extinguisher.follower.basis = cameraFrame.axis;
                if (!extinguisher.action.active) {
                    extinguisher.worldFrame = ExtinguisherPose::world(extinguisher.follower);
                    extinguisher.hasWorldFrame = true;
                }
            }
            if (extinguisherSelected && extinguisher.action.active) {
                const auto activePose = ExtinguisherPose::activeMotion(
                    gameplayRandom, dt, extinguisher.phase, {-20.f, -20.f, 30.f});
                extinguisher.phase = activePose.phase;
                extinguisher.worldFrame = ExtinguisherPose::world(activePose, extinguisher.follower);
                extinguisher.hasWorldFrame = true;

                extinguisherContacts.reset();
                float sampleMatrix[9];
                for (int row = 0; row < 3; ++row)
                    for (int column = 0; column < 3; ++column)
                        sampleMatrix[row * 3 + column] = extinguisher.worldFrame.axis[column][row];
                const auto contactPoints = ExtinguisherSamples::points(
                    sampleMatrix, extinguisher.worldFrame.position);
                ExtinguisherEffects::SpawnRequest lastSpraySpawn;
                for (auto& fire : fires) {
                    fire.runtime.visual.sizeUpdate(now);
                    for (const V point : contactPoints) {
                        const float influence = FireContact::influence(
                            fire.runtime.visual.sizes.size, fire.runtime.visual.position,
                            point, 60.f, fire.runtime.alpha);
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
                            ExtinguisherEffects::afterContact(
                                gameplayRandom, now, fire.runtime.suppression.strength,
                                extinguisher.action.cooldown, true, lastSpraySpawn);
                        }
                    }
                }
                fireMessages.drain(
                    [&](uint32_t route, uint32_t generation) -> FireMessageQueue::Receiver {
                        if (generation != 1) return {};
                        auto target = std::find_if(fires.begin(), fires.end(), [&](const PreviewFire& fire) {
                            return uint32_t(fire.entity) == route;
                        });
                        if (target == fires.end()) return {};
                        return [&, target](uint32_t message, const void* payload) {
                            ExtinguisherEffects::deliverContact(
                                message, payload, dt, target->runtime);
                        };
                    }, [](const FireMessageQueue::Delivery&) {});
                if (ExtinguisherPose::consumeCharge(extinguisher.charge, dt)) {
                    extinguisher.action = ExtinguisherPose::endWindow(
                        extinguisher.action, now, extinguisher.worldFrame);
                    extinguisher.charge = 0.f;
                }
            }
            std::map<int, bool> currentBModelContacts;
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
                        scriptRuntime->dispatch(bmodel.entity, "touch");
                        std::cout << "BModel contact result: state " << bmodel.motionState
                                  << ", speed " << bmodel.timeScale << ", PATH time "
                                  << bmodel.currentTime << "/" << bmodel.endTime << "\n";
                    }
                    if (inside) scriptRuntime->dispatch(bmodel.entity, "touching");
                    if (!inside && wasInside) scriptRuntime->dispatch(bmodel.entity, "untouch");
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
            if (scriptRuntime) {
                // 00470840 dispatches accumulated contacts in three event-type
                // passes. Contact production is sampled per host frame here;
                // the retail updater cadence is still unresolved.
                for (const auto* trigger : boundsTriggerTouches)
                    scriptRuntime->dispatch(trigger->entity, "touch");
                for (const auto* trigger : boundsTriggerTouching)
                    scriptRuntime->dispatch(trigger->entity, "touching");
                for (const auto* trigger : boundsTriggerUntouches)
                    scriptRuntime->dispatch(trigger->entity, "untouch");
            }
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
            if (scriptRuntime) {
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
            activeHintRemaining = std::max(0.f, activeHintRemaining - dt);
            if (activeHintRemaining == 0.f) activeHint.clear();
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
            if (rescueTransactions && hasPlayerRescueMessageRoute) {
                // 0045A070 publishes 0x7532 through the message mapper; the
                // Player receiver applies the +0x3438 and carry-empty gates.
                // Retail 00429520 runs contact manager +0xE0 before mapped
                // message drain +0x3BC/0049B610, so delivery occurs in that
                // same owner invocation. This host drains after its candidate
                // scan; mapping owner calls to render frames and Player update
                // order remain unresolved.
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
                std::string title = "FDNY scene preview — " + scene.key() + " | " +
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
                if (!activeHint.empty()) title += " | " + activeHint;
                else if (nearestUseEntity >= 0) title += " | E/Space: use";
                SDL_SetWindowTitle(window, title.c_str());
                previousTitleUpdate = now;
            }
            const auto performanceUpdateEnd = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            int width = 1, height = 1;
            SDL_GL_GetDrawableSize(window, &width, &height);
            RenderCounters renderCounters;
            const auto performanceSceneStart = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            renderScene(meshes, scene, collision, inspectionPoses, worldModelFlags,
                        {player.p.x, player.p.y + (player.crouch ? 10.f : 70.f), player.p.z},
                        yaw, pitch, width, height, performanceAudit ? &renderCounters : nullptr);
            drawPreviewPropModels(propModels, propModelAssets);
            const auto performanceSceneEnd = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            drawPreviewFires(fires, fireRenderer, now);
            drawPreviewVictims(victims, victimAssets, now);
            // Goggles target billboards still use world depth. Draw them before
            // clearing depth for the first-person equipment models below.
            gogglesRenderer.draw(gogglesScan, now, yaw, pitch);
            // The retail-selected primary Axe child is configured with ACT
            // model 6 (axel.act); model 1 is the secondary child. When the
            // Axe item is selected, its primary post-view callback is enabled.
            auto axeAsset = equipmentAssets.find("axel");
            if (selectedItem == "axe" && axeAsset != equipmentAssets.end() && axeAsset->second->renderer) {
                // Model 6's port render exposes the axe head with the camera
                // root. Retail's complete ActorInfo matrix is not mapped into
                // camera coordinates yet, so placement remains provisional.
                // Keep the exported swing's upward arc in the viewport around
                // the source-mapped 0.275-second hit window.
                ActorTransform axeRoot = rawToolRootFromCameraFrame(cameraFrame);
                axeRoot.t = cameraRelativePoint(cameraFrame, 35.f, -8.f, 140.f);
                const auto& actor = *axeAsset->second;
                const bool animate = axeSwingActive && actor.swingMotion >= 0 &&
                    axeSwingElapsed <= actor.swingEnd;
                glClear(GL_DEPTH_BUFFER_BIT); // Held models render over the opaque world, with self-depth retained.
                actor.renderer->draw(axeRoot, animate ? actor.swingMotion : -1,
                                     animate ? axeSwingElapsed : 0.f);
                if (axeSwingActive && !animate) axeSwingActive = false;
            }
            auto extinguisherAsset = equipmentAssets.find("ext");
            if (selectedItem == "extinguisher" && extinguisher.hasWorldFrame &&
                extinguisherAsset != equipmentAssets.end() && extinguisherAsset->second->renderer) {
                // The decoded extinguisher's labeled side faces the player
                // with the direct root. Keep the visual mesh separate from
                // the action/contact pose; exact retail placement/scale remain
                // unverified.
                ActorTransform toolRoot = rawToolRootFromCameraFrame(cameraFrame);
                toolRoot.t = cameraRelativePoint(cameraFrame, 42.f, -5.f, 145.f);
                scaleActorRoot(toolRoot, .25f); // Retail instance scale is not yet mapped.
                glClear(GL_DEPTH_BUFFER_BIT);
                extinguisherAsset->second->renderer->drawBindPose(toolRoot);
            }
            gogglesRenderer.drawVisor(selectedItem == "goggles", width, height);
            fireDirectionRenderer.draw(playerBurnState.directionalIndicators, width, height);
            hintOverlay.draw(activeHint, width, height);
            const auto performanceOverlayEnd = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            const auto performanceSwapStart = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            SDL_GL_SwapWindow(window);
            const auto performanceSwapEnd = performanceAudit
                ? PerformanceClock::now() : PerformanceClock::time_point{};
            SDL_Delay(1);
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
        if (fireDirectionRendererLoaded) fireDirectionRenderer.release();
        if (gogglesRendererLoaded) gogglesRenderer.release();
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
        fireDirectionRenderer.release();
        gogglesRenderer.release();
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
                        bool performanceAudit = false) {
    std::filesystem::path currentPackage = initialPackage;
    std::set<std::string> visited;
    for (int stageCount = 0; stageCount < 16; ++stageCount) {
        std::string currentKey = lower(currentPackage.filename().string());
        if (!visited.insert(currentKey).second) {
            std::cerr << "Rescue stage cycle stopped at " << currentKey << ".\n";
            return 0;
        }
        std::string nextStage;
        const int result = run(currentPackage, false, false, false, false, -1,
                               false, false, false, {}, &nextStage, performanceAudit);
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
    while (const auto selected = chooseRescueMission(choices)) {
        const auto& choice = choices[*selected];
        std::cout << "Selected rescue mission catalog ordinal " << choice.catalogOrdinal
                  << ": " << choice.key << " (" << choice.title << ")\n";
        runRescueSceneChain(choice.scenePackage, performanceAudit);
    }
    return 0;
}

int runRescueMissionCatalogCheck(const std::filesystem::path& resourcesRoot) {
    const auto choices = loadRescueMissionChoices(resourcesRoot);
    for (const auto& choice : choices) {
        const SceneData scene = SceneData::load(choice.scenePackage);
        if (scene.key() != choice.key)
            throw std::runtime_error("Bundled rescue key does not match scene package: " + choice.key);
        std::cout << "Mission package check: ordinal " << choice.catalogOrdinal << " "
                  << choice.key << " (" << choice.title << "), entities="
                  << count(scene.entities()) << ", meshes="
                  << count(field(scene.worldMesh(), "groups")) << "\n";
    }
    std::cout << "Mission catalog check: all " << choices.size()
              << " retail rescue entries load through SceneData.\n";
    return 0;
}

int runRescueMissionMenuRenderCheck(const std::filesystem::path& resourcesRoot) {
#if defined(__APPLE__)
    const auto choices = loadRescueMissionChoices(resourcesRoot);
    constexpr int width = 1120;
    constexpr int height = 800;
    HeadlessOpenGL headless;
    headless.create(width, height);
    std::vector<uint8_t> previous;
    std::vector<uint8_t> pixels(size_t(width) * size_t(height) * 4);
    size_t changedTransitions = 0;
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
    headless.destroy();
    if (error != GL_NO_ERROR)
        throw std::runtime_error("Mission chooser render check OpenGL error: " + std::to_string(error));
    std::cout << "Mission menu render check: " << choices.size() << " rows; "
              << changedTransitions << " selected-row transitions changed the frame; "
              << "saved /private/tmp/fdny-rescue-mission-menu.bmp.\n";
    return 0;
#else
    (void)resourcesRoot;
    throw std::runtime_error("--mission-menu-render-check currently requires the macOS CGL offscreen path");
#endif
}
} // namespace

int main(int argc, char** argv) {
    try {
        bool inspectOnly = false;
        bool renderCheck = false;
        bool interactionCheck = false;
        bool rescueCheck = false;
        bool hintRenderCheck = false;
        bool fireDirectionRenderCheck = false;
        bool fireCameraAudit = false;
        bool victimRoamMathCheck = false;
        bool missionSelect = false;
        bool performanceAudit = false;
        bool missionCatalogCheck = false;
        bool missionMenuRenderCheck = false;
        std::string equipmentOrientationCheck;
        int triggerRenderCheckEntity = -1;
        std::filesystem::path package;
        for (int i = 1; i < argc; ++i) {
            std::string argument(argv[i]);
            if (argument == "--help") {
                std::cout << "Usage: fdny-scene-viewer [scene-export-directory] [--inspect | --render-check]\n"
                          << "       fdny-scene-viewer [scene-export-directory] --hint-render-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --fire-direction-render-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --fire-camera-audit\n"
                          << "       fdny-scene-viewer [scene-export-directory] --performance-audit\n"
                          << "       fdny-scene-viewer [scene-export-directory] --tool-orientation-check axe|axel|extinguisher\n"
                          << "       fdny-scene-viewer [scene-export-directory] --trigger-render-check ENTITY\n"
                          << "       fdny-scene-viewer [scene-export-directory] --interaction-check\n"
                          << "       fdny-scene-viewer [scene-export-directory] --rescue-check\n"
                          << "       fdny-scene-viewer --mission-select\n"
                          << "       fdny-scene-viewer --mission-catalog-check\n"
                          << "       fdny-scene-viewer --mission-menu-render-check\n"
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
            if (argument == "--fire-camera-audit") {
                fireCameraAudit = true;
                continue;
            }
            if (argument == "--performance-audit") {
                performanceAudit = true;
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
        if (performanceAudit && (inspectOnly || renderCheck || interactionCheck || rescueCheck ||
            hintRenderCheck || fireDirectionRenderCheck || !equipmentOrientationCheck.empty() ||
            triggerRenderCheckEntity >= 0 || victimRoamMathCheck || missionCatalogCheck ||
            missionMenuRenderCheck))
            throw std::runtime_error("--performance-audit requires an interactive scene or mission selection");
        if (victimRoamMathCheck) {
            if (!package.empty() || inspectOnly || renderCheck || interactionCheck || rescueCheck ||
                hintRenderCheck || fireCameraAudit || !equipmentOrientationCheck.empty() ||
                fireDirectionRenderCheck || triggerRenderCheckEntity >= 0)
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
            if (!package.empty() || inspectOnly || renderCheck || interactionCheck || rescueCheck ||
                hintRenderCheck || fireDirectionRenderCheck || fireCameraAudit ||
                !equipmentOrientationCheck.empty() || triggerRenderCheckEntity >= 0 ||
                missionCatalogCheck || missionMenuRenderCheck ||
                victimRoamMathCheck)
                throw std::runtime_error("--mission-select cannot be combined with a scene or another mode");
            if (bundledResources.empty())
                throw std::runtime_error("--mission-select requires the rescue mission catalog bundled next to the executable");
            return runRescueMissionSelectionLoop(bundledResources, performanceAudit);
        }
        if (missionCatalogCheck || missionMenuRenderCheck) {
            if (!package.empty() || inspectOnly || renderCheck || interactionCheck || rescueCheck ||
                hintRenderCheck || fireDirectionRenderCheck || fireCameraAudit ||
                !equipmentOrientationCheck.empty() || triggerRenderCheckEntity >= 0 ||
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
        const int diagnosticModes = int(inspectOnly) + int(renderCheck) + int(interactionCheck) + int(rescueCheck);
        if (diagnosticModes > 1)
            throw std::runtime_error("Choose only one diagnostic mode: --inspect, --render-check, --interaction-check, or --rescue-check.");
        if (fireDirectionRenderCheck && (hintRenderCheck || !equipmentOrientationCheck.empty() ||
                                         triggerRenderCheckEntity >= 0))
            throw std::runtime_error("--fire-direction-render-check cannot be combined with hint, equipment, or trigger render checks.");
        const bool normalInteractiveRun = diagnosticModes == 0 &&
            triggerRenderCheckEntity < 0 && !hintRenderCheck &&
            !fireDirectionRenderCheck && !fireCameraAudit &&
            equipmentOrientationCheck.empty();
        if (normalInteractiveRun) return runRescueSceneChain(package, performanceAudit);
        return run(package, inspectOnly, renderCheck, interactionCheck, rescueCheck,
                   triggerRenderCheckEntity, hintRenderCheck, fireDirectionRenderCheck, fireCameraAudit,
                   equipmentOrientationCheck, nullptr, performanceAudit);
    } catch (const std::exception& error) {
        std::cerr << "Scene viewer: " << error.what() << "\n";
        return 1;
    }
}
