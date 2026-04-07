#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae {
    class GameObject;

    struct BurgerDroppedEvent final : public GameObjectEvent
    {
        BurgerDroppedEvent(GameObject* target)
            : GameObjectEvent(make_sdbm_hash("BurgerDropped"), target) {
        }
    };
}