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
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("OnCollisionEnter"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("OnCollisionExit"), this);
    }

    BaseCollisionHandler::~BaseCollisionHandler()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("OnCollisionOverlap"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("OnCollisionEnter"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("OnCollisionExit"), this);
    }

    void BaseCollisionHandler::HandleEvent(const Event* event)
    {
        if (!m_collider) m_collider = GetOwner()->GetComponent<BoxColliderComponent>();

        if (const auto* overlap = dynamic_cast<const CollisionEvent*>(event)) {
            if (overlap->fromComponent == m_collider) {
                auto other = overlap->toComponent->GetGameObject();
                OnCollision(other, other->GetComponent<TagComponent>());
            }
        }
        else if (const auto* enter = dynamic_cast<const CollisionEnterEvent*>(event)) {
            if (enter->fromComponent == m_collider) {
                auto other = enter->toComponent->GetGameObject();
                OnCollisionEnter(other, other->GetComponent<TagComponent>());
            }
        }
        else if (const auto* exit = dynamic_cast<const CollisionExitEvent*>(event)) {
            if (exit->fromComponent == m_collider) {
                auto other = exit->toComponent->GetGameObject();
                OnCollisionExit(other, other->GetComponent<TagComponent>());
            }
        }
    }
}