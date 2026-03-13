#include "HealthDisplayComponent.h"
#include "GameObject.h"
#include "Events/PlayerDiedEvent.h"

dae::HealthDisplayComponent::HealthDisplayComponent(GameObject* owner, int startingLives, int targetId) :
	Component(owner), m_lives(startingLives), m_targetId{targetId}
{
	EventManager::GetInstance().AttachEvent(make_sdbm_hash("PlayerDied"), this);
}


void dae::HealthDisplayComponent::HandleEvent(const Event* pEvent)
{
	switch (pEvent->id)
	{
	case make_sdbm_hash("PlayerDied"):
		const PlayerDiedEvent* pDiedEvent = dynamic_cast<const PlayerDiedEvent*>(pEvent);

		if (pDiedEvent)
		{
			if (pDiedEvent->playerId == m_targetId)
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

	m_textComponent->SetText("P" + std::to_string(m_targetId + 1) + " Lives: " + std::to_string(m_lives));
}