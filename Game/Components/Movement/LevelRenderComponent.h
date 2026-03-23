#pragma once
#include "Component.h"
#include <SDL3/SDL_rect.h>
#include <memory>
#include <string>

namespace dae
{
    class Texture2D;

    class LevelRendererComponent : public Component
    {
    public:
        LevelRendererComponent(GameObject* pOwner, const std::string& texturePath, const SDL_Rect& platformSrc, const SDL_Rect& ladderSrc);
        virtual void Render() const override;

    private:
        std::shared_ptr<Texture2D> m_Texture;
        SDL_Rect m_PlatformSrc;
        SDL_Rect m_LadderSrc;
    };
}