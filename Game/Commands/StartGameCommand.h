#pragma once
#include "Commands/Command.h"
#include "SceneManager.h"
#include "GameManager.h"
#include <string>

namespace dae
{
    class StartGameCommand final : public Command
    {
    public:
        StartGameCommand(const std::string& scenePath)
            : m_ScenePath(scenePath) {
        }

        void Execute() override
        {
            GameManager::GetInstance().ResetPersistentData();

            GameManager::GetInstance().ResetScore();
            GameManager::GetInstance().ResetLives();

            SceneManager::GetInstance().SetActiveScene(m_ScenePath);
        }

    private:
        std::string m_ScenePath;
    };
}