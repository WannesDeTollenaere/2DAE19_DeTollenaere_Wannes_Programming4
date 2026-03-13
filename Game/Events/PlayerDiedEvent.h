#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae {
    class GameObject;

    struct PlayerDiedEvent : public GameObjectEvent
    {
        PlayerDiedEvent(GameObject* actor)
            : GameObjectEvent(make_sdbm_hash("PlayerDied"), actor) {
        }
    };
}