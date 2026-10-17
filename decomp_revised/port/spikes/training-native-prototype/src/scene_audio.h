#pragma once

#include <SDL.h>
#include <json-c/json.h>
#include <mpg123.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <limits>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "scene_data.h"

struct SceneAudio {
    struct MusicSelection {
        std::string track;
        std::filesystem::path path;
    };

    struct Sample {
        std::vector<uint8_t> stereoPcm;
        bool spatial = false;
        float minimumDistance = 0.f;
        float maximumDistance = 0.f;
    };

    struct Voice {
        const Sample* sample = nullptr;
        std::string foldedName;
        int owner = 0;
        size_t frameOffset = 0;
        V source{};
        V listener{};
        float yaw = 0.f;
        int channelGroup = 0;
        bool ownerAware = false;
        bool looping = false;
    };

    SDL_AudioDeviceID device = 0;
    SDL_AudioSpec output{};
    std::map<std::string, Sample> samples;
    std::vector<Voice> voices;
    // FMOD's +0x690 MP3 stream is independent from the sample-category voices.
    // Decode the selected loop before playback; the real-time callback only mixes PCM.
    std::vector<uint8_t> musicPcm;
    size_t musicFrameOffset = 0;
    float musicGain = 1.f;
    std::string musicTrack;
    // Raw retail sound-instance category IDs from +0x14. The oxygen visor
    // selector changes categories 1 and 2; streamed music uses a separate channel.
    std::array<float, 3> soundGroupGains{1.f, 1.f, 1.f};
    bool loaded = false;

