#include "ScoreComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/EnemyCrushedEvent.h"
#include "Events/BurgerDroppedEvent.h"
#include "Events/BurgerCascadeEvent.h"
#include "Events/ScoreChangedEvent.h"
#include "Components/Enemy/EnemyComponent.h"
#include "sdbm_hash.h"
#include "GameObject.h"
#include "Events/BurgerCompletedEvent.h"
#include "GameManager.h"
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class ScoreComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            int startingScore = data.value("startingScore", 0);

            go->AddComponent<ScoreComponent>(startingScore);
        }
    };

    REGISTER_COMPONENT_PARSER(ScoreComponent, ScoreComponentParser);


    ScoreComponent::ScoreComponent(GameObject* owner, int startingScore)
        : Component(owner), m_score(startingScore)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("EnemyCrushed"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("BurgerDropped"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("BurgerCascaded"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("BurgerCompleted"), this);
    }

    ScoreComponent::~ScoreComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("EnemyCrushed"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("BurgerDropped"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("BurgerCascaded"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("BurgerCompleted"), this);
    }

    void ScoreComponent::HandleEvent(const Event* pEvent)
    {
        bool scoreChanged = false;

        int scoreToAdd{ 0 };

        // ENEMY CRUSHED
        if (pEvent->id == make_sdbm_hash("EnemyCrushed"))
        {
            if (const auto* pCrushEvent = dynamic_cast<const EnemyCrushedEvent*>(pEvent))
            {
                if (auto pWander = pCrushEvent->obj->GetComponent<EnemyComponent>())
                {
                    switch (pWander->GetEnemyType())
                    {
                    case EnemyType::HotDog: scoreToAdd += 100; break;
                    case EnemyType::Pickle: scoreToAdd += 200; break;
                    case EnemyType::Egg:    scoreToAdd += 300; break;
                    }
                    scoreChanged = true;
                }
            }
        }
        // BURGER DROPPED
        else if (pEvent->id == make_sdbm_hash("BurgerDropped"))
        {
            scoreToAdd += 50;
            scoreChanged = true;
        }
        // BURGER CASCADED
        else if (pEvent->id == make_sdbm_hash("BurgerCascaded"))
        {
            if (const auto* pCascadeEvent = dynamic_cast<const BurgerCascadedEvent*>(pEvent))
            {
                switch (pCascadeEvent->numEnemies)
                {
                case 1: scoreToAdd += 500; break;
                case 2: scoreToAdd += 1000; break;
                case 3: scoreToAdd += 2000; break;
                case 4: scoreToAdd += 4000; break;
                case 5: scoreToAdd += 8000; break;
                default:
                    if (pCascadeEvent->numEnemies >= 6) scoreToAdd += 16000;
                    break;
                }
                scoreChanged = true;
            }
        }
        // FINISHING BURGER
        else if (pEvent->id == make_sdbm_hash("BurgerCompleted"))
        {
            scoreToAdd += 1000;
            scoreChanged = true;
        } 

        // BROADCAST for scoredisplay
        if (scoreChanged)
        {
            GameManager::GetInstance().AddScore(scoreToAdd);

            ScoreChangedEvent changedEvent(GetOwner(), GameManager::GetInstance().GetScore());
            EventManager::GetInstance().SendEvent(&changedEvent);
        }
    }
}