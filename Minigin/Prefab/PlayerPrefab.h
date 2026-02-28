#pragma once
#include <memory>
#include "GameObject.h"
#include "../Components/TextureComponent.h"
#include "../Components/AnimatorComponent.h"
#include "../Components/PlayerComponent.h"

namespace dae
{
    class PlayerPrefab
    {
    public:
        static std::unique_ptr<GameObject> Create(float startX, float startY)
        {
            auto player = std::make_unique<GameObject>();
            player->SetPosition(startX, startY);

            auto texComp = player->AddComponent<TextureComponent>();
            texComp->SetTexture("burger_time_spritesheet.png");

            player->AddComponent<AnimatorComponent>(16, 16);
            player->AddComponent<PlayerComponent>();

            return player;
        }
    };
}