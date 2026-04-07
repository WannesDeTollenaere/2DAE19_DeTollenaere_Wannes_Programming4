#pragma once
#include "ObserverSys/Event.h"
#include "Components/BoxColliderComponent.h"

namespace dae
{

    class CollisionEvent final : public Event
    {
    public:
        CollisionEvent(BoxColliderComponent* from, BoxColliderComponent* to)
            : Event(make_sdbm_hash("OnCollisionOverlap")), fromComponent(from), toComponent(to) {
        }

        ~CollisionEvent() = default;

        BoxColliderComponent* fromComponent;
        BoxColliderComponent* toComponent;
    };

    class CollisionEnterEvent final : public Event
    {
    public:
        CollisionEnterEvent(BoxColliderComponent* from, BoxColliderComponent* to)
            : Event(make_sdbm_hash("OnCollisionEnter")), fromComponent(from), toComponent(to) {
        }
        BoxColliderComponent* fromComponent;
        BoxColliderComponent* toComponent;
    };

    class CollisionExitEvent final : public Event
    {
    public:
        CollisionExitEvent(BoxColliderComponent* from, BoxColliderComponent* to)
            : Event(make_sdbm_hash("OnCollisionExit")), fromComponent(from), toComponent(to) {
        }
        BoxColliderComponent* fromComponent;
        BoxColliderComponent* toComponent;
    };
}