#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct ItemCollisionHandlerDTO
    {
        static ItemCollisionHandlerDTO FromJson(const nlohmann::json&)
        {
            ItemCollisionHandlerDTO dto;
            return dto;
        }
    };
}