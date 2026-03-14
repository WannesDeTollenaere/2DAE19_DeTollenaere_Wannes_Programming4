#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace dae
{
    struct ScoreComponentDTO
    {
        int startingScore{ 0 };

        static ScoreComponentDTO FromJson(const nlohmann::json& data)
        {
            ScoreComponentDTO dto;
            dto.startingScore = data.value("startingScore", 0);
            return dto;
        }
    };
}