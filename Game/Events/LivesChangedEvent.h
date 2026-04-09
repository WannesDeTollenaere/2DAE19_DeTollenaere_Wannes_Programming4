#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae
{
    struct LivesChangedEvent : public Event
    {
        LivesChangedEvent(int currentLives) 
            : Event(make_sdbm_hash_rt("LivesChanged")), lives(currentLives) {}

        int lives;
    };
}