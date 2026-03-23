#pragma once
#include <Commands/GameObjectCommand.h>
#include <glm/glm.hpp>
#include "Components/AnimatorComponent.h"
#include "Components/Movement/GridMovementComponent.h" 

namespace dae
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* pActor, const glm::vec2& direction, const std::string& animName = "")
			: GameObjectCommand(pActor), m_Direction{ direction }, m_AnimName{ animName }
		{
			m_AnimComp = pActor->GetComponent<AnimatorComponent>();
			m_GridMovementComp = pActor->GetComponent<GridMovementComponent>();
		}

		void Execute() override
		{
			GameObject* pActor = GetGameObject();
			if (!pActor) return;

			if (m_GridMovementComp)
			{
				m_GridMovementComp->SetDesiredDirection(m_Direction.x, m_Direction.y);
			}


			if (!m_AnimName.empty() && m_AnimComp)
			{
				m_AnimComp->PlayAnimation(m_AnimName);
			}

		}

	private:
		glm::vec2 m_Direction;
		std::string m_AnimName;
		AnimatorComponent* m_AnimComp;
		GridMovementComponent* m_GridMovementComp;
	};
}