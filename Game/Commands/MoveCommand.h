#pragma once
#include <Commands/GameObjectCommand.h>
#include <glm/glm.hpp>
#include "GameTime.h"

namespace dae
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* pActor, const glm::vec2& direction, float speed)
			: GameObjectCommand(pActor), m_Direction{ direction }, m_Speed{ speed }
		{
		}

		void Execute() override
		{
			GameObject* pActor = GetGameObject();
			if (!pActor) return;

			const auto& pos = pActor->GetTransform().GetLocalPosition();

			glm::vec3 newPos = pos + glm::vec3(m_Direction.x, m_Direction.y, 0.0f) * m_Speed * GameTime::GetInstance().GetDeltaTime();

			pActor->GetTransform().SetLocalPosition(newPos);
		}

	private:
		glm::vec2 m_Direction;
		float m_Speed;
	};
}