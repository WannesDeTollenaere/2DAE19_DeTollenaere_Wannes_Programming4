#pragma once
#include "Component.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "Components/BaseCollisionHandler.h"
#include <memory>

namespace dae
{
    class AnimatorComponent;
    class PlayerState; 

    class CharacterControllerComponent final : public BaseCollisionHandler
    {
    public:
        CharacterControllerComponent(GameObject* owner, float speed, bool useKeyboard, int controllerIndex = 0);
        ~CharacterControllerComponent();

        void SetFacingDirection(const glm::vec2& dir) { m_FacingDirection = dir; }
        glm::vec2 GetFacingDirection() const { return m_FacingDirection; }

        void ThrowSalt();
        void Die();
        void Update() override;
        void HandleEvent(const Event* event) override; 

        void OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp) override;

        AnimatorComponent* GetAnimator() const { return m_Anim; }
        glm::vec3 GetLastPosition() const { return m_LastPosition; }
        void SetLastPosition(const glm::vec3& pos) { m_LastPosition = pos; }
        glm::vec3 GetSpawnPosition() const { return m_SpawnPosition; }

        void PerformThrowSalt(); 
        void UnbindInput();

        bool IsDead() const;

    private:
        bool m_useKeyboard;
        int m_controllerIndex;

        AnimatorComponent* m_Anim;
        glm::vec3 m_LastPosition{};
        glm::vec3 m_SpawnPosition{};
        glm::vec2 m_FacingDirection{ 1.0f, 0.0f };

        friend class PlayerState;
        std::unique_ptr<PlayerState> m_pCurrentState;

        void ChangeState(std::unique_ptr<PlayerState> newState);
    };
}