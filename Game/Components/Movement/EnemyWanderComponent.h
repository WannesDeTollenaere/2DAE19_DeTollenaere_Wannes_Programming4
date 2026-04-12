#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "ObserverSys/Observer.h"

namespace dae
{
    class GridMovementComponent;
    class AnimatorComponent;

    enum class EnemyType : uint8_t
    {
        HotDog,
        Pickle,
        Egg
    };

    enum class EnemyState : uint8_t
    {
        Wandering,
        Disabled,
        Cascading,
        Stunned,
        Dead
    };

    class EnemyWanderComponent final : public Component, public Observer
    {
    public:
        EnemyWanderComponent(GameObject* pOwner);
        ~EnemyWanderComponent();

        void Update() override;
        void RenderGUI() override;

        void HandleEvent(const Event* event) override;

        // DIE
        void Die();
        bool IsDead() const { return m_State == EnemyState::Dead; }
        void Respawn();

        // STUN
        void Stun();
        bool IsStunned() const { return m_State == EnemyState::Stunned; }

        bool IsDangerous() const { return !IsStunned() && !IsDead() && !IsCascading(); }

        void DisableMovement();
        void EnableMovement();
        bool IsMovementDisabled() const { return m_State == EnemyState::Disabled || IsCascading() || IsStunned() || IsDead(); }

        // CASCADING
        void SetCascading(bool cascading);
        bool IsCascading() const { return m_State == EnemyState::Cascading; }

        // TYPE
        EnemyType GetEnemyType() const { return m_EnemyType; }
        void SetEnemyType(EnemyType type) { m_EnemyType = type; }
    private:

        GridMovementComponent* m_pMovementComponent{ nullptr };
        AnimatorComponent* m_pAnimator{ nullptr };
        GameObject* m_pPlayer{ nullptr };


        glm::vec2 m_CurrentDirection{ 1.0f, 0.0f };
        glm::vec3 m_LastPosition{};

        const float m_CooldownDuration{ .3f };
        float m_RandomWanderChance{ 0.15f };
        bool m_DecisionAvailable{ true };

        std::vector<glm::ivec2> m_Path;

        // Die
        const float m_TimeBeforeDestroy{ 1.0f };

        // Stun
        const float m_StunDuration{ 3.0f };

        glm::vec3 m_OriginalSpawnPosition{};
        float m_RespawnDuration{ 4.0f };

        void PickNewDirection();
        void UpdateAnimation();
        void FindPlayer();

        EnemyState m_State{ EnemyState::Wandering };
        EnemyType m_EnemyType{ EnemyType::HotDog };
    };
}