#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>

namespace dae
{
    struct TextComponentDTO
    {
        std::string text{};
        std::string fontName{};
        uint8_t fontSize{16};
        bool hasColor{false};
        SDL_Color color{0, 0, 0, 255};

        static TextComponentDTO FromJson(const nlohmann::json& data)
        {
            TextComponentDTO dto;
            dto.text = data.value("text", "");
            dto.fontName = data.value("font", "Lingua.otf");
            dto.fontSize = static_cast<uint8_t>(data.value("fontSize", 36));

            dto.hasColor = data.contains("color");
            if (dto.hasColor)
            {
                auto colorData = data["color"];
                dto.color.r = static_cast<Uint8>(colorData.value("r", 255));
                dto.color.g = static_cast<Uint8>(colorData.value("g", 255));
                dto.color.b = static_cast<Uint8>(colorData.value("b", 255));
                dto.color.a = static_cast<Uint8>(colorData.value("a", 255));
            }
            return dto;
        }
    };
}