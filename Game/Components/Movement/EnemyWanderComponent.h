#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace dae
{
    class GridMovementComponent;
    class AnimatorComponent;

    class EnemyWanderComponent final : public Component
    {
    public:
        EnemyWanderComponent(GameObject* pOwner);
        ~EnemyWanderComponent() = default;

        void Update() override;

    private:
        GridMovementComponent* m_pMovementComponent{ nullptr };
        AnimatorComponent* m_pAnimator{ nullptr };

        glm::vec2 m_CurrentDirection{ 1.0f, 0.0f };
        glm::vec3 m_LastPosition{};

        const float m_CooldownDuration{ .3f };
        bool m_DecisionAvailable{ true };

        void PickNewDirection();
        void UpdateAnimation();
    };
}