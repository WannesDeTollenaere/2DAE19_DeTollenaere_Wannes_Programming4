#include "SceneTranistionComponent.h"
#include "InputManager.h"
#include "Commands/LoadSceneCommand.h"
#include <SDL3/SDL.h>

namespace dae
{
    SceneTransitionComponent::SceneTransitionComponent(GameObject* owner, const std::string& targetScene, int controllerIndex)
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
    }
}