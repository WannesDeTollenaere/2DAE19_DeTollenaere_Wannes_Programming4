#pragma once
#include "SceneLoader.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "GameTime.h"
#include "Components/TextureComponent.h"
#include "Components/AnimatorComponent.h"
#include "Components/TextComponent.h"
#include "Components/DynamicTextComponent.h"
#include "Components/RotatorComponent.h"
#include "Components/CacheProfilerComponent.h"

// DTO
#include "DTO/AnimatorComponentDTO.h"
#include "DTO/CacheProfilerComponentDTO.h"
#include "DTO/FPSDynamicTextComponentDTO.h"
#include "DTO/RotatorComponentDTO.h"
#include "DTO/TextComponentDTO.h"
#include "DTO/TextureComponentDTO.h"
#include "Components/TagComponent.h"
#include "DTO/TagComponentDTO.h"

#include "sdbm_hash.h"

#include <sstream>
#include <iomanip>

namespace dae
{
    class ComponentsRegistry
    {
    public:
        static void RegisterAll()
        {
            SceneLoader::RegisterComponentParser("TextureComponent", [](GameObject* go, const nlohmann::json& data) {
                auto dto = TextureComponentDTO::FromJson(data);
                auto texComp = go->AddComponent<TextureComponent>();
                if (!dto.texture.empty()) {
                    texComp->SetTexture(dto.texture);
                }
                });

            SceneLoader::RegisterComponentParser("AnimatorComponent", [](GameObject* go, const nlohmann::json& data) {
                auto dto = AnimatorComponentDTO::FromJson(data);

                auto animator = go->AddComponent<AnimatorComponent>(dto.frameWidth, dto.frameHeight);

                for (const auto& [name, clip] : dto.animations)
                {
                    animator->AddAnimation(name, clip);
                }
                if (!dto.playOnStart.empty())
                {
                    animator->PlayAnimation(dto.playOnStart);
                }
                });

            SceneLoader::RegisterComponentParser("TextComponent", [](GameObject* go, const nlohmann::json& data) {
                auto dto = TextComponentDTO::FromJson(data);
                auto font = ResourceManager::GetInstance().LoadFont(dto.fontName, dto.fontSize);
                auto textComp = go->AddComponent<TextComponent>(dto.text, font);

                if (dto.hasColor) {
                    textComp->SetColor(dto.color);
                }
                });

            SceneLoader::RegisterComponentParser("FPSDynamicTextComponent", [](GameObject* go, const nlohmann::json& data) {
                auto dto = FPSDynamicTextComponentDTO::FromJson(data);
                auto dynamicFPS = go->AddComponent<DynamicTextComponent>([]() {
                    float dt = GameTime::GetInstance().GetDeltaTime();
                    if (dt <= 0.0f) return std::string("0");

                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(1) << (1.0f / dt);
                    return ss.str();
                    });
                dynamicFPS->SetPostfix(dto.postfix);
                });

            SceneLoader::RegisterComponentParser("RotatorComponent", [](GameObject* go, const nlohmann::json& data) {
                auto dto = RotatorComponentDTO::FromJson(data);
                go->AddComponent<RotatorComponent>(dto.radius, dto.speed);
                });

            SceneLoader::RegisterComponentParser("CacheProfilerComponent", [](GameObject* go, const nlohmann::json&) {
                //auto dto = CacheProfilerComponentDTO::FromJson(data);
                go->AddComponent<CacheProfilerComponent>();
                });

            SceneLoader::RegisterComponentParser("TagComponent", [](dae::GameObject* go, const nlohmann::json& data) {
                auto dto = TagComponentDTO::FromJson(data);
                go->AddComponent<TagComponent>(make_sdbm_hash_rt(dto.tag));
                });
        }
    };
}