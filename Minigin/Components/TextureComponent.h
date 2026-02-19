#pragma once
#include "Component.h"
#include <string>
#include <memory>

namespace dae
{
	class Texture2D;

	class TextureComponent final : public Component
	{
	public:
		TextureComponent(GameObject* owner);
		virtual ~TextureComponent() = default;

		void Render() const override;
		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> texture) { m_texture = std::move(texture); }
	private:
		std::shared_ptr<Texture2D> m_texture{};
	};
}