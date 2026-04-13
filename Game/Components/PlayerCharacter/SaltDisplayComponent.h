#pragma once
#include "Component.h"
#include "ObserverSys/Observer.h"
#include "Components/TagComponent.h"

namespace dae
{
    class TextComponent;

    class SaltDisplayComponent final : public Component, public Observer
    {
    public:
        SaltDisplayComponent(GameObject* pOwner);
        ~SaltDisplayComponent();

        void HandleEvent(const Event* event) override;

        void SetTargetTag(Tag tag) { m_TargetTag = tag; }

    private:
        Tag m_TargetTag;
        TextComponent* m_pTextComponent{ nullptr };
    };
}