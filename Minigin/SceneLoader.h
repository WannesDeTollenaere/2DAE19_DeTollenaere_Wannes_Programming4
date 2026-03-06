#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>

namespace dae
{
    class Scene;
    class GameObject;

    class SceneLoader final
    {
    public:
        static void LoadScene(Scene& scene, const std::string& jsonFilePath);

        static void RegisterComponentParser(const std::string& type, std::function<void(GameObject*, const nlohmann::json&)> parser);

    private:
        static void ParseGameObject(const nlohmann::json& objData, Scene& scene, GameObject* parent = nullptr);

        static std::unordered_map<std::string, std::function<void(GameObject*, const nlohmann::json&)>> s_ComponentParsers;
    };
}
