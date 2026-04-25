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
        std::string path = dae::ResourceManager::GetInstance().GetFullPathForFile("Audio/Pepper Shake.wav");
        ServiceLocator::get_sound_system().load(SoundID::PepperShake, path);
        ServiceLocator::get_sound_system().play(SoundID::PepperShake, 1.0f, 0);

    }

    void SaltComponent::OnCollision(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(dae::make_sdbm_hash_rt("Enemy")))
        {

            auto pEnemyWander = otherObject->GetComponent<EnemyComponent>();
            if (pEnemyWander && !pEnemyWander->IsStunned() && !pEnemyWander->IsDead())
            {
                pEnemyWander->Stun();

            }
        }
    }
}