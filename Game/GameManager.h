#pragma once
#include "Singleton.h"
#include "ObserverSys/Observer.h" 

namespace dae
{
    class GameManager final : public Singleton<GameManager>
    {
    public:
        void AddScore(int amount) { m_Score += amount; }
        int GetScore() const { return m_Score; }
        void ResetScore() { m_Score = 0; }

        void LoseLife();
        int GetLives() const { return m_Lives; }
        void ResetLives();

    private:
        friend class Singleton<GameManager>;
        GameManager(); 

        int m_Score{ 0 };
        int m_Lives{ 3 };
    };
}