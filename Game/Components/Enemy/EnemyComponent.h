#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "ObserverSys/Observer.h"
#include <memory>

namespace dae
{
    class AnimatorComponent;
    class EnemyState; 

    enum class EnemyType : uint8_t
    {
        HotDog, Pickle, Egg
    };

    class EnemyComponent final : public Component, public Observer
    {
        friend class EnemyState; 
    public:
        EnemyComponent(GameObject* pOwner);
        ~EnemyComponent();

        void Update() override;
        void HandleEvent(const Event* event) override;

        void Die();
        bool IsDead() const;
        void Respawn();

        void Stun();
        bool IsStunned() const;

        bool IsDangerous() const;

        void DisableMovement();
        void EnableMovement();
        bool IsMovementDisabled() const;

        void SetCascading(bool cascading);
        bool IsCascading() const;

        EnemyType GetEnemyType() const { return m_EnemyType; }
        void SetEnemyType(EnemyType type) { m_EnemyType = type; }
        void SetCurrentDirection(const glm::vec2& dir) { m_CurrentDirection = dir; }

    private:
        std::unique_ptr<EnemyState> m_pCurrentState;
        void ChangeState(std::unique_ptr<EnemyState> newState);

        EnemyType m_EnemyType{ EnemyType::HotDog };
        AnimatorComponent* m_pAnimator{ nullptr };

        glm::vec2 m_CurrentDirection{ 0.0f, 1.0f };
        glm::vec3 m_OriginalSpawnPosition{};
    };
}