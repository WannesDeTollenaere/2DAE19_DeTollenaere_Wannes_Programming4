#include "EnemyWanderComponent.h"
#include "GameObject.h"
#include "Components/Movement/GridMovementComponent.h"
#include "Components/AnimatorComponent.h"
#include "Helpers/LevelGrid.h"
#include "GameTime.h"
#include <cstdlib> 
#include "SceneManager.h"
#include <queue>
#include <unordered_map>
#include "Components/BoxColliderComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/EnemyCrushedEvent.h"

namespace dae
{
    EnemyWanderComponent::EnemyWanderComponent(GameObject* pOwner)
        : Component(pOwner)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("EnemyCrushed"), this);
    }

    EnemyWanderComponent::~EnemyWanderComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("EnemyCrushed"), this);
    }

    void EnemyWanderComponent::Update()
    {
        if (!m_pMovementComponent) m_pMovementComponent = GetOwner()->GetComponent<GridMovementComponent>();
        if (!m_pAnimator) m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();
        if (!m_pMovementComponent) return;

        if (IsDead() || IsStunned()) return;

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

        if (m_DecisionAvailable && (isStuck || (currentTile == TileType::Intersection && isCentered)))
        {
            m_DecisionAvailable = false;
            GameTime::GetInstance().AddTimer(m_CooldownDuration, [&]() { m_DecisionAvailable = true; });
            PickNewDirection();
        }

        m_pMovementComponent->SetDesiredDirection(m_CurrentDirection.x, m_CurrentDirection.y);

        UpdateAnimation();

        m_LastPosition = pos;
    }

    void EnemyWanderComponent::PickNewDirection()
    {
        float randomChoice = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

        // wander randomly chance
        if (!m_pPlayer || randomChoice < m_RandomWanderChance)
        {
            m_Path.clear();

            int randDir = std::rand() % 4;
            switch (randDir)
            {
            case 0: m_CurrentDirection = glm::vec2(0.0f, -1.0f); break;
            case 1: m_CurrentDirection = glm::vec2(0.0f, 1.0f); break;
            case 2: m_CurrentDirection = glm::vec2(-1.0f, 0.0f); break;
            case 3: m_CurrentDirection = glm::vec2(1.0f, 0.0f); break;
            }
            return;
        }

        // BFS
        auto& grid = LevelGrid::GetInstance();
        float tileSize = grid.GetTileSize();

        auto pos = GetOwner()->GetTransform().GetLocalPosition();
        auto playerPos = m_pPlayer->GetTransform().GetLocalPosition();

        int startX = static_cast<int>((pos.x + tileSize / 2.0f) / tileSize);
        int startY = static_cast<int>((pos.y + tileSize / 2.0f) / tileSize);
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

            TileType currentTile = grid.GetTile(current.x, current.y);
            std::vector<glm::ivec2> neighbors;

            if (grid.IsWalkableHorizontal(currentTile))
            {
                if (grid.GetTile(current.x - 1, current.y) != TileType::Empty) neighbors.push_back({ current.x - 1, current.y });
                if (grid.GetTile(current.x + 1, current.y) != TileType::Empty) neighbors.push_back({ current.x + 1, current.y });
            }
            if (grid.IsClimbable(currentTile))
            {
                if (grid.GetTile(current.x, current.y - 1) != TileType::Empty) neighbors.push_back({ current.x, current.y - 1 });
                if (grid.GetTile(current.x, current.y + 1) != TileType::Empty) neighbors.push_back({ current.x, current.y + 1 });
            }

            for (const auto& next : neighbors)
            {
                int nextIdx = next.y * grid.GetCols() + next.x;
                if (cameFrom.find(nextIdx) == cameFrom.end()) // if not visited
                {
                    frontier.push(next);
                    cameFrom[nextIdx] = current;
                }
            }
        }

        // backtrack path
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
            // default to wandering if path is blocked completely
            int randDir = std::rand() % 4;
            switch (randDir)
            {
            case 0: m_CurrentDirection = glm::vec2(0.0f, -1.0f); break;
            case 1: m_CurrentDirection = glm::vec2(0.0f, 1.0f); break;
            case 2: m_CurrentDirection = glm::vec2(-1.0f, 0.0f); break;
            case 3: m_CurrentDirection = glm::vec2(1.0f, 0.0f); break;
            }
        }
    }
    

    // find player if player not already found
    void EnemyWanderComponent::FindPlayer()
    {
        if (m_pPlayer) return;

        Scene* pScene = SceneManager::GetInstance().GetActiveScene();
        if (pScene)
        {
            m_pPlayer = pScene->GetGameObjectByTag("Player");
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

    void EnemyWanderComponent::RenderGUI()
    {
        Component::RenderGUI();
        static bool showPath = true;
        ImGui::Checkbox("Show pathfinding", &showPath);
        ImGui::SliderFloat("Random wander chance", &m_RandomWanderChance, 0.0f, 1.0f, "%.2f");
        if (ImGui::Button("Die"))
        {
            Die();
        }
        if (ImGui::Button("Stun"))
        {
            Stun();
        }

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
    void EnemyWanderComponent::HandleEvent(const Event* event)
    {
        auto crushEvent = dynamic_cast<const EnemyCrushedEvent*>(event);

        if (crushEvent)
        {
            if (crushEvent->obj == GetOwner())
            {
                Die();
            }
        }
    }
    void EnemyWanderComponent::Die()
    {
        if (m_IsDead) return;

        m_IsDead = true;

        if (m_pAnimator)
        {
            m_pAnimator->PlayAnimation("Die");
        }

        auto collider = GetOwner()->GetComponent<BoxColliderComponent>();
        if (collider) collider->SetActive(false);

        GameTime::GetInstance().AddTimer(m_TimeBeforeDestroy, [&]() { GetOwner()->Destroy(); });
    }
    void dae::EnemyWanderComponent::Stun()
    {
        if (m_IsDead || m_IsStunned) return;

        m_IsStunned = true;

        if (m_pAnimator)
        {
            m_pAnimator->PlayAnimation("Pickled");
        }

        GameTime::GetInstance().AddTimer(m_StunDuration, [&]() 
            { 
                m_IsStunned = false; 
            
            });
    }
}