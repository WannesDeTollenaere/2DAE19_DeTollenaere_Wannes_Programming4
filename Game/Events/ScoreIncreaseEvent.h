#pragma once
#include "ObserverSys/Event.h"
#include "GameObject.h"
#include "sdbm_hash.h"

namespace dae
{
    class GameObject;

    struct ScoreIncreasedEvent final : public GameObjectEvent
    {
        int points;

        ScoreIncreasedEvent(GameObject* target, int amount)
            : GameObjectEvent(make_sdbm_hash("ScoreIncreased"), target), points(amount) {
        }
    };
}