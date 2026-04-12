#include "ItemCollisionHandler.h"
#include "Events/ScoreIncreaseEvent.h"
#include "ObserverSys/EventManager.h"
#include "SceneLoader.h"
#include <nlohmann/json.hpp>

namespace dae
{
    class ItemCollisionHandlerComponentParser final : public IComponentParser
    {
    public:
        void Parse(GameObject* go, const nlohmann::json&) override
        {
            go->AddComponent<ItemCollisionHandler>();
        }
    };

    REGISTER_COMPONENT_PARSER(ItemCollisionHandler, ItemCollisionHandlerComponentParser);

    void ItemCollisionHandler::OnCollision(GameObject* otherObject, TagComponent* otherTagComp)
    {
        if (otherTagComp && otherTagComp->HasTag(make_sdbm_hash("Player")))
        {
            ScoreIncreasedEvent score(otherObject, 100);
            EventManager::GetInstance().SendEvent(&score);

            GetOwner()->Destroy();
        }
    }
}

  