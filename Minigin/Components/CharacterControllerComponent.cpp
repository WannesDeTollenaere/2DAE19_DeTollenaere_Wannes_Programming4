#include "CharacterControllerComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Commands/MoveCommand.h"
#include <SDL3/SDL.h>

namespace dae
{
    CharacterControllerComponent::CharacterControllerComponent(GameObject* owner, float speed, bool useKeyboard, int controllerIndex)
        : Component(owner), m_useKeyboard(useKeyboard), m_controllerIndex(controllerIndex)
    {
        auto& input = InputManager::GetInstance();

        if (m_useKeyboard)
        {
            input.BindKeyboardCommand(SDL_SCANCODE_W, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1), speed));
            input.BindKeyboardCommand(SDL_SCANCODE_S, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1), speed));
            input.BindKeyboardCommand(SDL_SCANCODE_A, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0), speed));
            input.BindKeyboardCommand(SDL_SCANCODE_D, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0), speed));
        }
        else
        {
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadUp, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1), speed));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadDown, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1), speed));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadLeft, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0), speed));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadRight, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0), speed));
        }
    }

    CharacterControllerComponent::~CharacterControllerComponent()
    {
        //auto& input = InputManager::GetInstance();

        //if (m_useKeyboard)
        //{
        //    input.UnbindKeyboardCommand(SDL_SCANCODE_W, InputState::Pressed);
        //    input.UnbindKeyboardCommand(SDL_SCANCODE_S, InputState::Pressed);
        //    input.UnbindKeyboardCommand(SDL_SCANCODE_A, InputState::Pressed);
        //    input.UnbindKeyboardCommand(SDL_SCANCODE_D, InputState::Pressed);
        //}
        //else
        //{
        //    input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadUp, InputState::Pressed);
        //    input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadDown, InputState::Pressed);
        //    input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadLeft, InputState::Pressed);
        //    input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadRight, InputState::Pressed);
        //}
    }
}