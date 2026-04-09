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
#include "Events/LivesLostEvent.h"

namespace dae
{
    EnemyWanderComponent::EnemyWanderComponent(GameObject* pOwner)
        : Component(pOwner)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("EnemyCrushed"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("LivesLost"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("LevelCompleted"), this);

        m_OriginalSpawnPosition = GetOwner()->GetTransform().GetLocalPosition();
    }

    EnemyWanderComponent::~EnemyWanderComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("EnemyCrushed"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("LivesLost"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("LevelCompleted"), this);
    }

    void EnemyWanderComponent::Update()
    {
        if (!m_pMovementComponent) m_pMovementComponent = GetOwner()->GetComponent<GridMovementComponent>();
        if (!m_pAnimator) m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();
        if (!m_pMovementComponent) return;

        if (m_State != EnemyState::Wandering) return;

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

        if (m_DecisionAvailable && (isStuck || ((currentTile == TileType::Intersection && isCentered))|| (currentTile == TileType::IntersectionDownOnly && isCentered)))
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
        ImGui::SliderFloat("Respawn duration", &m_RespawnDuration, 0.5f, 20.0f, "%.1f sec");

        float pos[3] = { m_OriginalSpawnPosition.x, m_OriginalSpawnPosition.y, m_OriginalSpawnPosition.z };

        if (ImGui::DragFloat3("Spawn position", pos, 0.1f))
        {
            m_OriginalSpawnPosition.x = pos[0];
            m_OriginalSpawnPosition.y = pos[1];
            m_OriginalSpawnPosition.z = pos[2];
        }

        if (ImGui::Button("Die"))
        {
            Die();
        }
        if (ImGui::Button("Stun"))
        {
            Stun();
        }
        if (ImGui::Button("Disable movement"))
        {
            DisableMovement();
        }
        if (ImGui::Button("Enable movement"))
        {
            EnableMovement();
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
            return;
        }
        auto livesEvent = dynamic_cast<const LivesLostEvent*>(event);

        if (livesEvent)
        {
            Respawn(); 
            return;
        }

        if (event && event->id == make_sdbm_hash("LevelCompleted"))
        {
            DisableMovement();
        }
    }
    void EnemyWanderComponent::Die()
    {
        if (m_State == EnemyState::Dead) return;

        m_State = EnemyState::Dead;

        if (m_pAnimator)
        {
            m_pAnimator->PlayAnimation("Die");
        }

        auto collider = GetOwner()->GetComponent<BoxColliderComponent>();
        if (collider) collider->SetActive(false);

        GameTime::GetInstance().AddTimer(m_TimeBeforeDestroy, [this]() {

            GetOwner()->SetActive(false);

            GameTime::GetInstance().AddTimer(m_RespawnDuration, [this]() {
                Respawn();
                });

            });
    }
    void dae::EnemyWanderComponent::Stun()
    {
        if (m_State == EnemyState::Dead || m_State == EnemyState::Cascading || m_State == EnemyState::Stunned || m_State == EnemyState::Disabled) return;

        m_State = EnemyState::Stunned;

        if (m_pAnimator)
        {
            m_pAnimator->PlayAnimation("Pickled");
        }

        GameTime::GetInstance().AddTimer(m_StunDuration, [&]() 
            { 
                m_State = EnemyState::Wandering;
            
            });
    }
    void EnemyWanderComponent::DisableMovement()
    {
        m_State = EnemyState::Disabled;
        if (m_pAnimator)
        {
            m_pAnimator->SetActive(false); 
        }
    }
    void EnemyWanderComponent::EnableMovement()
    {
        m_State = EnemyState::Wandering;
        if (m_pAnimator)
        {
            m_pAnimator->SetActive(true);
        }
    }
    void EnemyWanderComponent::SetCascading(bool cascading)
    {
        if (m_State == EnemyState::Dead) return;

        if (cascading)
        {
            m_State = EnemyState::Cascading;
        }
        else
        {
            m_State = EnemyState::Wandering;
        }
    }
    void dae::EnemyWanderComponent::Respawn()
    {
        // Reset pos
        GetOwner()->GetTransform().SetLocalPosition(m_OriginalSpawnPosition);

        m_State = EnemyState::Wandering;

        GetOwner()->SetActive(true);

        auto collider = GetOwner()->GetComponent<BoxColliderComponent>();
        if (collider) collider->SetActive(true);

        if (m_pAnimator) 
        {
            m_pAnimator->PlayAnimation("WalkDown");
        }

        m_Path.clear();
        m_DecisionAvailable = true; 
    }
}