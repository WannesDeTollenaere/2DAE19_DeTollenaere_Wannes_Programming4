#include "DynamicTextComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h"
#include "SceneLoader.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <iomanip>

namespace dae
{
	class FPSDynamicTextComponentParser final : public IComponentParser
	{
	public:
		void Parse(GameObject* go, const nlohmann::json& data) override
		{
			std::string postfix = data.value("postfix", " FPS");

			auto dynamicFPS = go->AddComponent<DynamicTextComponent>([]() {
				float dt = GameTime::GetInstance().GetDeltaTime();
				if (dt <= 0.0f) return std::string("0");

				std::stringstream ss;
				ss << std::fixed << std::setprecision(1) << (1.0f / dt);
				return ss.str();
				});

			dynamicFPS->SetPostfix(postfix);
		}
	};

	REGISTER_COMPONENT_PARSER(FPSDynamicTextComponent, FPSDynamicTextComponentParser);
}

dae::DynamicTextComponent::DynamicTextComponent(GameObject* owner, std::function<std::string()> valueProvider)
	: Component(owner), m_valueProvider(std::move(valueProvider))
{
	EXPOSE(m_updateDelay);
}

void dae::DynamicTextComponent::Update()
{
	if (!m_textComponent)
	{
		m_textComponent = GetOwner()->GetComponent<TextComponent>();
		if (!m_textComponent) return;
	}

	float dt = GameTime::GetInstance().GetDeltaTime();
	m_timer += dt;

	if (m_timer >= m_updateDelay)
	{
		std::string newValue = m_valueProvider();

		if (newValue != m_lastValue)
		{
			m_textComponent->SetText(m_prefix + newValue + m_postfix);
			m_lastValue = newValue;
		}

		m_timer -= m_updateDelay;
	}
}