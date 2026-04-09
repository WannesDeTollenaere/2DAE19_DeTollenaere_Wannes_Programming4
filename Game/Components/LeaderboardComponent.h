#pragma once
#include "Component.h"
#include <string>

namespace dae
{
    class TextComponent;

    class LeaderboardComponent final : public Component
    {
    public:
        LeaderboardComponent(GameObject* pOwner);
        virtual ~LeaderboardComponent() = default;

        void Update() override;

    private:
        TextComponent* m_pTextComponent{ nullptr };

        void FormatAndDisplayScores();
    };
}