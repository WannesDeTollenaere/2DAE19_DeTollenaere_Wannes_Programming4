#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct PlayerCollisionHandlerDTO
    {
        static PlayerCollisionHandlerDTO FromJson(const nlohmann::json&)
        {
            PlayerCollisionHandlerDTO dto;
            return dto;
        }
    };
}