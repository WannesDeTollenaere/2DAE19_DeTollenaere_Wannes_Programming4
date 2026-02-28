#pragma once
#include "Component.h"
#include <string>

// TODO: needs to be moved to the actual game later
namespace dae
{
    class AnimatorComponent;

    class PlayerComponent final : public Component
    {
    public:
        PlayerComponent(GameObject* owner);
        virtual ~PlayerComponent() = default;

        void Update() override;
        void RenderGUI() override;

        void SetState(const std::string& stateName);

    private:
        void InitializeAnimations();

        AnimatorComponent* m_pAnimator{ nullptr };
        std::string m_currentState{ "" };
        bool m_animationsInitialized{ false };
    };
}