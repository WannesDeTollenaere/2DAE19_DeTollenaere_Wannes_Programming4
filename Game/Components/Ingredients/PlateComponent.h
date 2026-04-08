#pragma once
#include "Components/BaseCollisionHandler.h"
#include <vector>

namespace dae
{
    class PlateComponent final : public BaseCollisionHandler
    {
    public:
        PlateComponent(GameObject* owner, int requiredIngredients = 3);
        void RenderGUI() override;

    protected:
        void OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp) override;

    private:
        int m_RequiredIngredients;
        std::vector<GameObject*> m_CaughtIngredients;
        bool m_IsComplete{ false };

        void CalculateColliderHeight();
    };
}