#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "ObserverSys/Observer.h"

namespace dae
{
    class GridMovementComponent;
    class AnimatorComponent;

    class EnemyWanderComponent final : public Component, public Observer
    {
    public:
        EnemyWanderComponent(GameObject* pOwner);
        ~EnemyWanderComponent();

        void Update() override;
        void RenderGUI() override;

        void HandleEvent(const Event* event) override;

        void Die();
        bool IsDead() const { return m_IsDead; }
    private:

        GridMovementComponent* m_pMovementComponent{ nullptr };
        AnimatorComponent* m_pAnimator{ nullptr };
        GameObject* m_pPlayer{ nullptr };


        glm::vec2 m_CurrentDirection{ 1.0f, 0.0f };
        glm::vec3 m_LastPosition{};

        const float m_CooldownDuration{ .3f };
        float m_RandomWanderChance{ 0.3f };
        bool m_DecisionAvailable{ true };

        std::vector<glm::ivec2> m_Path;

        // Die
        bool m_IsDead{ false };
        const float m_TimeBeforeDestroy{ 1.0f };

        void PickNewDirection();
        void UpdateAnimation();
        void FindPlayer();

    };
}