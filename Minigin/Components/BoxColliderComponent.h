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
        BoxColliderComponent(GameObject* owner, float width, float height, float offsetX = 0.f, float offsetY = 0.f);
        ~BoxColliderComponent();
        BoxColliderComponent(const BoxColliderComponent& other) = delete;
        BoxColliderComponent(BoxColliderComponent&& other) = delete;
        BoxColliderComponent& operator=(const BoxColliderComponent& other) = delete;
        BoxColliderComponent& operator=(BoxColliderComponent&& other) = delete;

        AABB GetAABB() const;
        GameObject* GetGameObject() const { return GetOwner(); };

        void SetHeight(float height) { m_height = height; }
        void SetWidth(float width) { m_width = width; }
        void SetOffset(float offsetX, float offsetY) { m_offsetX = offsetX; m_offsetY = offsetY; }
        float GetHeight() const { return m_height; }

        virtual void RenderGUI() override;

        bool IsOverlapping(const BoxColliderComponent* other) const;
    private:
        float m_width;
        float m_height;
        float m_offsetX{ 0.0f };
        float m_offsetY{ 0.0f };
        bool m_ShowDebug{ true };

    };
}