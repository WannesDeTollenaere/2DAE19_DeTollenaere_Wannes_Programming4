#pragma once
#include "ObserverSys/Event.h"
#include "Components/TagComponent.h"
#include "Components/Ingredients/PickupComponent.h"

namespace dae
{
    struct PickupCollectedEvent final : public Event
    {
        Tag playerTag;
        PickupType pickupType;
        int scoreValue;

        PickupCollectedEvent(Tag tag, PickupType type, int score)
            : Event(make_sdbm_hash_rt("PickupCollected")), playerTag(tag), pickupType(type), scoreValue(score)
        {
        }
    };
}
