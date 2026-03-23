#include "GridDebugRenderer.h"
#include "Helpers/LevelGrid.h"
#include "Renderer.h"
#include <SDL3/SDL.h> 

namespace dae
{
    GridDebugRenderer::GridDebugRenderer(GameObject* pOwner) : Component(pOwner)
    {
    }

    void GridDebugRenderer::Render() const
    {
        auto& grid = LevelGrid::GetInstance();
        float tileSize = grid.GetTileSize();
        int cols = grid.GetCols();
        int rows = grid.GetRows();


        SDL_Renderer* sdlRenderer = Renderer::GetInstance().GetSDLRenderer();
        if (!sdlRenderer) return;
         
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                TileType type = grid.GetTile(c, r);
                if (type == TileType::Empty) continue;

                SDL_FRect rect{ c * tileSize, r * tileSize, tileSize, tileSize };

                switch (type)
                {
                case TileType::Platform:
                    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255); 
                    break;
                case TileType::Ladder:
                    SDL_SetRenderDrawColor(sdlRenderer, 0, 255, 0, 255); 
                    break;
                case TileType::Intersection:
                    SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255); 
                    break;
                default:
                    break;
                }

                SDL_RenderRect(sdlRenderer, &rect);
            }
        }

        SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    }
}