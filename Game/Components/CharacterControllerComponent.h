#pragma once
#include "Component.h"

namespace dae
{
    class CharacterControllerComponent final : public Component
    {
    public:
        CharacterControllerComponent(GameObject* owner, float speed, bool useKeyboard, int controllerIndex = 0);
        ~CharacterControllerComponent() = default;

    private:
        bool m_useKeyboard;
        int m_controllerIndex;
    };
}