#include "WinConditionComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/BurgerCompletedEvent.h"
#include "SceneManager.h"
#include <iostream>
#include "SceneLoader.h"
#include "GameTime.h"
#include "SceneLoader.h"
#include "Commands/LoadSceneCommand.h"
#include "InputManager.h"
#include <nlohmann/json.hpp>
#include "Sound/ServiceLocator.h"
#include "Helpers/SoundIDs.h"
#include "ResourceManager.h"

namespace dae
{
    class WinConditionComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            int totalBurgers = data.value("totalBurgers", 4);
            std::string winScene = data.value("winScene", "WinScene");

            go->AddComponent<WinConditionComponent>(totalBurgers, winScene);
        }
    };

    REGISTER_COMPONENT_PARSER(WinConditionComponent, WinConditionComponentParser);

    WinConditionComponent::WinConditionComponent(GameObject* owner, int totalBurgersNeeded, const std::string& winSceneName)
        : Component(owner), m_TotalBurgersNeeded(totalBurgersNeeded), m_WinSceneName(winSceneName)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("BurgerCompleted"), this);

        auto& input = InputManager::GetInstance();
        std::string path = dae::ResourceManager::GetInstance().GetFullPathForFile("Audio/Round Clear.wav");
        ServiceLocator::get_sound_system().load(SoundID::RoundClear, path);

        input.BindKeyboardCommand(
            SDL_SCANCODE_F1,
            InputState::Down,
            std::make_unique<LoadSceneCommand>(winSceneName)
        );
    }

    WinConditionComponent::~WinConditionComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("BurgerCompleted"), this);
    }

    void WinConditionComponent::HandleEvent(const Event* event)
    {
        if (event->id == make_sdbm_hash("BurgerCompleted"))
        {
            m_BurgersCompleted++;

            if (m_BurgersCompleted >= m_TotalBurgersNeeded)
            {
                Win();
            }
        } 
    }
    void WinConditionComponent::RenderGUI()
    {
        ImGui::Text("Win condition manager");
        ImGui::Separator();

        ImGui::Text("Burgers Completed: %d / %d", m_BurgersCompleted, m_TotalBurgersNeeded);

        float progress = 0.0f;
        if (m_TotalBurgersNeeded > 0)
        {
            progress = static_cast<float>(m_BurgersCompleted) / static_cast<float>(m_TotalBurgersNeeded);
        }
        ImGui::ProgressBar(progress, ImVec2(-1.0f, 0.0f));

        ImGui::Spacing();

        if (ImGui::Button("Win game!", ImVec2(-1.0f, 30.0f)))
        {
            Win();
        }
    }
    void WinConditionComponent::Win()
    {
        dae::EventManager::GetInstance().SendEvent(make_sdbm_hash("LevelCompleted"));
        ServiceLocator::get_sound_system().play(SoundID::RoundClear, 1.0f, 0);

        dae::GameTime::GetInstance().AddTimer(3.f, [&]() {
            dae::SceneManager::GetInstance().SetActiveScene(m_WinSceneName);
            });
    }
}