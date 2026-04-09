#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"

namespace dae
{
    class TextComponent; 

    class HighScoreDisplayComponent final : public Component, public Observer
    {
    public:
        HighScoreDisplayComponent(GameObject* pOwner);
        virtual ~HighScoreDisplayComponent();

        void Update() override;
        void HandleEvent(const Event* event) override;

    private:
        TextComponent* m_pTextComponent{ nullptr };

        int m_CurrentHighScore{ 0 };

        void UpdateVisualText();
    };
}