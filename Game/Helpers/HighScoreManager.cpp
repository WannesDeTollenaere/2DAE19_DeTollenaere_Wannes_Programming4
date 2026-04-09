#include "HighScoreManager.h"
#include "ResourceManager.h"
#include <sstream>
#include <algorithm>

namespace dae
{
    void HighScoreManager::AddScore(const std::string& name, int score)
    {
        m_Scores.push_back({ name, score });

        std::sort(m_Scores.begin(), m_Scores.end(), [](const HighScoreEntry& a, const HighScoreEntry& b) {
            return a.score > b.score;
            });

        if (m_Scores.size() > m_MaxScores)
        {
            m_Scores.resize(m_MaxScores);
        }

        Save();
    }

    void HighScoreManager::Save()
    {
        std::stringstream ss;
        for (const auto& entry : m_Scores)
        {
            ss << entry.name << "," << entry.score << "\n";
        }

        ResourceManager::GetInstance().WriteTextFile(m_FileName, ss.str());
    }

    void HighScoreManager::Load()
    {
        m_Scores.clear();

        std::string content = ResourceManager::GetInstance().ReadTextFile(m_FileName);
        if (content.empty()) return;

    
        std::stringstream ss(content);
        std::string line;

        while (std::getline(ss, line))
        {
            std::stringstream lineStream(line);
            std::string name;
            std::string scoreStr;

            if (std::getline(lineStream, name, ',') && std::getline(lineStream, scoreStr))
            {
                m_Scores.push_back({ name, std::stoi(scoreStr) });
            }
        }
    }
}