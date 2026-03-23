#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include "Components/Movement/LevelRenderComponent.h"

namespace dae
{
    struct LevelRendererDTO
    {
        std::string texture;
        int frameWidth{ 16 };
        int frameHeight{ 16 };
        std::unordered_map<int, TileRenderData> tileMappings;

        static LevelRendererDTO FromJson(const nlohmann::json& data)
        {
            LevelRendererDTO dto{};
            dto.texture = data.value("texture", "burger_time_spritesheet.png");
            dto.frameWidth = data.value("frameWidth", 16);
            dto.frameHeight = data.value("frameHeight", 16);

            if (data.contains("tileMappings"))
            {
                for (auto& [key, val] : data["tileMappings"].items())
                {
                    int tileId = std::stoi(key);
                    TileRenderData renderData{};

                    for (const auto& coord : val)
                    {
                        renderData.spriteCoords.push_back({ coord[0], coord[1] });
                    }
                    dto.tileMappings[tileId] = renderData;
                }
            }
            return dto;
        }
    };
}