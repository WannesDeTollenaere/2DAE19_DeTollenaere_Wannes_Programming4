#pragma once
#include "Components/CharacterControllerComponent.h"
#include "DTO/CharacterControllerDTO.h"

#include "Components/HealthComponent.h"
#include "DTO/HealthComponentDTO.h"
#include "Components/HealthDisplayComponent.h"
#include "DTO/HealthDisplayComponentDTO.h"
#include "Components/TagComponent.h"

#include "DTO/ScoreDisplayComponentDTO.h"
#include "Components/ScoreDisplayComponent.h"


#include "Components/ScoreComponent.h"
#include "DTO/ScoreComponentDTO.h"

#include "Components/Collision/PlayerCollisionHandler.h"
#include "DTO/PlayerCollisionHandlerDTO.h"

#include <SceneLoader.h> 
#include <GameObject.h>
#include "sdbm_hash.h"

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

            SceneLoader::RegisterComponentParser("HealthDisplayComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = LivesDisplayComponentDTO::FromJson(data);
                go->AddComponent<HealthDisplayComponent>(dto.startingLives, dae::make_sdbm_hash_rt(dto.targetTag));
                });

            SceneLoader::RegisterComponentParser("ScoreDisplayComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = ScoreDisplayComponentDTO::FromJson(data);
                go->AddComponent<ScoreDisplayComponent>(dto.startingScore, dae::make_sdbm_hash_rt(dto.targetTag));
                });

            SceneLoader::RegisterComponentParser("ScoreComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = ScoreComponentDTO::FromJson(data);
                go->AddComponent<ScoreComponent>(dto.startingScore);
                });
            SceneLoader::RegisterComponentParser("PlayerCollisionHandler", [](dae::GameObject* go, const nlohmann::json&) {
                //auto dto = PlayerCollisionHandlerDTO::FromJson(data);
                go->AddComponent<PlayerCollisionHandler>();
                });
        }
    };
}
