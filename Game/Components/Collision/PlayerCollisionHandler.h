#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"

namespace dae
{
    class BoxColliderComponent;
    class PlayerCollisionHandler final : public Component, public Observer
    {
    public:
        PlayerCollisionHandler(GameObject* owner);

        void HandleEvent(const Event* event) override;
    private:
        BoxColliderComponent* m_collider;
    };
}