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
#include "Components/LifeTimeComponent.h"
#include "GameManager.h"
#include "ObserverSys/EventManager.h"
#include "Events/SaltThrownEvent.h"
#include "Helpers/PrefabFactory.h"
#include "Components/Enemy/EnemyComponent.h"
#include "Components/AnimatorComponent.h"
#include "SceneLoader.h"
#include "Components/PlayerCharacter/SaltManagerComponent.h" 
#include <nlohmann/json.hpp>

namespace dae
{
    class CharacterControllerComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            float speed = data.value("speed", 50.0f);

            std::string inputType = data.value("inputType", "keyboard");
            bool useKeyboard = (inputType == "keyboard");

            int controllerIndex = data.value("controllerIndex", 0);

            go->AddComponent<CharacterControllerComponent>(speed, useKeyboard, controllerIndex);
        }
    };

    REGISTER_COMPONENT_PARSER(CharacterControllerComponent, CharacterControllerComponentParser);


    CharacterControllerComponent::CharacterControllerComponent(GameObject* owner, float, bool useKeyboard, int controllerIndex)
        : BaseCollisionHandler(owner), m_useKeyboard(useKeyboard), m_controllerIndex(controllerIndex)
    {
        m_Anim = GetOwner()->GetComponent<AnimatorComponent>();
        m_SpawnPosition = GetOwner()->GetTransform().GetLocalPosition();
        auto& input = InputManager::GetInstance();

        if (m_useKeyboard)
        {
            input.BindKeyboardCommand(SDL_SCANCODE_W, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1)));
            input.BindKeyboardCommand(SDL_SCANCODE_S, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1)));
            input.BindKeyboardCommand(SDL_SCANCODE_A, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0)));
            input.BindKeyboardCommand(SDL_SCANCODE_D, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0)));

            input.BindKeyboardCommand(SDL_SCANCODE_SPACE, InputState::Down, std::make_unique<ThrowSaltCommand>(owner));
             
            ////DAMAGE
            //input.BindKeyboardCommand(SDL_SCANCODE_C, InputState::Down, std::make_unique<DamageCommand>(owner, 1));
            //// SCORE
            //input.BindKeyboardCommand(SDL_SCANCODE_V, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 10));
            //input.BindKeyboardCommand(SDL_SCANCODE_B, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 100));
        }

            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadUp, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, -1)));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadDown, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(0, 1)));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadLeft, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(-1, 0)));
            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadRight, InputState::Pressed, std::make_unique<MoveCommand>(owner, glm::vec2(1, 0)));

            input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::A, InputState::Down, std::make_unique<ThrowSaltCommand>(owner));
         

        dae::EventManager::GetInstance().AttachEvent(make_sdbm_hash("LevelCompleted"), this);
        ////DAMAGE
        //input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::X, InputState::Down, std::make_unique<DamageCommand>(owner, 1));
        //// SCORE
        //input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::Y, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 10));
        //input.BindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::B, InputState::Down, std::make_unique<IncreaseScoreCommand>(owner, 100));
    }
    CharacterControllerComponent::~CharacterControllerComponent()
    {
        dae::EventManager::GetInstance().DetachEvent(make_sdbm_hash("LevelCompleted"), this);
        UnbindInput();
    }
    void CharacterControllerComponent::OnCollisionEnter(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash_rt("Enemy")))
        {
            auto enemyWander = otherObject->GetComponent<EnemyComponent>();
            if (enemyWander && enemyWander->IsDangerous())
            {
                Die();
            }
        }
    }
    void CharacterControllerComponent::HandleEvent(const Event* event)
    {
        BaseCollisionHandler::HandleEvent(event);

        if (event->id == make_sdbm_hash("LevelCompleted"))
        {
            m_IsLevelComplete = true; 

            if (m_Anim)
            {
                m_Anim->PlayAnimation("Victory");
                UnbindInput();
            }
        }
    }
    void CharacterControllerComponent::Update()
    {
        if (!m_Anim || m_IsDead || m_IsLevelComplete) return;

        const auto& currentPos = GetOwner()->GetTransform().GetLocalPosition();

        if (currentPos == m_LastPosition)
        {
            m_Anim->PlayAnimation("Idle");
        }
        else
        {
            if (m_FacingDirection.y < 0.0f) m_Anim->PlayAnimation("WalkUp");
            else if (m_FacingDirection.y > 0.0f) m_Anim->PlayAnimation("WalkDown");
            else if (m_FacingDirection.x < 0.0f) m_Anim->PlayAnimation("WalkLeft");
            else if (m_FacingDirection.x > 0.0f) m_Anim->PlayAnimation("WalkRight");
        }

        m_LastPosition = currentPos;
    }
     


    void CharacterControllerComponent::ThrowSalt()
    {
        auto saltComp = GetOwner()->GetComponent<SaltManagerComponent>();

        if (m_IsDead || m_IsLevelComplete || !saltComp || saltComp->GetSalt() <= 0) return;

        auto scene = SceneManager::GetInstance().GetActiveScene();
        if (!scene) return;

        auto playerPos = GetOwner()->GetTransform().GetWorldPosition();
        float tileSize = LevelGrid::GetInstance().GetTileSize();
        glm::vec3 spawnPos{
            playerPos.x + m_FacingDirection.x * tileSize,
            playerPos.y + m_FacingDirection.y * tileSize,
            0.0f
        };

        scene->Add(PrefabFactory::CreateSaltProjectile(spawnPos));

        saltComp->AddSalt(-1);
    }

    void CharacterControllerComponent::Die()
    {
        if (m_IsDead) return;
        m_IsDead = true;

        if (m_Anim)
        { 
            m_Anim->PlayAnimation("Die");   
        }
         
        dae::GameTime::GetInstance().AddTimer(1.5f, [&]() {
            dae::GameManager::GetInstance().LoseLife();
            m_IsDead = false;
            GetOwner()->GetTransform().SetLocalPosition(m_SpawnPosition);
            
            });  
    }
    void CharacterControllerComponent::UnbindInput()
    {
        auto& input = InputManager::GetInstance();
        if (m_useKeyboard)
        {
            input.UnbindKeyboardCommand(SDL_SCANCODE_W, InputState::Pressed);
            input.UnbindKeyboardCommand(SDL_SCANCODE_S, InputState::Pressed);
            input.UnbindKeyboardCommand(SDL_SCANCODE_A, InputState::Pressed);
            input.UnbindKeyboardCommand(SDL_SCANCODE_D, InputState::Pressed);
            input.UnbindKeyboardCommand(SDL_SCANCODE_SPACE, InputState::Down);
        }

        input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadUp, InputState::Pressed);
        input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadDown, InputState::Pressed);
        input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadLeft, InputState::Pressed);
        input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::DPadRight, InputState::Pressed);

        input.UnbindCommand(static_cast<uint16_t>(m_controllerIndex), Gamepad::ControllerButton::A, InputState::Down);
    }
}