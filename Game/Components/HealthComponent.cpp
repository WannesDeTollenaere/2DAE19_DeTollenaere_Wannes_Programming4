#include "HealthComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/PlayerDiedEvent.h"
#include "GameTime.h"

void dae::HealthComponent::Damage(int amount)
{

	if (m_health <= 0) return;

	m_health -= amount;

	if (m_health <= 0)
	{
		m_health = 0;
		//EventManager::GetInstance().SendEvent(make_sdbm_hash("PlayerDied"));

		PlayerDiedEvent deathEvent(m_id, GameTime::GetInstance().GetTotalTime());
		EventManager::GetInstance().SendEvent(&deathEvent);
	}
}
