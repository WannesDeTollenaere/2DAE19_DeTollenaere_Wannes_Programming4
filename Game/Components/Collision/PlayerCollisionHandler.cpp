#include "PlayerCollisionHandler.h"
#include "GameObject.h"
#include "ObserverSys/CollisionEvent.h"
#include "Components/TagComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/DealDamageEvent.h" 

namespace dae
{
    PlayerCollisionHandler::PlayerCollisionHandler(GameObject* owner)
        : Component(owner), m_collider(owner->GetComponent<BoxColliderComponent>())
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("OnCollisionOverlap"), this);
    }

    void PlayerCollisionHandler::HandleEvent(const Event* event)
    {
        if (!m_collider)
        {
            m_collider = GetOwner()->GetComponent<BoxColliderComponent>();
        }

        if (const auto* collision = dynamic_cast<const CollisionEvent*>(event))
        {
            if (collision->fromComponent == m_collider)
            {
                auto otherObject = collision->toComponent->GetGameObject();
                auto tagComp = otherObject->GetComponent<TagComponent>();
                if (tagComp && tagComp->HasTag(make_sdbm_hash("Enemy")))
                {
                    DealDamageEvent damage(GetOwner(), 1);
                    EventManager::GetInstance().SendEvent(&damage);

                }
            }
        }
    }
}