#pragma once

#include <filesystem>
#include <fstream>
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Draws a decoded ACT in its bind pose using the same JSON/RGBA export
// contract as VictimRenderer. The caller owns all equipment root transforms:
// this class does not guess a first-person attachment, scale, or swing pose.
// Include after the scene's J helpers, ActorPose/ActorTransform, SDL, and GL.
struct EquipmentActorRenderer {
    struct StaticVertex {
        float position[3];
        float uv[2];
    };

    J asset;
    ActorPose pose;
    std::filesystem::path bitmapDirectory;
    std::string stem;
    std::vector<GLuint> materialTextures;
    std::vector<std::vector<StaticVertex>> bindPoseMaterialVertices;

    EquipmentActorRenderer(J model, std::filesystem::path directory, std::string assetStem)
        : asset(model), pose(model), bitmapDirectory(std::move(directory)), stem(std::move(assetStem)) {}

    EquipmentActorRenderer(const EquipmentActorRenderer&) = delete;
    EquipmentActorRenderer& operator=(const EquipmentActorRenderer&) = delete;

    ~EquipmentActorRenderer() { release(); }

    void load() {
        const J materials = field(asset, "materials");
        materialTextures.assign(size_t(count(materials)), 0);
        try {
            for (int index = 0; index < count(materials); ++index) {
                const J material = at(materials, index);
                const int bitmapId = integer(field(material, "bitmapId"));
                if (bitmapId == 0) continue;

                // The current ACT export contract writes bitmap-directory
                // entries in numeric order as stem_0.bmp, stem_1.bmp, ... .
                // ACT material bitmapId is a runtime resource pointer, not
                // that directory entry number; the material slot selects the
                // corresponding exported bitmap for these retail assets.
                const auto rawPath = bitmapDirectory / (stem + "_" + std::to_string(index) + ".rgba");
                int textureWidth = 0, textureHeight = 0;
                std::vector<uint8_t> rawPixels;
                SDL_Surface* rgba = nullptr;
                const void* texturePixels = nullptr;
                if (std::filesystem::is_regular_file(rawPath)) {
                    std::ifstream stream(rawPath, std::ios::binary);
                    uint32_t width = 0, height = 0;
                    stream.read(reinterpret_cast<char*>(&width), sizeof(width));
                    stream.read(reinterpret_cast<char*>(&height), sizeof(height));
                    if (!stream || width == 0 || height == 0 || width > 16384 || height > 16384)
                        throw std::runtime_error("Invalid equipment ACT RGBA header: " + rawPath.string());
                    textureWidth = int(width);
                    textureHeight = int(height);
                    rawPixels.resize(size_t(width) * size_t(height) * 4);
                    stream.read(reinterpret_cast<char*>(rawPixels.data()),
                                std::streamsize(rawPixels.size()));
                    if (!stream || stream.peek() != std::char_traits<char>::eof())
                        throw std::runtime_error("Invalid equipment ACT RGBA payload: " + rawPath.string());
                    texturePixels = rawPixels.data();
                } else {
                    const auto path = bitmapDirectory / (stem + "_" + std::to_string(index) + ".bmp");
                    SDL_Surface* surface = SDL_LoadBMP(path.string().c_str());
                    if (!surface)
                        throw std::runtime_error("Cannot load equipment ACT material " + path.string() + ": " + SDL_GetError());
                    rgba = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
                    SDL_FreeSurface(surface);
                    if (!rgba) throw std::runtime_error(std::string("Cannot convert equipment ACT material: ") + SDL_GetError());
                    textureWidth = rgba->w;
                    textureHeight = rgba->h;
                    texturePixels = rgba->pixels;
                }

                GLuint texture = 0;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, texturePixels);
                if (rgba) SDL_FreeSurface(rgba);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                materialTextures[size_t(index)] = texture;
            }
            buildBindPoseArrays();
        } catch (...) {
            release();
            throw;
        }
    }

    int draw(const ActorTransform& root, int motion, float time) const {
        const auto vertices = pose.vertices(motion, time);
        const J lods = field(asset, "lods");
        if (count(lods) < 1) throw std::runtime_error("Equipment ACT has no LOD");
        const J faces = at(lods, 0);
        const J materials = field(asset, "materials");

        glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.f);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        int triangles = 0;
        for (int faceIndex = 0; faceIndex < count(faces); ++faceIndex) {
            const J face = at(faces, faceIndex);
            const int materialIndex = integer(field(face, "material"));
            if (materialIndex < 0 || materialIndex >= count(materials)) {
                glPopAttrib();
                throw std::runtime_error("Equipment ACT face references an invalid material");
            }
            const J material = at(materials, materialIndex);
            const auto texture = materialTextures[size_t(materialIndex)];
            const J color = field(material, "color");
            glColor4f(num(at(color, 0)), num(at(color, 1)), num(at(color, 2)), 1.0f);
            if (texture) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture);
            } else {
                glDisable(GL_TEXTURE_2D);
            }

            const J indices = field(face, "vertices");
            glBegin(GL_TRIANGLES);
            for (int corner = 0; corner < 3; ++corner) {
                const int index = integer(at(indices, corner));
                if (index < 0 || index >= int(vertices.size())) {
                    glEnd();
                    glPopAttrib();
                    throw std::runtime_error("Equipment ACT face references an invalid vertex");
                }
                const J source = at(field(asset, "vertices"), index);
                const J uv = field(source, "uv");
                const V point = root.point(vertices[size_t(index)]);
                if (texture) glTexCoord2f(num(at(uv, 0)), num(at(uv, 1)));
                glVertex3f(point.x, point.y, point.z);
            }
            glEnd();
            ++triangles;
        }
        glPopAttrib();
        return triangles;
    }

    int drawBindPose(const ActorTransform& root) const {
        if (bindPoseMaterialVertices.empty()) return draw(root, -1, 0.f);

        glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.f);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glPushMatrix();
        const GLfloat matrix[16] = {
            root.r[0], root.r[3], root.r[6], 0.f,
            root.r[1], root.r[4], root.r[7], 0.f,
            root.r[2], root.r[5], root.r[8], 0.f,
            root.t.x, root.t.y, root.t.z, 1.f,
        };
        glMultMatrixf(matrix);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        int triangles = 0;
        const J materials = field(asset, "materials");
        for (size_t materialIndex = 0; materialIndex < bindPoseMaterialVertices.size(); ++materialIndex) {
            const auto& vertices = bindPoseMaterialVertices[materialIndex];
            if (vertices.empty()) continue;
            const J material = at(materials, int(materialIndex));
            const J color = field(material, "color");
            glColor4f(num(at(color, 0)), num(at(color, 1)), num(at(color, 2)), 1.f);
            const GLuint texture = materialTextures[materialIndex];
            if (texture) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture);
            } else {
                glDisable(GL_TEXTURE_2D);
            }
            glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(StaticVertex)),
                            vertices.front().position);
            glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(StaticVertex)),
                              vertices.front().uv);
            glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size()));
            triangles += int(vertices.size() / 3);
        }
        glPopMatrix();
        glPopClientAttrib();
        glPopAttrib();
        return triangles;
    }

    void buildBindPoseArrays() {
        const J lods = field(asset, "lods");
        if (count(lods) < 1) throw std::runtime_error("Equipment ACT has no LOD");
        const J faces = at(lods, 0);
        const J materials = field(asset, "materials");
        bindPoseMaterialVertices.assign(size_t(count(materials)), {});
        const auto vertices = pose.vertices(-1, 0.f);
        const J sourceVertices = field(asset, "vertices");
        for (int faceIndex = 0; faceIndex < count(faces); ++faceIndex) {
            const J face = at(faces, faceIndex);
            const int material = integer(field(face, "material"));
            if (material < 0 || material >= count(materials))
                throw std::runtime_error("Equipment ACT face references an invalid material");
            const J indices = field(face, "vertices");
            auto& target = bindPoseMaterialVertices[size_t(material)];
            for (int corner = 0; corner < 3; ++corner) {
                const int index = integer(at(indices, corner));
                if (index < 0 || index >= int(vertices.size()))
                    throw std::runtime_error("Equipment ACT face references an invalid vertex");
                const J uv = field(at(sourceVertices, index), "uv");
                const V point = vertices[size_t(index)];
                target.push_back({{point.x, point.y, point.z},
                                  {num(at(uv, 0)), num(at(uv, 1))}});
            }
        }
    }

    void release() {
        for (GLuint texture : materialTextures)
            if (texture) glDeleteTextures(1, &texture);
        materialTextures.clear();
        bindPoseMaterialVertices.clear();
    }
};
