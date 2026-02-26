#include "RotatorComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include <cmath>
#include <glm/gtc/constants.hpp>

dae::RotatorComponent::RotatorComponent(GameObject* owner, float radius, float speed)
    : Component(owner), m_radius(radius), m_speed(speed), m_angle(0.0f)
{
}

void dae::RotatorComponent::FixedUpdate()
{
    m_angle += m_speed * GameTime::GetInstance().GetFixedTimeStep();

    constexpr float TwoTimesPi = glm::pi<float>() * 2.f;

    if (m_angle >= TwoTimesPi) m_angle -= TwoTimesPi;
    if (m_angle <= -TwoTimesPi) m_angle += TwoTimesPi;

    float x = std::cos(m_angle) * m_radius;
    float y = std::sin(m_angle) * m_radius;

    GetOwner()->SetPosition(x, y);
}