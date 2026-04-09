#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"

namespace dae
{
    class HealthDisplayComponent final : public Component, public Observer
    {
    public:
        HealthDisplayComponent(GameObject* pOwner);
        ~HealthDisplayComponent();

        void HandleEvent(const Event* event) override;

    private:
        void UpdateVisuals();
        int m_Lives;
    };
}