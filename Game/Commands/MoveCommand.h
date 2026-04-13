// MoveCommand.h
#pragma once
#include <Commands/GameObjectCommand.h>
#include <glm/glm.hpp>
#include "Components/Movement/GridMovementComponent.h" 
#include "Components/CharacterControllerComponent.h"
#include "Components/PlayerCharacter/PlayerEnemyController.h" 

namespace dae
{
    class MoveCommand final : public GameObjectCommand
    {
    public:
        MoveCommand(GameObject* pActor, const glm::vec2& direction)
            : GameObjectCommand(pActor), m_Direction{ direction } {
        }

        void Execute() override
        {
            GameObject* pActor = GetGameObject();
            if (!pActor) return;

            if (!m_pGridMovementComp) m_pGridMovementComp = pActor->GetComponent<GridMovementComponent>();

            // Handle chef
            if (auto charCtrl = pActor->GetComponent<CharacterControllerComponent>()) {
                if (charCtrl->IsDead()) return;
                charCtrl->SetFacingDirection(m_Direction);
            }
            // Handle playerenemy
            else if (auto enemyCtrl = pActor->GetComponent<PlayerEnemyControllerComponent>()) {
                if (enemyCtrl->IsMovementDisabled()) return;
                enemyCtrl->SetFacingDirection(m_Direction);
            }

            // Apply movement
            if (m_pGridMovementComp) {
                m_pGridMovementComp->SetDesiredDirection(m_Direction.x, m_Direction.y);
            }
        }

    private:
        glm::vec2 m_Direction;
        GridMovementComponent* m_pGridMovementComp{ nullptr };
    };
}