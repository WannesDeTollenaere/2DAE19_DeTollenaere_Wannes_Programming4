#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include <SDL3/SDL.h> 

namespace dae
{
	class Texture2D;

	class TextureComponent final : public Component
	{
	public:
		TextureComponent(GameObject* owner);
		TextureComponent(GameObject* owner, const SDL_FRect& srcRect);
		virtual ~TextureComponent() = default;

		void RenderGUI() override;
		void Render() const override;
		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> texture) { m_texture = std::move(texture); }

		void SetSourceRect(const SDL_FRect& rect) { m_srcRect = rect; m_useSourceRect = true; }
		void ClearSourceRect() { m_useSourceRect = false; }

	private:
		std::shared_ptr<Texture2D> m_texture{};
		std::string m_textureName;

		bool m_useSourceRect{ false };
		SDL_FRect m_srcRect{};
	};
}