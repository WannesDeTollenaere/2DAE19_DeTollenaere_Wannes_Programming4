#include "PickupComponent.h"
#include "GameObject.h"
#include "Components/PlayerCharacter/SaltManagerComponent.h"
#include "Events/PickupCollectedEvent.h"
#include "ObserverSys/EventManager.h"
#include "SceneLoader.h"
#include "sdbm_hash.h"
#include "Sound/ServiceLocator.h"
#include "Helpers/SoundIDs.h"
#include "ResourceManager.h"

namespace dae
{
    PickupType PickupTypeFromString(const std::string& str)
    {
        if (str == "Coffee") return PickupType::Coffee;
        if (str == "Fries") return PickupType::Fries;
        return PickupType::IceCream;
    }


    class PickupComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            PickupType type = PickupTypeFromString(data.value("pickupType", "IceCream"));
            int saltAmount = data.value("saltAmount", 1);
            int scoreValue = data.value("scoreValue", 0);
            go->AddComponent<PickupComponent>(type, saltAmount, scoreValue);
        }

    };
    REGISTER_COMPONENT_PARSER(PickupComponent, PickupComponentParser);

    PickupComponent::PickupComponent(GameObject* owner, PickupType type, int saltAmount, int scoreValue)
        : BaseCollisionHandler(owner)
        , m_Type(type)
        , m_SaltAmount(saltAmount)
        , m_ScoreValue(scoreValue)
    {
        std::string path = dae::ResourceManager::GetInstance().GetFullPathForFile("Audio/Bonus Obtained.wav");
        ServiceLocator::GetSoundSystem().load(SoundID::BonusObtained, path);
    }

    void PickupComponent::OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (m_IsCollected)
            return;

        if (!otherTagComp || !otherTagComp->HasTag(make_sdbm_hash_rt("Player")))
            return;

        auto saltManager = otherObject->GetComponent<SaltManagerComponent>();
        if (!saltManager)
            return;

        m_IsCollected = true;

        saltManager->AddSalt(m_SaltAmount);

        ServiceLocator::GetSoundSystem().play(SoundID::BonusObtained, 1.0f, 0);

        PickupCollectedEvent collectedEvent(otherTagComp->GetUniqueTag(), m_Type, m_ScoreValue);
        EventManager::GetInstance().SendEvent(&collectedEvent);

        GetOwner()->Destroy();
    }
}
