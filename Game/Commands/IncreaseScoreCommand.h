#pragma once
#include <Commands/GameObjectCommand.h>
#include "ObserverSys/EventManager.h"
#include "Events/ScoreIncreaseEvent.h"

namespace dae
{
    class IncreaseScoreCommand final : public GameObjectCommand
    {
    public:
        IncreaseScoreCommand(GameObject* targetActor, int amount)
            : GameObjectCommand(targetActor), m_amount(amount) {
        }

        void Execute() override
        {
            if (GetGameObject())
            {
                ScoreIncreasedEvent scoreEvent(GetGameObject(), m_amount);
                EventManager::GetInstance().SendEvent(&scoreEvent);
            }
        } 

    private:
        int m_amount;
    };
}