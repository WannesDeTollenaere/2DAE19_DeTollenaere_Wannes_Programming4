#pragma once
#include "Component.h"
#include "GameTime.h"
#include <vector>
#include <string>

namespace dae
{
    class PickupSpawnerComponent final : public Component
    {
    public:
        PickupSpawnerComponent(GameObject* owner, std::vector<std::string> prefabPaths,
            float minInterval = 8.0f, float maxInterval = 15.0f);
        ~PickupSpawnerComponent() override;

    private:
        void SpawnPickup();
        void ScheduleNext();

        std::vector<std::string> m_PrefabPaths;
        float m_MinInterval;
        float m_MaxInterval;
        TimerId m_TimerHandle{};
    };
}
