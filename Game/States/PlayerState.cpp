#include "PlayerState.h"
#include "Components/AnimatorComponent.h"
#include "GameTime.h"
#include "GameManager.h"
#include "GameObject.h"

namespace dae
{
    // ALIVE
    std::unique_ptr<PlayerState> AliveState::Update()
    {
        auto anim = m_pPlayer->GetAnimator();
        if (!anim) return nullptr;

        const auto& currentPos = GetOwner()->GetTransform().GetWorldPosition();

        if (currentPos == m_pPlayer->GetLastPosition())
        {
            anim->PlayAnimation("Idle");
        }
        else
        {
            auto dir = m_pPlayer->GetFacingDirection();
            if (dir.y < 0.0f) anim->PlayAnimation("WalkUp");
            else if (dir.y > 0.0f) anim->PlayAnimation("WalkDown");
            else if (dir.x < 0.0f) anim->PlayAnimation("WalkLeft");
            else if (dir.x > 0.0f) anim->PlayAnimation("WalkRight");
        }

        m_pPlayer->SetLastPosition(currentPos);
        return nullptr;
    }

    std::unique_ptr<PlayerState> AliveState::ThrowSalt()
    {
        m_pPlayer->PerformThrowSalt();
        return nullptr;
    }

    std::unique_ptr<PlayerState> AliveState::Die()
    {
        return std::make_unique<DeadState>(m_pPlayer);
    }

    std::unique_ptr<PlayerState> AliveState::CompleteLevel()
    {
        return std::make_unique<VictoryState>(m_pPlayer);
    }


    // DEAD
    void DeadState::OnEnter()
    {
        if (auto anim = m_pPlayer->GetAnimator()) {
            anim->PlayAnimation("Die");
        }
    }

    std::unique_ptr<PlayerState> DeadState::Update()
    {
        m_RespawnTimer -= dae::GameTime::GetInstance().GetDeltaTime();

        if (m_RespawnTimer <= 0.0f)
        {
            dae::GameManager::GetInstance().LoseLife();

            GetOwner()->GetTransform().SetLocalPosition(GetSpawnPosition());

            return std::make_unique<AliveState>(m_pPlayer);
        }

        return nullptr;
    }


    // VICTORY
    void VictoryState::OnEnter()
    {
        if (auto anim = m_pPlayer->GetAnimator()) {
            anim->PlayAnimation("Victory");
        }
        m_pPlayer->UnbindInput();
    }
}