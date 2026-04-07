#pragma once
#include <chrono>
#include <thread>
#include <functional>
#include <vector>
#include <algorithm>
#include "Singleton.h"

namespace dae
{
    struct TimerHandle
    {
        float timeLeft;
        float duration;
        bool isLooping;
        std::function<void()> callback;
        bool isDone{ false };
    };

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
            m_deltaTime = m_unscaledDeltaTime * m_timeScale;

            m_lag += m_deltaTime;

            std::chrono::duration<float> totalDuration = currentFrameTime - m_startTime;
            m_totalTime = totalDuration.count();

            m_lastFrameTime = currentFrameTime;

            for (auto& timer : m_Timers)
            {
                if (timer.isDone) continue;

                timer.timeLeft -= m_deltaTime;
                if (timer.timeLeft <= 0.0f)
                {
                    timer.callback();

                    if (timer.isLooping)
                    {
                        timer.timeLeft = timer.duration;
                    }
                    else
                    {
                        timer.isDone = true;
                    }
                }
            }

            m_Timers.erase(std::remove_if(m_Timers.begin(), m_Timers.end(),
                [](const TimerHandle& t) { return t.isDone; }), m_Timers.end());

        }

        void AddTimer(float duration, std::function<void()> callback, bool loop = false)
        {
            TimerHandle newTimer{ duration, duration, loop, std::move(callback), false };

            m_Timers.push_back(std::move(newTimer));
            
        }

        void ClearAllTimers()
        {
            m_Timers.clear();
        }

        float GetDeltaTime() const { return m_deltaTime; }
        float GetUnscaledDeltaTime() const { return m_unscaledDeltaTime; }
        float GetTotalTime() const { return m_totalTime; }
        float GetTimeScale() const { return m_timeScale; }

        float GetFixedTimeStep() const { return m_fixedTimeStep; }
        float GetLag() const { return m_lag; }
        int GetMsPerFrame() const { return m_msPerFrame; }

        bool ShouldDoFixedUpdate()
        {
            if (m_lag >= m_fixedTimeStep)
            {
                m_lag -= m_fixedTimeStep;
                return true;
            }
            return false;
        }

        void Sleep() const
        {
#ifndef __EMSCRIPTEN__
            auto sleep_time = m_lastFrameTime + std::chrono::milliseconds(m_msPerFrame) - std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(sleep_time);
#endif
        }

        void SetTimeScale(float scale) { m_timeScale = scale; }

    private:
        friend class Singleton<GameTime>;

        GameTime() : m_deltaTime(0.0f), m_unscaledDeltaTime(0.0f), m_totalTime(0.0f), m_timeScale(1.0f), m_lag(0.0f) {}

        float m_deltaTime;
        float m_unscaledDeltaTime;
        float m_totalTime;
        float m_timeScale;

        float m_lag;
        const float m_fixedTimeStep{ 0.02f }; // 50 updates per frame
        const int m_msPerFrame{ 6 };         // 144 fps

        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime;

        std::vector<TimerHandle> m_Timers;
    };
}