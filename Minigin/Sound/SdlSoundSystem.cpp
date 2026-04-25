#include "SdlSoundSystem.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <iostream>

namespace dae {
    struct SoundRequest {
        enum class Type { Load, Play };
        Type type;
        sound_id id;
        float volume{ 1.0f };
        std::string filepath{ "" };
        bool isMusic{ false }; // for streaming, distinguesh sound fx and music
        int loops{ 0 };
    };

    class SdlSoundSystem::SdlSoundSystemImpl {
    public:
        SdlSoundSystemImpl() {
            if (!MIX_Init()) {
                std::cerr << "MIX_Init Error: " << SDL_GetError() << std::endl;
            }

            m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

            if (!m_mixer) {
                std::cerr << "MIX_CreateMixerDevice Error: " << SDL_GetError() << std::endl;
            }

#ifndef __EMSCRIPTEN__
            m_thread = std::jthread([this](std::stop_token st) { AudioThread(st); });
#endif
        }

        ~SdlSoundSystemImpl() {
#ifndef __EMSCRIPTEN__
            m_thread.request_stop();
            
            // wait for the thread to finish processing its current item
            if (m_thread.joinable()) {
                m_thread.join();
            }
#endif


            // clean up tracks and audio data
            for (auto& pair : m_tracks) {
                MIX_DestroyTrack(pair.second);
            }
            for (auto& pair : m_audio) {
                MIX_DestroyAudio(pair.second);
            }

            if (m_mixer) {
                MIX_DestroyMixer(m_mixer);
            }
            MIX_Quit();
        }

        void play(const sound_id id, const float volume, int loops) {
#ifndef __EMSCRIPTEN__
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push({ SoundRequest::Type::Play, id, volume, "", false, loops });
            m_condition.notify_one();
#else
            ProcessRequest({ SoundRequest::Type::Play, id, volume, "", false, loops });
#endif
        }

        void load(const sound_id id, const std::string& filepath, bool isMusic) {
#ifndef __EMSCRIPTEN__
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push({ SoundRequest::Type::Load, id, 1.0f, filepath, isMusic, 0 });
            m_condition.notify_one();
#else
            ProcessRequest({ SoundRequest::Type::Load, id, 1.0f, filepath, isMusic, 0 });
#endif
        }

    private:
#ifndef __EMSCRIPTEN__
        void AudioThread(std::stop_token stopToken) {
            while (!stopToken.stop_requested()) {
                SoundRequest request;
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    bool hasWork = m_condition.wait(lock, stopToken, [this]() {
                        return !m_queue.empty();
                        });

                    if (!hasWork) break;

                    request = m_queue.front();
                    m_queue.pop();
                }
                ProcessRequest(request);
            }
        }
#endif

        void ProcessRequest(const SoundRequest& request) {
            if (!m_mixer) return;

            switch (request.type) {
            case SoundRequest::Type::Load: {
                if (m_audio.contains(request.id)) {
                    std::cout << "Sound ID " << request.id << " is already loaded. Skipping.\n";
                    break; 
                }

                MIX_Audio* audio = MIX_LoadAudio(m_mixer, request.filepath.c_str(), !request.isMusic);
                if (audio == nullptr) {
                    std::cerr << "Failed to load sound: " << request.filepath << " - " << SDL_GetError() << std::endl;
                }
                else {
                    m_audio[request.id] = audio;

                    MIX_Track* track = MIX_CreateTrack(m_mixer);
                    MIX_SetTrackAudio(track, audio);
                    m_tracks[request.id] = track;
                }
                break;
            }
            case SoundRequest::Type::Play: {
                auto it = m_tracks.find(request.id);
                if (it != m_tracks.end()) {
                    MIX_Track* track = it->second;
                    MIX_SetTrackGain(track, request.volume);

					// LOOPS: -1 means loop indefinitely, 0 means play once, n means play n+1 times
                    if (request.loops != 0) {
                        SDL_PropertiesID props = SDL_CreateProperties();
                        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, request.loops);

                        MIX_PlayTrack(track, props);

                        SDL_DestroyProperties(props);
                    }
                    else {
                        MIX_PlayTrack(track, 0);
                    }
                }
                else {
                    std::cerr << "Tried to play unloaded sound ID: " << request.id << std::endl;
                }
                break;
            }
            }
        }

#ifndef __EMSCRIPTEN__
        std::mutex m_mutex;
        std::condition_variable_any m_condition;
        std::queue<SoundRequest> m_queue;
        std::jthread m_thread;
#endif

        // SDL Mixer
        MIX_Mixer* m_mixer{ nullptr };
        std::unordered_map<sound_id, MIX_Audio*> m_audio;
        std::unordered_map<sound_id, MIX_Track*> m_tracks;
    };


    // PIMPL

    SdlSoundSystem::SdlSoundSystem()
        : m_pImpl(std::make_unique<SdlSoundSystemImpl>())
    {
    }

    SdlSoundSystem::~SdlSoundSystem() = default;

    void SdlSoundSystem::play(const sound_id id, const float volume, int loops) {
        m_pImpl->play(id, volume, loops);
    }

    void SdlSoundSystem::load(const sound_id id, const std::string& filepath, bool isMusic) {
        m_pImpl->load(id, filepath, isMusic);
    }
}