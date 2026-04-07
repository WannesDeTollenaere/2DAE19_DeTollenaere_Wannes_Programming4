#pragma once
#include "Component.h"
#include "Components/BaseCollisionHandler.h"

namespace dae
{
    class SaltComponent final : public BaseCollisionHandler
    {
    public:
        SaltComponent(GameObject* pOwner, float lifetime = 0.4f);
        ~SaltComponent() = default;

    protected:
        void OnCollision(GameObject* otherObject, TagComponent* otherTagComp) override;

    private:
        float m_Lifetime;
    };
}