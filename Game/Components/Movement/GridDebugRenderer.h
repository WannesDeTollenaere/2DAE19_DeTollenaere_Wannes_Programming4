#pragma once
#include "Component.h"

namespace dae
{
    class GridDebugRenderer : public Component
    {
    public:
        GridDebugRenderer(GameObject* pOwner);
        virtual void Render() const override;
    };
}