    static std::string fold(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });
        return value;
    }

    static void audioCallback(void* userdata, Uint8* stream, int length) {
        auto* self = static_cast<SceneAudio*>(userdata);
        if (!self || !stream || length <= 0) return;
        std::memset(stream, 0, static_cast<size_t>(length));
        if (self->output.format != AUDIO_S16SYS || self->output.channels != 2) return;

        auto* outputSamples = reinterpret_cast<int16_t*>(stream);
        const size_t outputFrames = static_cast<size_t>(length) / (2 * sizeof(int16_t));
        size_t writeIndex = 0;
        for (size_t voiceIndex = 0; voiceIndex < self->voices.size(); ++voiceIndex) {
            Voice& voice = self->voices[voiceIndex];
            if (!voice.sample) continue;
            const size_t sampleFrames = voice.sample->stereoPcm.size() / (2 * sizeof(int16_t));
            if (sampleFrames == 0) continue;
            const float groupGain = self->soundGroupGainForVoice(voice);
            const float leftGain = self->channelGain(voice, false) * groupGain;
            const float rightGain = self->channelGain(voice, true) * groupGain;
            const auto* sourceSamples = reinterpret_cast<const int16_t*>(voice.sample->stereoPcm.data());
            for (size_t frame = 0; frame < outputFrames; ++frame) {
                if (voice.frameOffset >= sampleFrames) {
                    if (voice.looping) voice.frameOffset %= sampleFrames;
                    else break;
                }
                const size_t sampleIndex = voice.frameOffset * 2;
                const size_t outputIndex = frame * 2;
                outputSamples[outputIndex] = clampMix(
                    static_cast<float>(outputSamples[outputIndex]) +
                    static_cast<float>(sourceSamples[sampleIndex]) * leftGain);
                outputSamples[outputIndex + 1] = clampMix(
                    static_cast<float>(outputSamples[outputIndex + 1]) +
                    static_cast<float>(sourceSamples[sampleIndex + 1]) * rightGain);
                ++voice.frameOffset;
            }
            if (voice.looping && voice.frameOffset >= sampleFrames)
                voice.frameOffset %= sampleFrames;
        }

        // Remove completed one-shot voices in place; the callback allocates no memory.
        for (size_t i = 0; i < self->voices.size(); ++i) {
            Voice& voice = self->voices[i];
            const size_t sampleFrames = voice.sample
                ? voice.sample->stereoPcm.size() / (2 * sizeof(int16_t)) : 0;
            const bool finished = !voice.looping && (sampleFrames == 0 || voice.frameOffset >= sampleFrames);
            if (finished) continue;
            if (writeIndex != i) self->voices[writeIndex] = std::move(voice);
            ++writeIndex;
        }
        self->voices.resize(writeIndex);

        const size_t musicFrames = self->musicPcm.size() / (2 * sizeof(int16_t));
        if (musicFrames != 0) {
            const auto* musicSamples = reinterpret_cast<const int16_t*>(self->musicPcm.data());
            for (size_t frame = 0; frame < outputFrames; ++frame) {
                if (self->musicFrameOffset >= musicFrames)
                    self->musicFrameOffset %= musicFrames;
                const size_t sourceIndex = self->musicFrameOffset * 2;
                const size_t outputIndex = frame * 2;
                outputSamples[outputIndex] = clampMix(
                    static_cast<float>(outputSamples[outputIndex]) +
                    static_cast<float>(musicSamples[sourceIndex]) * self->musicGain);
                outputSamples[outputIndex + 1] = clampMix(
                    static_cast<float>(outputSamples[outputIndex + 1]) +
                    static_cast<float>(musicSamples[sourceIndex + 1]) * self->musicGain);
                ++self->musicFrameOffset;
            }
            if (self->musicFrameOffset >= musicFrames)
                self->musicFrameOffset %= musicFrames;
        }
    }

    bool open() {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
            std::cerr << "Scene audio unavailable: " << SDL_GetError() << "\n";
            return false;
        }
        SDL_AudioSpec wanted{};
        wanted.freq = 44100;
        wanted.format = AUDIO_S16SYS;
        wanted.channels = 2;
        wanted.samples = 2048;
        wanted.callback = &SceneAudio::audioCallback;
        wanted.userdata = this;
        device = SDL_OpenAudioDevice(nullptr, 0, &wanted, &output,
                                     SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
        if (!device) {
            std::cerr << "Scene audio unavailable: " << SDL_GetError() << "\n";
            return false;
        }
        SDL_PauseAudioDevice(device, 0);
        return true;
    }

    void load(const SceneData& scene, const MusicSelection* inheritedMusic = nullptr) {
        if (!device) return;
        if (!loaded) {
            if (scene.sounds()) {
                SDL_LockAudioDevice(device);
                json_object* resources = SceneData::field(scene.sounds(), "resources");
                const size_t count = resources && json_object_get_type(resources) == json_type_array
                    ? json_object_array_length(resources) : 0;
                for (size_t i = 0; i < count; ++i) {
                    json_object* resource = json_object_array_get_idx(resources, i);
                    const std::string name = SceneData::stringField(resource, "name");
                    const std::string relativePath = SceneData::stringField(resource, "file");
                    if (name.empty() || relativePath.empty()) continue;
                    const auto path = scene.resolveAsset(relativePath);
                    SDL_AudioSpec source{};
                    Uint8* wav = nullptr;
                    Uint32 wavLength = 0;
                    if (!SDL_LoadWAV(path.string().c_str(), &source, &wav, &wavLength)) {
                        std::cerr << "Scene sound failed to load " << relativePath << ": " << SDL_GetError() << "\n";
                        continue;
                    }
                    SDL_AudioCVT conversion{};
                    const int conversionNeeded = SDL_BuildAudioCVT(&conversion, source.format, source.channels,
                                                                   source.freq, output.format, output.channels,
                                                                   output.freq);
                    if (conversionNeeded < 0) {
                        std::cerr << "Scene sound format is unsupported for " << relativePath << "\n";
                        SDL_FreeWAV(wav);
                        continue;
                    }
                    Sample sample;
                    sample.stereoPcm.resize(size_t(wavLength) * size_t(std::max(conversion.len_mult, 1)));
                    std::memcpy(sample.stereoPcm.data(), wav, wavLength);
                    SDL_FreeWAV(wav);
                    size_t convertedLength = wavLength;
                    if (conversionNeeded != 0) {
                        conversion.buf = sample.stereoPcm.data();
                        conversion.len = static_cast<int>(wavLength);
                        if (SDL_ConvertAudio(&conversion) != 0) {
                            std::cerr << "Scene sound conversion failed for " << relativePath << ": " << SDL_GetError() << "\n";
                            continue;
                        }
                        convertedLength = static_cast<size_t>(conversion.len_cvt);
                    }
                    sample.stereoPcm.resize(convertedLength);
                    sample.spatial = SceneData::stringField(resource, "kind") == "3D";
                    sample.minimumDistance = static_cast<float>(json_object_get_double(
                        SceneData::field(resource, "minimum_distance")));
                    sample.maximumDistance = static_cast<float>(json_object_get_double(
                        SceneData::field(resource, "maximum_distance")));
                    samples[fold(name)] = std::move(sample);
                }
                SDL_UnlockAudioDevice(device);
            }
            // Voice records hold pointers into this map. Keep all loaded samples immutable afterward.
            loaded = true;
        }
        loadMusic(scene, inheritedMusic);
    }

    // Preserve the original convenience API for unowned one-shot sounds.
    void play(const std::string& name, V source, V listener, float yaw, int channelGroup) {
        startVoice(0, false, name, source, listener, yaw, channelGroup, false);
    }

    // Owner-aware equivalents mirror the retail script sound manager's key.
    void playOwned(int owner, const std::string& name, V source, V listener, float yaw,
                   int channelGroup) {
        startVoice(owner, true, name, source, listener, yaw, channelGroup, false);
    }

    void loopOwned(int owner, const std::string& name, V source, V listener, float yaw,
                   int channelGroup) {
        // Deliberately append a voice every call: retail can accumulate duplicate loops.
        startVoice(owner, true, name, source, listener, yaw, channelGroup, true);
    }

    bool setSoundGroupGain(int channelGroup, float gain) {
        if (channelGroup < 1 || channelGroup > 2) return false;
        const float boundedGain = std::clamp(gain, 0.f, 1.f);
        if (device) SDL_LockAudioDevice(device);
        soundGroupGains[static_cast<size_t>(channelGroup)] = boundedGain;
        if (device) SDL_UnlockAudioDevice(device);
        return true;
    }

    bool applyOxygenVisorCategorySelector(int selector, float attenuatedCategory2Gain) {
        if (selector == 4) {
            const bool changed = setSoundGroupGain(2, attenuatedCategory2Gain);
            if (!changed) return false;
            // Retail computes int(0.3 * 255) = 76 for stream +0x690.
            setMusicGain(float(int(attenuatedCategory2Gain * 255.f)) / 255.f);
            return true;
        }
        if (selector == 5) {
            const bool changed = setSoundGroupGain(1, 1.f) && setSoundGroupGain(2, 1.f);
            if (!changed) return false;
            setMusicGain(1.f);
            return true;
        }
        return false;
    }

    void setMusicGain(float gain) {
        const float boundedGain = std::clamp(gain, 0.f, 1.f);
        if (device) SDL_LockAudioDevice(device);
        musicGain = boundedGain;
        if (device) SDL_UnlockAudioDevice(device);
    }

    void stopMusic() {
        if (device) SDL_LockAudioDevice(device);
        musicPcm.clear();
        musicFrameOffset = 0;
        musicTrack.clear();
        if (device) SDL_UnlockAudioDevice(device);
    }

    float soundGroupGainForVoice(const Voice& voice) const {
        if (voice.channelGroup < 1 || voice.channelGroup > 2) return 1.f;
        return soundGroupGains[static_cast<size_t>(voice.channelGroup)];
    }

    void updateOwned(int owner, const std::string& name, V source, V listener, float yaw) {
        if (!device) return;
        const std::string key = fold(name);
        SDL_LockAudioDevice(device);
        for (Voice& voice : voices) {
            if (voice.ownerAware && voice.owner == owner && voice.foldedName == key) {
                voice.source = source;
                voice.listener = listener;
                voice.yaw = yaw;
            }
        }
        SDL_UnlockAudioDevice(device);
    }

    void updateOwnedListeners(V listener, float yaw) {
        if (!device) return;
        SDL_LockAudioDevice(device);
        for (Voice& voice : voices) {
            if (!voice.ownerAware) continue;
            voice.listener = listener;
            voice.yaw = yaw;
        }
        SDL_UnlockAudioDevice(device);
    }

    void stopOwned(int owner, const std::string& name) {
        if (!device) return;
        const std::string key = fold(name);
        SDL_LockAudioDevice(device);
        voices.erase(std::remove_if(voices.begin(), voices.end(), [&](const Voice& voice) {
            return voice.ownerAware && voice.owner == owner && voice.foldedName == key;
        }), voices.end());
        SDL_UnlockAudioDevice(device);
    }

    void close() {
        if (device) {
            SDL_CloseAudioDevice(device);
            device = 0;
        }
        voices.clear();
        musicPcm.clear();
        musicFrameOffset = 0;
        musicTrack.clear();
        musicGain = 1.f;
        samples.clear();
        soundGroupGains = {1.f, 1.f, 1.f};
        loaded = false;
    }

    ~SceneAudio() { close(); }

