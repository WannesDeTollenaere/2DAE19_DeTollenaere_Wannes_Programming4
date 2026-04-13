#include "EnemyWanderComponent.h"
#include "GameObject.h"
#include "Components/Movement/GridMovementComponent.h"
#include "../Enemy/EnemyComponent.h" 
#include "Helpers/LevelGrid.h"
#include "GameTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include <queue>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "SceneLoader.h"

namespace dae
{
    class EnemyWanderComponentParser final : public IComponentParser 
    {
    public:
        void Parse(GameObject* go, const nlohmann::json&) override
        {
            go->AddComponent<EnemyWanderComponent>();
        }
    };

    REGISTER_COMPONENT_PARSER(EnemyWanderComponent, EnemyWanderComponentParser);

    EnemyWanderComponent::EnemyWanderComponent(GameObject* pOwner)
        : Component(pOwner)
    {
    }

    void EnemyWanderComponent::Update()
    {
        if (!m_pMovementComponent) m_pMovementComponent = GetOwner()->GetComponent<GridMovementComponent>();
        if (!m_pEnemyComp) m_pEnemyComp = GetOwner()->GetComponent<EnemyComponent>();

        if (!m_pMovementComponent || !m_pEnemyComp) return;

        if (m_pEnemyComp->IsMovementDisabled()) return;

        FindPlayer();

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

        if (m_DecisionAvailable && (isStuck || ((currentTile == TileType::Intersection && isCentered)) || (currentTile == TileType::IntersectionDownOnly && isCentered)))
        {
            m_DecisionAvailable = false;
            GameTime::GetInstance().AddTimer(m_CooldownDuration, [&]() { m_DecisionAvailable = true; });
            PickNewDirection();
        }

        m_pMovementComponent->SetDesiredDirection(m_CurrentDirection.x, m_CurrentDirection.y);

        m_pEnemyComp->SetCurrentDirection(m_CurrentDirection);

        m_LastPosition = pos;
    }

