#pragma once
#include "Component.h"
#include <vector>
#include <chrono>
#include <numeric>


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
                float sampleTime = 0.f;

                for (int s = 0; s < samples; ++s)
                {
                    auto start = std::chrono::high_resolution_clock::now();

                    for (size_t i = 0; i < buffer.size(); i += stepsize)
                    {
                        op(buffer[i]);
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                    sampleTime += duration;
                }

                float avg = sampleTime/samples;
                outPlotData.push_back(avg);
            }
        }
    };
}