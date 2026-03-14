#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"
#include "GameObject.h"
#include "Components/TagComponent.h"

namespace dae
{
    class BoxColliderComponent;
    class TagComponent;

    // INTERFACE FOR COLLISION HANDLING
    class BaseCollisionHandler : public Component, public Observer
    {
    public:
        BaseCollisionHandler(GameObject* owner);
        virtual ~BaseCollisionHandler();
        BaseCollisionHandler(const BaseCollisionHandler& other) = delete;
        BaseCollisionHandler(BaseCollisionHandler&& other) = delete;
        BaseCollisionHandler& operator=(const BaseCollisionHandler& other) = delete;
        BaseCollisionHandler& operator=(BaseCollisionHandler&& other) = delete;

        void HandleEvent(const Event* event) override;

    protected:
        virtual void OnCollision(GameObject* otherObject, TagComponent* otherTagComp) = 0;

    private:
        BoxColliderComponent* m_collider;
    };
}