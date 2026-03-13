#pragma once
#include <Commands/Command.h>
#include "ObserverSys/EventManager.h"
#include "Events/DealDamageEvent.h"

namespace dae
{
    class GameObject;

    class DamageCommand final : public GameObjectCommand
    {
    public:
        DamageCommand(GameObject* targetActor, int damageAmount)
            : GameObjectCommand(targetActor), m_damageAmount(damageAmount) {
        }

        void Execute() override
        {

            DealDamageEvent damageEvent(GetGameObject(), m_damageAmount);
            EventManager::GetInstance().SendEvent(&damageEvent);
            
        }

    private:
        int m_damageAmount;
    };
}