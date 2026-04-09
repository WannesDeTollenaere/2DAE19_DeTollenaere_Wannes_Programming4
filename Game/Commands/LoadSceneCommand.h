#pragma once
#include "Commands/Command.h"
#include "SceneManager.h"
#include <string>

namespace dae
{
    class LoadSceneCommand final : public Command
    {
    public:
        LoadSceneCommand(const std::string& sceneName)
            : m_SceneName(sceneName) {
        }

        void Execute() override
        {
            SceneManager::GetInstance().SetActiveScene(m_SceneName);
        }

    private:
        std::string m_SceneName;
    };
}