#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct GridMovementComponentDTO
    {
        float speed{ 50.0f }; 

        static GridMovementComponentDTO FromJson(const nlohmann::json& data)
        {
            GridMovementComponentDTO dto{};

            dto.speed = data.value("speed", 50.0f);
            return dto;
        }

    };
}