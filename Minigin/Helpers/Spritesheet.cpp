#include "Helpers/Spritesheet.h"
#include "ResourceManager.h"

namespace dae
{
    SpriteSheet::SpriteSheet(std::shared_ptr<Texture2D> pTexture, int frameWidth, int frameHeight)
        : m_Texture(pTexture), m_FrameWidth(frameWidth), m_FrameHeight(frameHeight)
    {
    }

    SDL_Rect SpriteSheet::GetSourceRect(int col, int row) const
    {
        SDL_Rect src{};
        src.x = col * m_FrameWidth;
        src.y = row * m_FrameHeight;
        src.w = m_FrameWidth;
        src.h = m_FrameHeight;
        return src;
    }
}