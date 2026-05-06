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
    public:
        virtual ~EnemyState() = default;

        virtual void OnEnter(EnemyComponent*) {}
        virtual std::unique_ptr<EnemyState> Update(EnemyComponent*) { return nullptr; }

        virtual std::unique_ptr<EnemyState> OnDie(EnemyComponent* ) { return nullptr; }
        virtual std::unique_ptr<EnemyState> OnStun(EnemyComponent* ) { return nullptr; }
        virtual std::unique_ptr<EnemyState> OnDisable(EnemyComponent* ) { return nullptr; }
        virtual std::unique_ptr<EnemyState> OnEnable(EnemyComponent* ) { return nullptr; }
        virtual std::unique_ptr<EnemyState> OnSetCascading(EnemyComponent* , bool ) { return nullptr; }

        virtual bool IsMovementDisabled() const { return true; }
        virtual bool IsDangerous() const { return false; }
        virtual bool IsDead() const { return false; }
        virtual bool IsStunned() const { return false; }
        virtual bool IsCascading() const { return false; }

    protected:
        GameObject* GetOwner(EnemyComponent* enemy) const { return enemy->GetOwner(); }
        AnimatorComponent* GetAnimator(EnemyComponent* enemy) const { return enemy->m_pAnimator; }
        const glm::vec2& GetCurrentDirection(EnemyComponent* enemy) const { return enemy->m_CurrentDirection; }
        const glm::vec3& GetOriginalSpawnPosition(EnemyComponent* enemy) const { return enemy->m_OriginalSpawnPosition; }
    };

    class EnemyWanderingState final : public EnemyState
    {
    public:
        void OnEnter(EnemyComponent* enemy) override;
        std::unique_ptr<EnemyState> Update(EnemyComponent* enemy) override;

        std::unique_ptr<EnemyState> OnDie(EnemyComponent* enemy) override;
        std::unique_ptr<EnemyState> OnStun(EnemyComponent* enemy) override;
        std::unique_ptr<EnemyState> OnDisable(EnemyComponent* enemy) override;
        std::unique_ptr<EnemyState> OnSetCascading(EnemyComponent* enemy, bool cascading) override;

        bool IsMovementDisabled() const override { return false; }
        bool IsDangerous() const override { return true; }
    };

    class EnemyStunnedState final : public EnemyState
    {
    public:
        void OnEnter(EnemyComponent* enemy) override;
        std::unique_ptr<EnemyState> Update(EnemyComponent* enemy) override;
        std::unique_ptr<EnemyState> OnDie(EnemyComponent* enemy) override;

        bool IsStunned() const override { return true; }
    private:
        float m_Timer{ 3.0f };
    };

    class EnemyDeadState final : public EnemyState
    {
    public:
        void OnEnter(EnemyComponent* enemy) override;
        std::unique_ptr<EnemyState> Update(EnemyComponent* enemy) override;

        bool IsDead() const override { return true; }
    private:
        float m_DespawnTimer{ 1.0f };
        float m_RespawnTimer{ 4.0f };
        bool m_IsDespawned{ false };
    };

    class EnemyDisabledState final : public EnemyState
    {
    public:
        std::unique_ptr<EnemyState> OnEnable(EnemyComponent* enemy) override;
    };

    class EnemyCascadingState final : public EnemyState
    {
    public:
        std::unique_ptr<EnemyState> OnSetCascading(EnemyComponent* enemy, bool cascading) override;
        std::unique_ptr<EnemyState> OnDie(EnemyComponent* enemy) override;
        bool IsCascading() const override { return true; }
    };
}