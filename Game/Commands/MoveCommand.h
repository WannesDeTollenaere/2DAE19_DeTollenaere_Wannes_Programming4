#pragma once
#include <Commands/GameObjectCommand.h>
#include <glm/glm.hpp>
#include "Components/Movement/GridMovementComponent.h" 
#include "Components/CharacterControllerComponent.h"

namespace dae
{
    class MoveCommand final : public GameObjectCommand
    {
    public:
        MoveCommand(GameObject* pActor, const glm::vec2& direction)
            : GameObjectCommand(pActor), m_Direction{ direction }
        {
        }

        void Execute() override
        {
            GameObject* pActor = GetGameObject();
            if (!pActor) return;

            // lazy load
            if (!m_pGridMovementComp) m_pGridMovementComp = pActor->GetComponent<GridMovementComponent>();
            if (!m_pCharCtrlComp) m_pCharCtrlComp = pActor->GetComponent<CharacterControllerComponent>();

            if (m_pCharCtrlComp->IsDead()) return;

            if (m_pGridMovementComp)
            {
                m_pGridMovementComp->SetDesiredDirection(m_Direction.x, m_Direction.y);
            }

            if (m_pCharCtrlComp)
            {
                m_pCharCtrlComp->SetFacingDirection(m_Direction);
            }
        }

    private:
        glm::vec2 m_Direction;

        GridMovementComponent* m_pGridMovementComp{ nullptr };
        CharacterControllerComponent* m_pCharCtrlComp{ nullptr };
    };
}