#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <SDL3/SDL_rect.h>

namespace dae
{
    struct LevelRendererDTO
    {
        std::string texture;
        SDL_Rect platformSrc{ 0, 0, 16, 16 };
        SDL_Rect ladderSrc{ 0, 0, 16, 16 };

        static LevelRendererDTO FromJson(const nlohmann::json& data)
        {
            LevelRendererDTO dto{};
            dto.texture = data.value("texture", "burger_time_spritesheet.png");

            if (data.contains("platformSrc"))
            {
                auto& src = data["platformSrc"];
                dto.platformSrc = { src[0], src[1], src[2], src[3] };
            }
            if (data.contains("ladderSrc"))
            {
                auto& src = data["ladderSrc"];
                dto.ladderSrc = { src[0], src[1], src[2], src[3] };
            }
            return dto;
        }
    };
}