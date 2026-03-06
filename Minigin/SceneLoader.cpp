#include "SceneLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceManager.h"

#include "Components/AnimatorComponent.h"
#include "Components/CacheProfilerComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/DynamicTextComponent.h"
#include "Components/RotatorComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "GameTime.h"

#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void dae::SceneLoader::LoadScene(Scene& scene, const std::string& jsonFilePath)
{
    std::ifstream file("Data/" +jsonFilePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open scene file: " << jsonFilePath << "\n";
        return;
    }

    json sceneData;
    file >> sceneData;

    std::string sceneName = sceneData.value("name", "Unnamed Scene");
    std::cout << "Loading Scene: " << sceneName << "\n";

    if (sceneData.contains("objects") && sceneData["objects"].is_array())
    {
        for (const auto& objData : sceneData["objects"])
        {
            ParseGameObject(objData, scene, nullptr);
        }
    }
}

void dae::SceneLoader::ParseGameObject(const json& objData, Scene& scene, GameObject* parent)
{

    auto gameObject = std::make_unique<GameObject>();
    GameObject* pGameObject = gameObject.get(); 

    if (objData.contains("transform"))
    {
        float x = objData["transform"].value("x", 0.0f);
        float y = objData["transform"].value("y", 0.0f);
        pGameObject->SetPosition(x, y);
    }

    if (parent != nullptr)
    {
        bool keepWorldPos = objData.value("keepWorldPosition", false);
        pGameObject->SetParent(parent, keepWorldPos);
    }

    if (objData.contains("components") && objData["components"].is_array())
    {
        for (const auto& compData : objData["components"])
        {
            std::string type = compData.value("type", "");

            if (type == "TextureComponent")
            {
                auto texComp = gameObject->AddComponent<TextureComponent>();
                if (compData.contains("texture"))
                {
                    texComp->SetTexture(compData["texture"].get<std::string>());
                }
            }
            else if (type == "AnimatorComponent")
            {
                int width = compData.value("frameWidth", 16);
                int height = compData.value("frameHeight", 16);
                gameObject->AddComponent<AnimatorComponent>(width, height);
            }
            else if (type == "PlayerComponent")
            {
                gameObject->AddComponent<PlayerComponent>();
            }
            else if (type == "TextComponent")
            {
                std::string text = compData.value("text", "");
                std::string fontName = compData.value("font", "Lingua.otf");
                uint8_t fontSize = static_cast<uint8_t>(compData.value("fontSize", 36));

                auto font = ResourceManager::GetInstance().LoadFont(fontName, fontSize);
                auto textComp = gameObject->AddComponent<TextComponent>(text, font);


                if (compData.contains("color")) {
                    auto colorData = compData["color"];
                    textComp->SetColor({
                        static_cast<unsigned char>(colorData.value("r", 255)),
                        static_cast<unsigned char>(colorData.value("g", 255)),
                        static_cast<unsigned char>(colorData.value("b", 255)),
                        static_cast<unsigned char>(colorData.value("a", 255))
                        });
                }
            }
            else if (type == "FPSDynamicTextComponent")
            {
                auto dynamicFPS = gameObject->AddComponent<DynamicTextComponent>([]() {
                    float dt = GameTime::GetInstance().GetDeltaTime();
                    if (dt <= 0.0f) return std::string("0");

                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(1) << (1.0f / dt);
                    return ss.str();
                    });
                dynamicFPS->SetPostfix(compData.value("postfix", " FPS"));
            }
            else if (type == "RotatorComponent")
            {
                float radius = compData.value("radius", 10.f);
                float speed = compData.value("speed", 10.f);
                gameObject->AddComponent<RotatorComponent>(radius, speed);
            }
            else if (type == "CacheProfilerComponent")
            {
                gameObject->AddComponent<CacheProfilerComponent>();
            }
        }
    }

    scene.Add(std::move(gameObject));

    if (objData.contains("children") && objData["children"].is_array())
    {
        for (const auto& childData : objData["children"])
        {
            ParseGameObject(childData, scene, pGameObject);
        }
    }
}


