#pragma once
#include "SoundSystem.h"
#include <memory>
namespace dae
{

    class SdlSoundSystem final : public SoundSystem
    {
    public:
        SdlSoundSystem();
        ~SdlSoundSystem() override;

        SdlSoundSystem(const SdlSoundSystem&) = delete;
        SdlSoundSystem(SdlSoundSystem&&) = delete;
        SdlSoundSystem& operator=(const SdlSoundSystem&) = delete;
        SdlSoundSystem& operator=(SdlSoundSystem&&) = delete;

        void play(const sound_id id, const float volume, int loops = 0) override;
        void load(const sound_id id, const std::string& filepath, bool isMusic = false) override;

    private:
        class SdlSoundSystemImpl;
        std::unique_ptr<SdlSoundSystemImpl> m_pImpl;
    };
}