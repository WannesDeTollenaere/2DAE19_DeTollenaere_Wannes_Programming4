#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct RotatorComponentDTO
    {
        float radius;
        float speed;

        static RotatorComponentDTO FromJson(const nlohmann::json& data)
        {
            RotatorComponentDTO dto;
            dto.radius = data.value("radius", 10.f);
            dto.speed = data.value("speed", 10.f);
            return dto;
        }
    };
}