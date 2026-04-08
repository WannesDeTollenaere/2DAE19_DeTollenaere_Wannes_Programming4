#include "LifeTimeComponent.h"
#include "GameObject.h"

namespace dae
{
    LifetimeComponent::LifetimeComponent(GameObject* owner, float lifetime)
        : Component(owner)
    {
        m_TimerHandle = GameTime::GetInstance().AddTimer(lifetime, [this]() {
            GetOwner()->Destroy();
            });
    }

    LifetimeComponent::~LifetimeComponent()
    {
        GameTime::GetInstance().RemoveTimer(m_TimerHandle);
    }
}