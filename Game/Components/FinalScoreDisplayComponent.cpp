#include "FinalScoreDisplayComponent.h"
#include "GameObject.h"
#include "Components/TextComponent.h"
#include "GameManager.h"
#include "SceneLoader.h"
#include <string>

namespace dae
{
    REGISTER_COMPONENT_PARSER(FinalScoreDisplayComponent, SimpleParser<FinalScoreDisplayComponent>);

    FinalScoreDisplayComponent::FinalScoreDisplayComponent(GameObject* pOwner)
        : Component(pOwner)
    {
    }

    void FinalScoreDisplayComponent::Update()
    {
        if (m_Initialized) return;

        if (!m_pTextComponent)
            m_pTextComponent = GetOwner()->GetComponent<TextComponent>();

        if (m_pTextComponent)
        {
            m_pTextComponent->SetText("SCORE  " + std::to_string(GameManager::GetInstance().GetScore()));
            m_Initialized = true;
        }
    }
}
