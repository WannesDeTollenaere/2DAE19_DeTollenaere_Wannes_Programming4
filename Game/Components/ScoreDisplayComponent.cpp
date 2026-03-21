#include "ScoreDisplayComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/ScoreChangedEvent.h"
#include "sdbm_hash.h"
#include "GameObject.h"

namespace dae
{
    ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner, int initialScore, Tag targetTag)
        : Component(owner), m_currentScore(initialScore), m_TargetTag(targetTag),
        m_targetPlayer{ TagComponent::FindGameObject(targetTag) }
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("ScoreChanged"), this);
    }

    ScoreDisplayComponent::~ScoreDisplayComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("ScoreChanged"), this);
    }

    void ScoreDisplayComponent::HandleEvent(const Event* pEvent)
    {
        if (m_targetPlayer == nullptr)
        {
            m_targetPlayer = TagComponent::FindGameObject(m_TargetTag);
            if (m_targetPlayer == nullptr) return;
        }
        if (pEvent->id == make_sdbm_hash("ScoreChanged"))
        {
            if (const auto* pScoreChangedEvent = dynamic_cast<const ScoreChangedEvent*>(pEvent))
            {
                if (!m_targetPlayer) m_targetPlayer = TagComponent::FindGameObject(m_TargetTag);

                if (m_targetPlayer && pScoreChangedEvent->obj == m_targetPlayer)
                {
                    m_currentScore = pScoreChangedEvent->newScore;
                    m_textIsInvalid = true;
                }
            }
        }
    }

    void ScoreDisplayComponent::Update()
    {
        if (!m_textComponent)
        {
            m_textComponent = GetOwner()->GetComponent<TextComponent>();
            m_textIsInvalid = true;
        }

        if (!m_textIsInvalid) return;

        m_textComponent->SetText("Score: " + std::to_string(m_currentScore));
        m_textIsInvalid = false;
    }
}