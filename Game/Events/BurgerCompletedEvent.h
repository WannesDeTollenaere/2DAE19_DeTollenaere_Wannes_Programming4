#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae {
    class GameObject;

    struct BurgerCompletedEvent final : public GameObjectEvent
    {
        BurgerCompletedEvent(GameObject* target)
            : GameObjectEvent(make_sdbm_hash("BurgerCompleted"), target) {
        }
    };
}