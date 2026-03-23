#pragma once
#include <memory>
#include <SDL3/SDL_rect.h>

namespace dae
{
    class Texture2D;

    class SpriteSheet final
    {
    public:
        SpriteSheet(std::shared_ptr<Texture2D> pTexture, int frameWidth, int frameHeight);
        ~SpriteSheet() = default;

        SpriteSheet(const SpriteSheet&) = delete;
        SpriteSheet& operator=(const SpriteSheet&) = delete;

        std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
        SDL_Rect GetSourceRect(int col, int row) const;

        int GetFrameWidth() const { return m_FrameWidth; }
        int GetFrameHeight() const { return m_FrameHeight; }

    private:
        std::shared_ptr<Texture2D> m_Texture;
        int m_FrameWidth;
        int m_FrameHeight;
    };
}