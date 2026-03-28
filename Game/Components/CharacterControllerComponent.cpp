#include "CharacterControllerComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Commands/MoveCommand.h"
#include "Commands/DamageCommand.h"
#include "Commands/IncreaseScoreCommand.h"
#include <SDL3/SDL.h>

namespace dae
{
    CharacterControllerComponent::CharacterControllerComponent(GameObject* owner, float, bool useKeyboard, int controllerIndex)
        : Component(owner), m_useKeyboard(useKeyboard), m_controllerIndex(controllerIndex)
    {
        m_Anim = GetOwner()->GetComponent<AnimatorComponent>();

        auto& input = InputManager::GetInstance();

        if (m_useKeyboard)
        {
            input.BindKeyboardCommand(SDL_SCANCODE_W, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1), "WalkUp"));
            input.BindKeyboardCommand(SDL_SCANCODE_S, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1), "WalkDown"));
            input.BindKeyboardCommand(SDL_SCANCODE_A, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0), "WalkLeft"));
            input.BindKeyboardCommand(SDL_SCANCODE_D, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0), "WalkRight"));

            //DAMAGE
            input.BindKeyboardCommand(SDL_SCANCODE_C, InputState::Down, std::make_unique<DamageCommand>(owner, 1));
            // SCORE
            input.BindKeyboardCommand(SDL_SCANCODE_V, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 10));
            input.BindKeyboardCommand(SDL_SCANCODE_B, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 100));
        }
        else
        {
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadUp, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1), "WalkUp"));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadDown, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1), "WalkDown"));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadLeft, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0), "WalkLeft"));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadRight, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0), "WalkRight"));
            //DAMAGE
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::X, InputState::Down, std::make_unique<DamageCommand>(owner, 1));
            // SCORE
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::Y, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 10));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::B, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 100));
        }
    }
    void CharacterControllerComponent::Update()
    {
        if (!m_Anim) return;

        const auto& currentPos = GetOwner()->GetTransform().GetLocalPosition();
        if (currentPos == m_LastPosition)
        {
            m_Anim->PlayAnimation("Idle");
        }
        m_LastPosition = currentPos;
    }
}