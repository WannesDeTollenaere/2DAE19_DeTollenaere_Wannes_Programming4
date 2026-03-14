#include "CollisionManager.h"
#include "Components/BoxColliderComponent.h"
#include "GameObject.h"
#include <iostream>
#include "ObserverSys/EventManager.h"
#include "ObserverSys/CollisionEvent.h"

namespace dae
{
    void CollisionManager::RegisterCollider(BoxColliderComponent* collider)
    {
        m_colliders.push_back(collider);
    }

    void CollisionManager::RemoveCollider(BoxColliderComponent* collider)
    {
        if (m_colliders.empty()) return;
        std::erase(m_colliders, collider);
    }

    void CollisionManager::Update()
    {
        for (size_t i = 0; i < m_colliders.size(); ++i)
        {
            for (size_t j = i + 1; j < m_colliders.size(); ++j)
            {
                auto colA = m_colliders[i];
                auto colB = m_colliders[j];

                if (!colA->IsActive() || !colB->IsActive()) continue;
                if (colA->IsMarkedForDeletion() || colB->IsMarkedForDeletion()) continue;

                if (colA->IsOverlapping(colB))
                {
                    CollisionEvent event(colA, colB);
                    EventManager::GetInstance().SendEvent(&event);
                }
                if (colB->IsOverlapping(colA))
                {
                    CollisionEvent event(colB, colA);
                    EventManager::GetInstance().SendEvent(&event);
                }
            }
        }
    }
}