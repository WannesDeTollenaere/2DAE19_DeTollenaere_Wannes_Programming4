#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae {
    class GameObject;

    struct PlayerTookDamageEvent : public GameObjectEvent
    {
        int amount;
        int currentHp;

        PlayerTookDamageEvent(GameObject* actor, int am, int cHp)
            : GameObjectEvent(make_sdbm_hash("PlayerTookDamage"), actor), amount(am), currentHp(cHp)  {
        }
    };
}