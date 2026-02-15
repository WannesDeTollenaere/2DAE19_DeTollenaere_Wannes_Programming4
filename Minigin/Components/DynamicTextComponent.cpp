#include "DynamicTextComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "GameTime.h"

dae::DynamicTextComponent::DynamicTextComponent(GameObject* owner, std::function<std::string()> valueProvider)
	: Component(owner), m_valueProvider(std::move(valueProvider))
{
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