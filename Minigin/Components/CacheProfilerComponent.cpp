#include "CacheProfilerComponent.h"
#include <imgui.h>
#include <chrono>
#include <numeric>
#include <algorithm>

namespace dae
{
    CacheProfilerComponent::CacheProfilerComponent(GameObject* owner) : Component(owner) {}


    void CacheProfilerComponent::RenderGUI()
    {
        auto DrawPlot = [](const char* label, const std::vector<float>& data, ImVec4 color, int highlightIndex = -1)
            {
                if (data.empty()) return;

                float maxVal = *std::max_element(data.begin(), data.end());
                if (maxVal == 0.0f) maxVal = 1.0f; 

                ImGui::PushStyleColor(ImGuiCol_PlotLines, color);
                ImGui::PlotLines(label, data.data(), static_cast<int>(data.size()), 0, nullptr, 0.0f, maxVal, ImVec2(250, 100));
                ImGui::PopStyleColor();

                ImVec2 p0 = ImGui::GetItemRectMin(); 
                ImVec2 p1 = ImGui::GetItemRectMax();
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                int count = static_cast<int>(data.size());

                // VERTICAL LINE
                if (highlightIndex >= 0 && highlightIndex < count)
                {
                    float xPos = p0.x + (p1.x - p0.x) * (static_cast<float>(highlightIndex) / (count - 1));
                    drawList->AddLine(ImVec2(xPos, p0.y), ImVec2(xPos, p1.y), IM_COL32(255, 0, 0, 255), 1.5f);
                }
                if (ImGui::IsItemHovered())
                {
                    ImVec2 mousePos = ImGui::GetMousePos();
                    float relX = std::clamp((mousePos.x - p0.x) / (p1.x - p0.x), 0.0f, 1.0f);
                    int idx = std::clamp(static_cast<int>(relX * (count - 1) + 0.5f), 0, count - 1);

                    int stepSize = 1 << idx;
                    ImGui::SetTooltip("Step Size (X): %d\nTime (Y): %.2f us", stepSize, data[idx]);
                }
            };

        ImGui::Begin("Exercise 1");
        ImGui::InputInt("# samples##1", &m_samplesEx1);


        if (ImGui::Button("Thrash the cache##1"))
        {
            MeasureCacheThrashing<int>(
                m_plotEx1,
                m_samplesEx1,
                10000000,
                [](int& val) { val *= 2; } 
            );
        }

        DrawPlot("##PlotEx1", m_plotEx1, ImVec4(1.0f, 0.5f, 0.0f, 1.0f), 4);
        ImGui::End();


        ImGui::Begin("Exercise 2");
        ImGui::InputInt("# samples##2", &m_samplesEx2);

        if (ImGui::Button("Thrash the cache with GameObject3D"))
        {
            MeasureCacheThrashing<GameObject3D>(
                m_plotEx2_1,
                m_samplesEx2,
                1000000,
                [](GameObject3D& obj) { obj.id *= 2; } 
            );
        }
        DrawPlot("##PlotEx2_1", m_plotEx2_1, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

        if (ImGui::Button("Thrash the cache with GameObject3DAlt"))
        {
            MeasureCacheThrashing<GameObject3DAlt>(
                m_plotEx2_2,
                m_samplesEx2,
                1000000,
                [](GameObject3DAlt& obj) { obj.id *= 2; } 
            );
        }

        DrawPlot("##PlotEx2_2", m_plotEx2_2, ImVec4(0.0f, 0.8f, 1.0f, 1.0f));

        if (!m_plotEx2_1.empty() && !m_plotEx2_2.empty())
        {
            ImGui::Text("Combined:");

            float max1 = *std::max_element(m_plotEx2_1.begin(), m_plotEx2_1.end());
            float max2 = *std::max_element(m_plotEx2_2.begin(), m_plotEx2_2.end());
            float combinedMax = std::max(max1, max2);
            if (combinedMax == 0.0f) combinedMax = 1.0f;

            ImVec2 plotSize(250, 100);

            ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::PlotLines("##Combined1", m_plotEx2_1.data(), static_cast<int>(m_plotEx2_1.size()), 0, nullptr, 0.0f, combinedMax, plotSize);
            ImGui::PopStyleColor();

            // plot one over the other
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - plotSize.y - ImGui::GetStyle().ItemSpacing.y);
            ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 0.8f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
            ImGui::PlotLines("##Combined2", m_plotEx2_2.data(), static_cast<int>(m_plotEx2_2.size()), 0, nullptr, 0.0f, combinedMax, plotSize);
            ImGui::PopStyleColor(2);

            if (ImGui::IsItemHovered())
            {
                ImVec2 mousePos = ImGui::GetMousePos();
                ImVec2 p0 = ImGui::GetItemRectMin();
                ImVec2 p1 = ImGui::GetItemRectMax();

                float relX = std::clamp((mousePos.x - p0.x) / (p1.x - p0.x), 0.0f, 1.0f);
                int count = static_cast<int>(m_plotEx2_1.size());
                int idx = std::clamp(static_cast<int>(relX * (count - 1) + 0.5f), 0, count - 1);


                int stepSize = 1 << idx;
                ImGui::SetTooltip("Step Size (X): %d\nGameObject3D (Y): %.2f us\nGameObject3DAlt (Y): %.2f us",
                    stepSize, m_plotEx2_1[idx], m_plotEx2_2[idx]);
            }
        }
        ImGui::End();
    }
}