#pragma once
#include "SoundSystem.h"
#include <memory>
#include <iostream>

namespace dae
{
	class LoggingSoundSystem final : public SoundSystem {
		std::unique_ptr<SoundSystem> m_RealSs;
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : m_RealSs(std::move(ss)) {}
		virtual ~LoggingSoundSystem() = default;
		void play(const sound_id id, const float volume, int loops = 0) override {
			m_RealSs->play(id, volume, loops);
			std::cout << "playing " << id << " at volume " << volume << " with loops " << loops << std::endl;
		}

		void load(const sound_id id, const std::string& path, bool isMusic = false) override
		{
			m_RealSs->load(id, path, isMusic);
			std::cout << "loading " << id << " from path " << path << " (isMusic: " << isMusic << ")" << std::endl;
		}

		void ToggleMute() override 
		{
			m_RealSs->ToggleMute();
			std::cout << "Toggling mute" << std::endl;
		}
	};
}