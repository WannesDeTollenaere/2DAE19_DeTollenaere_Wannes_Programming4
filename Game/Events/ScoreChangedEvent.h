// Game/Events/ScoreChangedEvent.h
#pragma once
#include "ObserverSys/Event.h"
#include "sdbm_hash.h"

namespace dae
{
    struct ScoreChangedEvent : public GameObjectEvent
    {
        int newScore;

        ScoreChangedEvent(GameObject* pObj, int score) : GameObjectEvent(make_sdbm_hash("ScoreChanged"), pObj), newScore(score)
        {
        }
    };
}