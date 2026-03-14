#pragma once
#include "ObserverSys/Observer.h"

namespace dae
{
    class AchievementManager final : public Observer
    {
    public:
        AchievementManager();

        void HandleEvent(const Event* pEvent) override;
    };
}