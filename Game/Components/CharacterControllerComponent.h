#pragma once
#include "Component.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace dae
{
    class AnimatorComponent;
    class CharacterControllerComponent final : public Component
    {
    public:
        CharacterControllerComponent(GameObject* owner, float speed, bool useKeyboard, int controllerIndex = 0);
        ~CharacterControllerComponent() = default;

        void SetFacingDirection(const glm::vec2& dir) { m_FacingDirection = dir; }
        void ThrowSalt();

        void Update() override;
    private:
        bool m_useKeyboard;
        int m_controllerIndex;

        AnimatorComponent* m_Anim;
        glm::vec3 m_LastPosition{};
        glm::vec2 m_FacingDirection{ 1.0f, 0.0f };
    };
}