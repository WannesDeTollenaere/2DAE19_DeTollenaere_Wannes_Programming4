#pragma once
#include "Component.h"
#include <string>

namespace dae
{
    class SceneTransitionComponent final : public Component
    {
    public:
        SceneTransitionComponent(GameObject* owner, const std::string& targetScene, int controllerIndex = 0);
        virtual ~SceneTransitionComponent() = default;
    };
}