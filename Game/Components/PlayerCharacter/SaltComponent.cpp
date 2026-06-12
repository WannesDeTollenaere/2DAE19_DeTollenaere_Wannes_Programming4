#include "SaltComponent.h"
#include "GameObject.h"
#include "Components/TagComponent.h"
#include "Components/Enemy/EnemyComponent.h"
#include "GameTime.h"
#include "SceneLoader.h"
#include "Sound/ServiceLocator.h"
#include "Helpers/SoundIDs.h"
#include "ResourceManager.h"

namespace dae
{
    REGISTER_COMPONENT_PARSER(SaltComponent, SimpleParser<SaltComponent>);

    SaltComponent::SaltComponent(GameObject* pOwner)
        : BaseCollisionHandler(pOwner)
    {
        auto& resources = dae::ResourceManager::GetInstance();
        ServiceLocator::GetSoundSystem().load(SoundID::PepperShake, resources.GetFullPathForFile("Audio/Pepper Shake.wav"));
        ServiceLocator::GetSoundSystem().load(SoundID::EnemySprayed, resources.GetFullPathForFile("Audio/Enemy Sprayed.wav"));
        ServiceLocator::GetSoundSystem().play(SoundID::PepperShake, 1.0f, 0);

    }

    void SaltComponent::OnCollision(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(dae::make_sdbm_hash_rt("Enemy")))
        {

            auto pEnemyWander = otherObject->GetComponent<EnemyComponent>();
            if (pEnemyWander && !pEnemyWander->IsStunned() && !pEnemyWander->IsDead())
            {
                pEnemyWander->Stun();
                ServiceLocator::GetSoundSystem().play(SoundID::EnemySprayed, 1.0f, 0);
            }
        }
    }
}