#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae
{
    struct LivesLostEvent : public Event
    {
        LivesLostEvent(int amount)
            : Event(make_sdbm_hash_rt("LivesLost")), amountLost(amount) {
        }

        int amountLost;
    };
}