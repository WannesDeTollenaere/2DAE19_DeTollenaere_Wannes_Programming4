#include "MenuComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Components/TextComponent.h"
#include "MenuItemComponent.h"
#include "Commands/MenuNavigationCommand.h" 
#include "SceneLoader.h"

namespace dae
{
    REGISTER_COMPONENT_PARSER(MenuComponent, SimpleParser<MenuComponent>);


    MenuComponent::MenuComponent(GameObject* owner, const SDL_Color& sel, const SDL_Color& def)
        : Component(owner), m_SelectedColor(sel), m_DefaultColor(def)
    {
        BindInputs();
    }

    MenuComponent::~MenuComponent()
    {
        auto& input = InputManager::GetInstance();
        input.UnbindKeyboardCommand(SDL_SCANCODE_W, InputState::Down);
        input.UnbindKeyboardCommand(SDL_SCANCODE_S, InputState::Down);
        input.UnbindKeyboardCommand(SDL_SCANCODE_RETURN, InputState::Down);

        input.UnbindCommand(0, Gamepad::ControllerButton::DPadDown, InputState::Down);
        input.UnbindCommand(0, Gamepad::ControllerButton::DPadUp, InputState::Down);
        input.UnbindCommand(0, Gamepad::ControllerButton::A, InputState::Down);
    }

    void MenuComponent::BindInputs()
    {
        auto& input = InputManager::GetInstance();
        input.BindKeyboardCommand(SDL_SCANCODE_W, InputState::Down, std::make_unique<MenuMoveCommand>(this, -1));
        input.BindKeyboardCommand(SDL_SCANCODE_S, InputState::Down, std::make_unique<MenuMoveCommand>(this, 1));
        input.BindKeyboardCommand(SDL_SCANCODE_RETURN, InputState::Down, std::make_unique<MenuConfirmCommand>(this));

        input.BindCommand(0, Gamepad::ControllerButton::DPadDown, InputState::Down, std::make_unique<MenuMoveCommand>(this, 1));
        input.BindCommand(0, Gamepad::ControllerButton::DPadUp, InputState::Down, std::make_unique<MenuMoveCommand>(this, -1));
        input.BindCommand(0, Gamepad::ControllerButton::A, InputState::Down, std::make_unique<MenuConfirmCommand>(this));
    }

    void MenuComponent::MoveSelection(int direction)
    {
        auto buttons = GetMenuButtons();
        if (buttons.empty()) return;

        m_SelectedIndex = (m_SelectedIndex + direction + static_cast<int>(buttons.size())) % static_cast<int>(buttons.size());
        UpdateVisuals();
    }

    void MenuComponent::ConfirmSelection()
    {
        auto buttons = GetMenuButtons();
        if (m_SelectedIndex >= 0 && m_SelectedIndex < static_cast<int>(buttons.size()))
        {
            if (auto item = buttons[m_SelectedIndex]->GetComponent<MenuItemComponent>())
                item->OnConfirm();
        }
    }

    void MenuComponent::UpdateVisuals()
    {
        auto buttons = GetMenuButtons();
        for (int i = 0; i < static_cast<int>(buttons.size()); ++i)
        {
            if (auto text = buttons[i]->GetComponent<TextComponent>())
                text->SetColor(i == m_SelectedIndex ? m_SelectedColor : m_DefaultColor);
        }
    }

    std::vector<GameObject*> MenuComponent::GetMenuButtons() const
    {
        std::vector<GameObject*> buttons;
        for (auto child : GetOwner()->GetChildren())
        {
            if (child->GetComponent<MenuItemComponent>())
                buttons.push_back(child);
        }
        return buttons;
    }
}