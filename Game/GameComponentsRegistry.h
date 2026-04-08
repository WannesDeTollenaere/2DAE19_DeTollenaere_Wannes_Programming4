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

#include "Components/Collision/ItemCollisionHandler.h"
#include "DTO/ItemCollisionHandlerDTO.h"

#include "DTO/LevelGridDTO.h"
#include "Helpers/LevelGrid.h"

#include "Components/Movement/GridMovementComponent.h"
#include "DTO/GridMovementComponentDTO.h"
#include "Components/Movement/GridRenderComponent.h"
#include "DTO/LevelRendererDTO.h"

#include "DTO/BurgerIngredientDTO.h"
#include "Components/Ingredients/BurgerIngredientComponent.h"

#include "Components/Movement/GridDebugRenderer.h"

#include "Components/Movement/EnemyWanderComponent.h"
#include "DTO/EnemyWanderComponentDTO.h"
#include "Components/ScoreVFXComponent.h"

#include "DTO/PlateComponentDTO.h"
#include "Components/Ingredients/PlateComponent.h"

#include "ResourceManager.h"
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

            SceneLoader::RegisterComponentParser("ItemCollisionHandler", [](dae::GameObject* go, const nlohmann::json&) {
                //auto dto = PlayerCollisionHandlerDTO::FromJson(data);
                go->AddComponent<ItemCollisionHandler>();
                });

            SceneLoader::RegisterComponentParser("LevelGridSetup", [](dae::GameObject*, const nlohmann::json& data) {
                auto dto = LevelGridDTO::FromJson(data);

                LevelGrid::GetInstance().Initialize(dto.cols, dto.rows, dto.tileSize);

                for (size_t i = 0; i < dto.layout.size(); ++i)
                {
                    int x = i % dto.cols;
                    int y = static_cast<int>(i) / dto.cols;
                    LevelGrid::GetInstance().SetTile(x, y, static_cast<TileType>(dto.layout[i]));
                }
                //go->AddComponent<GridDebugRenderer>();
                });

            SceneLoader::RegisterComponentParser("GridMovementComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = GridMovementComponentDTO::FromJson(data);
                go->AddComponent<GridMovementComponent>(dto.speed);
                });

            SceneLoader::RegisterComponentParser("LevelRendererComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = LevelRendererDTO::FromJson(data);

                auto sheet = dae::ResourceManager::GetInstance().LoadSpriteSheet(dto.texture, dto.frameWidth, dto.frameHeight);

                go->AddComponent<GridRendererComponent>(sheet, dto.tileMappings);
                });

            SceneLoader::RegisterComponentParser("BurgerIngredientComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = BurgerIngredientComponentDTO::FromJson(data);


                go->AddComponent<BurgerIngredientComponent>(dto.widthInTiles);
                });

            SceneLoader::RegisterComponentParser("EnemyWanderComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = EnemyWanderComponentDTO::FromJson(data);

                auto wanderComp = go->AddComponent<EnemyWanderComponent>();

                wanderComp->SetEnemyType(dto.type);
                });

            SceneLoader::RegisterComponentParser("ScoreVfxSpawnerComponent", [](dae::GameObject* go, const nlohmann::json&) {
                go->AddComponent<ScoreVfxSpawnerComponent>();
                });

            SceneLoader::RegisterComponentParser("PlateComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = PlateComponentDTO::FromJson(data);
                go->AddComponent<PlateComponent>(dto.requiredIngredients);
                }); 


        }
    };
}
