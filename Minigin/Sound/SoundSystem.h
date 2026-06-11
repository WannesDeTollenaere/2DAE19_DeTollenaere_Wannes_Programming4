#pragma once
#include <string>

namespace dae
{
    using sound_id = unsigned short;

    class SoundSystem
    {
    public:
        virtual ~SoundSystem() = default;
        virtual void play(const sound_id id, const float volume, int loops = 0) = 0;
        virtual void load(const sound_id id, const std::string& filepath, bool isMusic = false) = 0;
        virtual void ToggleMute() = 0;
    };

    class NullSoundSystem final : public SoundSystem
    {
    public:
        void play(const sound_id, const float, int) override {}
        void load(const sound_id, const std::string&, bool) override {}
        void ToggleMute() override {}
    };
}