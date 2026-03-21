#pragma once
#include "Component.h"
#include "glm/vec3.hpp"

namespace dae
{
    class AnimatorComponent;
    class CharacterControllerComponent final : public Component
    {
    public:
        CharacterControllerComponent(GameObject* owner, float speed, bool useKeyboard, int controllerIndex = 0);
        ~CharacterControllerComponent() = default;

        void Update() override;
    private:
        bool m_useKeyboard;
        int m_controllerIndex;

        AnimatorComponent* m_Anim;
        glm::vec3 m_LastPosition{};
    };
}