#include "EnemyComponent.h"
#include "GameObject.h"
#include "Components/AnimatorComponent.h"
#include "Components/BoxColliderComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/EnemyCrushedEvent.h"
#include "Events/LivesLostEvent.h"
#include "GameTime.h"
#include "SceneLoader.h"

namespace dae
{
    class EnemyComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json& data) override
        {
            EnemyType type = EnemyType::HotDog;
            std::string typeStr = data.value("enemyType", "HotDog");

            if (typeStr == "Pickle")
            {
                type = EnemyType::Pickle;
            }
            else if (typeStr == "Egg")
            {
                type = EnemyType::Egg;
            }
            else
            {
                type = EnemyType::HotDog;
            }

            auto enemyComp = go->AddComponent<EnemyComponent>();
            enemyComp->SetEnemyType(type);
        }
    };

    REGISTER_COMPONENT_PARSER(EnemyComponent, EnemyComponentParser);

    EnemyComponent::EnemyComponent(GameObject* pOwner)
        : Component(pOwner)
    {
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("EnemyCrushed"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("LivesLost"), this);
        EventManager::GetInstance().AttachEvent(make_sdbm_hash("LevelCompleted"), this);

        m_OriginalSpawnPosition = GetOwner()->GetTransform().GetLocalPosition();
    }

    EnemyComponent::~EnemyComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("EnemyCrushed"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("LivesLost"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("LevelCompleted"), this);
    }

    void EnemyComponent::Update()
    {
        if (!m_pAnimator) m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();
        if (m_State != EnemyState::Wandering) return;

        UpdateAnimation();
    }

    void EnemyComponent::HandleEvent(const Event* event)
    {
        auto crushEvent = dynamic_cast<const EnemyCrushedEvent*>(event);
        if (crushEvent && crushEvent->obj == GetOwner())
        {
            Die();
            return;
        } 

        if (dynamic_cast<const LivesLostEvent*>(event))
        {
            Respawn();
            return;
        }

        if (event && event->id == make_sdbm_hash("LevelCompleted"))
        {
            DisableMovement(); 
        }
    }

    void EnemyComponent::UpdateAnimation()
    {
        if (!m_pAnimator) return;

        if (m_CurrentDirection.x > 0) m_pAnimator->PlayAnimation("WalkRight");
        else if (m_CurrentDirection.x < 0) m_pAnimator->PlayAnimation("WalkLeft");
        else if (m_CurrentDirection.y > 0) m_pAnimator->PlayAnimation("WalkDown");
        else if (m_CurrentDirection.y < 0) m_pAnimator->PlayAnimation("WalkUp");
    }

    void EnemyComponent::Die()
    {
        if (m_State == EnemyState::Dead) return;
        m_State = EnemyState::Dead;

        if (m_pAnimator) m_pAnimator->PlayAnimation("Die");

        auto collider = GetOwner()->GetComponent<BoxColliderComponent>();
        if (collider) collider->SetActive(false);

        GameTime::GetInstance().AddTimer(m_TimeBeforeDestroy, [this]() {
            GetOwner()->SetActive(false);
            GameTime::GetInstance().AddTimer(m_RespawnDuration, [this]() {
                Respawn();
                });
            });
    }

    void EnemyComponent::Stun()
    {
        if (m_State != EnemyState::Wandering) return;
        m_State = EnemyState::Stunned;

        if (m_pAnimator) m_pAnimator->PlayAnimation("Pickled");

        GameTime::GetInstance().AddTimer(m_StunDuration, [this]() {
            m_State = EnemyState::Wandering;
            });
    }

    void EnemyComponent::Respawn()
    {
        GetOwner()->GetTransform().SetLocalPosition(m_OriginalSpawnPosition);
        m_State = EnemyState::Wandering;
        GetOwner()->SetActive(true);

        auto collider = GetOwner()->GetComponent<BoxColliderComponent>();
        if (collider) collider->SetActive(true);

        if (m_pAnimator) m_pAnimator->PlayAnimation("WalkDown");
    }

    void EnemyComponent::DisableMovement() { m_State = EnemyState::Disabled; }
    void EnemyComponent::EnableMovement() { m_State = EnemyState::Wandering; }
    void EnemyComponent::SetCascading(bool cascading) { m_State = cascading ? EnemyState::Cascading : EnemyState::Wandering; }
}