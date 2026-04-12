#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <imgui.h>
#include "Singleton.h"
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    enum class TileType
    {
        Empty = 0,
        Platform = 1,
        Ladder = 2,
        Intersection = 3,
        IntersectionDownOnly = 4
    };

    class LevelGrid final: public Singleton<LevelGrid>
    {
    public:
        void Initialize(int cols, int rows, float tileSize)
        {
            m_Cols = cols;
            m_Rows = rows;
            m_TileSize = tileSize;
            m_Grid.resize(cols * rows, TileType::Empty);
        }

        void SetTile(int x, int y, TileType type)
        {
            if (x >= 0 && x < m_Cols && y >= 0 && y < m_Rows)
                m_Grid[y * m_Cols + x] = type;
        }

        void Resize(int newCols, int newRows)
        {
            if (newCols <= 0 || newRows <= 0) return;

            std::vector<TileType> newGrid(newCols * newRows, TileType::Empty);

            int minCols = std::min(m_Cols, newCols);
            int minRows = std::min(m_Rows, newRows);

            for (int r = 0; r < minRows; ++r)
            {
                for (int c = 0; c < minCols; ++c)
                {
                    newGrid[r * newCols + c] = m_Grid[r * m_Cols + c];
                }
            }

            m_Cols = newCols;
            m_Rows = newRows;
            m_Grid = std::move(newGrid);
        }

        TileType GetTile(int x, int y) const
        {
            if (x >= 0 && x < m_Cols && y >= 0 && y < m_Rows)
                return m_Grid[y * m_Cols + x];
            return TileType::Empty;
        }

        bool IsWalkableHorizontal(TileType type) const
        {
            return type != TileType::Ladder;
        }

        bool IsClimbable(TileType type) const
        {
            return type != TileType::Platform;
        }

        bool CanClimbUp(TileType type) const
        {
            return type == TileType::Ladder || type == TileType::Intersection;
        }

        bool CanClimbDown(TileType type) const
        {
            return type == TileType::Ladder || type == TileType::Intersection || type == TileType::IntersectionDownOnly;
        }

        float GetTileSize() const { return m_TileSize; }

        int GetCols() const { return m_Cols; }
        int GetRows() const { return m_Rows; }

        void RenderGUI()
        {

            static int selectedTileType = 1;
            ImGui::Text("Brush:");
            ImGui::RadioButton("Empty", &selectedTileType, 0); ImGui::SameLine();
            ImGui::RadioButton("Platform", &selectedTileType, 1); ImGui::SameLine();
            ImGui::RadioButton("Ladder", &selectedTileType, 2); ImGui::SameLine();
            ImGui::RadioButton("Intersection", &selectedTileType, 3); ImGui::SameLine();
            ImGui::RadioButton("Down Only", &selectedTileType, 4);
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Grid settings", ImGuiTreeNodeFlags_DefaultOpen))
            {
                static int inputCols = m_Cols;
                static int inputRows = m_Rows;

                if (inputCols == 0) inputCols = m_Cols;
                if (inputRows == 0) inputRows = m_Rows;

                ImGui::InputInt("Columns", &inputCols);
                ImGui::InputInt("Rows", &inputRows);

                if (inputCols < 1) inputCols = 1;
                if (inputRows < 1) inputRows = 1;

                if (ImGui::Button("Apply resize"))
                {
                    Resize(inputCols, inputRows);
                }
                ImGui::Separator();
            }

            ImGui::Text("Click cells to paint:");
            for (int r = 0; r < m_Rows; ++r)
            {
                for (int c = 0; c < m_Cols; ++c)
                {
                    if (c > 0) ImGui::SameLine();

                    ImGui::PushID(r * m_Cols + c); 

                    int currentType = static_cast<int>(GetTile(c, r));

                    std::string label;
                    ImVec4 color;
                    switch (currentType)
                    {
                    case 0: label = " "; color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); break; 
                    case 1: label = "P"; color = ImVec4(0.2f, 0.6f, 0.2f, 1.0f); break; 
                    case 2: label = "L"; color = ImVec4(0.8f, 0.6f, 0.2f, 1.0f); break; 
                    case 3: label = "X"; color = ImVec4(0.8f, 0.2f, 0.2f, 1.0f); break; 
                    case 4: label = "V"; color = ImVec4(0.6f, 0.2f, 0.8f, 1.0f); break;
                    }

                    ImGui::PushStyleColor(ImGuiCol_Button, color);
                    if (ImGui::Button(label.c_str(), ImVec2(30, 30)))
                    {
                        SetTile(c, r, static_cast<TileType>(selectedTileType));
                    }
                    ImGui::PopStyleColor();
                    ImGui::PopID();
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Copy to clipboard"))
            {
                std::stringstream ss;
                ss << "[\n";
                for (int r = 0; r < m_Rows; ++r)
                {
                    ss << "  ";
                    for (int c = 0; c < m_Cols; ++c)
                    {
                        ss << static_cast<int>(GetTile(c, r));
                        if (r < m_Rows - 1 || c < m_Cols - 1) ss << ", ";
                    }
                    ss << "\n";
                }
                ss << "]";

                ImGui::SetClipboardText(ss.str().c_str());
            }


        }

    private:


        int m_Cols{ 0 }, m_Rows{ 0 };
        float m_TileSize{ 16.0f };
        std::vector<TileType> m_Grid;
    };

    class LevelGridSetupParser final : public IComponentParser
    {
    public:
        void Parse(GameObject*, const nlohmann::json& data) override
        {
            int cols = data.value("cols", 0);
            int rows = data.value("rows", 0);
            float tileSize = data.value("tileSize", 16.0f);

            std::vector<int> layout;
            if (data.contains("layout") && data["layout"].is_array())
            {
                layout = data["layout"].get<std::vector<int>>();
            }

            LevelGrid::GetInstance().Initialize(cols, rows, tileSize);

            for (size_t i = 0; i < layout.size(); ++i)
            {
                int x = static_cast<int>(i % static_cast<size_t>(cols));
                int y = static_cast<int>(i / static_cast<size_t>(cols));
                LevelGrid::GetInstance().SetTile(x, y, static_cast<TileType>(layout[i]));
            }
        }
    };

    REGISTER_COMPONENT_PARSER(LevelGridSetup, LevelGridSetupParser);
}