#pragma once
#include <Component.h>
#include "ObserverSys/EventManager.h"
#include "ObserverSys/Observer.h"
#include "Components/TextComponent.h"
#include <string>

namespace dae
{
	class HealthDisplayComponent final : public Component, public Observer
	{
	public:
		HealthDisplayComponent(GameObject* owner, int startingLives, int targetId =0);

		void HandleEvent(const Event* pEvent) override;

		void Update() override;


	private:
		bool m_textIsInvalid{ true };
		int m_targetId;
		int m_lives;
		TextComponent* m_textComponent{ nullptr };
	};
}