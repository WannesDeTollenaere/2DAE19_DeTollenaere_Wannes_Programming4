#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct HealthComponentDTO
    {
        int maxHealth{ 100 };
        int id{ 0 };

        static HealthComponentDTO FromJson(const nlohmann::json& data)
        {
            HealthComponentDTO dto;
            dto.id = data.value("id", 0);
            dto.maxHealth = data.value("maxHealth", 100);
            return dto;
        }
    };
}