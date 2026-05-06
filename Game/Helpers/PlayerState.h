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
    public:
        virtual ~PlayerState() = default;

        virtual void OnEnter(CharacterControllerComponent*) {}

        virtual std::unique_ptr<PlayerState> Update(CharacterControllerComponent*) { return nullptr; }
        virtual std::unique_ptr<PlayerState> ThrowSalt(CharacterControllerComponent*) { return nullptr; }
        virtual std::unique_ptr<PlayerState> Die(CharacterControllerComponent*) { return nullptr; }
        virtual std::unique_ptr<PlayerState> CompleteLevel(CharacterControllerComponent* ) { return nullptr; }

        GameObject* GetOwner(CharacterControllerComponent* player) const
        {
            return player->GetOwner(); 
        }

        AnimatorComponent* GetAnimator(CharacterControllerComponent* player) const
        {
            return player->m_Anim;
        }

        glm::vec3 GetSpawnPosition(CharacterControllerComponent* player) const
        {
            return player->m_SpawnPosition;
        }
    };

    class AliveState final : public PlayerState
    {
    public:
        std::unique_ptr<PlayerState> Update(CharacterControllerComponent* player) override;
        std::unique_ptr<PlayerState> ThrowSalt(CharacterControllerComponent* player) override;
        std::unique_ptr<PlayerState> Die(CharacterControllerComponent* player) override;
        std::unique_ptr<PlayerState> CompleteLevel(CharacterControllerComponent* player) override;
    };

    class DeadState final : public PlayerState
    {
    public:
        void OnEnter(CharacterControllerComponent* player) override;
        std::unique_ptr<PlayerState> Update(CharacterControllerComponent* player) override;
    private:
        float m_RespawnTimer{ 1.5f }; 
    };

    class VictoryState final : public PlayerState
    {
    public:
        void OnEnter(CharacterControllerComponent* player) override;
    };
}