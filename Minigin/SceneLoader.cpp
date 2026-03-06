#include "SceneLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceManager.h"

#include "Components/AnimatorComponent.h"
#include "Components/CacheProfilerComponent.h"
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

std::unordered_map<std::string, std::function<void(dae::GameObject*, const nlohmann::json&)>> dae::SceneLoader::s_ComponentParsers{};

void dae::SceneLoader::LoadScene(Scene& scene, const std::string& jsonFilePath)
{
    json sceneData;

    try
    {
        sceneData = ResourceManager::GetInstance().LoadJson(jsonFilePath);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "SceneLoader Error: " << e.what() << "\n";
        return;
    }

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

            auto it = s_ComponentParsers.find(type);
            if (it != s_ComponentParsers.end())
            {
                it->second(pGameObject, compData);
            }
            else
            {
                std::cerr << "Warning: Unknown component type in JSON: " << type << "\n";
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

void dae::SceneLoader::RegisterComponentParser(const std::string& type, std::function<void(dae::GameObject*, const nlohmann::json&) > parser)
{
    s_ComponentParsers[type] = parser;
}