#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace dae
{
    struct ScoreDisplayComponentDTO
    {
        int startingScore{ 0 };
        std::string targetTag{};

        static ScoreDisplayComponentDTO FromJson(const nlohmann::json& data)
        {
            ScoreDisplayComponentDTO dto;
            dto.startingScore = data.value("startingScore", 0);
            dto.targetTag = data.value("targetTag", "Player1");
            return dto;
        }
    };
}