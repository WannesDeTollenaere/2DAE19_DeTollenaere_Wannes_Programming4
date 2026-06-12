#pragma once
#include "Component.h"

namespace dae
{
    class TextComponent;

    class FinalScoreDisplayComponent final : public Component
    {
    public:
        explicit FinalScoreDisplayComponent(GameObject* pOwner);
        virtual ~FinalScoreDisplayComponent() = default;

        void Update() override;

    private:
        TextComponent* m_pTextComponent{ nullptr };
        bool m_Initialized{ false };
    };
}
