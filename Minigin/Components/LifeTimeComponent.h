#pragma once
#include "Component.h"
#include "GameTime.h"

namespace dae
{
    class LifetimeComponent final : public Component
    {
    public:
        LifetimeComponent(GameObject* owner, float lifetime);
        ~LifetimeComponent() override;

    private:
        TimerId m_TimerHandle;
    };
}