#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"
#include <string>

namespace dae
{
    class WinConditionComponent final : public Component, public Observer
    {
    public:
        WinConditionComponent(GameObject* owner, int totalBurgersNeeded, const std::string& winSceneName);
        ~WinConditionComponent();

        void HandleEvent(const Event* event) override;
        void RenderGUI() override;

    private:
        int m_TotalBurgersNeeded;
        int m_BurgersCompleted{ 0 };
        std::string m_WinSceneName;

        void Win();
    };
}