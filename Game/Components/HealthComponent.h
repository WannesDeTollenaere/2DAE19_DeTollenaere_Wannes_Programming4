#pragma once
#include "Component.h"
//#include <ObserverSys/Subject.h>

namespace dae
{
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(GameObject* owner, int maxHealth, int id = 0)
			: Component(owner), m_health(maxHealth), m_id( id) {
		}

		void Damage(int amount);

		int GetHealth() const { return m_health; }

		//Subject& GetSubject() { return m_subject; }

	private:
		int m_health;
		int m_id;
		//Subject m_subject;
	};
}