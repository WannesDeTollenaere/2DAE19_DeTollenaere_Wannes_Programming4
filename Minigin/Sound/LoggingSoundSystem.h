#pragma once
#include "SoundSystem.h"
#include <memory>
#include <iostream>

class LoggingSoundSystem final : public SoundSystem {
	std::unique_ptr<SoundSystem> m_RealSs;
public:
	LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : m_RealSs(std::move(ss)) {}
	virtual ~LoggingSoundSystem() = default;
	void play(const sound_id id, const float volume) override {
		m_RealSs->play(id, volume);
		std::cout << "playing " << id << " at volume " << volume << std::endl;
	}
};