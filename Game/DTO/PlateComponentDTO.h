#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct PlateComponentDTO
    {
        int requiredIngredients{ 3 };

        static PlateComponentDTO FromJson(const nlohmann::json& data)
        {
            PlateComponentDTO dto;
            dto.requiredIngredients = data.value("requiredIngredients", 3);
            return dto;
        }
    };
}