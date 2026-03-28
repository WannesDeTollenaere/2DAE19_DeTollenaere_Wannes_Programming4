#pragma once
#include "Component.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace dae
{
    class SpriteSheet; 

    struct TileRenderData
    {
        std::vector<std::pair<int, int>> spriteCoords;
    };

    class GridRendererComponent : public Component
    {
    public:
        GridRendererComponent(GameObject* pOwner, std::shared_ptr<SpriteSheet> spriteSheet,
            const std::unordered_map<int, TileRenderData>& tileMappings);

        virtual void Render() const override;
        virtual void RenderGUI() override;
    private:
        std::shared_ptr<SpriteSheet> m_pSpriteSheet;
        std::unordered_map<int, TileRenderData> m_TileMappings;
    };
}