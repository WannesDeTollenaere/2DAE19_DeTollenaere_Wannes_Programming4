#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h" 

namespace dae
{
    class SaltDisplayComponent final : public Component, public Observer
    {
    public:
        SaltDisplayComponent(GameObject* pOwner);
        ~SaltDisplayComponent();

        void HandleEvent(const Event* event) override;

    private:
        void UpdateText();
        int m_Salt;
    };
}