private:
    static std::optional<MusicSelection> musicSelectionForScene(const SceneData& scene) {
        const std::string relativePath = SceneData::stringField(scene.music(), "file");
        const std::string requestedTrack = SceneData::stringField(scene.music(), "track");
        if (relativePath.empty() || requestedTrack.empty()) return std::nullopt;
        return MusicSelection{requestedTrack, scene.resolveAsset(relativePath)};
    }

    bool loadMusic(const SceneData& scene, const MusicSelection* inheritedMusic) {
        std::optional<MusicSelection> selectedMusic = musicSelectionForScene(scene);
        if (!selectedMusic && inheritedMusic) selectedMusic = *inheritedMusic;
        if (!selectedMusic) {
            stopMusic();
            return false;
        }
        const std::string& requestedTrack = selectedMusic->track;
        const std::filesystem::path& path = selectedMusic->path;
        if (requestedTrack == musicTrack && !musicPcm.empty()) return true;

        // Retail selector 2 stops/releases the previous stream before opening its replacement.
        stopMusic();
        static std::once_flag mpg123Initialization;
        static int mpg123InitializationResult = MPG123_ERR;
        std::call_once(mpg123Initialization, [] {
            mpg123InitializationResult = mpg123_init();
        });
        if (mpg123InitializationResult != MPG123_OK) {
            std::cerr << "MP3 music decoder initialization failed\n";
            return false;
        }

        int error = MPG123_OK;
        mpg123_handle* decoder = mpg123_new(nullptr, &error);
        if (!decoder) {
            std::cerr << "MP3 music decoder allocation failed: " << mpg123_plain_strerror(error) << "\n";
            return false;
        }
        auto destroyDecoder = [&] { mpg123_delete(decoder); };
        if (mpg123_open_fixed(decoder, path.string().c_str(), MPG123_STEREO,
                              MPG123_ENC_SIGNED_16) != MPG123_OK) {
            std::cerr << "MP3 music failed to open " << path.string() << ": "
                      << mpg123_strerror(decoder) << "\n";
            destroyDecoder();
            return false;
        }

        long sourceRate = 0;
        int sourceChannels = 0, sourceEncoding = 0;
        if (mpg123_getformat(decoder, &sourceRate, &sourceChannels, &sourceEncoding) != MPG123_OK ||
            sourceRate <= 0 || sourceChannels != 2 || sourceEncoding != MPG123_ENC_SIGNED_16) {
            std::cerr << "MP3 music has an unsupported decoded format: " << path.string() << "\n";
            destroyDecoder();
            return false;
        }

        std::vector<uint8_t> decoded;
        std::array<uint8_t, 64 * 1024> buffer{};
        for (;;) {
            size_t produced = 0;
            const int status = mpg123_read(decoder, buffer.data(), buffer.size(), &produced);
            decoded.insert(decoded.end(), buffer.data(), buffer.data() + produced);
            if (status == MPG123_DONE) break;
            if (status == MPG123_NEW_FORMAT) {
                long nextRate = 0;
                int nextChannels = 0, nextEncoding = 0;
                if (mpg123_getformat(decoder, &nextRate, &nextChannels, &nextEncoding) != MPG123_OK ||
                    nextRate != sourceRate || nextChannels != sourceChannels ||
                    nextEncoding != sourceEncoding) {
                    std::cerr << "MP3 music changes format mid-stream: " << path.string() << "\n";
                    destroyDecoder();
                    return false;
                }
                continue;
            }
            if (status != MPG123_OK) {
                std::cerr << "MP3 music decode failed for " << path.string() << ": "
                          << mpg123_strerror(decoder) << "\n";
                destroyDecoder();
                return false;
            }
        }
        destroyDecoder();
        if (decoded.empty()) {
            std::cerr << "MP3 music decoded no audio: " << path.string() << "\n";
            return false;
        }

        SDL_AudioCVT conversion{};
        const int conversionNeeded = SDL_BuildAudioCVT(&conversion, AUDIO_S16SYS, 2,
            static_cast<int>(sourceRate), output.format, output.channels, output.freq);
        if (conversionNeeded < 0) {
            std::cerr << "MP3 music format is unsupported by SDL: " << path.string() << "\n";
            return false;
        }
        std::vector<uint8_t> converted(decoded.size() * size_t(std::max(conversion.len_mult, 1)));
        std::memcpy(converted.data(), decoded.data(), decoded.size());
        size_t convertedLength = decoded.size();
        if (conversionNeeded != 0) {
            if (decoded.size() > size_t(std::numeric_limits<int>::max())) {
                std::cerr << "MP3 music decoded size exceeds SDL conversion limits: " << path.string() << "\n";
                return false;
            }
            conversion.buf = converted.data();
            conversion.len = static_cast<int>(decoded.size());
            if (SDL_ConvertAudio(&conversion) != 0) {
                std::cerr << "MP3 music conversion failed for " << path.string() << ": "
                          << SDL_GetError() << "\n";
                return false;
            }
            convertedLength = static_cast<size_t>(conversion.len_cvt);
        }
        converted.resize(convertedLength);
        if (device) SDL_LockAudioDevice(device);
        musicPcm = std::move(converted);
        musicFrameOffset = 0;
        musicTrack = requestedTrack;
        if (device) SDL_UnlockAudioDevice(device);
        std::cout << "Level music loop started: " << requestedTrack << "\n";
        return true;
    }

    static int16_t clampMix(float value) {
        return static_cast<int16_t>(std::clamp(value, -32768.f, 32767.f));
    }

    float channelGain(const Voice& voice, bool right) const {
        float attenuation = 1.f;
        float pan = 0.f;
        const Sample& sample = *voice.sample;
        if (sample.spatial) {
            const V direction = voice.source - voice.listener;
            const float distance = length(direction);
            if (sample.maximumDistance > 0.f && distance >= sample.maximumDistance) return 0.f;
            if (sample.maximumDistance > sample.minimumDistance && distance > sample.minimumDistance)
                attenuation = (sample.maximumDistance - distance) /
                              (sample.maximumDistance - sample.minimumDistance);
            const V horizontal{direction.x, 0.f, direction.z};
            const float horizontalLength = length(horizontal);
            if (horizontalLength > 0.f) {
                const V rightVector{std::cos(voice.yaw), 0.f, -std::sin(voice.yaw)};
                pan = std::clamp((horizontal.x * rightVector.x + horizontal.z * rightVector.z) /
                                 horizontalLength, -1.f, 1.f);
            }
        }
        // This keeps the prior conservative stereo approximation; FMOD's exact 3D spatializer
        // and rolloff curve are not reproduced.
        return attenuation * 0.55f * std::sqrt((right ? (1.f + pan) : (1.f - pan)) * 0.5f);
    }

    void startVoice(int owner, bool ownerAware, const std::string& name,
                    V source, V listener, float yaw, int channelGroup, bool looping) {
        if (!device) return;
        if (channelGroup < 1 || channelGroup > 2) {
            std::cerr << "Unsupported scene sound group " << channelGroup << " for " << name << "\n";
            return;
        }
        const std::string key = fold(name);
        auto found = samples.find(key);
        if (found == samples.end()) {
            std::cerr << "Unbundled scene sound: " << name << "\n";
            return;
        }
        SDL_LockAudioDevice(device);
        Voice voice;
        voice.sample = &found->second;
        voice.foldedName = key;
        voice.owner = owner;
        voice.ownerAware = ownerAware;
        voice.looping = looping;
        voice.source = source;
        voice.listener = listener;
        voice.yaw = yaw;
        voice.channelGroup = channelGroup;
        voices.push_back(std::move(voice));
        SDL_UnlockAudioDevice(device);
    }
};
