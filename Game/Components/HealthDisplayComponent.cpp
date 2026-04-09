#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include "ObserverSys/EventManager.h"
#include "Events/LivesChangedEvent.h"
#include "GameManager.h"

namespace dae
{
    HealthDisplayComponent::HealthDisplayComponent(GameObject* pOwner)
        : Component(pOwner)
    {
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