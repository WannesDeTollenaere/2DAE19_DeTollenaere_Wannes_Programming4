#include "BurgerIngredientComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "Helpers/LevelGrid.h"
#include "GameTime.h" 
#include <algorithm>
#include <iostream>

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
    }

    void BurgerIngredientComponent::StartFalling()
    {
        m_IsFalling = true;

        m_TargetDropY = FindNextPlatformY();

        std::fill(m_SteppedSegments.begin(), m_SteppedSegments.end(), false);
    }

    void BurgerIngredientComponent::HandleFalling(float deltaTime)
    {
        auto pos = GetOwner()->GetTransform().GetLocalPosition();

        pos.y += m_FallSpeed * deltaTime;

        if (pos.y >= m_TargetDropY)
        {
            pos.y = m_TargetDropY;
            m_IsFalling = false;
        }

        GetOwner()->GetTransform().SetLocalPosition(pos.x, pos.y, pos.z);
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
}