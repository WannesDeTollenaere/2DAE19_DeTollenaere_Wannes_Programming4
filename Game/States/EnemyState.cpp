#include "EnemyState.h"
#include "GameTime.h"

namespace dae
{
    // WANDERING
    void EnemyWanderingState::OnEnter()
    {
        GetOwner()->SetActive(true);
        if (auto collider = GetOwner()->GetComponent<BoxColliderComponent>()) {
            collider->SetActive(true);
        }
    }

    std::unique_ptr<EnemyState> EnemyWanderingState::Update()
    {
        auto anim = GetAnimator();
        if (!anim) return nullptr;

        auto dir = GetCurrentDirection();
        if (dir.x > 0) anim->PlayAnimation("WalkRight");
        else if (dir.x < 0) anim->PlayAnimation("WalkLeft");
        else if (dir.y > 0) anim->PlayAnimation("WalkDown");
        else if (dir.y < 0) anim->PlayAnimation("WalkUp");

        return nullptr;
    }

    std::unique_ptr<EnemyState> EnemyWanderingState::OnDie() { return std::make_unique<EnemyDeadState>(m_pEnemy); }
    std::unique_ptr<EnemyState> EnemyWanderingState::OnStun() { return std::make_unique<EnemyStunnedState>(m_pEnemy); }
    std::unique_ptr<EnemyState> EnemyWanderingState::OnDisable() { return std::make_unique<EnemyDisabledState>(m_pEnemy); }
    std::unique_ptr<EnemyState> EnemyWanderingState::OnSetCascading(bool cascading)
    {
        if (cascading) return std::make_unique<EnemyCascadingState>(m_pEnemy);
        return nullptr;
    }

    //STUNNED
    void EnemyStunnedState::OnEnter()
    {
        if (auto anim = GetAnimator()) anim->PlayAnimation("Pickled");
    }

    std::unique_ptr<EnemyState> EnemyStunnedState::Update()
    {
        m_Timer -= GameTime::GetInstance().GetDeltaTime();
        if (m_Timer <= 0.0f) return std::make_unique<EnemyWanderingState>(m_pEnemy);
        return nullptr;
    }

    std::unique_ptr<EnemyState> EnemyStunnedState::OnDie() { return std::make_unique<EnemyDeadState>(m_pEnemy); }

    // DEAD
    void EnemyDeadState::OnEnter()
    {
        if (auto anim = GetAnimator()) anim->PlayAnimation("Die");
        if (auto collider = GetOwner()->GetComponent<BoxColliderComponent>()) collider->SetActive(false);
    }
    std::unique_ptr<EnemyState> EnemyDeadState::Update()
    {
        float dt = dae::GameTime::GetInstance().GetDeltaTime();

        // TEMP SET TO SPACE outside of screen
        if (!m_IsDespawned)
        {
            m_DespawnTimer -= dt;
            if (m_DespawnTimer <= 0.0f)
            {
                m_IsDespawned = true;
                GetOwner()->GetTransform().SetLocalPosition(glm::vec3{ -9999.0f, -9999.0f, 0.0f });
            }
        }
        else
        {
            m_RespawnTimer -= dt;
            if (m_RespawnTimer <= 0.0f)
            {
                GetOwner()->GetTransform().SetLocalPosition(GetOriginalSpawnPosition());
                if (auto collider = GetOwner()->GetComponent<BoxColliderComponent>()) collider->SetActive(true);
                return std::make_unique<EnemyWanderingState>(m_pEnemy);
            }
        }
        return nullptr;
    }

    // DIABLED
    std::unique_ptr<EnemyState> EnemyDisabledState::OnEnable()
    {
        return std::make_unique<EnemyWanderingState>(m_pEnemy);
    }

    // Cascading
    std::unique_ptr<EnemyState> EnemyCascadingState::OnSetCascading(bool cascading)
    {
        if (!cascading) return std::make_unique<EnemyWanderingState>(m_pEnemy);
        return nullptr;
    }

    std::unique_ptr<EnemyState> EnemyCascadingState::OnDie() { return std::make_unique<EnemyDeadState>(m_pEnemy); }
}