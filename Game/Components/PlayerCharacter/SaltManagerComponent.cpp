#include "SaltManagerComponent.h"
#include "GameObject.h"
#include "ObserverSys/EventManager.h"
#include "Events/SaltChangedEvent.h"
#include <nlohmann/json.hpp>
#include "GameManager.h"
#include "SceneLoader.h"

namespace dae
{
    class SaltManagerComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            int startingSalt = data.value("startingSalt", 5);
            go->AddComponent<SaltManagerComponent>(startingSalt);
        }
    };
    REGISTER_COMPONENT_PARSER(SaltManagerComponent, SaltManagerComponentParser);


    SaltManagerComponent::SaltManagerComponent(GameObject* pOwner, int startingSalt)
        : Component(pOwner), m_Salt(startingSalt), m_StartingSalt(startingSalt)
    {
        EXPOSE(m_Salt);
        EXPOSE(m_StartingSalt);
    }

    Tag SaltManagerComponent::GetPlayerTag() const
    {
        auto tagComp = GetOwner()->GetComponent<TagComponent>();

        return tagComp ? tagComp->GetUniqueTag() : make_sdbm_hash("Player1");
    }

    void SaltManagerComponent::Update()
    {
        if (!m_IsInitialized)
        {
            Tag myTag = GetPlayerTag();

            m_Salt = GameManager::GetInstance().GetPersistentSalt(myTag);

            SaltChangedEvent saltEvent(myTag, m_Salt);
            EventManager::GetInstance().SendEvent(&saltEvent);

            m_IsInitialized = true;
        }
    }

    void SaltManagerComponent::AddSalt(int amount)
    {
        m_Salt += amount;
        if (m_Salt < 0) m_Salt = 0;  

        Tag myTag = GetPlayerTag();

        SaltChangedEvent saltEvent(myTag, m_Salt);
        EventManager::GetInstance().SendEvent(&saltEvent);
    }

    void SaltManagerComponent::ResetSalt()
    {
        m_Salt = m_StartingSalt;

        Tag myTag = GetPlayerTag();
        SaltChangedEvent saltEvent(myTag, m_Salt);
        EventManager::GetInstance().SendEvent(&saltEvent);
    }
}