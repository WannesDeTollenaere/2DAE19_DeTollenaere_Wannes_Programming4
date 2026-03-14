#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "CollisionManager.h" 

namespace dae
{
    BoxColliderComponent::BoxColliderComponent(GameObject* owner, float width, float height)
        : Component(owner), m_width(width), m_height(height)
    {
        CollisionManager::GetInstance().RegisterCollider(this);
    }

    BoxColliderComponent::~BoxColliderComponent()
    {
        CollisionManager::GetInstance().RemoveCollider(this);
    }

    AABB BoxColliderComponent::GetAABB() const
    {
        const auto& pos = GetOwner()->GetTransform().GetWorldPosition();

        return AABB{
            pos.x,
            pos.y,
            m_width,
            m_height
        };
    }

    bool BoxColliderComponent::IsOverlapping(const BoxColliderComponent* other) const
    {
        AABB a = GetAABB();
        AABB b = other->GetAABB();

        return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
    }
}