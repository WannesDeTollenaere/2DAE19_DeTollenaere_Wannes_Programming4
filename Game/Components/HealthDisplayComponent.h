#pragma once
#include <Component.h>
#include "ObserverSys/EventManager.h"
#include "ObserverSys/Observer.h"
#include "Components/TextComponent.h"
#include <string>
#include <Components/TagComponent.h>

namespace dae
{
	class GameObject;

	class HealthDisplayComponent final : public Component, public Observer
	{
	public:
		HealthDisplayComponent(GameObject* owner, int startingLives, Tag targetPlayer);

		void HandleEvent(const Event* pEvent) override;

		void Update() override;


	private:
		bool m_textIsInvalid{ true };
		GameObject* m_targetPlayer;
		Tag m_TargetTag;
		int m_lives;
		TextComponent* m_textComponent{ nullptr };
	};
}