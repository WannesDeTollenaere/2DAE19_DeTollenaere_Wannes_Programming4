#pragma once
#include "Component.h"
#include <vector>
#include <chrono>
#include <numeric> 
#include <algorithm>


namespace dae
{
    struct TestTransform
    {
        float matrix[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };

    class GameObject3D
    {
    public:
        TestTransform local{};
        int id{ 1 };
    };

    class GameObject3DAlt
    {
    public:
        TestTransform* local{};
        int id{ 1 };
    };

    class CacheProfilerComponent final : public Component
    {
    public:
        CacheProfilerComponent(GameObject* owner);
        ~CacheProfilerComponent() = default;

        void RenderGUI() override;

    private:
        // EX1
        int m_samplesEx1{ 10 };
        std::vector<float> m_plotEx1{};

        // EX2
        int m_samplesEx2{ 100 };
        std::vector<float> m_plotEx2_1{};
        std::vector<float> m_plotEx2_2{};

        template <typename T, typename Operation>
        void MeasureCacheThrashing(std::vector<float>& outPlotData, int samples, int bufferSize, Operation op)
        {
            outPlotData.clear();

            std::vector<T> buffer(bufferSize);

            for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
            {
                std::vector<float> sampleDurations;
                sampleDurations.reserve(samples);

                for (int s = 0; s < samples; ++s)
                {
                    auto start = std::chrono::high_resolution_clock::now();

                    for (size_t i = 0; i < buffer.size(); i += stepsize)
                    {
                        op(buffer[i]);
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                    sampleDurations.push_back(static_cast<float>(duration));
                }

                float avg = 0.f;

                if (samples > 2) {
                    auto minmax = std::minmax_element(sampleDurations.begin(), sampleDurations.end());
                    float totalSum = std::accumulate(sampleDurations.begin(), sampleDurations.end(), 0.0f);
                    float trimmedSum = totalSum - (*minmax.first) - (*minmax.second);

                    avg = trimmedSum / (samples - 2);
                } else if (samples > 0){
                    float totalSum = std::accumulate(sampleDurations.begin(), sampleDurations.end(), 0.0f);
                    avg = totalSum / samples;
                }

                outPlotData.push_back(avg);
            }
        }
    };
}