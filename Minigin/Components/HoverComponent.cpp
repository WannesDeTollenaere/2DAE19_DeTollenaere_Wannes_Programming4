#include "HoverComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include <cmath>
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class HoverComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            float amplitude = data.value("amplitude", 5.f);
            float speed = data.value("speed", 2.f);

            go->AddComponent<HoverComponent>(amplitude, speed);
        }
    };

    REGISTER_COMPONENT_PARSER(HoverComponent, HoverComponentParser);
}

dae::HoverComponent::HoverComponent(GameObject* owner, float amplitude, float speed)
    : Component(owner), m_amplitude(amplitude), m_speed(speed), m_time(0.0f)
    , m_basePosition(0.0f)
{
    EXPOSE(m_amplitude);
    EXPOSE(m_speed);
}

void dae::HoverComponent::FixedUpdate()
{
    if (!m_initialized)
    {
        m_basePosition = GetOwner()->GetTransform().GetLocalPosition();
        m_initialized = true;
    }

    m_time += m_speed * GameTime::GetInstance().GetFixedTimeStep();

    float offset = std::sin(m_time) * m_amplitude;

    GetOwner()->GetTransform().SetLocalPosition({ m_basePosition.x, m_basePosition.y + offset, m_basePosition.z });
}
