#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae {
    class GameObject;

    struct DealDamageEvent final : public GameObjectEvent
    {
        int damageAmount;

        DealDamageEvent(GameObject* target, int amount)
            : GameObjectEvent(make_sdbm_hash("DealDamage"), target), damageAmount(amount) {
        }
    };
}