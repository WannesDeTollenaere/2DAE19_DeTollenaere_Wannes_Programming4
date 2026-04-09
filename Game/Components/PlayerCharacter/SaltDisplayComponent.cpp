#include "SaltDisplayComponent.h"
#include "GameObject.h"
#include "Components/TextComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/SaltChangedEvent.h"
#include "GameManager.h"
#include <string>

namespace dae
{
    SaltDisplayComponent::SaltDisplayComponent(GameObject* pOwner)
        : Component(pOwner)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash_rt("SaltChanged"), this);

        m_Salt = GameManager::GetInstance().GetSalt();
        UpdateText();
    }

    SaltDisplayComponent::~SaltDisplayComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash_rt("SaltChanged"), this);
    }


    void SaltDisplayComponent::HandleEvent(const Event* event)
    {
        if (auto pEvent = dynamic_cast<const SaltChangedEvent*>(event))
        {
            m_Salt = pEvent->currentSalt;
            UpdateText();
        }
    }

    void SaltDisplayComponent::UpdateText()
    {
        if (auto textComp = GetOwner()->GetComponent<TextComponent>())
        {
            textComp->SetText("" + std::to_string(m_Salt));
        }
    }
}