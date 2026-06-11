#pragma once
#include "Components/BaseCollisionHandler.h"
#include <string>

namespace dae
{
    enum class PickupType
    {
        IceCream,
        Coffee,
        Fries
    };

    class PickupComponent final : public BaseCollisionHandler
    {
    public:
        PickupComponent(GameObject* owner, PickupType type, int saltAmount = 1, int scoreValue = 0);
        ~PickupComponent() = default;

        PickupType GetType() const { return m_Type; }

    protected:
        void OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp) override;

    private:
        PickupType m_Type;
        int m_SaltAmount;
        int m_ScoreValue;
        bool m_IsCollected{ false };
    };

    PickupType PickupTypeFromString(const std::string& str);
}
