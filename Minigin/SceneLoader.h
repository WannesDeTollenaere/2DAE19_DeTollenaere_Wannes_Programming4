#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace dae
{
    class Scene;
    class GameObject;

    class SceneLoader final
    {
    public:
        static void LoadScene(Scene& scene, const std::string& jsonFilePath);

    private:
        static void ParseGameObject(const nlohmann::json& objData, Scene& scene, GameObject* parent = nullptr);
    };
}