#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct LivesDisplayComponentDTO
    {
        int startingLives{ 3 };
        std::string targetTag{};

        static LivesDisplayComponentDTO FromJson(const nlohmann::json& data)
        {
            LivesDisplayComponentDTO dto;
            dto.startingLives = data.value("startingLives", 3);
            dto.targetTag = data.value("targetTag", "Player1");
            return dto;
        }

    };
}