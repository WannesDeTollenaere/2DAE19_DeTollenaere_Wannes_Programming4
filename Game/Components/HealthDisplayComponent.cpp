#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include "ObserverSys/EventManager.h"
#include "Events/LivesChangedEvent.h"
#include "GameManager.h"
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class HealthDisplayComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& ) override
        {

            //int startingLives = data.value("startingLives", 3);
            //std::string targetTag = data.value("targetTag", "Player1");

            go->AddComponent<HealthDisplayComponent>();
        }
    };

    REGISTER_COMPONENT_PARSER(HealthDisplayComponent, HealthDisplayComponentParser);

    HealthDisplayComponent::HealthDisplayComponent(GameObject* pOwner)
        : Component(pOwner)
    {
        EXPOSE(m_Lives);

        EventManager::GetInstance().AttachEvent(make_sdbm_hash_rt("LivesChanged"), this);
        m_Lives = GameManager::GetInstance().GetLives();
        UpdateVisuals();
    }

    HealthDisplayComponent::~HealthDisplayComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash_rt("LivesChanged"), this);
    }

    void HealthDisplayComponent::HandleEvent(const Event* event)
    {
        if (auto pEvent = dynamic_cast<const LivesChangedEvent*>(event))
        {
            m_Lives = pEvent->lives;
            UpdateVisuals();
        }
    }

    void HealthDisplayComponent::UpdateVisuals()
    {
      
        const auto children = GetOwner()->GetChildren();

        for (size_t i = 0; i < children.size(); ++i)
        {
            if (children[i])
            {
                children[i]->SetActive(i < static_cast<size_t>(m_Lives));
            }
        }
    }
}