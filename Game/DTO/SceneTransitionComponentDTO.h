#pragma once
#include <nlohmann/json.hpp>
#include <unordered_set>

namespace dae
{
    struct SceneTransitionComponentDTO
    {
        std::string targetScene{ "" };

        static SceneTransitionComponentDTO FromJson(const nlohmann::json& data)
        {
            SceneTransitionComponentDTO dto;
            dto.targetScene = data.value("targetScene", "");

            return dto;
        }
    };
}