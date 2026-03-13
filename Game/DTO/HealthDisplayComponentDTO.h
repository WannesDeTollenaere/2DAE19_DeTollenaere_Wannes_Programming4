#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct LivesDisplayComponentDTO
    {
        int startingLives{ 3 };
        int targetId{ 0 };

        static LivesDisplayComponentDTO FromJson(const nlohmann::json& data)
        {
            LivesDisplayComponentDTO dto;
            dto.startingLives = data.value("startingLives", 3);
            dto.targetId = data.value("targetId", 0);
            return dto;
        }

    };
}