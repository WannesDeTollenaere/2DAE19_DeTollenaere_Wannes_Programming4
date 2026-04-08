#pragma once
#include "Component.h"
#include "Components/BaseCollisionHandler.h"

namespace dae
{
    class SaltComponent final : public BaseCollisionHandler
    {
    public:
        SaltComponent(GameObject* pOwner);
        ~SaltComponent() = default;

    protected:
        void OnCollision(GameObject* otherObject, TagComponent* otherTagComp) override;

    private:
    };
}