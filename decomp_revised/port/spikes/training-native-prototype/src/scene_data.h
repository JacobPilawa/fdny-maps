#pragma once

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <json-c/json.h>

struct SceneJsonDeleter {
    void operator()(json_object* value) const {
        if (value) json_object_put(value);
    }
};

using SceneJson = std::unique_ptr<json_object, SceneJsonDeleter>;

// A scene package keeps the source-hashed exporter report and every decoded
// JSON document alive for its full runtime lifetime. Entity epairs and unknown
// fields remain available through entities() instead of being normalized away.
class SceneData {
public:
    static SceneData load(const std::filesystem::path& packagePath) {
        SceneData scene;
        scene.root_ = std::filesystem::absolute(packagePath);
        if (!std::filesystem::is_directory(scene.root_))
            throw std::runtime_error("Scene package directory does not exist: " + scene.root_.string());

        scene.report_ = read(scene.root_ / "report.json");
        scene.geometry_ = read(scene.root_ / "geometry.json");
        scene.worldMesh_ = read(scene.root_ / "world_mesh.json");
        scene.collision_ = read(scene.root_ / "collision.json");
        scene.entities_ = read(scene.root_ / "entities.json");
        scene.textureManifest_ = read(scene.root_ / "texture_manifest.json");

        const std::string format = stringField(scene.report_.get(), "format");
        if (format != "fdny-gbsp-v15-scene-v2")
            throw std::runtime_error("Unsupported scene export format: " + format);
        scene.key_ = scene.root_.filename().string();
        scene.sourceFile_ = stringField(scene.report_.get(), "source_file");
        scene.sourceSha256_ = stringField(scene.report_.get(), "source_sha256");
        if (scene.sourceSha256_.size() != 64)
            throw std::runtime_error("Scene report has no SHA-256 source identity: " + scene.root_.string());
        if (json_object_get_type(scene.worldMesh_.get()) != json_type_object ||
            json_object_get_type(scene.collision_.get()) != json_type_object ||
            json_object_get_type(scene.entities_.get()) != json_type_array)
            throw std::runtime_error("Scene package has an unexpected top-level JSON shape: " + scene.root_.string());
        if (json_object_get_type(field(scene.worldMesh_.get(), "groups")) != json_type_array ||
            json_object_get_type(field(scene.collision_.get(), "models")) != json_type_array ||
            json_object_get_type(field(scene.collision_.get(), "nodes")) != json_type_array ||
            json_object_get_type(field(scene.collision_.get(), "leafs")) != json_type_array ||
            json_object_get_type(field(scene.collision_.get(), "leaf_sides")) != json_type_array ||
            json_object_get_type(field(scene.collision_.get(), "planes")) != json_type_array)
            throw std::runtime_error("Scene package is missing required mesh or collision arrays: " + scene.root_.string());
        return scene;
    }

    const std::filesystem::path& root() const { return root_; }
    const std::string& key() const { return key_; }
    const std::string& sourceFile() const { return sourceFile_; }
    const std::string& sourceSha256() const { return sourceSha256_; }
    json_object* report() const { return report_.get(); }
    json_object* geometry() const { return geometry_.get(); }
    json_object* worldMesh() const { return worldMesh_.get(); }
    json_object* collision() const { return collision_.get(); }
    json_object* entities() const { return entities_.get(); }
    json_object* textureManifest() const { return textureManifest_.get(); }

    // Resolve each path component without assuming the host filesystem's
    // case-sensitivity. The returned path preserves the spelling on disk.
    std::filesystem::path resolveAsset(const std::string& relativePath) const {
        namespace fs = std::filesystem;
        fs::path current = root_;
        fs::path relative(relativePath);
        if (relative.empty() || relative.is_absolute())
            throw std::runtime_error("Scene asset path must be relative: " + relativePath);
        for (const auto& component : relative) {
            const std::string wanted = component.string();
            if (wanted.empty() || wanted == ".") continue;
            if (wanted == "..") throw std::runtime_error("Scene asset path escapes its package: " + relativePath);
            if (!fs::is_directory(current))
                throw std::runtime_error("Scene asset directory is missing while resolving: " + relativePath);
            std::vector<fs::path> matches;
            for (const auto& entry : fs::directory_iterator(current)) {
                std::string actual = entry.path().filename().string();
                if (fold(actual) == fold(wanted)) matches.push_back(entry.path());
            }
            if (matches.empty()) throw std::runtime_error("Scene asset is missing: " + relativePath);
            if (matches.size() > 1)
                throw std::runtime_error("Scene asset path is case-ambiguous on disk: " + relativePath);
            current = matches.front();
        }
        if (!fs::is_regular_file(current)) throw std::runtime_error("Scene asset is not a file: " + relativePath);
        return current;
    }

    static json_object* field(json_object* object, const char* key) {
        json_object* value = nullptr;
        if (object) json_object_object_get_ex(object, key, &value);
        return value;
    }

    static std::string stringField(json_object* object, const char* key) {
        json_object* value = field(object, key);
        return value && json_object_get_type(value) == json_type_string
            ? json_object_get_string(value) : std::string{};
    }

private:
    std::filesystem::path root_;
    std::string key_, sourceFile_, sourceSha256_;
    SceneJson report_, geometry_, worldMesh_, collision_, entities_, textureManifest_;

    static std::string fold(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });
        return value;
    }

    static json_object* field(json_object* object, const char* key, bool) {
        return SceneData::field(object, key);
    }

    static SceneJson read(const std::filesystem::path& path) {
        json_object* value = json_object_from_file(path.string().c_str());
        if (!value) throw std::runtime_error("Cannot load scene data: " + path.string());
        return SceneJson(value);
    }
};
