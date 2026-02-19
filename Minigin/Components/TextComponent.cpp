#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h" 
#include "TextureComponent.h"

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
	m_textureComponent->Render();
}

void dae::TextComponent::SetText(const std::string& text)
{
	if (text == m_text)
		return;

	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_color = color;
	m_needsUpdate = true;
}