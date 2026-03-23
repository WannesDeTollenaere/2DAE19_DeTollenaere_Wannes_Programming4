#include "LevelRenderComponent.h"
#include "Helpers/LevelGrid.h"
#include "Helpers/Spritesheet.h" 
#include "Renderer.h"
#include "Texture2D.h"
#include <SDL3/SDL.h>

namespace dae
{
    LevelRendererComponent::LevelRendererComponent(GameObject* pOwner, std::shared_ptr<SpriteSheet> spriteSheet,
        int platformCol, int platformRow,
        int ladderCol, int ladderRow)
        : Component(pOwner), m_pSpriteSheet(spriteSheet),
        m_PlatformCol(platformCol), m_PlatformRow(platformRow),
        m_LadderCol(ladderCol), m_LadderRow(ladderRow)
    {
    }

    void LevelRendererComponent::Render() const
    {

        if (!m_pSpriteSheet || !m_pSpriteSheet->GetTexture()) return;

        auto& grid = LevelGrid::GetInstance();
        float tileSize = grid.GetTileSize();
        int cols = grid.GetCols();
        int rows = grid.GetRows();

        SDL_Renderer* sdlRenderer = Renderer::GetInstance().GetSDLRenderer();
        SDL_Texture* pTex = m_pSpriteSheet->GetTexture()->GetSDLTexture();
        if (!sdlRenderer || !pTex) return;

        SDL_Rect pSrc = m_pSpriteSheet->GetSourceRect(m_PlatformCol, m_PlatformRow);
        SDL_Rect lSrc = m_pSpriteSheet->GetSourceRect(m_LadderCol, m_LadderRow);

        SDL_FRect platformSrc{ (float)pSrc.x, (float)pSrc.y, (float)pSrc.w, (float)pSrc.h };
        SDL_FRect ladderSrc{ (float)lSrc.x, (float)lSrc.y, (float)lSrc.w, (float)lSrc.h };

        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                TileType type = grid.GetTile(c, r);
                if (type == TileType::Empty) continue;

                SDL_FRect destRect{ c * tileSize, r * tileSize, tileSize, tileSize };

                if (type == TileType::Platform || type == TileType::Intersection)
                {
                    SDL_RenderTexture(sdlRenderer, pTex, &platformSrc, &destRect);
                }

                if (type == TileType::Ladder || type == TileType::Intersection)
                {
                    SDL_RenderTexture(sdlRenderer, pTex, &ladderSrc, &destRect);
                }
            }
        }
    }
}