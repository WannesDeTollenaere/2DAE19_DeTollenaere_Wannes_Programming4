#pragma once
#include "Components/CharacterControllerComponent.h"
#include "DTO/CharacterControllerDTO.h"

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
        }
    };
}
