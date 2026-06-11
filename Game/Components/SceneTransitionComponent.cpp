#include "SceneTranistionComponent.h"
#include "InputManager.h"
#include "Commands/LoadSceneCommand.h"
#include <SDL3/SDL.h>
#include "SceneLoader.h"
#include <nlohmann/json.hpp>
#include "GameTime.h"

namespace dae
{
    class SceneTransitionComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            std::string targetScene = data.value("targetScene", "");
            float amountOfTime = data.value("timeAmount", -1.f);

            go->AddComponent<SceneTransitionComponent>(targetScene, amountOfTime);
        }
    };

    REGISTER_COMPONENT_PARSER(SceneTransitionComponent, SceneTransitionComponentParser);


    SceneTransitionComponent::SceneTransitionComponent(GameObject* owner, const std::string& targetScene, float amountOfTime, int controllerIndex)
        : Component(owner)
    {
        auto& input = InputManager::GetInstance();

        input.BindCommand(
            static_cast<uint16_t>(controllerIndex),
            Gamepad::ControllerButton::Start,
            InputState::Down,
            std::make_unique<LoadSceneCommand>(targetScene)
        );

        input.BindKeyboardCommand(
            SDL_SCANCODE_RETURN,
            InputState::Down,
            std::make_unique<LoadSceneCommand>(targetScene)
        );

        if (amountOfTime > 0.f)
        {
            GameTime::GetInstance().AddTimer(amountOfTime, [targetScene]() {
                SceneManager::GetInstance().SetActiveScene(targetScene);
                });
        }
    }
    SceneTransitionComponent::~SceneTransitionComponent()
    {
        auto& input = InputManager::GetInstance();

        input.UnbindCommand(
            static_cast<uint16_t>(0),
            Gamepad::ControllerButton::Start,
            InputState::Down
		);
        input.UnbindKeyboardCommand(
            SDL_SCANCODE_RETURN,
            InputState::Down
		);
    }
}