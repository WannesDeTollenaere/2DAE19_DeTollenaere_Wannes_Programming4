#include "LeaderboardComponent.h"
#include "GameObject.h"
#include "Components/TextComponent.h"
#include "Helpers/HighScoreManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Components/TextureComponent.h"
#include "SceneLoader.h"

namespace dae
{
    REGISTER_COMPONENT_PARSER(LeaderboardComponent, SimpleParser<LeaderboardComponent>); 


    LeaderboardComponent::LeaderboardComponent(GameObject* pOwner)
        : Component(pOwner)
    {
    }

    void LeaderboardComponent::Update()
    {
        if (!m_pTextComponent)
        {
            m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
            if (m_pTextComponent)
            {
                FormatAndDisplayScores();
            }
        }
    }

    void LeaderboardComponent::FormatAndDisplayScores()
    {
        const auto& scores = HighScoreManager::GetInstance().GetScores();
        auto pScene = SceneManager::GetInstance().GetActiveScene();
        if (!pScene) return;

        auto font = ResourceManager::GetInstance().LoadFont("PressStart2P.ttf", 20);
        const float yOffset = 30.0f;
        const float initialOffset = 20.f;
        if (scores.empty())
        {
            auto go = std::make_unique<dae::GameObject>();

            go->SetParent(this->GetOwner());

            go->GetTransform().SetLocalPosition(glm::vec3(0.0f, initialOffset+ yOffset, 0.0f));
            go->AddComponent<dae::TextComponent>("NO SCORES YET. BE THE FIRST!", font);
            go->AddComponent<dae::TextureComponent>();

            pScene->Add(std::move(go));
        }
        else
        {
            int rank = 1;
            for (const auto& entry : scores)
            {
                std::string entryText = std::to_string(rank) + ". " + entry.name + " - " + std::to_string(entry.score);

                auto go = std::make_unique<dae::GameObject>();

                go->SetParent(this->GetOwner());

                go->GetTransform().SetLocalPosition(glm::vec3(0.0f, initialOffset+ yOffset * rank, 0.0f));
                go->AddComponent<dae::TextComponent>(entryText, font);
                go->AddComponent<dae::TextureComponent>();


                pScene->Add(std::move(go));

                rank++;
            }
        }
    }
}