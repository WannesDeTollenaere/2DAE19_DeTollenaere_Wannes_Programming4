#pragma once
#include "Commands/Command.h"
#include "Components/UI/MenuComponent.h"

namespace dae {
    class MenuComponent;

    class MenuMoveCommand final : public Command
    {
        MenuComponent* m_pMenu;
        int m_Dir;
    public:
        MenuMoveCommand(MenuComponent* menu, int dir) : m_pMenu(menu), m_Dir(dir) {}
        void Execute() override { m_pMenu->MoveSelection(m_Dir); }
    };

    class MenuConfirmCommand final : public Command
    {
        MenuComponent* m_pMenu;
    public:
        MenuConfirmCommand(MenuComponent* menu) : m_pMenu(menu) {}
        void Execute() override { m_pMenu->ConfirmSelection(); }
    };
}