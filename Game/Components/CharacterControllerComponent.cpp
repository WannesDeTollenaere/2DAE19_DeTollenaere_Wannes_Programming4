#include "CharacterControllerComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Commands/MoveCommand.h"
#include "Commands/DamageCommand.h"
#include "Commands/IncreaseScoreCommand.h"
#include <SDL3/SDL.h>
#include "SceneManager.h"
#include "Scene.h"
#include "Helpers/LevelGrid.h"
#include "Components/BoxColliderComponent.h"
#include "Components/TagComponent.h"
#include "Components/TextureComponent.h"
#include "Components/PlayerCharacter/SaltComponent.h"
#include "Commands/ThrowSaltCommand.h"

namespace dae
{
    CharacterControllerComponent::CharacterControllerComponent(GameObject* owner, float, bool useKeyboard, int controllerIndex)
        : Component(owner), m_useKeyboard(useKeyboard), m_controllerIndex(controllerIndex)
    {
        m_Anim = GetOwner()->GetComponent<AnimatorComponent>();

        auto& input = InputManager::GetInstance();

        if (m_useKeyboard)
        {
            input.BindKeyboardCommand(SDL_SCANCODE_W, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1), "WalkUp"));
            input.BindKeyboardCommand(SDL_SCANCODE_S, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1), "WalkDown"));
            input.BindKeyboardCommand(SDL_SCANCODE_A, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0), "WalkLeft"));
            input.BindKeyboardCommand(SDL_SCANCODE_D, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0), "WalkRight"));

            input.BindKeyboardCommand(SDL_SCANCODE_SPACE, InputState::Down, std::make_unique<ThrowSaltCommand>(owner));

            //DAMAGE
            input.BindKeyboardCommand(SDL_SCANCODE_C, InputState::Down, std::make_unique<DamageCommand>(owner, 1));
            // SCORE
            input.BindKeyboardCommand(SDL_SCANCODE_V, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 10));
            input.BindKeyboardCommand(SDL_SCANCODE_B, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 100));
        }
        else
        {
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadUp, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1), "WalkUp"));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadDown, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1), "WalkDown"));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadLeft, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0), "WalkLeft"));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadRight, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0), "WalkRight"));
            
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::A, InputState::Down, std::make_unique<ThrowSaltCommand>(owner));
            
            //DAMAGE
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::X, InputState::Down, std::make_unique<DamageCommand>(owner, 1));
            // SCORE
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::Y, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 10));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::B, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 100));
        }
    }
    void CharacterControllerComponent::Update()
    {
        if (!m_Anim) return;

        const auto& currentPos = GetOwner()->GetTransform().GetLocalPosition();
        if (currentPos == m_LastPosition)
        {
            m_Anim->PlayAnimation("Idle");
        }
        m_LastPosition = currentPos;
    }

    void dae::CharacterControllerComponent::ThrowSalt()
    {
        auto scene = SceneManager::GetInstance().GetActiveScene();
        if (!scene) return;

        auto saltObj = std::make_unique<GameObject>("SaltProjectile");

        auto playerPos = GetOwner()->GetTransform().GetLocalPosition();
        float tileSize = LevelGrid::GetInstance().GetTileSize();

        saltObj->GetTransform().SetLocalPosition(
            playerPos.x + m_FacingDirection.x * tileSize,
            playerPos.y + m_FacingDirection.y * tileSize,
            0.0f
        );

        auto tex = saltObj->AddComponent<TextureComponent>();
        tex->SetTexture("burger_time_spritesheet_x3.png");

        auto anim = saltObj->AddComponent<AnimatorComponent>(48, 48); 
        anim->AddAnimation("Splash", { 1, 12, 4, 0.1f, false }); // Row 0, Col 5, 4 frames, 0.1s per frame
        anim->PlayAnimation("Splash");

        // 4. Add Collision
        saltObj->AddComponent<BoxColliderComponent>(tileSize, tileSize);

        // 5. Add Tags
        std::unordered_set<Tag> tags = { make_sdbm_hash_rt("Salt") };
        saltObj->AddComponent<TagComponent>(tags);

        // 6. Add Salt Component (0.4f lifetime matches 4 frames * 0.1s)
        saltObj->AddComponent<SaltComponent>(0.4f);

        // 7. Add to the scene
        scene->Add(std::move(saltObj));
    }
}