#include "HealthComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/DealDamageEvent.h"
#include "GameTime.h"
#include "sdbm_hash.h"
#include "Events/PlayerTookDamageEvent.h"
#include "SceneLoader.h"

namespace dae {
	class HealthComponentParser final : public IComponentParser {
		REGISTER_COMPONENT_PARSER(HealthComponent, HealthComponentParser);
	public:
		void Parse(GameObject* go, const nlohmann::json& data) override {
			int maxHealth = data.value("maxHealth", 100);
			go->AddComponent<HealthComponent>(maxHealth);
		}
	};
}


dae::HealthComponent::HealthComponent(GameObject* owner, int maxHealth) : Component(owner), m_health(maxHealth)
{
	EventManager::GetInstance().AttachEvent(make_sdbm_hash("DealDamage"), this);
}

dae::HealthComponent::~HealthComponent()
{
	EventManager::GetInstance().DetachEvent(make_sdbm_hash("DealDamage"), this);

}

void dae::HealthComponent::Damage(int amount)
{

	m_health -= amount;

	if (m_health <= 0)
	{
		m_health = 0;
		//EventManager::GetInstance().SendEvent(make_sdbm_hash("PlayerDied"));
	}

	GetOwner()->SetPosition(200.f, 200.f); // TEMP COLLISION SOLUTION

	PlayerTookDamageEvent hitEvent(GetOwner(), amount, m_health);
	EventManager::GetInstance().SendEvent(&hitEvent);

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