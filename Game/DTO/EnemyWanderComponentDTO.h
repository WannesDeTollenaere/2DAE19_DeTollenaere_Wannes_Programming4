#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct EnemyWanderComponentDTO
    {
        static EnemyWanderComponentDTO FromJson(const nlohmann::json&)
        {
            EnemyWanderComponentDTO dto;

            return dto;
        }
    };
}