#include "WinConditionComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/BurgerCompletedEvent.h"
#include "SceneManager.h"
#include <iostream>
#include "SceneLoader.h"
#include "GameTime.h"

namespace dae
{
    WinConditionComponent::WinConditionComponent(GameObject* owner, int totalBurgersNeeded, const std::string& winSceneName)
        : Component(owner), m_TotalBurgersNeeded(totalBurgersNeeded), m_WinSceneName(winSceneName)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("BurgerCompleted"), this);
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

        dae::GameTime::GetInstance().AddTimer(2.5f, [&]() {
            dae::SceneManager::GetInstance().SetActiveScene(m_WinSceneName);
            });
    }
}