    void EnemyWanderComponent::PickNewDirection()
    {
        auto& grid = LevelGrid::GetInstance();
        float tileSize = grid.GetTileSize();

        auto pos = GetOwner()->GetTransform().GetLocalPosition();
        int startX = static_cast<int>((pos.x + tileSize / 2.0f) / tileSize);
        int startY = static_cast<int>((pos.y + tileSize / 2.0f) / tileSize);

        TileType startTile = grid.GetTile(startX, startY);
        std::vector<glm::ivec2> startNeighbors;

        if (grid.IsWalkableHorizontal(startTile))
        {
            if (grid.GetTile(startX - 1, startY) != TileType::Empty) startNeighbors.push_back({ startX - 1, startY });
            if (grid.GetTile(startX + 1, startY) != TileType::Empty) startNeighbors.push_back({ startX + 1, startY });
        }
        if (grid.CanClimbUp(startTile))
        {
            if (grid.GetTile(startX, startY - 1) != TileType::Empty) startNeighbors.push_back({ startX, startY - 1 });
        }
        if (grid.CanClimbDown(startTile))
        {
            TileType tileBelow = grid.GetTile(startX, startY + 1);
            if (tileBelow != TileType::Empty)
            {
                if (!(tileBelow == TileType::IntersectionDownOnly && startTile != TileType::IntersectionDownOnly))
                {
                    startNeighbors.push_back({ startX, startY + 1 });
                }
            }
        }

        // prevent 180 degree turns unless stuck
        glm::ivec2 reverseTile = { startX - static_cast<int>(m_CurrentDirection.x), startY - static_cast<int>(m_CurrentDirection.y) };
        if (startNeighbors.size() > 1)
        {
            startNeighbors.erase(std::remove_if(startNeighbors.begin(), startNeighbors.end(), [&](const glm::ivec2& n) {
                return n.x == reverseTile.x && n.y == reverseTile.y;
                }), startNeighbors.end());
        }

        if (startNeighbors.empty()) return;

        // random wander chance
        float randomChoice = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        if (!m_pPlayer || randomChoice < m_RandomWanderChance)
        {
            m_Path.clear();
            int randIdx = std::rand() % startNeighbors.size();
            glm::ivec2 chosen = startNeighbors[randIdx];
            m_CurrentDirection = glm::vec2(static_cast<float>(chosen.x - startX), static_cast<float>(chosen.y - startY));
            return;
        }

        // BFS
        auto playerPos = m_pPlayer->GetTransform().GetLocalPosition();
        int targetX = static_cast<int>((playerPos.x + tileSize / 2.0f) / tileSize);
        int targetY = static_cast<int>((playerPos.y + tileSize / 2.0f) / tileSize);

        std::queue<glm::ivec2> frontier;
        frontier.push({ startX, startY });

        std::unordered_map<int, glm::ivec2> cameFrom;
        int startIdx = startY * grid.GetCols() + startX;
        cameFrom[startIdx] = { startX, startY };

        bool found = false;

        while (!frontier.empty())
        {
            auto current = frontier.front();
            frontier.pop();

            if (current.x == targetX && current.y == targetY)
            {
                found = true;
                break;
            }

            std::vector<glm::ivec2> neighbors;

            // prevent turning around
            if (current.x == startX && current.y == startY)
            {
                neighbors = startNeighbors;
            }
            else
            {
                TileType currentTile = grid.GetTile(current.x, current.y);
                if (grid.IsWalkableHorizontal(currentTile))
                {
                    if (grid.GetTile(current.x - 1, current.y) != TileType::Empty) neighbors.push_back({ current.x - 1, current.y });
                    if (grid.GetTile(current.x + 1, current.y) != TileType::Empty) neighbors.push_back({ current.x + 1, current.y });
                }
                if (grid.CanClimbUp(currentTile))
                {
                    if (grid.GetTile(current.x, current.y - 1) != TileType::Empty) neighbors.push_back({ current.x, current.y - 1 });
                }
                if (grid.CanClimbDown(currentTile))
                {
                    TileType tileBelow = grid.GetTile(current.x, current.y + 1);
                    if (tileBelow != TileType::Empty)
                    {

                        if (!(tileBelow == TileType::IntersectionDownOnly && currentTile != TileType::IntersectionDownOnly))
                        {
                            neighbors.push_back({ current.x, current.y + 1 });
                        }
                    }
                }
            }

            for (const auto& next : neighbors)
            {
                int nextIdx = next.y * grid.GetCols() + next.x;
                if (cameFrom.find(nextIdx) == cameFrom.end())
                {
                    frontier.push(next);
                    cameFrom[nextIdx] = current;
                }
            }
        }

        m_Path.clear();

        if (found)
        {
            glm::ivec2 current = { targetX, targetY };
            glm::ivec2 nextStep = current;

            if (!(startX == targetX && startY == targetY))
            {
                while (current.x != startX || current.y != startY)
                {
                    m_Path.push_back(current);
                    nextStep = current;
                    int currentIdx = current.y * grid.GetCols() + current.x;
                    current = cameFrom[currentIdx];
                }

                m_Path.push_back({ startX, startY });
                std::reverse(m_Path.begin(), m_Path.end());
            }

            m_CurrentDirection = glm::vec2(static_cast<float>(nextStep.x - startX), static_cast<float>(nextStep.y - startY));
        }
        else
        {
            int randIdx = std::rand() % startNeighbors.size();
            glm::ivec2 chosen = startNeighbors[randIdx];
            m_CurrentDirection = glm::vec2(static_cast<float>(chosen.x - startX), static_cast<float>(chosen.y - startY));
        }
    }


    // find player if player not already found
    void EnemyWanderComponent::FindPlayer()
    {
        if (m_pPlayer) return;
        Scene* pScene = SceneManager::GetInstance().GetActiveScene();
        if (pScene) m_pPlayer = pScene->GetGameObjectByTag("Player");
    }


    void EnemyWanderComponent::RenderGUI()
    {
        Component::RenderGUI();
        static bool showPath = true;
        ImGui::Checkbox("Show pathfinding", &showPath);
        ImGui::SliderFloat("Random wander chance", &m_RandomWanderChance, 0.0f, 1.0f, "%.2f");

        if (showPath && m_Path.size() > 1)
        {
            auto& grid = LevelGrid::GetInstance();
            float tileSize = grid.GetTileSize();

            ImDrawList* drawList = ImGui::GetBackgroundDrawList();

            for (size_t i = 0; i < m_Path.size() - 1; ++i)
            {
                ImVec2 p1(m_Path[i].x * tileSize + tileSize / 2.0f, m_Path[i].y * tileSize + tileSize / 2.0f);
                ImVec2 p2(m_Path[i + 1].x * tileSize + tileSize / 2.0f, m_Path[i + 1].y * tileSize + tileSize / 2.0f);

                drawList->AddLine(p1, p2, IM_COL32(255, 50, 50, 200), 4.0f);
            }

            // green dot at end
            if (!m_Path.empty())
            {
                ImVec2 target(m_Path.back().x * tileSize + tileSize / 2.0f, m_Path.back().y * tileSize + tileSize / 2.0f);
                drawList->AddCircleFilled(target, 6.0f, IM_COL32(50, 255, 50, 255));
            }

        }
    }
}