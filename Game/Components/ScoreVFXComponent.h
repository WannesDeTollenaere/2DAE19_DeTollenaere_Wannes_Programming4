#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"

namespace dae
{
    class ScoreVfxSpawnerComponent final : public Component, public Observer
    {
    public:
        ScoreVfxSpawnerComponent(GameObject* owner);
        ~ScoreVfxSpawnerComponent();
        void HandleEvent(const Event* event) override;
    };

    class FloatingVfxComponent final : public Component
    {
    public:
        FloatingVfxComponent(GameObject* owner, float floatSpeed = 30.0f)
            : Component(owner), m_Speed(floatSpeed) {
        }

        void Update() override;
    private:
        float m_Speed;
    };
}