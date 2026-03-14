#include "PlayerCollisionHandler.h"
#include "ObserverSys/EventManager.h"
#include "Events/DealDamageEvent.h"

namespace dae
{
    void PlayerCollisionHandler::OnCollision(GameObject*, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash("Enemy")))
        {
            DealDamageEvent damage(GetOwner(), 1);
            EventManager::GetInstance().SendEvent(&damage);
        }
    }
}