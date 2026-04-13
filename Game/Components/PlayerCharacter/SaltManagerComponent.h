#pragma once
#include "Component.h"
#include "Components/TagComponent.h" 

namespace dae
{
    class SaltManagerComponent final : public Component
    {
    public:
        SaltManagerComponent(GameObject* pOwner, int startingSalt = 5);
        ~SaltManagerComponent() = default;

        void Update() override;

        void AddSalt(int amount);
        int GetSalt() const { return m_Salt; }
        void ResetSalt();

    private:
        int m_Salt{ 5 };
        int m_StartingSalt{ 5 };

        bool m_IsInitialized{ false };

        Tag GetPlayerTag() const;
    };
}