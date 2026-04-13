#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace dae
{
    class GridMovementComponent;
    class EnemyComponent; 

    class EnemyWanderComponent final : public Component
    {
    public:
        EnemyWanderComponent(GameObject* pOwner);
        ~EnemyWanderComponent() = default;

        void Update() override;
        void RenderGUI() override;

    private:
        GridMovementComponent* m_pMovementComponent{ nullptr };
        EnemyComponent* m_pEnemyComp{ nullptr };
        GameObject* m_pPlayer{ nullptr };

        glm::vec2 m_CurrentDirection{ 1.0f, 0.0f };
        glm::vec3 m_LastPosition{};

        const float m_CooldownDuration{ .3f };
        float m_RandomWanderChance{ 0.15f };
        bool m_DecisionAvailable{ true };

        std::vector<glm::ivec2> m_Path;

        void PickNewDirection();
        void FindPlayer();
    };
}