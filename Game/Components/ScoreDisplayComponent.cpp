#include "ScoreDisplayComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/ScoreIncreaseEvent.h"
#include "sdbm_hash.h"
#include "GameObject.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner, int startingScore, Tag targetTag)
    : Component(owner),
    m_score(startingScore),
    m_TargetTag(targetTag),
    m_targetPlayer{ TagComponent::FindGameObject(targetTag) }
{
    EventManager::GetInstance().AttachEvent(make_sdbm_hash("ScoreIncreased"), this);
}


void dae::ScoreDisplayComponent::HandleEvent(const Event* pEvent)
{
    if (pEvent->id == make_sdbm_hash("ScoreIncreased"))
    {
        if (const auto* pScoreEvent = dynamic_cast<const ScoreIncreasedEvent*>(pEvent))
        {
            if (m_targetPlayer && pScoreEvent->obj == m_targetPlayer)
            {
                m_score += pScoreEvent->scoreAdded;
                m_textIsInvalid =  true ;
            }
        }
    }
}

void dae::ScoreDisplayComponent::Update()
{
    if (!m_textComponent)
    {
        m_textComponent = GetOwner()->GetComponent<TextComponent>();
        m_textIsInvalid =  true;
    }
    if (!m_targetPlayer)
    {
        m_targetPlayer = TagComponent::FindGameObject(m_TargetTag);
    }
    if (!m_textIsInvalid) return;

    m_textComponent->SetText("Score: " + std::to_string(m_score));
    m_textIsInvalid = false;
}
