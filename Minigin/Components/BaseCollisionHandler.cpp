#include "BaseCollisionHandler.h"
#include "GameObject.h"
#include "ObserverSys/CollisionEvent.h"
#include "Components/TagComponent.h"
#include "ObserverSys/EventManager.h"

namespace dae
{
    BaseCollisionHandler::BaseCollisionHandler(GameObject* owner)
        : Component(owner), m_collider(owner->GetComponent<BoxColliderComponent>())
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("OnCollisionOverlap"), this);
    }

    BaseCollisionHandler::~BaseCollisionHandler()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("OnCollisionOverlap"), this);
    }

    void BaseCollisionHandler::HandleEvent(const Event* event)
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

                OnCollision(otherObject, tagComp);
            }
        }
    }
}