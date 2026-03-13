#pragma once
#include "Components/CharacterControllerComponent.h"
#include "DTO/CharacterControllerDTO.h"

#include "Components/HealthComponent.h"
#include "DTO/HealthComponentDTO.h"
#include "Components/HealthDisplayComponent.h"
#include "DTO/HealthDisplayComponentDTO.h"

#include <SceneLoader.h> 
#include <GameObject.h>

namespace dae
{
    class GameComponentsRegistry
    {
    public:
        static void RegisterAll()
        {
            SceneLoader::RegisterComponentParser("CharacterControllerComponent", [](dae::GameObject* go, const nlohmann::json& data) {

                auto dto = CharacterControllerDTO::FromJson(data);

                go->AddComponent<CharacterControllerComponent>(dto.speed, dto.useKeyboard, dto.controllerIndex);
                });

            SceneLoader::RegisterComponentParser("HealthComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = HealthComponentDTO::FromJson(data);
                go->AddComponent<HealthComponent>(dto.maxHealth);
                });

            // Register Lives Display Component
            SceneLoader::RegisterComponentParser("HealthDisplayComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = LivesDisplayComponentDTO::FromJson(data);
                go->AddComponent<HealthDisplayComponent>(dto.startingLives, dto.targetId);
                });
        }
    };
}
