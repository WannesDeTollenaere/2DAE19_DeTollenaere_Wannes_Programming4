#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

namespace dae
{
    class EnemyComponent;

    class PlayerEnemyControllerComponent final : public Component
    {
    public:
        PlayerEnemyControllerComponent(GameObject* owner, int controllerIndex);
        ~PlayerEnemyControllerComponent();

        void Update() override;

        void SetFacingDirection(const glm::vec2& dir);
        bool IsMovementDisabled() const;

    private:
        int m_controllerIndex;
        EnemyComponent* m_EnemyComp{ nullptr };

        void UnbindInput();
    };
}