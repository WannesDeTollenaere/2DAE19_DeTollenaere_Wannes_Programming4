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
        std::set<std::pair<BoxColliderComponent*, BoxColliderComponent*>> currentCollisions;

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
                    auto pair = (colA < colB) ? std::make_pair(colA, colB) : std::make_pair(colB, colA);
                    currentCollisions.insert(pair);

                    if (m_ActiveCollisions.find(pair) == m_ActiveCollisions.end())
                    {
                        CollisionEnterEvent enterA(colA, colB);
                        EventManager::GetInstance().SendEvent(&enterA);
                        CollisionEnterEvent enterB(colB, colA);
                        EventManager::GetInstance().SendEvent(&enterB);
                    }

                    CollisionEvent eventA(colA, colB);
                    EventManager::GetInstance().SendEvent(&eventA);
                    CollisionEvent eventB(colB, colA);
                    EventManager::GetInstance().SendEvent(&eventB);
                }
            }
        }

        for (const auto& pair : m_ActiveCollisions)
        {
            if (currentCollisions.find(pair) == currentCollisions.end())
            {
                if (!pair.first->IsMarkedForDeletion() && !pair.second->IsMarkedForDeletion())
                {
                    CollisionExitEvent exitA(pair.first, pair.second);
                    EventManager::GetInstance().SendEvent(&exitA);
                    CollisionExitEvent exitB(pair.second, pair.first);
                    EventManager::GetInstance().SendEvent(&exitB);
                }
            }
        }

        m_ActiveCollisions = currentCollisions;
    }
}