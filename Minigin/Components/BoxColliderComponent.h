#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
    struct AABB
    {
        float x, y, width, height;
    };

    class BoxColliderComponent final : public Component
    {
    public:
        BoxColliderComponent(GameObject* owner, float width, float height);
        ~BoxColliderComponent();
        BoxColliderComponent(const BoxColliderComponent& other) = delete;
        BoxColliderComponent(BoxColliderComponent&& other) = delete;
        BoxColliderComponent& operator=(const BoxColliderComponent& other) = delete;
        BoxColliderComponent& operator=(BoxColliderComponent&& other) = delete;

        AABB GetAABB() const;
        GameObject* GetGameObject() const { return GetOwner(); };

        virtual void RenderGUI() override;

        bool IsOverlapping(const BoxColliderComponent* other) const;
    private:
        float m_width;
        float m_height;
        bool m_ShowDebug{ false };

    };
}