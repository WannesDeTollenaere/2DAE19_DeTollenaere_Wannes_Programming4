#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae {
    class GameObject;

    struct BurgerCascadedEvent final : public GameObjectEvent
    {
        int numEnemies;

        BurgerCascadedEvent(GameObject* target, int enemiesCount)
            : GameObjectEvent(make_sdbm_hash("BurgerCascaded"), target), numEnemies(enemiesCount) {
        }
    };
}