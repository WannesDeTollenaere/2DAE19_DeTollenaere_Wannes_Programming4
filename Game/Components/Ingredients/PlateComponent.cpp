#include "PlateComponent.h"
#include "GameObject.h"
#include "ObserverSys/EventManager.h"
#include "Events/BurgerCompletedEvent.h"
#include <algorithm>

namespace dae
{
    PlateComponent::PlateComponent(GameObject* owner, int requiredIngredients)
        : BaseCollisionHandler(owner), m_RequiredIngredients(requiredIngredients) {
    }

    void PlateComponent::OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (m_IsComplete) return;

        if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash_rt("Ingredient")))
        {
            if (std::find(m_CaughtIngredients.begin(), m_CaughtIngredients.end(), otherObject) == m_CaughtIngredients.end())
            {
                m_CaughtIngredients.push_back(otherObject);

                if (static_cast<int>(m_CaughtIngredients.size()) >= m_RequiredIngredients)
                {
                    m_IsComplete = true;


                    BurgerCompletedEvent event(GetOwner());
                    EventManager::GetInstance().SendEvent(&event);
                }
            }
        }
    }
}