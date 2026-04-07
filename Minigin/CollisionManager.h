#pragma once
#include "Singleton.h" 
#include <vector>
#include <functional>
#include <set>

namespace dae
{
    class BoxColliderComponent;

    class CollisionManager final : public Singleton<CollisionManager>
    {
    public:
        void RegisterCollider(BoxColliderComponent* collider);
        void RemoveCollider(BoxColliderComponent* collider);

        void Update();

    private:
        friend class Singleton<CollisionManager>;
        CollisionManager() = default;

        std::vector<BoxColliderComponent*> m_colliders;

        std::set<std::pair<BoxColliderComponent*, BoxColliderComponent*>> m_ActiveCollisions;
    };
}