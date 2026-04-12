#include "PlayerCollisionHandler.h"
#include "ObserverSys/EventManager.h"
#include "Events/DealDamageEvent.h"
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class PlayerCollisionHandlerComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json&) override
        {
            go->AddComponent<PlayerCollisionHandler>();
        }
    };

    REGISTER_COMPONENT_PARSER(PlayerCollisionHandler, PlayerCollisionHandlerComponentParser);



    void PlayerCollisionHandler::OnCollision(GameObject*, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash("Enemy")))
        {
            DealDamageEvent damage(GetOwner(), 1);
            EventManager::GetInstance().SendEvent(&damage);
        }
    }
}