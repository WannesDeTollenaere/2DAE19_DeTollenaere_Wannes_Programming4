#pragma once	
#include "SoundSystem.h"
#include <memory>
#include <utility>

class ServiceLocator final
{
	static std::unique_ptr<SoundSystem> m_SsInstance;
public:
	static SoundSystem& get_sound_system() { return *m_SsInstance; }
	static void register_sound_system(std::unique_ptr<SoundSystem>&& ss) { m_SsInstance = std::move(ss); }
	static void register_sound_system(std::unique_ptr<SoundSystem>&& ss) {
		m_SsInstance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
	}
};