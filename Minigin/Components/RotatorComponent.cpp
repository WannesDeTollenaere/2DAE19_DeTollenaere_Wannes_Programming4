#include "RotatorComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include <cmath>
#include <glm/gtc/constants.hpp>
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class RotatorComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            float radius = data.value("radius", 10.f);
            float speed = data.value("speed", 10.f);

            go->AddComponent<RotatorComponent>(radius, speed);
        }
    };

    REGISTER_COMPONENT_PARSER(RotatorComponent, RotatorComponentParser);
}

dae::RotatorComponent::RotatorComponent(GameObject* owner, float radius, float speed)
    : Component(owner), m_radius(radius), m_speed(speed), m_angle(0.0f)
{
    EXPOSE(m_radius);
    EXPOSE(m_speed);
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