// Game/Components/ScoreComponent.cpp
#include "ScoreComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/ScoreIncreaseEvent.h"
#include "Events/ScoreChangedEvent.h"
#include "sdbm_hash.h"
#include "GameObject.h"

namespace dae
{
    ScoreComponent::ScoreComponent(GameObject* owner, int startingScore)
        : Component(owner), m_score(startingScore)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("ScoreIncreased"), this);
    }


    void ScoreComponent::HandleEvent(const Event* pEvent)
    {
        if (pEvent->id == make_sdbm_hash("ScoreIncreased"))
        {
            if (const auto* pScoreEvent = dynamic_cast<const ScoreIncreasedEvent*>(pEvent))
            {

                if (pScoreEvent->obj == GetOwner())
                {
                    m_score += pScoreEvent->scoreAdded;

                    ScoreChangedEvent changedEvent(GetOwner(), m_score);
                    EventManager::GetInstance().SendEvent(&changedEvent);
                }
            }
        }
    }
}