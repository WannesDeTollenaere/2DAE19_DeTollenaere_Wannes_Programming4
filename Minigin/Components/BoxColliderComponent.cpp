#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "CollisionManager.h" 
#include <imgui.h>

namespace dae
{
    BoxColliderComponent::BoxColliderComponent(GameObject* owner, float width, float height, float offsetX, float offsetY)
        : Component(owner), m_width(width), m_height(height), m_offsetX(offsetX), m_offsetY(offsetY)
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
            pos.x + m_offsetX,
            pos.y + m_offsetY,
            m_width,
            m_height
        };
    }


    void BoxColliderComponent::RenderGUI()
    {
        ImGui::Checkbox("Show Collider", &m_ShowDebug);

        ImGui::Separator();

        float size[2] = { m_width, m_height };
        if (ImGui::DragFloat2("Size (W, H)", size, 1.0f, 0.0f, 2000.0f))
        {
            m_width = size[0];
            m_height = size[1];
        }

        float offset[2] = { m_offsetX, m_offsetY };
        if (ImGui::DragFloat2("Offset (X, Y)", offset, 1.0f))
        {
            m_offsetX = offset[0];
            m_offsetY = offset[1];
        }

        ImGui::Text("Area: %.1f sqr px", m_width * m_height);

        if (m_ShowDebug)
        {
            const auto& pos = GetOwner()->GetTransform().GetWorldPosition();

            ImVec2 rect_min = ImVec2(pos.x + m_offsetX, pos.y + m_offsetY);
            ImVec2 rect_max = ImVec2(pos.x + m_offsetX + m_width, pos.y + m_offsetY + m_height);

            ImGui::GetForegroundDrawList()->AddRect(
                rect_min,
                rect_max,
                IM_COL32(0, 255, 0, 255),
                0.0f,
                0,
                2.0f
            );
        }
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