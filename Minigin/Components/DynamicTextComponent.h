#pragma once
#include "Component.h"
#include <string>
#include <functional>

namespace dae
{
	class TextComponent;

	class DynamicTextComponent final : public Component
	{
	public:
		DynamicTextComponent(GameObject* owner, std::function<std::string()> valueProvider);
		~DynamicTextComponent() = default;

		void Update() override;

		void SetPrefix(const std::string& prefix) { m_prefix = prefix; }
		void SetPostfix(const std::string& postfix) { m_postfix = postfix; }
		void SetUpdateDelay(float delay) { m_updateDelay = delay; }

	private:
		TextComponent* m_textComponent{ nullptr };
		std::function<std::string()> m_valueProvider;

		std::string m_prefix{ "" };
		std::string m_postfix{ "" };
		std::string m_lastValue{ "" };

		float m_updateDelay{ 1.f };
		float m_timer{ 0.0f };
	};
}