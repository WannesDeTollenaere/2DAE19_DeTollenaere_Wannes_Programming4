#include "EnemyState.h"
#include "GameTime.h"

namespace dae
{
    // WANDERING
    void EnemyWanderingState::OnEnter(EnemyComponent* enemy)
    {
        GetOwner(enemy)->SetActive(true);
        if (auto collider = GetOwner(enemy)->GetComponent<BoxColliderComponent>()) {
            collider->SetActive(true);
        }
    }

    std::unique_ptr<EnemyState> EnemyWanderingState::Update(EnemyComponent* enemy)
    {
        auto anim = GetAnimator(enemy);
        if (!anim) return nullptr;

        auto dir = GetCurrentDirection(enemy);
        if (dir.x > 0) anim->PlayAnimation("WalkRight");
        else if (dir.x < 0) anim->PlayAnimation("WalkLeft");
        else if (dir.y > 0) anim->PlayAnimation("WalkDown");
        else if (dir.y < 0) anim->PlayAnimation("WalkUp");

        return nullptr;
    }

    std::unique_ptr<EnemyState> EnemyWanderingState::OnDie(EnemyComponent*) { return std::make_unique<EnemyDeadState>(); }
    std::unique_ptr<EnemyState> EnemyWanderingState::OnStun(EnemyComponent*) { return std::make_unique<EnemyStunnedState>(); }
    std::unique_ptr<EnemyState> EnemyWanderingState::OnDisable(EnemyComponent*) { return std::make_unique<EnemyDisabledState>(); }
    std::unique_ptr<EnemyState> EnemyWanderingState::OnSetCascading(EnemyComponent*, bool cascading)
    {
        if (cascading) return std::make_unique<EnemyCascadingState>();
        return nullptr;
    }

    //STUNNED
    void EnemyStunnedState::OnEnter(EnemyComponent* enemy)
    {
        if (auto anim = GetAnimator(enemy)) anim->PlayAnimation("Pickled");
    }

    std::unique_ptr<EnemyState> EnemyStunnedState::Update(EnemyComponent*)
    {
        m_Timer -= GameTime::GetInstance().GetDeltaTime();
        if (m_Timer <= 0.0f) return std::make_unique<EnemyWanderingState>();
        return nullptr;
    }

    std::unique_ptr<EnemyState> EnemyStunnedState::OnDie(EnemyComponent*) { return std::make_unique<EnemyDeadState>(); }

    // DEAD
    void EnemyDeadState::OnEnter(EnemyComponent* enemy)
    {
        if (auto anim = GetAnimator(enemy)) anim->PlayAnimation("Die");
        if (auto collider = GetOwner(enemy)->GetComponent<BoxColliderComponent>()) collider->SetActive(false);
    }
    std::unique_ptr<EnemyState> EnemyDeadState::Update(EnemyComponent* enemy)
    {
        float dt = dae::GameTime::GetInstance().GetDeltaTime();

        // TEMP SET TO SPACE outside of screen
        if (!m_IsDespawned)
        {
            m_DespawnTimer -= dt;
            if (m_DespawnTimer <= 0.0f)
            {
                m_IsDespawned = true;
                GetOwner(enemy)->GetTransform().SetLocalPosition(glm::vec3{ -9999.0f, -9999.0f, 0.0f });
            }
        }
        else
        {
            m_RespawnTimer -= dt;
            if (m_RespawnTimer <= 0.0f)
            {
                GetOwner(enemy)->GetTransform().SetLocalPosition(GetOriginalSpawnPosition(enemy));
                if (auto collider = GetOwner(enemy)->GetComponent<BoxColliderComponent>()) collider->SetActive(true);
                return std::make_unique<EnemyWanderingState>();
            }
        }
        return nullptr;
    }

    // DIABLED
    std::unique_ptr<EnemyState> EnemyDisabledState::OnEnable(EnemyComponent*)
    {
        return std::make_unique<EnemyWanderingState>();
    }

    // Cascading
    std::unique_ptr<EnemyState> EnemyCascadingState::OnSetCascading(EnemyComponent*, bool cascading)
    {
        if (!cascading) return std::make_unique<EnemyWanderingState>();
        return nullptr;
    }

    std::unique_ptr<EnemyState> EnemyCascadingState::OnDie(EnemyComponent*) { return std::make_unique<EnemyDeadState>(); }
}