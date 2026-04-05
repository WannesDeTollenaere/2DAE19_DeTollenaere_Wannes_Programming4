#pragma once
#include <nlohmann/json.hpp>

namespace dae
{
    struct BurgerIngredientComponentDTO
    {
        int widthInTiles;

        static BurgerIngredientComponentDTO FromJson(const nlohmann::json& data)
        {
            BurgerIngredientComponentDTO dto;

            dto.widthInTiles = data.value("widthInTiles", 1);

            return dto;
        }
    };
}