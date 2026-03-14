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
}