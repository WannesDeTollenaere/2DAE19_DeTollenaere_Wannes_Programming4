#pragma once
#include <memory>
#include "Components/CharacterControllerComponent.h"
#include "glm/vec3.hpp"

namespace dae
{
    class AnimatorComponent;
    class GameObject;

    class PlayerState
    {
    protected:
        CharacterControllerComponent* m_pPlayer;

    public:
        PlayerState(CharacterControllerComponent* player) : m_pPlayer(player) {}
        virtual ~PlayerState() = default;

        virtual void OnEnter() {}

        virtual std::unique_ptr<PlayerState> Update() { return nullptr; }
        virtual std::unique_ptr<PlayerState> ThrowSalt() { return nullptr; }
        virtual std::unique_ptr<PlayerState> Die() { return nullptr; }
        virtual std::unique_ptr<PlayerState> CompleteLevel() { return nullptr; }

        GameObject* GetOwner() const
        {
            return m_pPlayer->GetOwner();
        }

        AnimatorComponent* GetAnimator() const
        {
            return m_pPlayer->GetAnimator();
        }

        glm::vec3 GetSpawnPosition() const
        {
            return m_pPlayer->GetSpawnPosition();
        }
    };

    class AliveState final : public PlayerState
    {
    public:
        AliveState(CharacterControllerComponent* player) : PlayerState(player) {}
        std::unique_ptr<PlayerState> Update() override;
        std::unique_ptr<PlayerState> ThrowSalt() override;
        std::unique_ptr<PlayerState> Die() override;
        std::unique_ptr<PlayerState> CompleteLevel() override;
    };

    class DeadState final : public PlayerState
    {
    public:
        DeadState(CharacterControllerComponent* player) : PlayerState(player) {}
        void OnEnter() override;
        std::unique_ptr<PlayerState> Update() override;
    private:
        float m_RespawnTimer{ 1.5f };
    };

    class VictoryState final : public PlayerState
    {
    public:
        VictoryState(CharacterControllerComponent* player) : PlayerState(player) {}
        void OnEnter() override;
    };
}