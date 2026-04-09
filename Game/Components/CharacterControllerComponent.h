#pragma once
#include "Component.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "Components/BaseCollisionHandler.h"

namespace dae
{
    class AnimatorComponent;
    class CharacterControllerComponent final : public BaseCollisionHandler
    {
    public:
        CharacterControllerComponent(GameObject* owner, float speed, bool useKeyboard, int controllerIndex = 0);
        ~CharacterControllerComponent() = default;

        void SetFacingDirection(const glm::vec2& dir) { m_FacingDirection = dir; }
        void ThrowSalt();

        void OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp) override;
        void Update() override;

        void Die();
        bool IsDead() const { return m_IsDead; }
    private:
        bool m_useKeyboard;
        int m_controllerIndex;

        AnimatorComponent* m_Anim;
        glm::vec3 m_LastPosition{};
        glm::vec3 m_SpawnPosition{};
        glm::vec2 m_FacingDirection{ 1.0f, 0.0f };

        bool m_IsDead{ false };
    };
}