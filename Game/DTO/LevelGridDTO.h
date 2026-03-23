#pragma once
#include <nlohmann/json.hpp>
#include <vector>

namespace dae
{
    struct LevelGridDTO
    {
        int cols{ 0 };
        int rows{ 0 };
        float tileSize{ 16.0f };
        std::vector<int> layout{};

        static LevelGridDTO FromJson(const nlohmann::json& data)
        {
            LevelGridDTO dto{};
            dto.cols = data.value("cols", 0);
            dto.rows = data.value("rows", 0);
            dto.tileSize = data.value("tileSize", 16.0f);

            if (data.contains("layout") && data["layout"].is_array())
            {
                dto.layout = data["layout"].get<std::vector<int>>();
            }

            return dto;
        }
    };
}