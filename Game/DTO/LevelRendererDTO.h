#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace dae
{
    struct LevelRendererDTO
    {
        std::string texture;
        int frameWidth{ 16 };
        int frameHeight{ 16 };
        int platformCol{ 0 };
        int platformRow{ 0 };
        int ladderCol{ 0 };
        int ladderRow{ 0 };

        static LevelRendererDTO FromJson(const nlohmann::json& data)
        {
            LevelRendererDTO dto{};
        
            dto.texture = data.value("texture", "burger_time_spritesheet.png");
            dto.frameWidth = data.value("frameWidth", 16);
            dto.frameHeight = data.value("frameHeight", 16);

            dto.platformCol = data.value("platformCol", 0);
            dto.platformRow = data.value("platformRow", 0);
            dto.ladderCol = data.value("ladderCol", 0);
            dto.ladderRow = data.value("ladderRow", 0);

            return dto;
        }
    };
}