#include "SaltComponent.h"
#include "GameObject.h"
#include "Components/TagComponent.h"
#include "Components/Movement/EnemyWanderComponent.h"
#include "GameTime.h"

namespace dae
{
    SaltComponent::SaltComponent(GameObject* pOwner)
        : BaseCollisionHandler(pOwner)
    {
    }

    void SaltComponent::OnCollision(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(dae::make_sdbm_hash_rt("Enemy")))
        {

            auto pEnemyWander = otherObject->GetComponent<EnemyWanderComponent>();
            if (pEnemyWander && !pEnemyWander->IsStunned() && !pEnemyWander->IsDead())
            {
                pEnemyWander->Stun();
            }
        }
    }
}