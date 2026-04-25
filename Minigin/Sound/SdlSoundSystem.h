#pragma once
#include "SoundSystem.h"


class SdlSoundSystem final : public SoundSystem 
{
public:
	void play(const sound_id id, const float volume) override;
};
