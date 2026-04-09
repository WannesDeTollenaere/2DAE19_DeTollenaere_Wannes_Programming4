#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae
{
    struct SaltChangedEvent : public Event
    {
        SaltChangedEvent(int currentSaltAmount)
            : Event(make_sdbm_hash("SaltChanged")), currentSalt(currentSaltAmount) {
        }

        int currentSalt;
    };
}