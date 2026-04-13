#include "SaltDisplayComponent.h"
#include "GameObject.h"
#include "ObserverSys/EventManager.h"
#include "Events/SaltChangedEvent.h"
#include "Components/TextComponent.h"
#include <nlohmann/json.hpp>
#include "SceneLoader.h"
#include "sdbm_hash.h" 

namespace dae
{
    class SaltDisplayComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            auto comp = go->AddComponent<SaltDisplayComponent>();

            std::string targetStr = data.value("targetTag", "Player1");

            Tag targetTag = make_sdbm_hash_rt(targetStr.c_str());

            comp->SetTargetTag(targetTag);
        }
    };
    REGISTER_COMPONENT_PARSER(SaltDisplayComponent, SaltDisplayComponentParser);


    SaltDisplayComponent::SaltDisplayComponent(GameObject* pOwner)
        : Component(pOwner)
    {
        m_TargetTag = make_sdbm_hash("Player1");

        EventManager::GetInstance().AttachEvent(make_sdbm_hash("SaltChanged"), this);
    }

    SaltDisplayComponent::~SaltDisplayComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("SaltChanged"), this);
    }

    void SaltDisplayComponent::HandleEvent(const Event* event)
    {
        if (event->id == make_sdbm_hash("SaltChanged"))
        {
            auto saltEvent = static_cast<const SaltChangedEvent*>(event);

            if (saltEvent->playerTag == m_TargetTag)
            {
                if (!m_pTextComponent) m_pTextComponent = GetOwner()->GetComponent<TextComponent>();

                if (m_pTextComponent)
                {
                    m_pTextComponent->SetText(std::to_string(saltEvent->currentSalt));
                }
            }
        }
    }
}