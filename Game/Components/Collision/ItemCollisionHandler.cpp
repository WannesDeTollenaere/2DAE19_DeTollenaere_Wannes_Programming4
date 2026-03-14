#include "ItemCollisionHandler.h"
#include "Events/ScoreIncreaseEvent.h"
#include "ObserverSys/EventManager.h"

void dae::ItemCollisionHandler::OnCollision(GameObject* otherObject, TagComponent* otherTagComp)
{
    if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash("Player")))
    {
        ScoreIncreasedEvent score(otherObject, 100);
        EventManager::GetInstance().SendEvent(&score);

        GetOwner()->Destroy();
    }
}
