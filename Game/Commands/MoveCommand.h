#pragma once
#include <Commands/GameObjectCommand.h>
#include <glm/glm.hpp>
#include "Components/AnimatorComponent.h"
#include "Components/Movement/GridMovementComponent.h" 
#include "Components/CharacterControllerComponent.h"

namespace dae
{
    class MoveCommand final : public GameObjectCommand
    {
    public:
        MoveCommand(GameObject* pActor, const glm::vec2& direction, const std::string& animName = "")
            : GameObjectCommand(pActor), m_Direction{ direction }, m_AnimName{ animName }
        {
        }

        void Execute() override
        {
            GameObject* pActor = GetGameObject();
            if (!pActor) return;

            // lazy load
            if (!m_pGridMovementComp) m_pGridMovementComp = pActor->GetComponent<GridMovementComponent>();
            if (!m_pCharCtrlComp) m_pCharCtrlComp = pActor->GetComponent<CharacterControllerComponent>();
            if (!m_pAnimComp) m_pAnimComp = pActor->GetComponent<AnimatorComponent>();

            if (m_pGridMovementComp)
            {
                m_pGridMovementComp->SetDesiredDirection(m_Direction.x, m_Direction.y);
            }

            if (m_pCharCtrlComp)
            {
                m_pCharCtrlComp->SetFacingDirection(m_Direction);
            }

            if (!m_AnimName.empty() && m_pAnimComp)
            {
                m_pAnimComp->PlayAnimation(m_AnimName);
            }
        }

    private:
        glm::vec2 m_Direction;
        std::string m_AnimName;

        AnimatorComponent* m_pAnimComp{ nullptr };
        GridMovementComponent* m_pGridMovementComp{ nullptr };
        CharacterControllerComponent* m_pCharCtrlComp{ nullptr };
    };
}