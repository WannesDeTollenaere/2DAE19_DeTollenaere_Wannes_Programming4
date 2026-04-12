#include "GridRenderComponent.h"
#include "Helpers/LevelGrid.h"
#include "Helpers/Spritesheet.h"
#include "Renderer.h"
#include "Texture2D.h"
#include <SDL3/SDL.h>
#include "GameObject.h"
#include "SceneLoader.h"
#include "ResourceManager.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class LevelRendererComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            std::string texture = data.value("texture", "burger_time_spritesheet.png");
            int frameWidth = data.value("frameWidth", 16);
            int frameHeight = data.value("frameHeight", 16);

            std::unordered_map<int, TileRenderData> tileMappings;
            if (data.contains("tileMappings"))
            {
                for (auto& [key, val] : data["tileMappings"].items())
                {
                    int tileId = std::stoi(key);
                    TileRenderData renderData{};

                    for (const auto& coord : val)
                    {
                        renderData.spriteCoords.push_back({ coord[0].get<int>(), coord[1].get<int>() });
                    }
                    tileMappings[tileId] = renderData;
                }
            }

            auto sheet = dae::ResourceManager::GetInstance().LoadSpriteSheet(texture, frameWidth, frameHeight);
            go->AddComponent<GridRendererComponent>(sheet, tileMappings);
        }
    };
    REGISTER_COMPONENT_PARSER(LevelRendererComponent, LevelRendererComponentParser);

    GridRendererComponent::GridRendererComponent(GameObject* pOwner, std::shared_ptr<SpriteSheet> spriteSheet,
        const std::unordered_map<int, TileRenderData>& tileMappings)
        : Component(pOwner), m_pSpriteSheet(spriteSheet), m_TileMappings(tileMappings)
    {
    }

    void GridRendererComponent::Render() const
    {
        if (!m_pSpriteSheet || !m_pSpriteSheet->GetTexture()) return;

        auto& grid = LevelGrid::GetInstance();
        float tileSize = grid.GetTileSize();
        SDL_Renderer* sdlRenderer = Renderer::GetInstance().GetSDLRenderer();
        SDL_Texture* pTex = m_pSpriteSheet->GetTexture()->GetSDLTexture();

        const glm::vec3& ownerPos = GetOwner()->GetTransform().GetWorldPosition();

        if (!sdlRenderer || !pTex) return;

        for (int r = 0; r < grid.GetRows(); ++r)
        {
            for (int c = 0; c < grid.GetCols(); ++c)
            {
                int tileTypeInt = static_cast<int>(grid.GetTile(c, r));

                auto it = m_TileMappings.find(tileTypeInt);
                if (it == m_TileMappings.end()) continue;

                SDL_FRect destRect{ ownerPos.x + c * tileSize,ownerPos.y+ r * tileSize, tileSize, tileSize };

                for (const auto& coords : it->second.spriteCoords)
                {
                    SDL_Rect src = m_pSpriteSheet->GetSourceRect(coords.first, coords.second);
                    SDL_FRect srcFRect{ (float)src.x, (float)src.y, (float)src.w, (float)src.h };
                    SDL_RenderTexture(sdlRenderer, pTex, &srcFRect, &destRect);
                }
            }
        }
    }
    void GridRendererComponent::RenderGUI()
    {
        dae::LevelGrid::GetInstance().RenderGUI();
    }
}