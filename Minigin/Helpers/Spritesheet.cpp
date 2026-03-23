#include "SpriteSheet.h"
#include "ResourceManager.h"

namespace dae
{
    SpriteSheet::SpriteSheet(const std::string& texturePath, int frameWidth, int frameHeight)
        : m_FrameWidth(frameWidth), m_FrameHeight(frameHeight)
    {
        m_Texture = ResourceManager::GetInstance().LoadTexture(texturePath);
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