#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct BoxColliderComponentDTO
    {
        float width{ 16.0f };
        float height{ 16.0f };

        float offsetX{ 0.f };
        float offsetY{ 0.f };

        static BoxColliderComponentDTO FromJson(const nlohmann::json& data)
        {
            BoxColliderComponentDTO dto;

            dto.width = data.value("width", 16.0f);
            dto.height = data.value("height", 16.0f);
            dto.offsetX = data.value("offsetX", 0.f);
            dto.offsetY = data.value("offsetY", 0.f);

            return dto;
        }
    };
}