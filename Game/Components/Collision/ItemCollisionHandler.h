#pragma once
#include "Components/BaseCollisionHandler.h"

namespace dae
{
    class ItemCollisionHandler final : public BaseCollisionHandler
    {
    public:
        ItemCollisionHandler(GameObject* owner) : BaseCollisionHandler(owner) {}

    protected:
        void OnCollision(GameObject* otherObject, TagComponent* otherTagComp) override;
    };
}