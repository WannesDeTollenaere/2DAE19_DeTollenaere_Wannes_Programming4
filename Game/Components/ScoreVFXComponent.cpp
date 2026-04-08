#include "ScoreVFXComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/EnemyCrushedEvent.h"
#include "Components/Movement/EnemyWanderComponent.h"
#include "SceneManager.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/LifetimeComponent.h" 
#include "ResourceManager.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Helpers/Spritesheet.h"

namespace dae
{
    // FLOATING COMPONENT
    void FloatingVfxComponent::Update()
    {
        auto pos = GetOwner()->GetTransform().GetLocalPosition();
        pos.y -= m_Speed * GameTime::GetInstance().GetDeltaTime(); 
        GetOwner()->GetTransform().SetLocalPosition(pos.x, pos.y, pos.z);
    }

    // SPAWNER
    ScoreVfxSpawnerComponent::ScoreVfxSpawnerComponent(GameObject* owner) : Component(owner)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("EnemyCrushed"), this);
    }

    ScoreVfxSpawnerComponent::~ScoreVfxSpawnerComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("EnemyCrushed"), this);
    }

    void dae::ScoreVfxSpawnerComponent::HandleEvent(const Event* event)
    {
        if (auto crushEvent = dynamic_cast<const EnemyCrushedEvent*>(event))
        {
            auto scene = SceneManager::GetInstance().GetActiveScene();

            if (!scene || !crushEvent->obj || crushEvent->obj->IsMarkedForDeletion()) return;

            int scoreCol = 0;
            int scoreRow = 10;

            if (auto wander = crushEvent->obj->GetComponent<EnemyWanderComponent>())
            {
                switch (wander->GetEnemyType())
                {
                case EnemyType::HotDog:
                    scoreCol = 0; // 100 
                    break;
                case EnemyType::Pickle:
                    scoreCol = 1; // 200 
                    break;
                case EnemyType::Egg:
                    scoreCol = 2; // 300 
                    break;
                default:
                    scoreCol = 0;
                    break;
                }
            }

            auto popup = std::make_unique<GameObject>("ScorePopup");

            auto pos = crushEvent->obj->GetTransform().GetWorldPosition();
            popup->GetTransform().SetLocalPosition(pos.x, pos.y - 20.0f, pos.z);

            auto sheet = ResourceManager::GetInstance().LoadSpriteSheet("burger_time_spritesheet_x3.png", 48, 48);

            SDL_Rect srcRect = sheet->GetSourceRect(scoreCol, scoreRow);

            SDL_FRect srcFRect = {
                static_cast<float>(srcRect.x),
                static_cast<float>(srcRect.y),
                static_cast<float>(srcRect.w),
                static_cast<float>(srcRect.h)
            };

            auto textureComp = popup->AddComponent<TextureComponent>();
            textureComp->SetTexture(sheet->GetTexture());
            textureComp->SetSourceRect(srcFRect);

            popup->AddComponent<LifetimeComponent>(1.5f);
            popup->AddComponent<FloatingVfxComponent>(45.0f);

            scene->Add(std::move(popup));
        }
    }
}