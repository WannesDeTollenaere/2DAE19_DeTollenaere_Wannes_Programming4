#pragma once
#include "Component.h"

namespace dae
{
    class RotatorComponent final : public Component
    {
    public:
        RotatorComponent(GameObject* owner, float radius, float speed);
        virtual ~RotatorComponent() = default;

        void FixedUpdate() override;

    private:
        float m_radius;
        float m_speed;
        float m_angle;
    };
}