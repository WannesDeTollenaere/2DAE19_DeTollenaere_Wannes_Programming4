#include "ScoreComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/EnemyCrushedEvent.h"
#include "Events/BurgerDroppedEvent.h"
#include "Events/BurgerCascadeEvent.h"
#include "Events/ScoreChangedEvent.h"
#include "Components/Movement/EnemyWanderComponent.h"
#include "sdbm_hash.h"
#include "GameObject.h"
#include "Events/BurgerCompletedEvent.h"

namespace dae
{
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

        // ENEMY CRUSHED
        if (pEvent->id == make_sdbm_hash("EnemyCrushed"))
        {
            if (const auto* pCrushEvent = dynamic_cast<const EnemyCrushedEvent*>(pEvent))
            {
                if (auto pWander = pCrushEvent->obj->GetComponent<EnemyWanderComponent>())
                {
                    switch (pWander->GetEnemyType())
                    {
                    case EnemyType::HotDog: m_score += 100; break;
                    case EnemyType::Pickle: m_score += 200; break;
                    case EnemyType::Egg:    m_score += 300; break;
                    }
                    scoreChanged = true;
                }
            }
        }
        // BURGER DROPPED
        else if (pEvent->id == make_sdbm_hash("BurgerDropped"))
        {
            m_score += 50;
            scoreChanged = true;
        }
        // BURGER CASCADED
        else if (pEvent->id == make_sdbm_hash("BurgerCascaded"))
        {
            if (const auto* pCascadeEvent = dynamic_cast<const BurgerCascadedEvent*>(pEvent))
            {
                switch (pCascadeEvent->numEnemies)
                {
                case 1: m_score += 500; break;
                case 2: m_score += 1000; break;
                case 3: m_score += 2000; break;
                case 4: m_score += 4000; break;
                case 5: m_score += 8000; break;
                default:
                    if (pCascadeEvent->numEnemies >= 6) m_score += 16000;
                    break;
                }
                scoreChanged = true;
            }
        }
        // FINISHING BURGER
        else if (pEvent->id == make_sdbm_hash("BurgerCompleted"))
        {
            m_score += 1000; 
            scoreChanged = true;
        }

        // BROADCAST for scoredisplay
        if (scoreChanged)
        {
            ScoreChangedEvent changedEvent(GetOwner(), m_score);
            EventManager::GetInstance().SendEvent(&changedEvent);
        }
    }
}