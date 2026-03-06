#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace dae
{
    struct TextureComponentDTO
    {
        std::string texture;

        static TextureComponentDTO FromJson(const nlohmann::json& data)
        {
            TextureComponentDTO dto;
            dto.texture = data.value("texture", "");
            return dto;
        }
    };
}