#pragma once
#include "Component.h"
#include "Commands/Command.h"
#include <memory>

#include "MenuItemComponent.h"
#include "SceneLoader.h"
#include "Commands/LoadSceneCommand.h"
#include "Commands/QuitCommand.h" 
#include <nlohmann/json.hpp>


namespace dae
{
    class MenuItemComponent final : public Component
    {
    public:
        MenuItemComponent(GameObject* owner, std::unique_ptr<Command> pCommand)
            : Component(owner), m_pCommand(std::move(pCommand)) {
        }

        void OnConfirm() { if (m_pCommand) m_pCommand->Execute(); }

    private:
        std::unique_ptr<Command> m_pCommand;
    };


    class MenuItemComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            std::string action = data.value("action", "");
            std::unique_ptr<Command> pCmd = nullptr;

            if (action == "load_scene") {
                std::string scene = data.value("scene", "");
                pCmd = std::make_unique<LoadSceneCommand>(scene);
            }
            else if (action == "quit") {
                pCmd = std::make_unique<QuitCommand>();
            }

            go->AddComponent<MenuItemComponent>(std::move(pCmd));
        }
    };

    REGISTER_COMPONENT_PARSER(MenuItemComponent, MenuItemComponentParser);
}