#pragma once
#include <Commands/Command.h>
#include "ObserverSys/EventManager.h"
#include "Events/DealDamageEvent.h"

namespace dae
{
    class GameObject;

    class DamageCommand final : public Command
    {
    public:
        DamageCommand(GameObject* targetActor, int damageAmount)
            : m_targetActor(targetActor), m_damageAmount(damageAmount) {
        }

        void Execute() override
        {
            if (m_targetActor)
            {
                DealDamageEvent damageEvent(m_targetActor, m_damageAmount);
                EventManager::GetInstance().SendEvent(&damageEvent);
            }
        }

    private:
        GameObject* m_targetActor;
        int m_damageAmount;
    };
}