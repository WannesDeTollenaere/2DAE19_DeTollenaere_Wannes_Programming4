#pragma once
#include "Singleton.h"
#include "ObserverSys/Observer.h" 

namespace dae
{
    class GameManager final : public Singleton<GameManager>, public Observer
    {
    public:
        void AddScore(int amount) { m_Score += amount; }
        int GetScore() const { return m_Score; }
        void ResetScore() { m_Score = 0; }

        void AddSalt(int amount);
        int GetSalt() const { return m_Salt; }
        void ResetSalt();

        void LoseLife();
        int GetLives() const { return m_Lives; }
        void ResetLives();

        void HandleEvent(const Event* event) override;

    private:
        friend class Singleton<GameManager>;
        GameManager(); 
        ~GameManager();

        int m_Score{ 0 };
        int m_Salt{ 5 };
        int m_Lives{ 3 };
    };
}