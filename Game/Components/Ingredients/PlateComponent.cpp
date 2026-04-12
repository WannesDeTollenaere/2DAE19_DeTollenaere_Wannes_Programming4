#include "PlateComponent.h"
#include "GameObject.h"
#include "ObserverSys/EventManager.h"
#include "Events/BurgerCompletedEvent.h"
#include "Components/BoxColliderComponent.h"
#include <algorithm>
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class PlateComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            int requiredIngredients = data.value("requiredIngredients", 3);

            go->AddComponent<PlateComponent>(requiredIngredients);
        }
    };

    REGISTER_COMPONENT_PARSER(PlateComponent, PlateComponentParser);


    PlateComponent::PlateComponent(GameObject* owner, int requiredIngredients)
        : BaseCollisionHandler(owner), m_RequiredIngredients(requiredIngredients) {
        CalculateColliderHeight();
        
    }

    void PlateComponent::RenderGUI()
    {
        if (ImGui::InputInt("Required Ingredients", &m_RequiredIngredients))
        {
            if (m_RequiredIngredients < 1) m_RequiredIngredients = 1;
            CalculateColliderHeight();
        }

        ImGui::Separator();

        ImGui::Text("Caught Ingredients: %d / %d", static_cast<int>(m_CaughtIngredients.size()), m_RequiredIngredients);

        if (m_IsComplete)
        {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "BURGER COMPLETE!");
        }

        if (ImGui::TreeNode("Ingredient List"))
        {
            if (m_CaughtIngredients.empty())
            {
                ImGui::TextDisabled("Empty");
            }
            else
            {
                for (size_t i = 0; i < m_CaughtIngredients.size(); ++i)
                {
                    auto go = m_CaughtIngredients[i];
                    if (go)
                    {
                        ImGui::Text("[%zu] %s", i, go->GetName().c_str());
                    }
                }
            }
            ImGui::TreePop(); 
        }
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
    void dae::PlateComponent::CalculateColliderHeight()
    {
        auto collider = GetOwner()->GetComponent<BoxColliderComponent>();
        if (collider)
        {
            float currentOffsetY = collider->GetAABB().y - GetOwner()->GetTransform().GetWorldPosition().y;
            float oldHeight = collider->GetAABB().height;

            float newHeight = (static_cast<float>(m_RequiredIngredients) * 24.0f) + 10.0f;

            float heightDifference = newHeight - oldHeight;

            float newOffsetY = currentOffsetY - heightDifference;

            collider->SetHeight(newHeight);
            collider->SetOffset(30.0f, newOffsetY);
        }
    }
}