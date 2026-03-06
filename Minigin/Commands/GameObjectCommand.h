#pragma once
#include "Command.h"
#include "../GameObject.h" 

namespace dae
{
	class GameObjectCommand : public Command
	{
	public:
		explicit GameObjectCommand(GameObject* pActor) : m_pActor{ pActor } {}
		virtual ~GameObjectCommand() = default;

	protected:
		GameObject* GetGameObject() const { return m_pActor; }

	private:
		GameObject* m_pActor;
	};
}