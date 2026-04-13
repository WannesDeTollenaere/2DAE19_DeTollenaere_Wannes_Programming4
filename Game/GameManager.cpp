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
            ResetLives();

            SceneManager::GetInstance().SetActiveScene("Levels/GameOver.json");
        }
    }
    void GameManager::ResetLives()
    {
        m_Lives = 3;

        LivesChangedEvent livesEvent(m_Lives);
        EventManager::GetInstance().SendEvent(&livesEvent);
    }
}