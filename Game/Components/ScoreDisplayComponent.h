#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"
#include "Components/TextComponent.h"
#include "Components/TagComponent.h"
#include <string>

namespace dae
{
    class ScoreDisplayComponent final : public Component, public Observer
    {
    public:
        ScoreDisplayComponent(GameObject* owner, int startingScore, Tag targetTag);

        void HandleEvent(const Event* pEvent) override;
        void Update() override;

    private:
        int m_score;
        Tag m_TargetTag;
        GameObject* m_targetPlayer{ nullptr };
        TextComponent* m_textComponent{ nullptr };
        bool m_textIsInvalid{ true };

    };
}