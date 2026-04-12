#include "TextureComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include <imgui.h>
#include "SceneLoader.h"
#include "Helpers/Spritesheet.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class TextureComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            std::string texturePath = data.value("texture", "");

            auto texComp = go->AddComponent<TextureComponent>();

            if (!texturePath.empty())
            {
                texComp->SetTexture(texturePath);
            }

            if (data.contains("spriteSheet") && data["spriteSheet"].is_object() && !texturePath.empty())
            {
                const auto& sheetData = data["spriteSheet"];
                int frameWidth = sheetData.value("frameWidth", 48);
                int frameHeight = sheetData.value("frameHeight", 48);
                int col = sheetData.value("col", 0);
                int row = sheetData.value("row", 0);
                int colSpan = sheetData.value("colSpan", 1);

                auto pTexture = dae::ResourceManager::GetInstance().LoadSpriteSheet(texturePath, frameWidth, frameHeight);
                texComp->SetTexture(pTexture->GetTexture());

                SDL_Rect srcRect = pTexture->GetSourceRect(col, row);
                srcRect.w *= colSpan;

                SDL_FRect fRect{
                    static_cast<float>(srcRect.x),
                    static_cast<float>(srcRect.y),
                    static_cast<float>(srcRect.w),
                    static_cast<float>(srcRect.h)
                };
                texComp->SetSourceRect(fRect);
            }
        }
    };

    REGISTER_COMPONENT_PARSER(TextureComponent, TextureComponentParser);
}


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