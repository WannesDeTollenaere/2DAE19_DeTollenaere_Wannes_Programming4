#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include "Components/Movement/EnemyWanderComponent.h" 

namespace dae
{
    struct EnemyWanderComponentDTO
    {
        EnemyType type{ EnemyType::HotDog }; 

        static EnemyWanderComponentDTO FromJson(const nlohmann::json& data) 
        {
            EnemyWanderComponentDTO dto;

            std::string typeStr = data.value("enemyType", "HotDog");

            if (typeStr == "Pickle")
            {
                dto.type = EnemyType::Pickle;
            }
            else if (typeStr == "Egg")
            {
                dto.type = EnemyType::Egg;
            }
            else
            {
                dto.type = EnemyType::HotDog;
            }

            return dto;
        }
    };
}