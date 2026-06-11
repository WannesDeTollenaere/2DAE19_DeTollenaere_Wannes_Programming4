#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
    class HoverComponent final : public Component
    {
    public:
        HoverComponent(GameObject* owner, float amplitude, float speed);
        virtual ~HoverComponent() = default;

        void FixedUpdate() override;

    private:
        float m_amplitude;
        float m_speed;
        float m_time;
        glm::vec3 m_basePosition;
        bool m_initialized{ false };
    };
}
