#pragma once
#include "ObserverSys/Event.h"

namespace dae {
    struct PlayerDiedEvent : public Event
    {
        int playerId;
        float timeOfDeath;

        PlayerDiedEvent(int id, float time)
            : Event(make_sdbm_hash("PlayerDied")), playerId(id), timeOfDeath(time) {
        }
    };
}