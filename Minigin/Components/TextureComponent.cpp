#include "TextureComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include <imgui.h>

dae::TextureComponent::TextureComponent(GameObject* owner) : Component(owner) {}

dae::TextureComponent::TextureComponent(GameObject* owner, const SDL_FRect& srcRect) : Component(owner)
{
	SetSourceRect(srcRect);
}

void dae::TextureComponent::RenderGUI()
{
	ImGui::Text("Choose new asset file:");

	
	static char textureBuffer[256] = "";

	if (ImGui::InputText("Filename", textureBuffer, sizeof(textureBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		try
		{
			SetTexture(textureBuffer);
		}
		catch (...) {};
	}
}

void dae::TextureComponent::Render() const
{
	if (m_texture)
	{
		const auto& pos = GetOwner()->GetTransform().GetWorldPosition();

		if (m_useSourceRect)
		{
			SDL_FRect dstRect{ pos.x, pos.y, m_srcRect.w, m_srcRect.h };
			Renderer::GetInstance().RenderTexture(*m_texture, m_srcRect, dstRect);
		}
		else
		{
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}