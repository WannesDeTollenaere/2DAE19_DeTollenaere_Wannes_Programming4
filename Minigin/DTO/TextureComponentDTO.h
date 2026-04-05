#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace dae
{
    struct TextureComponentDTO
    {
        std::string texture;

        bool useSpriteSheet{ false };
        int frameWidth{ 48 };
        int frameHeight{ 48 };
        int col{ 0 };
        int row{ 0 };
        int colSpan{ 1 }; 

        static TextureComponentDTO FromJson(const nlohmann::json& data)
        {
            TextureComponentDTO dto;
            dto.texture = data.value("texture", "");

            if (data.contains("spriteSheet") && data["spriteSheet"].is_object())
            {
                dto.useSpriteSheet = true;
                const auto& sheetData = data["spriteSheet"];

                dto.frameWidth = sheetData.value("frameWidth", 48);
                dto.frameHeight = sheetData.value("frameHeight", 48);
                dto.col = sheetData.value("col", 0);
                dto.row = sheetData.value("row", 0);
                dto.colSpan = sheetData.value("colSpan", 1);
            }

            return dto;
        }
    };
}