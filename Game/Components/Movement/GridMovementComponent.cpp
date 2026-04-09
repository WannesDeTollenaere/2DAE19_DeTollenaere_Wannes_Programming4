#include "GridMovementComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Helpers/LevelGrid.h"
#include <cmath>
#include <algorithm>

namespace dae
{
    GridMovementComponent::GridMovementComponent(GameObject* pOwner, float speed)
        : Component(pOwner), m_Speed(speed)
    {
    }

    void GridMovementComponent::SetDesiredDirection(float x, float y)
    {
        if (x != 0) y = 0;

        m_DesiredDirX = x;
        m_DesiredDirY = y;
    }

    void GridMovementComponent::Update()
    {
        if (m_DesiredDirX == 0.0f && m_DesiredDirY == 0.0f) return;

        const auto pOwner = GetOwner();
        const auto pos = pOwner->GetTransform().GetLocalPosition();

        auto& grid = LevelGrid::GetInstance(); 
        float tileSize = grid.GetTileSize();

        int gridX = static_cast<int>((pos.x + tileSize / 2.0f) / tileSize);
        int gridY = static_cast<int>((pos.y + tileSize / 2.0f) / tileSize);

        TileType currentTile = grid.GetTile(gridX, gridY);

        float dt = GameTime::GetInstance().GetDeltaTime();
        float newX = pos.x;
        float newY = pos.y;

        float snappedX = gridX * tileSize;
        float snappedY = gridY * tileSize;

        if (m_DesiredDirX != 0.0f && grid.IsWalkableHorizontal(currentTile))
        {
            newX += m_DesiredDirX * m_Speed * dt;

            int nextX = gridX + (m_DesiredDirX > 0.0f ? 1 : -1);
            if (grid.GetTile(nextX, gridY) == TileType::Empty)
            {

                newX = (m_DesiredDirX > 0.0f) ? std::min(newX, snappedX) : std::max(newX, snappedX);
            }

            newY = snappedY;
        }
        else if (m_DesiredDirY != 0.0f)
        {
            bool canMoveY = false;
            bool forceClamp = false;

            if (m_DesiredDirY < 0.0f)  // moving up
            {
                if (grid.CanClimbUp(currentTile))
                {
                    canMoveY = true;
                }
                else if (currentTile == TileType::IntersectionDownOnly && pos.y > snappedY)
                {
                    canMoveY = true;
                    forceClamp = true;
                }
            }
            else // Moving DOWN
            {
                canMoveY = grid.CanClimbDown(currentTile);

                if (canMoveY)
                {
                    int nextY = gridY + 1;
                    TileType nextTile = grid.GetTile(gridX, nextY);

                    if (nextTile == TileType::IntersectionDownOnly && currentTile != TileType::IntersectionDownOnly)
                    {
                        if (pos.y >= snappedY)
                        {
                           
                            canMoveY = false;
                        }
                        else
                        {
                           
                            forceClamp = true;
                        }
                    }
                }
            }

            if (canMoveY && std::abs(pos.x - snappedX) < (tileSize * 0.4f))
            {
                newY += m_DesiredDirY * m_Speed * dt;

                int nextY = gridY + (m_DesiredDirY > 0.0f ? 1 : -1);

                if (forceClamp || grid.GetTile(gridX, nextY) == TileType::Empty)
                {
                    newY = (m_DesiredDirY > 0.0f) ? std::min(newY, snappedY) : std::max(newY, snappedY);
                }

                newX = snappedX;
            }
        }

        float maxGridX = (grid.GetCols() - 1) * tileSize;
        float maxGridY = (grid.GetRows() - 1) * tileSize;
        newX = std::clamp(newX, 0.0f, maxGridX);
        newY = std::clamp(newY, 0.0f, maxGridY);

        pOwner->GetTransform().SetLocalPosition(glm::vec3(newX, newY, 0.0f));

        m_DesiredDirX = 0.f;
        m_DesiredDirY = 0.f;
    }

}
