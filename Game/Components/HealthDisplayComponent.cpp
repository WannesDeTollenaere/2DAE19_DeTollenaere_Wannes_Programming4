#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include "Events/PlayerDiedEvent.h"
#include "sdbm_hash.h"

dae::HealthDisplayComponent::HealthDisplayComponent(GameObject* owner, int startingLives, Tag targetPlayer) :
	Component(owner),
	m_targetPlayer{ TagComponent::FindGameObject(targetPlayer) },
	m_TargetTag{targetPlayer},
	m_lives{ startingLives }
{
	EventManager::GetInstance().AttachEvent(make_sdbm_hash("PlayerDied"), this);
}


void dae::HealthDisplayComponent::HandleEvent(const Event* pEvent)
{
	if (m_targetPlayer == nullptr)
	{
		m_targetPlayer = TagComponent::FindGameObject(m_TargetTag);
		if (m_targetPlayer == nullptr) return;
	}

	switch (pEvent->id)
	{
	case make_sdbm_hash("PlayerDied"):
		const PlayerDiedEvent* pDiedEvent = dynamic_cast<const PlayerDiedEvent*>(pEvent);

		if (pDiedEvent)
		{
			if (pDiedEvent->obj == m_targetPlayer)
			{
				m_lives--;
				if (m_lives < 0) m_lives = 0;
				m_textIsInvalid = true;
			}
		}
		break;
	}
}

void dae::HealthDisplayComponent::Update()
{
	if (!m_textIsInvalid) return;
	if (!m_textComponent)
	{
		m_textComponent = GetOwner()->GetComponent<TextComponent>();
		return;
	}

	m_textComponent->SetText("Lives: " + std::to_string(m_lives));
	m_textIsInvalid = false;
}