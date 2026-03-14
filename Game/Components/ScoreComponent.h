#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"
#include "Components/TagComponent.h"

namespace dae
{
    class ScoreComponent final : public Component, public Observer
    {
    public:
        ScoreComponent(GameObject* owner, int startingScore);
        ~ScoreComponent();

        void HandleEvent(const Event* pEvent) override;
        void Update() override {}

        int GetScore() const { return m_score; }

    private:
        int m_score;
    };
}