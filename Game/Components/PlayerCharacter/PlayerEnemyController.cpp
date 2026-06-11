#include "PlayerEnemyController.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Commands/MoveCommand.h"
#include "../Enemy/EnemyComponent.h"
#include <nlohmann/json.hpp>
#include "SceneLoader.h"

namespace dae
{
    class PlayerEnemyControllerParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            int controllerIndex = data.value("controllerIndex", 1);
            go->AddComponent<PlayerEnemyControllerComponent>(controllerIndex);
        }
    };

    REGISTER_COMPONENT_PARSER(PlayerEnemyControllerComponent, PlayerEnemyControllerParser);

    PlayerEnemyControllerComponent::PlayerEnemyControllerComponent(GameObject* owner, int controllerIndex)
        : Component(owner), m_controllerIndex(controllerIndex)
    {
        EXPOSE(m_controllerIndex);

        auto& input = InputManager::GetInstance();
        uint16_t idx = static_cast<uint16_t>(m_controllerIndex);

        input.BindCommand(idx, Gamepad::ControllerButton::DPadUp, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1)));
        input.BindCommand(idx, Gamepad::ControllerButton::DPadDown, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1)));
        input.BindCommand(idx, Gamepad::ControllerButton::DPadLeft, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0)));
        input.BindCommand(idx, Gamepad::ControllerButton::DPadRight, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0)));
    }

    PlayerEnemyControllerComponent::~PlayerEnemyControllerComponent()
    {
        UnbindInput();
    }

    void PlayerEnemyControllerComponent::Update()
    {
        if (!m_EnemyComp)
        {
            m_EnemyComp = GetOwner()->GetComponent<EnemyComponent>();
        }
    }

    void PlayerEnemyControllerComponent::SetFacingDirection(const glm::vec2& dir)
    {
        if (m_EnemyComp)
        {
            m_EnemyComp->SetCurrentDirection(dir);
        }
    }

    bool PlayerEnemyControllerComponent::IsMovementDisabled() const
    {
        return m_EnemyComp ? m_EnemyComp->IsMovementDisabled() : false;
    }

    void PlayerEnemyControllerComponent::UnbindInput()
    {
        auto& input = InputManager::GetInstance();
        uint16_t idx = static_cast<uint16_t>(m_controllerIndex);

        input.UnbindCommand(idx, Gamepad::ControllerButton::DPadUp, InputState::Pressed);
        input.UnbindCommand(idx, Gamepad::ControllerButton::DPadDown, InputState::Pressed);
        input.UnbindCommand(idx, Gamepad::ControllerButton::DPadLeft, InputState::Pressed);
        input.UnbindCommand(idx, Gamepad::ControllerButton::DPadRight, InputState::Pressed);
    }
}