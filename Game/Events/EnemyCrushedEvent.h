#pragma once
#include "ObserverSys/Event.h"
#include "GameObject.h"

namespace dae
{
    struct EnemyCrushedEvent final : public GameObjectEvent
    {
        EnemyCrushedEvent(GameObject* crushedEnemy)
            : GameObjectEvent(make_sdbm_hash("EnemyCrushed"), crushedEnemy) {
        }

        ~EnemyCrushedEvent() override = default;
    };
}