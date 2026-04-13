#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "ObserverSys/Observer.h"

namespace dae
{
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

    class EnemyComponent final : public Component, public Observer
    {
    public:
        EnemyComponent(GameObject* pOwner);
        ~EnemyComponent();

        void Update() override;
        void HandleEvent(const Event* event) override;

        void Die();
        bool IsDead() const { return m_State == EnemyState::Dead; }
        void Respawn();

        void Stun();
        bool IsStunned() const { return m_State == EnemyState::Stunned; }

        bool IsDangerous() const { return !IsStunned() && !IsDead() && !IsCascading(); }

        void DisableMovement();
        void EnableMovement();
        bool IsMovementDisabled() const { return m_State == EnemyState::Disabled || IsCascading() || IsStunned() || IsDead(); }

        void SetCascading(bool cascading);
        bool IsCascading() const { return m_State == EnemyState::Cascading; }

        EnemyType GetEnemyType() const { return m_EnemyType; }
        void SetEnemyType(EnemyType type) { m_EnemyType = type; }

        void SetCurrentDirection(const glm::vec2& dir) { m_CurrentDirection = dir; }

    private:
        EnemyState m_State{ EnemyState::Wandering };
        EnemyType m_EnemyType{ EnemyType::HotDog };
        AnimatorComponent* m_pAnimator{ nullptr };

        glm::vec2 m_CurrentDirection{ 0.0f, 1.0f };
        glm::vec3 m_OriginalSpawnPosition{};

        const float m_TimeBeforeDestroy{ 1.0f };
        const float m_StunDuration{ 3.0f };
        float m_RespawnDuration{ 4.0f };

        void UpdateAnimation();
    };
}