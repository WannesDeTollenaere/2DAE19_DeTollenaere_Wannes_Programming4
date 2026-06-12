#include "GameManager.h"
#include "ObserverSys/EventManager.h"
#include "sdbm_hash.h"
#include "Events/SaltChangedEvent.h"
#include "Events/PickupCollectedEvent.h"
#include "Events/LivesChangedEvent.h"
#include "Events/LivesLostEvent.h"
#include "SceneManager.h"
#include <iostream>
#include "ResourceManager.h"


namespace dae
{
    GameManager::GameManager()
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash_rt("SaltChanged"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash_rt("PickupCollected"), this);

        auto& soundSys = ServiceLocator::GetSoundSystem();
        auto& resources = dae::ResourceManager::GetInstance();

        soundSys.load(SoundID::BGM, resources.GetFullPathForFile("Audio/BGM.wav"));

        soundSys.play(SoundID::BGM, 0.5f, -1);
    }

    GameManager::~GameManager()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash_rt("SaltChanged"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash_rt("PickupCollected"), this);
    }


    void GameManager::HandleEvent(const Event* event)
    {
        if (event->id == make_sdbm_hash_rt("SaltChanged"))
        {
            auto saltEvent = static_cast<const SaltChangedEvent*>(event);
            m_PersistentSalts[saltEvent->playerTag] = saltEvent->currentSalt;
        }
        else if (event->id == make_sdbm_hash_rt("PickupCollected"))
        {
            auto pickupEvent = static_cast<const PickupCollectedEvent*>(event);
            if (pickupEvent->scoreValue != 0)
                AddScore(pickupEvent->scoreValue);
        }
    }
    int GameManager::GetPersistentSalt(Tag playerTag)
    {
        if (m_PersistentSalts.find(playerTag) == m_PersistentSalts.end())
        {
            return 5; 
        }
        return m_PersistentSalts[playerTag];
    }
    void GameManager::ResetPersistentData()
    {
        m_PersistentSalts.clear();
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
            ResetPersistentData();

            SceneManager::GetInstance().SetActiveScene("Levels/GameOver.json");
        }
    }
    void GameManager::ResetLives()
    {
        m_Lives = 4;

        LivesChangedEvent livesEvent(m_Lives);
        EventManager::GetInstance().SendEvent(&livesEvent);
    }
}