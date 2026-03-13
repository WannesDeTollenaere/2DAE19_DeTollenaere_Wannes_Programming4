#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae
{
    class GameObject;

    struct ScoreIncreasedEvent final : public GameObjectEvent
    {
        int scoreAdded;

        ScoreIncreasedEvent(GameObject* target, int amount)
            : GameObjectEvent(make_sdbm_hash("ScoreIncreased"), target), scoreAdded(amount) {
        }
    };
}