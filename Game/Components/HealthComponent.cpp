#include "HealthComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/PlayerDiedEvent.h"
#include "GameTime.h"
#include "sdbm_hash.h"
#include "Events/DealDamageEvent.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int maxHealth) : Component(owner), m_health(maxHealth)
{
	EventManager::GetInstance().AttachEvent(make_sdbm_hash("DealDamage"), this);
}

void dae::HealthComponent::Damage(int amount)
{

	m_health -= amount;

	if (m_health <= 0)
	{
		m_health = 0;
		//EventManager::GetInstance().SendEvent(make_sdbm_hash("PlayerDied"));

		PlayerDiedEvent deathEvent(GetOwner());
		EventManager::GetInstance().SendEvent(&deathEvent);
	}
}

void dae::HealthComponent::HandleEvent(const Event* pEvent)
{
	if (pEvent->id == make_sdbm_hash("DealDamage"))
	{
		if (const auto* pDamageEvent = dynamic_cast<const DealDamageEvent*>(pEvent))
		{
			if (pDamageEvent->obj == GetOwner())
			{
				Damage(pDamageEvent->damageAmount);
			}
		}
	}
}