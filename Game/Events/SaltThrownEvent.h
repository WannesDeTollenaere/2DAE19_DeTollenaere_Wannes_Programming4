#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae
{
    struct SaltThrownEvent : public Event
    {
        SaltThrownEvent() : Event(make_sdbm_hash("SaltThrown")) {}
    };
}