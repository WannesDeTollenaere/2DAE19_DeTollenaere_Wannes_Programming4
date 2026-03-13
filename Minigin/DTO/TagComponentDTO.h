#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace dae
{
    struct TagComponentDTO
    {
        std::string tag{};

        static TagComponentDTO FromJson(const nlohmann::json& data)
        {
            TagComponentDTO dto;
            dto.tag = data.value("tag", "Untagged");
            return dto;
        }
    };
}