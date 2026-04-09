#pragma once
#include "Singleton.h"

namespace dae
{
    class GameManager final : public Singleton<GameManager>
    {
    public:
        void AddScore(int amount) { m_Score += amount; }
        int GetScore() const { return m_Score; }
        void ResetScore() { m_Score = 0; }

    private:
        friend class Singleton<GameManager>;
        GameManager() = default;

        int m_Score{ 0 };
    };
}