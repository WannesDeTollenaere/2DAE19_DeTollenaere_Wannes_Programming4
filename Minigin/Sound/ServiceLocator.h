#pragma once	
#include "SoundSystem.h"
#include <memory>
#include <utility>

namespace dae
{
	class ServiceLocator final
	{
		inline static std::unique_ptr<SoundSystem> m_SsInstance;
	public:
		static SoundSystem& GetSoundSystem() { return *m_SsInstance; }
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss) {
			m_SsInstance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
		}
	};
}