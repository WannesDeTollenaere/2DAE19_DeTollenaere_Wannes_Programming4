#include "EnemyWanderComponent.h"
#include "GameObject.h"
#include "Components/Movement/GridMovementComponent.h"
#include "Components/AnimatorComponent.h"
#include "Helpers/LevelGrid.h"
#include <cstdlib> 

namespace dae
{
    EnemyWanderComponent::EnemyWanderComponent(GameObject* pOwner)
        : Component(pOwner)
    {
    }

    void EnemyWanderComponent::Update()
    {
        if (!m_pMovementComponent) m_pMovementComponent = GetOwner()->GetComponent<GridMovementComponent>();
        if (!m_pAnimator) m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();
        if (!m_pMovementComponent) return;

        const auto pos = GetOwner()->GetTransform().GetLocalPosition();
        auto& grid = LevelGrid::GetInstance();
        float tileSize = grid.GetTileSize();


        bool isStuck = (std::abs(pos.x - m_LastPosition.x) < 0.001f &&
            std::abs(pos.y - m_LastPosition.y) < 0.001f);


        int gridX = static_cast<int>((pos.x + tileSize / 2.0f) / tileSize);
        int gridY = static_cast<int>((pos.y + tileSize / 2.0f) / tileSize);
        TileType currentTile = grid.GetTile(gridX, gridY);

        float snappedX = gridX * tileSize;
        float snappedY = gridY * tileSize;
        bool isCentered = (std::abs(pos.x - snappedX) < 1.0f && std::abs(pos.y - snappedY) < 1.0f);

        if (isStuck || (currentTile == TileType::Intersection && isCentered))
        {
            PickNewDirection();
        }

        m_pMovementComponent->SetDesiredDirection(m_CurrentDirection.x, m_CurrentDirection.y);

        UpdateAnimation();

        m_LastPosition = pos;
    }

    void EnemyWanderComponent::PickNewDirection()
    {
        int randDir = std::rand() % 4;

        switch (randDir)
        {
        case 0: m_CurrentDirection = glm::vec2(0.0f, -1.0f); break; // Up
        case 1: m_CurrentDirection = glm::vec2(0.0f, 1.0f); break;  // Down
        case 2: m_CurrentDirection = glm::vec2(-1.0f, 0.0f); break; // Left
        case 3: m_CurrentDirection = glm::vec2(1.0f, 0.0f); break;  // Right
        }
    }

    void EnemyWanderComponent::UpdateAnimation()
    {
        if (!m_pAnimator) return;

        if (m_CurrentDirection.x > 0) m_pAnimator->PlayAnimation("WalkRight");
        else if (m_CurrentDirection.x < 0) m_pAnimator->PlayAnimation("WalkLeft");
        else if (m_CurrentDirection.y > 0) m_pAnimator->PlayAnimation("WalkDown");
        else if (m_CurrentDirection.y < 0) m_pAnimator->PlayAnimation("WalkUp");
    }
}