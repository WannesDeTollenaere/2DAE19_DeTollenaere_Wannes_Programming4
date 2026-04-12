#include "HighScoreDisplayComponent.h"
#include "GameObject.h"
#include "Components/TextComponent.h"
#include "Helpers/HighScoreManager.h"
#include "ObserverSys/EventManager.h"
#include "Events/ScoreChangedEvent.h" 
#include "GameManager.h"
#include "SceneLoader.h"

namespace dae
{
    REGISTER_COMPONENT_PARSER(HighScoreDisplayComponent, SimpleParser<HighScoreDisplayComponent>);


    HighScoreDisplayComponent::HighScoreDisplayComponent(GameObject* pOwner)
        : Component(pOwner)
    {
        const auto scores = HighScoreManager::GetInstance().GetScores();
        if (!scores.empty())
        {
            m_CurrentHighScore = scores.front().score;
        }

        int currentScore = GameManager::GetInstance().GetScore();

        if (m_CurrentHighScore < currentScore) m_CurrentHighScore = currentScore;

        EventManager::GetInstance().AttachEvent(make_sdbm_hash("ScoreChanged"), this);
    }

    HighScoreDisplayComponent::~HighScoreDisplayComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("ScoreChanged"), this);
    }

    void HighScoreDisplayComponent::Update()
    {
        if (!m_pTextComponent)
        {
            m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
            if (m_pTextComponent)
            {
                UpdateVisualText(); 
            }
        }
    }

    void HighScoreDisplayComponent::HandleEvent(const Event* event)
    {
        if (event->id == make_sdbm_hash("ScoreChanged"))
        {
            auto scoreEvent = static_cast<const ScoreChangedEvent*>(event);

            if (scoreEvent && scoreEvent->newScore > m_CurrentHighScore)
            {
                m_CurrentHighScore = scoreEvent->newScore;
                UpdateVisualText();
            }
        }
    }

    void HighScoreDisplayComponent::UpdateVisualText()
    {
        if (m_pTextComponent)
        {
            m_pTextComponent->SetText(std::to_string(m_CurrentHighScore));
        }
    }
}