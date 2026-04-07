#include "BurgerIngredientComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "Helpers/LevelGrid.h"
#include "GameTime.h" 
#include <algorithm>
#include <iostream>
#include "Events/EnemyCrushedEvent.h"
#include "ObserverSys/EventManager.h"
#include "SceneManager.h"
#include "Events/BurgerDroppedEvent.h"
#include "Events/BurgerCascadeEvent.h"
#include "Components/BoxColliderComponent.h"
#include "Scene.h"

namespace dae
{
    BurgerIngredientComponent::BurgerIngredientComponent(GameObject* owner, int widthInTiles)
        : BaseCollisionHandler(owner)
        , m_WidthInTiles(widthInTiles)
    {
        m_SteppedSegments.resize(m_WidthInTiles, false);
    }

    void BurgerIngredientComponent::Update()
    {
        if (m_IsFalling)
        {
            float deltaTime = dae::GameTime::GetInstance().GetDeltaTime();

            HandleFalling(deltaTime);
        }
    }

    void BurgerIngredientComponent::StepOnSegment(int colOffset)
    {
        if (m_IsFalling) return;
        if (colOffset < 0 || colOffset >= m_WidthInTiles) return;

        if (!m_SteppedSegments[colOffset])
        {
            m_SteppedSegments[colOffset] = true;

            bool allStepped = true;
            for (bool stepped : m_SteppedSegments)
            {
                if (!stepped)
                {
                    allStepped = false;
                    break;
                }
            }

            if (allStepped)
            {
                StartFalling();
            }
        }
    }

    void BurgerIngredientComponent::ForceDrop()
    {
        if (!m_IsFalling)
        {
            StartFalling();
        }
    }

    void BurgerIngredientComponent::OnCollision(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash_rt("Player")))
        {
            float myX = GetOwner()->GetTransform().GetWorldPosition().x;
            float playerX = otherObject->GetTransform().GetWorldPosition().x;

            float tileSize = dae::LevelGrid::GetInstance().GetTileSize();

            float playerCenterX = playerX + (tileSize / 2.0f);

            float diffX = playerCenterX - myX;

            if (diffX >= 0)
            {
                int colOffset = static_cast<int>(diffX / tileSize);

                StepOnSegment(colOffset);
            }
        }
        else if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash_rt("Ingredient")))
        {
            if (!m_IsFalling)
            {
                auto otherIngredient = otherObject->GetComponent<dae::BurgerIngredientComponent>();

                if (otherIngredient && otherIngredient->IsFalling())
                {
                    float myY = GetOwner()->GetTransform().GetWorldPosition().y;
                    float otherY = otherObject->GetTransform().GetWorldPosition().y;

                    if (otherY < myY - 2.0f)
                    {
                        StartFalling();
                    }
                }
            }
        }
        else if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash_rt("Enemy")))
        {
            if (m_IsFalling)
            {
                auto it = std::find(m_CascadingEnemies.begin(), m_CascadingEnemies.end(), otherObject);
                if (it == m_CascadingEnemies.end())
                {

                    dae::EnemyCrushedEvent crushEvent(otherObject);
                    EventManager::GetInstance().SendEvent(&crushEvent);

                }
            }
        }
    }

    void dae::BurgerIngredientComponent::StartFalling()
    {
        m_IsFalling = true;
        m_TargetDropY = FindNextPlatformY();
        std::fill(m_SteppedSegments.begin(), m_SteppedSegments.end(), false);

        for (auto enemy : m_CascadingEnemies)
        {
            if (auto wander = enemy->GetComponent<EnemyWanderComponent>()) {
                wander->SetCascading(true);
            }
        }

        BurgerDroppedEvent dropEvent(GetOwner());
        EventManager::GetInstance().SendEvent(&dropEvent);

        if (!m_CascadingEnemies.empty())
        {
            BurgerCascadedEvent cascadeEvent(GetOwner(), static_cast<int>(m_CascadingEnemies.size()));
            EventManager::GetInstance().SendEvent(&cascadeEvent);
        }
    }

    void BurgerIngredientComponent::HandleFalling(float deltaTime)
    {
        auto pos = GetOwner()->GetTransform().GetLocalPosition();
        float step = m_FallSpeed * deltaTime;
        pos.y += step;

        if (pos.y >= m_TargetDropY)
        {
            step -= (pos.y - m_TargetDropY);
            pos.y = m_TargetDropY;
            m_IsFalling = false;
            for (auto enemy : m_CascadingEnemies)
            {
                if (auto wander = enemy->GetComponent<EnemyWanderComponent>()) {
                    wander->EnableMovement();
                }
            }
        }

        GetOwner()->GetTransform().SetLocalPosition(pos.x, pos.y, pos.z);

        for (auto enemy : m_CascadingEnemies)
        {
            auto enemyPos = enemy->GetTransform().GetLocalPosition();
            enemyPos.y += step;
            enemy->GetTransform().SetLocalPosition(enemyPos);
        }
    }

    float BurgerIngredientComponent::FindNextPlatformY()
    {
        auto& grid = dae::LevelGrid::GetInstance();
        float tileSize = grid.GetTileSize();

        auto pos = GetOwner()->GetTransform().GetLocalPosition();

        int currentRow = static_cast<int>(pos.y / tileSize);
        int col = static_cast<int>(pos.x / tileSize);

        int targetRow = currentRow + 1;
        // check if under player is a platform or intersection - sort of raycast
        while (targetRow < grid.GetRows())
        {
            dae::TileType type = grid.GetTile(col, targetRow);

            if (type == dae::TileType::Platform || type == dae::TileType::Intersection)
            {
                break;
            }
            targetRow++;
        }

        return targetRow * tileSize + tileSize/2;
    }
    void BurgerIngredientComponent::OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (m_IsFalling) return;

        if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash_rt("Enemy")))
        {
            if (auto wander = otherObject->GetComponent<EnemyWanderComponent>()) {
                if(!wander->IsCascading())
                    m_CascadingEnemies.push_back(otherObject);

            }
        }
    }

    void BurgerIngredientComponent::OnCollisionExit(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash_rt("Enemy")))
        {
            std::erase(m_CascadingEnemies, otherObject);
        }
    }
}