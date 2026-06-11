#include "ScoreDisplayComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/ScoreChangedEvent.h"
#include "sdbm_hash.h"
#include "GameObject.h"
#include "GameManager.h"
#include "SceneLoader.h"
#include "sdbm_hash.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class ScoreDisplayComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            int startingScore = data.value("startingScore", 0);
            std::string targetTag = data.value("targetTag", "Player1");

            go->AddComponent<ScoreDisplayComponent>(startingScore, dae::make_sdbm_hash_rt(targetTag));
        }
    };

    REGISTER_COMPONENT_PARSER(ScoreDisplayComponent, ScoreDisplayComponentParser);


    ScoreDisplayComponent::ScoreDisplayComponent(GameObject* owner, int initialScore, Tag targetTag)
        : Component(owner), m_currentScore(initialScore), m_TargetTag(targetTag),
        m_targetPlayer{ TagComponent::FindGameObject(targetTag) }
    {
        EXPOSE(m_currentScore);

        EventManager::GetInstance().AttachEvent(make_sdbm_hash("ScoreChanged"), this);

        m_currentScore = GameManager::GetInstance().GetScore();
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

        m_textComponent->SetText(std::to_string(m_currentScore));
        m_textIsInvalid = false;
    }
}