#pragma once
#include "Singleton.h"
#include "ObserverSys/Observer.h" 
#include "Components/TagComponent.h"

namespace dae
{
    class GameManager final : public Singleton<GameManager>, public Observer
    {
    public:
        void AddScore(int amount) { m_Score += amount; }
        int GetScore() const { return m_Score; }
        void ResetScore() { m_Score = 0; }

        void LoseLife();
        int GetLives() const { return m_Lives; }
        void ResetLives();

        int GetPersistentSalt(Tag playerTag);
        void ResetPersistentData();

        void HandleEvent(const Event* event) override;
    private:
        friend class Singleton<GameManager>;
        GameManager();
        ~GameManager();

        std::map<Tag, int> m_PersistentSalts;

        int m_Score{ 0 };
        int m_Lives{ 3 };
    };
}