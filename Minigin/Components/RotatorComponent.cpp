#include "RotatorComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include <cmath>

dae::RotatorComponent::RotatorComponent(GameObject* owner, float radius, float speed)
    : Component(owner), m_radius(radius), m_speed(speed), m_angle(0.0f)
{
}

void dae::RotatorComponent::FixedUpdate()
{
    m_angle += m_speed * GameTime::GetInstance().GetFixedTimeStep();

    float x = std::cos(m_angle) * m_radius;
    float y = std::sin(m_angle) * m_radius;

    GetOwner()->SetPosition(x, y);
}