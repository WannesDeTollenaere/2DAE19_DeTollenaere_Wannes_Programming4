#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

namespace dae
{
    class GridMovementComponent : public Component
    {
    public:
        GridMovementComponent(GameObject* pOwner, float speed);
        virtual void Update() override;

        void SetDesiredDirection(float x, float y);

    private:
        float m_Speed;
        float m_DesiredDirX{ 0.0f };
        float m_DesiredDirY{ 0.0f };

    };
}