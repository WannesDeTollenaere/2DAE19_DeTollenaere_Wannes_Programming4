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

void dae::SceneLoader::ParseGameObject(const nlohmann::json& objData, Scene& scene, GameObject* parent)
{
    std::string objectName = objData.value("name", "GameObject");
    auto gameObject = std::make_unique<GameObject>(objectName);
    GameObject* pGameObject = gameObject.get();

    // transform
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

    // PREFAB LOADING
    if (objData.contains("prefab"))
    {
        std::string prefabPath = objData.value("prefab", "");
        try
        {
            ParsePrefab(prefabPath, scene, pGameObject);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to load prefab: " << prefabPath << " - " << e.what() << "\n";
        }
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

    // children
    if (objData.contains("children") && objData["children"].is_array())
    {
        for (const auto& childData : objData["children"])
        {
            ParseGameObject(childData, scene, pGameObject);
        }
    }
}

void dae::SceneLoader::ParsePrefab(const std::string& prefabPath, Scene& scene, GameObject* pGameObject)
{
    nlohmann::json prefabData = ResourceManager::GetInstance().LoadJson(prefabPath);

    if (prefabData.contains("prefab"))
    {
        ParsePrefab(prefabData.value("prefab", ""), scene, pGameObject);
    }

    if (prefabData.contains("components") && prefabData["components"].is_array())
    {
        for (const auto& compData : prefabData["components"])
        {
            std::string type = compData.value("type", "");
            auto it = s_ComponentParsers.find(type);
            if (it != s_ComponentParsers.end())
            {
                it->second(pGameObject, compData);
            }
            else
            {
                std::cerr << "Warning: Unknown prefab component type: " << type << "\n";
            }
        }
    }

    if (prefabData.contains("children") && prefabData["children"].is_array())
    {
        for (const auto& childData : prefabData["children"])
        {
            ParseGameObject(childData, scene, pGameObject);
        }
    } 
}

void dae::SceneLoader::RegisterComponentParser(const std::string& type, std::function<void(dae::GameObject*, const nlohmann::json&) > parser)
{
    s_ComponentParsers[type] = parser;
}