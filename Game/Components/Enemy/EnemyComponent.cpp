#include "EnemyComponent.h"
#include "GameObject.h"
#include "Components/AnimatorComponent.h"
#include "Components/BoxColliderComponent.h"
#include "ObserverSys/EventManager.h"
#include "Events/EnemyCrushedEvent.h"
#include "Events/LivesLostEvent.h"
#include "GameTime.h"
#include "States/EnemyState.h"
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

        ChangeState(std::make_unique<EnemyWanderingState>(this));
    }

    EnemyComponent::~EnemyComponent()
    {
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("EnemyCrushed"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("LivesLost"), this);
        EventManager::GetInstance().DetachEvent(make_sdbm_hash("LevelCompleted"), this);
    }

    void EnemyComponent::ChangeState(std::unique_ptr<EnemyState> newState)
    {
        if (newState)
        {
            m_pCurrentState = std::move(newState);
            m_pCurrentState->OnEnter();
        }
    }

    void EnemyComponent::Update()
    {
        if (!m_pAnimator) m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();

        ChangeState(m_pCurrentState->Update());
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

    void EnemyComponent::Die() { ChangeState(m_pCurrentState->OnDie()); }
    void EnemyComponent::Stun() { ChangeState(m_pCurrentState->OnStun()); }
    void EnemyComponent::DisableMovement() { ChangeState(m_pCurrentState->OnDisable()); }
    void EnemyComponent::EnableMovement() { ChangeState(m_pCurrentState->OnEnable()); }
    void EnemyComponent::SetCascading(bool cascading) { ChangeState(m_pCurrentState->OnSetCascading(cascading)); }

    void EnemyComponent::Respawn()
    {
        GetOwner()->GetTransform().SetLocalPosition(m_OriginalSpawnPosition);
        ChangeState(std::make_unique<EnemyWanderingState>(this));
    }

    bool EnemyComponent::IsDead() const { return m_pCurrentState->IsDead(); }
    bool EnemyComponent::IsStunned() const { return m_pCurrentState->IsStunned(); }
    bool EnemyComponent::IsDangerous() const { return m_pCurrentState->IsDangerous(); }
    bool EnemyComponent::IsMovementDisabled() const { return m_pCurrentState->IsMovementDisabled(); }
    bool EnemyComponent::IsCascading() const { return m_pCurrentState->IsCascading(); }
}