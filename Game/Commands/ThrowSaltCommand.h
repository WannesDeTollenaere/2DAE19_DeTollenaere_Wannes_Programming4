#pragma once
#include "Commands/GameObjectCommand.h"
#include "Components/CharacterControllerComponent.h"
#include "GameTime.h"

namespace dae
{
    class ThrowSaltCommand final : public GameObjectCommand
    {
    public:
        ThrowSaltCommand(GameObject* targetActor)
            : GameObjectCommand(targetActor) {
        }

        void Execute() override
        {
            if (m_isOnCd) return;
            if (!m_pCharCtrl)
                m_pCharCtrl = GetGameObject()->GetComponent<CharacterControllerComponent>();

            if (m_pCharCtrl)
            {
                m_isOnCd = true;
                m_pCharCtrl->ThrowSalt();
                GameTime::GetInstance().AddTimer(m_CooldownTime, [&]() {m_isOnCd = false; });
            }
        }

    private:
        CharacterControllerComponent* m_pCharCtrl{ nullptr };

        const float m_CooldownTime{ .6f };
        bool m_isOnCd{ false };
    };
}