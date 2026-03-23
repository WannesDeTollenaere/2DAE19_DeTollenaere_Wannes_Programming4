#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
    class SpriteSheet; 

    class LevelRendererComponent : public Component
    {
    public:
        LevelRendererComponent(GameObject* pOwner, std::shared_ptr<SpriteSheet> spriteSheet,
            int platformCol, int platformRow,
            int ladderCol, int ladderRow);

        virtual void Render() const override;

    private:
        std::shared_ptr<SpriteSheet> m_pSpriteSheet;
        int m_PlatformCol;
        int m_PlatformRow;
        int m_LadderCol;
        int m_LadderRow;
    };
}