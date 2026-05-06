#include "PlayerState.h"
#include "Components/AnimatorComponent.h"
#include "GameTime.h"
#include "GameManager.h"
#include "GameObject.h"

namespace dae
{
    // ALIVE
    std::unique_ptr<PlayerState> AliveState::Update(CharacterControllerComponent* player)
    {
        auto anim = player->GetAnimator();
        if (!anim) return nullptr;

        const auto& currentPos = GetOwner(player)->GetTransform().GetWorldPosition();

        if (currentPos == player->GetLastPosition())
        {
            anim->PlayAnimation("Idle");
        }
        else
        {
            auto dir = player->GetFacingDirection();
            if (dir.y < 0.0f) anim->PlayAnimation("WalkUp");
            else if (dir.y > 0.0f) anim->PlayAnimation("WalkDown");
            else if (dir.x < 0.0f) anim->PlayAnimation("WalkLeft");
            else if (dir.x > 0.0f) anim->PlayAnimation("WalkRight");
        }

        player->SetLastPosition(currentPos);
        return nullptr; 
    }

    std::unique_ptr<PlayerState> AliveState::ThrowSalt(CharacterControllerComponent* player)
    {
        player->PerformThrowSalt();
        return nullptr;
    }


    std::unique_ptr<PlayerState> AliveState::Die(CharacterControllerComponent* )
    {
        return std::make_unique<DeadState>();
    }

    std::unique_ptr<PlayerState> AliveState::CompleteLevel(CharacterControllerComponent* )
    {
        return std::make_unique<VictoryState>();
    }


    // DEAD
    void DeadState::OnEnter(CharacterControllerComponent* player)
    {
        if (auto anim = player->GetAnimator()) {
            anim->PlayAnimation("Die");
        }
    }

    std::unique_ptr<PlayerState> DeadState::Update(CharacterControllerComponent* player)
    {
        m_RespawnTimer -= dae::GameTime::GetInstance().GetDeltaTime();

        if (m_RespawnTimer <= 0.0f)
        {
            dae::GameManager::GetInstance().LoseLife();

            GetOwner(player)->GetTransform().SetLocalPosition(GetSpawnPosition(player));

            return std::make_unique<AliveState>();
        }

        return nullptr;
    }


    // VICTORY
    void VictoryState::OnEnter(CharacterControllerComponent* player)
    {
        if (auto anim = player->GetAnimator()) {
            anim->PlayAnimation("Victory");
        }
        player->UnbindInput();
    }
}