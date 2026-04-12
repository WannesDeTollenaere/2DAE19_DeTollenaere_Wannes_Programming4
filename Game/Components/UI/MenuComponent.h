#pragma once
#include "Component.h"
#include <SDL3/SDL.h>
#include <vector>
#include <string>

namespace dae
{
    class MenuComponent final : public Component
    {
    public:
        MenuComponent(GameObject* owner,
            const SDL_Color& selectedColor = { 255, 255, 0, 255 },
            const SDL_Color& defaultColor = { 255, 255, 255, 255 });

        virtual ~MenuComponent() override;

        void MoveSelection(int direction);
        void ConfirmSelection();

    private:
        int m_SelectedIndex{ 0 };
        SDL_Color m_SelectedColor;
        SDL_Color m_DefaultColor;

        void UpdateVisuals();
        void BindInputs();

        std::vector<GameObject*> GetMenuButtons() const;
    };
}