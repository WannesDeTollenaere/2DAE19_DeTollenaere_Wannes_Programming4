#pragma once
#include <nlohmann/json.hpp>
#include <unordered_set>

namespace dae
{
    struct TagComponentDTO
    {
        std::string  uniqueTag{ ""};
        std::unordered_set<std::string > tags{};

        static TagComponentDTO FromJson(const nlohmann::json& data)
        {
            TagComponentDTO dto;
            dto.uniqueTag = data.value("uniqueTag", "");

            if (data.contains("tags") && data["tags"].is_array())
            {
                dto.tags = data["tags"].get<std::unordered_set<std::string >>();
            }

            return dto;
        }
    };
}