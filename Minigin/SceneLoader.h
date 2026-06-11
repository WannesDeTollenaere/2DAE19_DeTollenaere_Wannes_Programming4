#pragma once
#include <string>
#include <unordered_map>
#include <functional>   
#include <memory>
#include <nlohmann/json.hpp>
#include "GameObject.h"

namespace dae
{

    class IComponentParser {
    public:
        virtual ~IComponentParser() = default;
        virtual void Parse(GameObject* go, const nlohmann::json& data) = 0;
    };

    // a generic parser for components that dont need json data 
    template <typename T>
    class SimpleParser final : public IComponentParser {
    public:
        void Parse(GameObject* go, const nlohmann::json&) override {
            go->AddComponent<T>();
        }
    };

    class Scene;
    class GameObject;

    class SceneLoader final
    {
    public:
        static void LoadScene(Scene& scene, const std::string& jsonFilePath);

        static void RegisterComponentParser(const std::string& type, std::unique_ptr<IComponentParser> parser);

        static GameObject* Instantiate(Scene& scene, const std::string& prefabPath, float x, float y, GameObject* parent = nullptr);

    private:
        static void ParseGameObject(const nlohmann::json& objData, Scene& scene, GameObject* parent = nullptr); 
        static void ParsePrefab(const std::string& prefabPath, Scene& scene, GameObject* pGameObject);

        static std::unordered_map<std::string, std::unique_ptr<IComponentParser>>& GetParsersMap();
    };
}
#define REGISTER_COMPONENT_PARSER(Type, ParserClass) \
    static struct Type##Registrar { \
        Type##Registrar() { \
            dae::SceneLoader::RegisterComponentParser(#Type, std::make_unique<ParserClass>()); \
        } \
    } Type##RegistrarInstance;