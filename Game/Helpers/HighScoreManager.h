#pragma once
#include "Singleton.h"
#include <string>
#include <vector>

namespace dae
{
    struct HighScoreEntry
    {
        std::string name;
        int score;
    };

    class HighScoreManager final : public Singleton<HighScoreManager>
    {
    public:
        void Load();
        void AddScore(const std::string& name, int score);

        const std::vector<HighScoreEntry>& GetScores() const { return m_Scores; }

    private:
        friend class Singleton<HighScoreManager>;
        HighScoreManager() = default;

        void Save();

        const std::string m_FileName{ "highscores.txt" };
        std::vector<HighScoreEntry> m_Scores;
        const size_t m_MaxScores{ 10 };
    };
}