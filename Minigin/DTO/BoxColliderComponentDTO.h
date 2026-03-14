#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct BoxColliderComponentDTO
    {
        float width{ 16.0f };
        float height{ 16.0f };

        static BoxColliderComponentDTO FromJson(const nlohmann::json& data)
        {
            BoxColliderComponentDTO dto;

            dto.width = data.value("width", 16.0f);
            dto.height = data.value("height", 16.0f);

            return dto;
        }
    };
}