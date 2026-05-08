#pragma once
#include <memory>
#include "Components/Enemy/EnemyComponent.h"
#include "GameObject.h"
#include "Components/AnimatorComponent.h"
#include "Components/BoxColliderComponent.h"

namespace dae
{
    class EnemyState
    {
    protected:
        EnemyComponent* m_pEnemy;

    public:
        EnemyState(EnemyComponent* enemy) : m_pEnemy(enemy) {}
        virtual ~EnemyState() = default;

        virtual void OnEnter() {}
        virtual std::unique_ptr<EnemyState> Update() { return nullptr; }

        virtual std::unique_ptr<EnemyState> OnDie() { return nullptr; }
        virtual std::unique_ptr<EnemyState> OnStun() { return nullptr; }
        virtual std::unique_ptr<EnemyState> OnDisable() { return nullptr; }
        virtual std::unique_ptr<EnemyState> OnEnable() { return nullptr; }
        virtual std::unique_ptr<EnemyState> OnSetCascading(bool) { return nullptr; }

        virtual bool IsMovementDisabled() const { return true; }
        virtual bool IsDangerous() const { return false; }
        virtual bool IsDead() const { return false; }
        virtual bool IsStunned() const { return false; }
        virtual bool IsCascading() const { return false; }

    protected:
        GameObject* GetOwner() const { return m_pEnemy->GetOwner(); }
        AnimatorComponent* GetAnimator() const { return m_pEnemy->m_pAnimator; }
        const glm::vec2& GetCurrentDirection() const { return m_pEnemy->m_CurrentDirection; }
        const glm::vec3& GetOriginalSpawnPosition() const { return m_pEnemy->m_OriginalSpawnPosition; }
    };

    class EnemyWanderingState final : public EnemyState
    {
    public:
        EnemyWanderingState(EnemyComponent* enemy) : EnemyState(enemy) {}
        void OnEnter() override;
        std::unique_ptr<EnemyState> Update() override;

        std::unique_ptr<EnemyState> OnDie() override;
        std::unique_ptr<EnemyState> OnStun() override;
        std::unique_ptr<EnemyState> OnDisable() override;
        std::unique_ptr<EnemyState> OnSetCascading(bool cascading) override;

        bool IsMovementDisabled() const override { return false; }
        bool IsDangerous() const override { return true; }
    };

    class EnemyStunnedState final : public EnemyState
    {
    public:
        EnemyStunnedState(EnemyComponent* enemy) : EnemyState(enemy) {}
        void OnEnter() override;
        std::unique_ptr<EnemyState> Update() override;
        std::unique_ptr<EnemyState> OnDie() override;

        bool IsStunned() const override { return true; }
    private:
        float m_Timer{ 3.0f };
    };

    class EnemyDeadState final : public EnemyState
    {
    public:
        EnemyDeadState(EnemyComponent* enemy) : EnemyState(enemy) {}
        void OnEnter() override;
        std::unique_ptr<EnemyState> Update() override;

        bool IsDead() const override { return true; }
    private:
        float m_DespawnTimer{ 1.0f };
        float m_RespawnTimer{ 4.0f };
        bool m_IsDespawned{ false };
    };

    class EnemyDisabledState final : public EnemyState
    {
    public:
        EnemyDisabledState(EnemyComponent* enemy) : EnemyState(enemy) {}
        std::unique_ptr<EnemyState> OnEnable() override;
    };

    class EnemyCascadingState final : public EnemyState
    {
    public:
        EnemyCascadingState(EnemyComponent* enemy) : EnemyState(enemy) {}
        std::unique_ptr<EnemyState> OnSetCascading(bool cascading) override;
        std::unique_ptr<EnemyState> OnDie() override;
        bool IsCascading() const override { return true; }
    };
}