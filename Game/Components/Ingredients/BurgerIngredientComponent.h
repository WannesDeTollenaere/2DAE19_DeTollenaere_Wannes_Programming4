#pragma once
#include "Components/BaseCollisionHandler.h" 
#include "Components/Movement/EnemyWanderComponent.h"
#include <vector>

namespace dae
{
    class BurgerIngredientComponent final : public BaseCollisionHandler 
    {
    public:
        BurgerIngredientComponent(GameObject* owner, int widthInTiles);
        ~BurgerIngredientComponent() = default;

        virtual void Update() override;

        void StepOnSegment(int colOffset);
        void ForceDrop();
        bool IsFalling() const { return m_IsFalling; }

        void StopFallingAt(float targetY); 
        void SetInPlate(bool inPlate) { m_IsInPlate = inPlate; }
        bool IsInPlate() const { return m_IsInPlate; }
    protected:
        void OnCollision(GameObject* otherObject, TagComponent* otherTagComp) override;
        void OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp) override;
        void OnCollisionExit(GameObject* otherObject, TagComponent* otherTagComp) override;

    private:
        void StartFalling();
        void HandleFalling(float deltaTime); 
        float FindNextPlatformY(int levelsToDrop);

        int m_WidthInTiles;
        std::vector<bool> m_SteppedSegments;

        bool m_IsFalling{ false };
        float m_FallSpeed{ 150.0f };
        float m_TargetDropY{ 0.0f };

        bool m_IsInPlate{ false };

        std::vector<GameObject*> m_CascadingEnemies;
    };
}