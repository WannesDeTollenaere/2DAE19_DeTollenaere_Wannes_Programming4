#include "Components/Movement/LevelRenderComponent.h"
#include "Helpers/LevelGrid.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include <SDL3/SDL.h>

namespace dae
{
    LevelRendererComponent::LevelRendererComponent(GameObject* pOwner, const std::string& texturePath, const SDL_Rect& platformSrc, const SDL_Rect& ladderSrc)
        : Component(pOwner), m_PlatformSrc(platformSrc), m_LadderSrc(ladderSrc)
    {
        m_Texture = ResourceManager::GetInstance().LoadTexture(texturePath);
    }

    void LevelRendererComponent::Render() const
    {
        if (!m_Texture) return;

        auto& grid = LevelGrid::GetInstance();
        float tileSize = grid.GetTileSize();
        int cols = grid.GetCols();
        int rows = grid.GetRows();

        SDL_Renderer* sdlRenderer = Renderer::GetInstance().GetSDLRenderer();

        SDL_Texture* pTex = m_Texture->GetSDLTexture();
        if (!sdlRenderer || !pTex) return;

        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                TileType type = grid.GetTile(c, r);
                if (type == TileType::Empty) continue;

                SDL_FRect destRect{ c * tileSize, r * tileSize, tileSize, tileSize };

                if (type == TileType::Platform || type == TileType::Intersection)
                {
                    SDL_FRect srcRect{ (float)m_PlatformSrc.x, (float)m_PlatformSrc.y, (float)m_PlatformSrc.w, (float)m_PlatformSrc.h };
                    SDL_RenderTexture(sdlRenderer, pTex, &srcRect, &destRect);
                }

                if (type == TileType::Ladder || type == TileType::Intersection)
                {
                    SDL_FRect srcRect{ (float)m_LadderSrc.x, (float)m_LadderSrc.y, (float)m_LadderSrc.w, (float)m_LadderSrc.h };
                    SDL_RenderTexture(sdlRenderer, pTex, &srcRect, &destRect);
                }
            }
        }
    }
}