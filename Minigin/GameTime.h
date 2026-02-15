#pragma once
#include <chrono>
#include "Singleton.h"

namespace dae
{
    class GameTime final : public Singleton<GameTime>
    {
    public:
        void Initialize() {
            m_startTime = std::chrono::high_resolution_clock::now();
            m_lastFrameTime = m_startTime;
        }

        void Tick() {
            auto currentFrameTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> duration = currentFrameTime - m_lastFrameTime;

            m_unscaledDeltaTime = duration.count();

            if (m_unscaledDeltaTime > 0.1f) {
                m_unscaledDeltaTime = 0.1f;
            }

            m_deltaTime = m_unscaledDeltaTime * m_timeScale;

            std::chrono::duration<float> totalDuration = currentFrameTime - m_startTime;
            m_totalTime = totalDuration.count();

            m_lastFrameTime = currentFrameTime;
        }

        float GetDeltaTime() const { return m_deltaTime; }
        float GetUnscaledDeltaTime() const { return m_unscaledDeltaTime; }
        float GetTotalTime() const { return m_totalTime; }
        float GetTimeScale() const { return m_timeScale; }

        void SetTimeScale(float scale) { m_timeScale = scale; }

    private:
        friend class Singleton<GameTime>;
        GameTime() : m_deltaTime(0.0f), m_unscaledDeltaTime(0.0f), m_totalTime(0.0f), m_timeScale(1.0f) {}

        float m_deltaTime;
        float m_unscaledDeltaTime;
        float m_totalTime;
        float m_timeScale;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime;
    };
}