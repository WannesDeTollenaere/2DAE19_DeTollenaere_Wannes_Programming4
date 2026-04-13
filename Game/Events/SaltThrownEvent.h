#pragma once
#include "ObserverSys/Event.h"
#include "Components/TagComponent.h"
 
namespace dae
{
    struct SaltThrownEvent : public Event
    {
        Tag playerTag;

        SaltThrownEvent(Tag tag) : Event(make_sdbm_hash("SaltThrown")), playerTag(tag) {}
    };
}