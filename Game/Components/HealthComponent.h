#pragma once
#include "Component.h"
#include <ObserverSys/Observer.h>

namespace dae
{
	class HealthComponent final : public Component, public Observer
	{
	public:
		HealthComponent(GameObject* owner, int maxHealth);

		void Damage(int amount);

		void HandleEvent(const Event* pEvent) override;

		int GetHealth() const { return m_health; }

		//Subject& GetSubject() { return m_subject; }

	private:
		int m_health;
		//Subject m_subject;
	};
}