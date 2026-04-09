#include "GameManager.h"
#include "ObserverSys/EventManager.h"
#include "sdbm_hash.h"
#include "Events/SaltChangedEvent.h"
#include "Events/LivesChangedEvent.h"
#include "Events/LivesLostEvent.h"
#include "SceneManager.h"
#include <iostream>


namespace dae
{
    GameManager::GameManager()
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash_rt("SaltThrown"), this);
    }

    GameManager::~GameManager()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash_rt("SaltThrown"), this);
    }

    void GameManager::HandleEvent(const Event* event)
    {
        if (event->id == make_sdbm_hash_rt("SaltThrown"))
        {
            AddSalt(-1);
        }
    }

    void GameManager::AddSalt(int amount)
    {
        m_Salt += amount;

        SaltChangedEvent saltEvent(m_Salt);
        EventManager::GetInstance().SendEvent(&saltEvent);
    }

    void GameManager::ResetSalt()
    {
        m_Salt = 5;

        SaltChangedEvent saltEvent(m_Salt);
        EventManager::GetInstance().SendEvent(&saltEvent);
    }
    void GameManager::LoseLife()
    {
        m_Lives--;
      
        LivesChangedEvent livesEvent(m_Lives);
        EventManager::GetInstance().SendEvent(&livesEvent);

        LivesLostEvent livesLostEvent(1);
        EventManager::GetInstance().SendEvent(&livesLostEvent);

        if (m_Lives <= 0)
        {
            ResetScore();
            ResetSalt();
            ResetLives();

            SceneManager::GetInstance().SetActiveScene("Levels/level1.json");
        }
    }
    void GameManager::ResetLives()
    {
        m_Lives = 3;

        LivesChangedEvent livesEvent(m_Lives);
        EventManager::GetInstance().SendEvent(&livesEvent);
    }
}