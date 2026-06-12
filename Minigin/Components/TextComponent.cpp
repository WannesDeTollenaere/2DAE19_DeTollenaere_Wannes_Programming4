#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h" 
#include "TextureComponent.h"
#include <imgui.h>
#include <cstring>
#include "SceneLoader.h"
#include "ResourceManager.h"
#include <nlohmann/json.hpp>

namespace dae
{
	class TextComponentParser final : public IComponentParser
	{
	public:
		void Parse(GameObject* go, const nlohmann::json& data) override
		{
			std::string text = data.value("text", "");
			std::string fontName = data.value("font", "PressStart2P.ttf");
			uint8_t fontSize = static_cast<uint8_t>(data.value("fontSize", 36));

			auto font = ResourceManager::GetInstance().LoadFont(fontName, fontSize);
			auto textComp = go->AddComponent<TextComponent>(text, font);

			if (data.contains("color"))
			{
				auto colorData = data["color"];
				SDL_Color color;
				color.r = static_cast<Uint8>(colorData.value("r", 255));
				color.g = static_cast<Uint8>(colorData.value("g", 255));
				color.b = static_cast<Uint8>(colorData.value("b", 255));
				color.a = static_cast<Uint8>(colorData.value("a", 255));

				textComp->SetColor(color);
			}
		}
	};

	REGISTER_COMPONENT_PARSER(TextComponent, TextComponentParser);
}

dae::TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font)
	: Component(owner),
	m_needsUpdate(true),
	m_text(text),
	m_color({ 255, 255, 255, 255 }), 
	m_font(std::move(font)),
	m_textureComponent{nullptr}
{
}

void dae::TextComponent::Update()
{
	if (!m_textureComponent)
	{
		m_textureComponent = GetOwner()->GetComponent<TextureComponent>();
		if (!m_textureComponent) return;
	}

	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);
		m_textureComponent->SetTexture(std::make_shared<Texture2D>(texture));
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (!m_textureComponent) return;
	m_textureComponent->Render();
}

void dae::TextComponent::RenderGUI()
{
	char textBuffer[1024];
#ifdef __EMSCRIPTEN__
	strncpy(textBuffer, m_text.c_str(), sizeof(textBuffer));

#else
	strncpy_s(textBuffer, m_text.c_str(), sizeof(textBuffer));

#endif // EMSCRIPTEN


	textBuffer[sizeof(textBuffer) - 1] = '\0';

	if (ImGui::InputTextMultiline("Content", textBuffer, sizeof(textBuffer)))
	{
		SetText(textBuffer);
	}

	ImGui::Separator();

	float color[4] = {
		m_color.r / 255.0f,
		m_color.g / 255.0f,
		m_color.b / 255.0f,
		m_color.a / 255.0f
	};

	if (ImGui::ColorEdit4("Text Color", color))
	{
		SDL_Color newColor;
		newColor.r = static_cast<uint8_t>(color[0] * 255.0f);
		newColor.g = static_cast<uint8_t>(color[1] * 255.0f);
		newColor.b = static_cast<uint8_t>(color[2] * 255.0f);
		newColor.a = static_cast<uint8_t>(color[3] * 255.0f);

		SetColor(newColor);
	}

}

void dae::TextComponent::SetText(const std::string& text)
{
	if (text == m_text || text == "")
		return;

	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_color = color;
	m_needsUpdate = true;
}