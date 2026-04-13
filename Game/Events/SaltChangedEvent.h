#pragma once
#include "ObserverSys/Event.h"
#include "Components/TagComponent.h" 
namespace dae
{
    struct SaltChangedEvent final : public Event
    {
        Tag playerTag;
        int currentSalt;

        SaltChangedEvent(Tag tag, int amount)
            : Event(make_sdbm_hash_rt("SaltChanged")), playerTag(tag), currentSalt(amount)
        {
        }
    };
}