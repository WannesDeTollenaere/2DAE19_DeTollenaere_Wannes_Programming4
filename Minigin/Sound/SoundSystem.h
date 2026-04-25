#pragma once

using sound_id = unsigned short;

class SoundSystem
{
public:
	virtual ~SoundSystem() = default;
	virtual void play(const sound_id id, const float volume) = 0;
};

class NullSoundSystem final : public SoundSystem
{
	void play(const sound_id, const float) override {}
};